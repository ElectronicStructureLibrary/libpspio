/*
 Copyright (C) 2011 J. Alberdi, M. Oliveira, Y. Pouillon, and M. Verstraete

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation; either version 3 of the License, or 
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

 $Id$
*/


#include <stdio.h>
#include <string.h>

#include "pspio_error.h"
#include "libxccodes.h"
#include "pspio_xc_funcs.h"

#if defined HAVE_CONFIG_H
#include "config.h"
#endif


/**
* subroutine converts abinit pspxc code to libxc codes
*@param[in]  pspxc: abinit xc code
*@param[out] exchange: libxc code for exchange
*@param[out] correlation: libxc code for correlation
*/
int ab2libxc (const int pspxc, int exchange, int correlation){

// local vars
  int xccode[2];

// inferred from abinit web site http://www.abinit.org/documentation/helpfiles/for-v6.8/input_variables/varbas.html#ixc


  // negative values are combined from 2 libxc codes
  if (pspxc < 0){
    xccode[1] = abs(pspxc) % 1000;
    xccode[0] = (int) ((abs(pspxc) - xccode[0]) / 1000);

    /// for the moment, presume exchange is first in abinit symbol
    exchange = xccode[0];
    correlation = xccode[1];

    /// we are done
    return PSPIO_SUCCESS;
  }

  // positive value of pspxc: we need to convert
  switch(pspxc){
    case 0:
      exchange = XC_NONE;
      correlation = XC_NONE;
      break;
    case 1:
      exchange = XC_LDA_XC_TETER93;
      correlation = XC_LDA_XC_TETER93;
      break;
    case 2:
      exchange = XC_LDA_X;
      correlation = XC_LDA_C_PZ_MOD;
      break;
    case 4:
      exchange = XC_LDA_X;
      correlation = XC_LDA_C_WIGNER;
      break;
    case 5:
      exchange = XC_LDA_X;
      correlation = XC_LDA_C_HL;
      break;
    case 6:
      exchange = XC_LDA_X;
      correlation = XC_LDA_C_XALPHA;
      break;
    case 7:
      exchange = XC_LDA_X;
      correlation = XC_LDA_C_PW;
      break;
    // FIXME: not in LibXC 1.0
    //case 9:
      //(*psp_data).exchange = XC_LDA_X;
      //(*psp_data).correlation = XC_LDA_C_PW_RPA;
      break;
    case 11:
      exchange = XC_GGA_X_PBE;
      correlation = XC_GGA_C_PBE;
      break;
    case 12:
      exchange = XC_GGA_X_PBE;
      correlation = XC_NONE;
      break;
    // FIXME: not in LibXC 1.0
    //case 13:
      //exchange = XC_GGA_X_LB;
      //correlation = XC_GGA_C_PW91; // this is probably incorrect - which correlation do VL Baerends use?
      break;
    case 14:
      exchange = XC_GGA_X_PBE_R;
      correlation = XC_GGA_C_PBE;
      break;
    case 15:
      exchange = XC_GGA_X_RPBE;
      correlation = XC_GGA_C_PBE; // is this correct? correlation for RPBE?
      break;
    case 16:
      exchange = XC_GGA_XC_HCTH_93;
      correlation = XC_GGA_XC_HCTH_93;
      break;
    case 17:
      exchange = XC_GGA_XC_HCTH_120;
      correlation = XC_GGA_XC_HCTH_120;
      break;
    case 23:
      exchange = XC_GGA_X_WC;
      correlation = XC_GGA_C_PBE;
      break;
    case 26:
      exchange = XC_GGA_XC_HCTH_147;
      correlation = XC_GGA_XC_HCTH_147;
      break;
    case 27:
      exchange = XC_GGA_XC_HCTH_407;
      correlation = XC_GGA_XC_HCTH_407;
      break;

// the following are not in libxc?
    case 3:
    case 8:
    case 20:
    case 21:
    case 22:
    case 24:

// the following are real errors
    case 10:
    case 18:
    case 19:
    case 25:
    case 28:

// unknown abinit pspxc
    default:
      return PSPIO_EVALUE;
  }

  return PSPIO_SUCCESS;

}
