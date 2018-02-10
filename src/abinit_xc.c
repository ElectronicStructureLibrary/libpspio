/* Copyright (C) 2011-2016 Matthieu Verstraete <matthieu.jean.verstraete@gmail.com>
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


#include "pspio_error.h"
#include "pspio_xc.h"

#if defined HAVE_CONFIG_H
#include "config.h"
#endif

/**
 * Convert an Abinit exchange-correlation index into LibXC identifiers
 * Negative values are combined from 2 libxc codes following Abinit
 * conventions:
 *   pspxc = -(exchange * 1000 + correlation)
 * For positive values of pspxc, we need to make an explicit conversion.
 * @param[in] pspxc: the Abinit XC index
 * @param[out] exchange: LibXC identifier for exchange
 * @param[out] correlation: LibXC identifier for correlation
 *
 * \note
 * pspxc = 13 is a bit problematic because correlation in LB94 is
 * undetermined and this is up to the codes to check the physics.
 * Here we use the same correlation as in Abinit, i.e. PW91.
 */
int abinit_to_libxc(int pspxc, int *exchange, int *correlation)
{
  if ( pspxc < 0 ) {

    *correlation = abs(pspxc) % 1000;
    *exchange = (abs(pspxc) - *correlation) / 1000;

  } else {

    switch(pspxc) {
    case 0:
      *exchange = XC_NONE;
      *correlation = XC_NONE;
      break;
    case 1:
      *exchange = XC_LDA_XC_TETER93;
      *correlation = XC_LDA_XC_TETER93;
      break;
    case 2:
      *exchange = XC_LDA_X;
      *correlation = XC_LDA_C_PZ_MOD;
      break;
    case 4:
      *exchange = XC_LDA_X;
      *correlation = XC_LDA_C_WIGNER;
      break;
    case 5:
      *exchange = XC_LDA_X;
      *correlation = XC_LDA_C_HL;
      break;
    case 6:
      *exchange = XC_LDA_X;
      *correlation = XC_LDA_C_XALPHA;
      break;
    case 7:
      *exchange = XC_LDA_X;
      *correlation = XC_LDA_C_PW;
      break;
    case 8:
      *exchange = XC_LDA_X;
      *correlation = XC_NONE;
      break;
    case 9:
      *exchange = XC_LDA_X;
      *correlation = XC_LDA_C_PW_RPA;
      break;
    case 11:
      *exchange = XC_GGA_X_PBE;
      *correlation = XC_GGA_C_PBE;
      break;
    case 12:
      *exchange = XC_GGA_X_PBE;
      *correlation = XC_NONE;
      break;
    case 13:
      *exchange = XC_GGA_X_LB;
      *correlation = XC_GGA_C_PW91;
      break;
    case 14:
      *exchange = XC_GGA_X_PBE_R;
      *correlation = XC_GGA_C_PBE;
      break;
    case 15:
      *exchange = XC_GGA_X_RPBE;
      *correlation = XC_GGA_C_PBE;
      break;
    case 16:
      *exchange = XC_GGA_XC_HCTH_93;
      *correlation = XC_GGA_XC_HCTH_93;
      break;
    case 17:
      *exchange = XC_GGA_XC_HCTH_120;
      *correlation = XC_GGA_XC_HCTH_120;
      break;
    case 23:
      *exchange = XC_GGA_X_WC;
      *correlation = XC_GGA_C_PBE;
      break;
    case 24:
      *exchange = XC_GGA_X_C09X;
      *correlation = XC_NONE;
      break;
    case 26:
      *exchange = XC_GGA_XC_HCTH_147;
      *correlation = XC_GGA_XC_HCTH_147;
      break;
    case 27:
      *exchange = XC_GGA_XC_HCTH_407;
      *correlation = XC_GGA_XC_HCTH_407;
      break;

      /* The following are not in LibXC */
    case 3:
    case 20:
    case 21:
    case 22:

      /* The following are real errors */
    case 10:
    case 18:
    case 19:
    case 25:
    case 28:

      /* Unknown abinit pspxc */
    default:
      return PSPIO_EVALUE;
    }

  }

  return PSPIO_SUCCESS;
}


int libxc_to_abinit(int exchange, int correlation, int *pspxc)
{
  *pspxc = -(exchange * 1000 + correlation);

  return PSPIO_SUCCESS;
}
