/* Copyright (C) 2011-2018 Damien Caliste <dcaliste@free.fr>
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

#include <string.h>
#include <assert.h>

#include "util.h"
#include "pspio_error.h"

#if defined HAVE_CONFIG_H
#include "config.h"
#endif


const char symbols[][4] = {"H",  "He", "Li", "Be", "B",  "C",  "N",  "O",  "F",  "Ne", "Na",
                           "Mg", "Al", "Si", "P",  "S",  "Cl", "Ar", "K",  "Ca", "Sc", "Ti",
                           "V",  "Cr", "Mn", "Fe", "Co", "Ni", "Cu", "Zn", "Ga", "Ge", "As",
                           "Se", "Br", "Kr", "Rb", "Sr", "Y",  "Zr", "Nb", "Mo", "Tc", "Ru",
                           "Rh", "Pd", "Ag", "Cd", "In", "Sn", "Sb", "Te", "I",  "Xe", "Cs",
                           "Ba", "La", "Ce", "Pr", "Nd", "Pm", "Sm", "Eu", "Gd", "Tb", "Dy",
                           "Ho", "Er", "Tm", "Yb", "Lu", "Hf", "Ta", "W",  "Re", "Os", "Ir",
                           "Pt", "Au", "Hg", "Tl", "Pb", "Bi", "Po", "At", "Rn", "Fr", "Ra",
                           "Ac", "Th", "Pa", "U",  "Np", "Pu", "Am", "Cm", "Bk", "Cf", "Es",
                           "Fm", "Md", "No", "Lr", "Rf", "Db", "Sg", "Bh", "Hs", "Mt",
                           "Uun", "Uuu", "Uub"};


int symbol_to_z(const char symbol[4], double *z)
{
  int i;

  assert(symbol != NULL);

  for (i=0; i<112; i++) {
    if (!strcmp(symbol, symbols[i])) {
      *z = i + 1.0;
      return PSPIO_SUCCESS;
    }
  }

  return PSPIO_ERROR;
}


int z_to_symbol(double z, char symbol[4])
{
  FULFILL_OR_RETURN(z < 113.0 && z > 0.0, PSPIO_EVALUE);

  strcpy(symbol, symbols[(int)z-1]);

  return PSPIO_SUCCESS;
}
  

double linear_extrapolation(double x1, double x2, double f1,
			    double f2, double x)
{
  double mm, f;

  mm = (f2 - f1) / (x2 - x1);
  f = f1 + mm * (x - x1);

  return f;
}


char * psp_scheme_name(int scheme)
{
  switch (scheme) {
    case PSPIO_SCM_BHS:
      return "Bachelet-Hamann-Schluter";
    case PSPIO_SCM_GTH:
      return "Goedecker-Teter-Hutter";
    case PSPIO_SCM_HAMANN:
      return "Hamann scheme";
    case PSPIO_SCM_HGH:
      return "Hartwigsen-Goedecker-Hutter";
    case PSPIO_SCM_HSC:
      return "Hamann-Schluter-Chiang";
    case PSPIO_SCM_KERKER:
      return "Kerker scheme";
    case PSPIO_SCM_MRPP:
      return "Multi-reference TM";
    case PSPIO_SCM_ONCV:
      return "Hamann ONCV scheme";
    case PSPIO_SCM_RRKJ:
      return "Rappe-Rabe-Kaxiras-Joannopoulos";
    case PSPIO_SCM_TM:
      return "Troullier-Martins";
    case PSPIO_SCM_TM2:
      return "Troullier-Martins 2";
    case PSPIO_SCM_RTM:
      return "Relativistic Troullier-Martins";
    case PSPIO_SCM_UNKNOWN:
    default:
      return "Unknown";
  }
}

char * psp_relativitic_treatment_name(int wave_eq)
{
  switch (wave_eq) {
  case PSPIO_EQN_SCHRODINGER:
    return "non-relativistic";
  case PSPIO_EQN_SCALAR_REL:
    return "scalar-relativistic";
  case PSPIO_EQN_DIRAC:
    return "fully-relativistic";
  default:
    return "unknown";
  }
}

int read_array_4by4(FILE *fp, double *array, int npts) {

  char line[PSPIO_STRLEN_LINE];
  int i, j, nargs, nsup;
  double tmp[4];

  if ( (npts % 4) == 0 ) {
    nsup = npts - 4;
  } else {
    nsup = npts - (npts % 4);
  }
  for (i=0; i<nsup; i+=4) {
    FULFILL_OR_RETURN( fgets(line, PSPIO_STRLEN_LINE, fp) != NULL, PSPIO_EIO );
    nargs = sscanf(line, "%lf %lf %lf %lf", &tmp[0], &tmp[1], &tmp[2], &tmp[3]);
    FULFILL_OR_RETURN( nargs == 4, PSPIO_EFILE_CORRUPT );
    for (j=0; j<nargs; j++) array[i+j] = tmp[j];
  }
  FULFILL_OR_RETURN( fgets(line, PSPIO_STRLEN_LINE, fp) != NULL, PSPIO_EIO );
  nargs = sscanf(line, "%lf %lf %lf %lf", &tmp[0], &tmp[1], &tmp[2], &tmp[3]);
  FULFILL_OR_RETURN( nargs == (npts - nsup), PSPIO_EFILE_CORRUPT );
  for (j=0; j<nargs; j++) array[nsup+j] = tmp[j];

  return PSPIO_SUCCESS;
}
