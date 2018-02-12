/* Copyright (C) 2012-2017 Micael Oliveira <micael.oliveira@mpsd.mpg.de>
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
 * @file pspio_upf.c
 * @brief implementation to read and write in PSPIO_FMT_UPF files 
 */
#include <stdlib.h>
#include <string.h>

#include "upf.h"

#if defined HAVE_CONFIG_H
#include "config.h"
#endif


int pspio_upf_read(FILE *fp, pspio_pspdata_t *pspdata)
{
  int np;

  SUCCEED_OR_RETURN( upf_read_info(fp, pspdata) );

  /*
    At the moment the wave equation type is not defined in the
    header, so we set it to 0 if the ADDINFO tag is not present, and
    to PSPIO_EQN_DIRAC if it is
  */
  if (upf_tag_isdef(fp,"PP_ADDINFO")){
    pspdata->wave_eq = PSPIO_EQN_DIRAC;
  } else {
    pspdata->wave_eq = 0;
  }

  SUCCEED_OR_RETURN( upf_read_header(fp, &np, pspdata) );
  SUCCEED_OR_RETURN( upf_read_mesh(fp, np, pspdata) );
  if ( pspio_xc_has_nlcc(pspdata->xc) ) {
    SUCCEED_OR_RETURN( upf_read_nlcc(fp, np, pspdata) );
  }
  SUCCEED_OR_RETURN( upf_read_nonlocal(fp, np, pspdata) );
  SUCCEED_OR_RETURN( upf_read_pswfc(fp, np, pspdata) );
  SUCCEED_OR_RETURN( upf_read_local(fp, np, pspdata) );
  SUCCEED_OR_RETURN( upf_read_rhoatom(fp, np, pspdata) );

  return PSPIO_SUCCESS;
}

int pspio_upf_write(FILE *fp, const pspio_pspdata_t *pspdata)
{
  assert(pspdata != NULL);

  upf_write_info(fp, pspdata);
  SUCCEED_OR_RETURN( upf_write_header(fp, pspdata) );
  upf_write_mesh(fp, pspdata);
  if (pspio_xc_has_nlcc(pspdata->xc)) {
    upf_write_nlcc(fp, pspdata);
  }
  upf_write_local(fp, pspdata);
  upf_write_nonlocal(fp, pspdata);
  upf_write_pswfc(fp, pspdata);
  upf_write_rhoatom(fp, pspdata);
  if ( pspdata->wave_eq == PSPIO_EQN_DIRAC ) {
    upf_write_addinfo(fp, pspdata);
  }

  return PSPIO_SUCCESS;
}
