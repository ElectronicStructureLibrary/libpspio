/* Copyright (C) 2012-2017 Matteo Giantomassi <gmatteo@gmac>
 *                         Micael Oliveira <micael.oliveira@mpsd.mpg.de>
 *                         Yann Pouillon <devops@materialsevolution.es>
 *
 * This file is part of Libpspio.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * version 2.0. If a copy of the MPL was not distributed with this file, You
 * can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Libpspio is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the Mozilla Public License version 2.0 for
 * more details.
 */

/** 
 * @file abinit_util.c
 * @brief Utility routines to read/write Abinit files
 */
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "abinit.h"
#include "util.h"
#include "pspio_pspinfo.h"

#if defined HAVE_CONFIG_H
#include "config.h"
#endif

/* Prototypes of the replacement functions for MacOSX */
static char *my_strndup (const char *s, size_t n);


int abinit_read_header(FILE *fp, int format, pspio_pspdata_t *pspdata)
{
  char line[PSPIO_STRLEN_LINE], symbol[4], tmp[256], code_name[PSPIO_STRLEN_LINE], description[PSPIO_STRLEN_DESCRIPTION];
  char *line4;
  int format_read, year, month, day, pspcod, pspxc, lmax, lloc, mmax;
  int exchange, correlation;
  int ppl[6], npso[2];
  double zatom, zval, r2well, rchrg, fchrg, qchrg;

  /* Line 1: read title */
  FULFILL_OR_RETURN( fgets(line, PSPIO_STRLEN_LINE, fp) != NULL, PSPIO_EIO );
  FULFILL_OR_RETURN( sscanf(line, "%s %s : %s", tmp, code_name, description) == 3, PSPIO_EFILE_CORRUPT );
  SUCCEED_OR_RETURN( pspio_pspinfo_alloc(&pspdata->pspinfo) );
  SUCCEED_OR_RETURN( pspio_pspinfo_set_code_name(pspdata->pspinfo, code_name) );
  SUCCEED_OR_RETURN( pspio_pspinfo_set_description(pspdata->pspinfo, description) );

  /* Line 2: read atomic number, Z valence */
  FULFILL_OR_RETURN( fgets(line, PSPIO_STRLEN_LINE, fp) != NULL, PSPIO_EIO );
  FULFILL_OR_RETURN( sscanf(line, "%lf %lf %2d%2d%2d", &zatom, &zval, &year, &month, &day) == 5, PSPIO_EFILE_CORRUPT );
  SUCCEED_OR_RETURN( pspio_pspdata_set_z(pspdata, zatom) );
  SUCCEED_OR_RETURN( pspio_pspdata_set_zvalence(pspdata, zval) );
  SUCCEED_OR_RETURN( z_to_symbol(zatom, symbol) );
  SUCCEED_OR_RETURN( pspio_pspdata_set_symbol(pspdata, symbol) );
  SUCCEED_OR_RETURN( pspio_pspinfo_set_generation_day(pspdata->pspinfo, day) );
  SUCCEED_OR_RETURN( pspio_pspinfo_set_generation_month(pspdata->pspinfo, month) );
  SUCCEED_OR_RETURN( pspio_pspinfo_set_generation_year(pspdata->pspinfo, year) );

  /* Line 3: read pspcod, pspxc, lmax, lloc, mmax, r2well */
  FULFILL_OR_RETURN( fgets(line, PSPIO_STRLEN_LINE, fp) != NULL, PSPIO_EIO );
  FULFILL_OR_RETURN( sscanf(line, "%d %d %d %d %d %lf",
    &pspcod, &pspxc, &lmax, &lloc, &mmax, &r2well) == 6, PSPIO_EFILE_CORRUPT );
  SUCCEED_OR_RETURN( pspio_pspdata_set_l_max(pspdata, lmax) );
  SUCCEED_OR_RETURN( pspio_pspdata_set_l_local(pspdata, lloc) );

  /* Following APE conventions: pspxc = -(exchange + correlation * 1000) */
  if ( pspxc < 0 ) {
    exchange = -pspxc % 1000;
    correlation = -pspxc / 1000;
  } else {
    SUCCEED_OR_RETURN( abinit_to_libxc(pspxc, &exchange, &correlation) );
  }
  SUCCEED_OR_RETURN( pspio_xc_alloc(&pspdata->xc) );
  SUCCEED_OR_RETURN( pspio_xc_set_exchange(pspdata->xc, exchange) );
  SUCCEED_OR_RETURN( pspio_xc_set_correlation(pspdata->xc, correlation) );

  /* Line 4: read rchrg, fchrg, qchrg if NLCC */
  /* Note: tolerance copied from Abinit */
  /* Note: qchrg is not used and might be removed from future formats */
  FULFILL_OR_RETURN( fgets(line, PSPIO_STRLEN_LINE, fp) != NULL, PSPIO_EIO );
  line4 = my_strndup(line, 3);
  if ( strcmp("4--", line4) != 0 ) {
    DEFER_TEST_ERROR( sscanf(line, "%lf %lf %lf",
      &rchrg, &fchrg, &qchrg) == 3, PSPIO_EFILE_CORRUPT );
    if ( fabs(fchrg) >= 1.0e-14 ) {
      pspio_xc_set_nlcc_scheme(pspdata->xc, PSPIO_NLCC_FHI);
      pspio_xc_set_nlcc_prefactors(pspdata->xc, rchrg, fchrg);
    }
  }
  free(line4);
  RETURN_ON_DEFERRED_ERROR;

  /* Ignore lines 5-7, except with format 8 */
  /* FIXME: do something with multiple projectors */
  /* FIXME: add spin-orbit support */
  if ( pspcod == 8 ) {
    FULFILL_OR_RETURN( fgets(line, PSPIO_STRLEN_LINE, fp) != NULL, PSPIO_EIO );
    FULFILL_OR_RETURN( sscanf(line, "%d %d %d %d %d", &ppl[0], &ppl[1], &ppl[2], &ppl[3], &ppl[4]) == 5, PSPIO_EFILE_CORRUPT );
    FULFILL_OR_RETURN( fgets(line, PSPIO_STRLEN_LINE, fp) != NULL, PSPIO_EIO );
    FULFILL_OR_RETURN( sscanf(line, "%d %d", &npso[0], &npso[1]) == 2, PSPIO_EFILE_CORRUPT );
    FULFILL_OR_RETURN( ((npso[0] == 1) && (npso[1] == 1)), PSPIO_ENOSUPPORT );
  } else {
    FULFILL_OR_RETURN( fgets(line, PSPIO_STRLEN_LINE, fp) != NULL, PSPIO_EIO );
    FULFILL_OR_RETURN( fgets(line, PSPIO_STRLEN_LINE, fp) != NULL, PSPIO_EIO );
    FULFILL_OR_RETURN( fgets(line, PSPIO_STRLEN_LINE, fp) != NULL, PSPIO_EIO );
  }

  /* Check that the format found is the one we expected */
  switch (pspcod) {
  case 1:
    format_read = PSPIO_FMT_ABINIT_1;
    break;
  case 2:
    format_read = PSPIO_FMT_ABINIT_2;
    break;
  case 3:
    format_read = PSPIO_FMT_ABINIT_3;
    break;
  case 4:
    format_read = PSPIO_FMT_ABINIT_4;
    break;
  case 5:
    format_read = PSPIO_FMT_ABINIT_5;
    break;
  case 6:
    format_read = PSPIO_FMT_ABINIT_6;
    break;
  case 7:
    format_read = PSPIO_FMT_ABINIT_7;
    break;
  case 8:
    format_read = PSPIO_FMT_ABINIT_8;
    break;
  case 9:
    format_read = PSPIO_FMT_ABINIT_9;
    break;
  case 10:
    format_read = PSPIO_FMT_ABINIT_10;
    break;
  case 11:
    format_read = PSPIO_FMT_ABINIT_11;
    break;
  case 17:
    format_read = PSPIO_FMT_ABINIT_17;
    break;
  default:
    format_read = PSPIO_FMT_UNKNOWN;
  }
  FULFILL_OR_RETURN( format_read == format, PSPIO_EFILE_FORMAT );

  return PSPIO_SUCCESS;
}

int abinit_write_header(FILE *fp, int format, const pspio_pspdata_t *pspdata)
{
  char pspdate[7];
  int pspxc, have_nlcc;
  int *ppl;
  double rchrg, fchrg, qchrg;

  assert(fp != NULL);
  assert(pspdata != NULL);

  /* Init auxiliary data */
  have_nlcc = pspio_xc_has_nlcc(pspdata->xc);
  SUCCEED_OR_RETURN(libxc_to_abinit(pspdata->xc->exchange, \
    pspdata->xc->correlation, &pspxc));

  /* Line 1: write title */
  if ( format == 8 ) {
    /* FIXME: write core radii */
    fprintf(fp, "%-3s   %s  r_core= %9.5f %9.5f %9.5f\n",
            pspio_pspdata_get_symbol(pspdata),
            pspio_pspinfo_get_code_name(pspdata->pspinfo),
            0.0, 0.0, 0.0);
  } else {
    fprintf(fp, " %3s %s: %s\n", pspio_pspdata_get_symbol(pspdata),
            pspio_pspinfo_get_code_name(pspdata->pspinfo),
            psp_scheme_name(pspio_pspdata_get_scheme(pspdata)));
  }

  /* Line 2: write atomic number, Z valence, psp date */
  sprintf(pspdate, "%2.2d%2.2d%2.2d",
          pspio_pspinfo_get_generation_year(pspdata->pspinfo),
          pspio_pspinfo_get_generation_month(pspdata->pspinfo),
          pspio_pspinfo_get_generation_day(pspdata->pspinfo));
  pspdate[6] = '\0';
  FULFILL_OR_RETURN( fprintf(fp, " %11.4f %11.4f %11s   zatom, zion, pspd\n",
    pspdata->z, pspdata->zvalence, pspdate) > 0, PSPIO_EIO );
  
  /* Line 3: write pspcod, pspxc, lmax, lloc, mmax, r2well */
  /* Line 4: write rchrg, fchrg, qchrg if NLCC */
  /* Lines 5-7: free comments, except for 8 */
  switch ( format ) {
  case 4:
  case 5:
  case 6:
  case 8:
    FULFILL_OR_RETURN(
        fprintf(fp, " %5d %7d %3d %5d %5d %5.1f   pspcod, pspxc, lmax, lloc, mmax, r2well\n",
        format, pspxc, pspdata->l_max, pspdata->l_local,
        pspdata->mesh->np, 0.0) > 0, PSPIO_EIO );

    if ( have_nlcc ) {
      rchrg = pspdata->xc->nlcc_pf_scale;
      fchrg = pspdata->xc->nlcc_pf_value;
      qchrg = 0.0;
    } else {
      rchrg = 0.0;
      fchrg = 0.0;
      qchrg = 0.0;
    }
    FULFILL_OR_RETURN(
        fprintf(fp, " %11.8f %11.8f %11.8f   rchrg, fchrg, qchrg\n",
        rchrg, fchrg, qchrg) > 0, PSPIO_EIO );

    /* FIXME: get spin-orbit information, no spin-orbit for now */
    if ( format == 8 ) {
      ppl = pspio_projectors_per_l(pspdata->projectors, pspdata->n_projectors);
      FULFILL_OR_RETURN( fprintf(fp, " %5d %5d %5d %5d %5d          nproj\n",
        ppl[0],ppl[1],ppl[2],ppl[3],ppl[4]) > 0, PSPIO_EIO );
      FULFILL_OR_RETURN( fprintf(fp, " %5d %5d                      extension_switch\n", 1, 1) > 0, PSPIO_EIO );
    } else {
      FULFILL_OR_RETURN( fprintf(fp, "5--- These two lines are available for giving more information, later\n6\n") > 0, PSPIO_EIO );
      FULFILL_OR_RETURN( fprintf(fp, "7-Here follows the cpi file from the fhi98pp code-\n") > 0, PSPIO_EIO );
    }
    break;
  default:
    return PSPIO_EFILE_FORMAT;
  }

  return PSPIO_SUCCESS;
}

/**
 * A replacement function for systems that lack strndup (MacOSX).
 * @param[in] s: string to duplicate.
 * @param[in] n: number of characters to duplicate.
 * @return the duplicated string.
 */
char *my_strndup(const char *s, size_t n)
{
#if defined HAVE_STRNDUP
  return strndup(s, n);
#else
  const char *end = memchr(s, '\0', n);
  size_t len = end ? (size_t) (end - s) : n;
  char *new = malloc(len + 1);

  if ( new == NULL ) return NULL;

  new[len] = '\0';
  return memcpy(new, s, len);
#endif
}
