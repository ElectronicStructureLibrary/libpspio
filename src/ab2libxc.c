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
#include "pspio.h"
#include "pspio_error.h"
#include "libxccodes.h"

#ifdef HAVE_LIBXC
#include <xc.h>
#endif

/**
* subroutine converts abinit pspxc code to libxc codes
*@param[in]  pspxc      abinit xc code
*@param[out] psp_data   pseudopotential xc info is filled with libxc codes
*/
int ab2libxc (int pspxc, psp_data_t *psp_data){

// local vars
  int xccode[2];
#ifdef HAVE_LIBXC
  int nexch;
  int nxocc;
  xc_func_type func;
#endif

// inferred from abinit web site http://www.abinit.org/documentation/helpfiles/for-v6.8/input_variables/varbas.html#ixc


  // negative values are combined from 2 libxc codes
  if (pspxc < 0){
    xccode[1] = abs(pspxc) % 1000;
    xccode[0] = int ((abs(pspxc) - xccode[0]) / 1000);

/// if we have libxc we can call 
#ifdef HAVE_LIBXC
    nexch = 0;
    ncorr = 0;

    for 
ii=0; ii<2; ii++){
      if(xc_func_init(&func, xccode[ii], XC_UNPOLARIZED) != 0){
        return PSPIO_VALUE_ERROR;
      }
      switch(func.info->kind) {
        case XC_EXCHANGE:
          nexch++;
          psp_data.exchange = xccode[ii];
          break;
        case XC_CORRELATION:
          ncorr++;
          psp_data.correlation = xccode[ii]
          break;
        default:
      }
      xc_func_end(&func);
    }

    if (nexch > 1) return PSPIO_VALUE_ERROR;
    if (ncorr > 1) return PSPIO_VALUE_ERROR;

#else
/// for the moment, presume exchange is first in abinit symbol
    psp_data.exchange = xccode[0];
    psp_data.correlation = xccode[1];
#endif
    
    /// we are done
    return 0
  }

  // positive value of pspxc: we need to convert
  switch(pspxc){
    case 0:
      psp_data.exchange = XC_NONE;
      psp_data.correlation = XC_NONE;
      break;
    case 1:
      psp_data.exchange = XC_LDA_XC_TETER93;
      psp_data.correlation = XC_LDA_XC_TETER93;
      break;
    case 2:
      psp_data.exchange = XC_LDA_X;
      psp_data.correlation = XC_LDA_C_PZ_MOD;
      break;
    case 4:
      psp_data.exchange = XC_LDA_X;
      psp_data.correlation = XC_LDA_C_WIGNER;
      break;
    case 5:
      psp_data.exchange = XC_LDA_X;
      psp_data.correlation = XC_LDA_C_HL;
      break;
    case 6:
      psp_data.exchange = XC_LDA_X;
      psp_data.correlation = XC_LDA_C_XALPHA;
      break;
    case 7:
      psp_data.exchange = XC_LDA_X;
      psp_data.correlation = XC_LDA_C_PW;
      break;
    case 9:
      psp_data.exchange = XC_LDA_X;
      psp_data.correlation = XC_LDA_C_PW_RPA;
      break;
    case 11:
      psp_data.exchange = XC_GGA_X_PBE;
      psp_data.correlation = XC_GGA_C_PBE;
      break;
    case 12:
      psp_data.exchange = XC_GGA_X_PBE;
      psp_data.correlation = XC_NONE;
      break;
    case 13:
      psp_data.exchange = XC_GGA_XC_LB;
      psp_data.correlation = XC_GGA_XC_LB;
      break;
    case 14:
      psp_data.exchange = XC_GGA_X_PBE_R;
      psp_data.correlation = XC_GGA_C_PBE;
      break;
    case 15:
      psp_data.exchange = XC_GGA_X_RPBE;
      psp_data.correlation = XC_GGA_C_PBE; // is this correct?
      break;
    case 16:
      psp_data.exchange = XC_GGA_XC_HCTH_93;
      psp_data.correlation = XC_GGA_XC_HCTH_93;
      break;
    case 17:
      psp_data.exchange = XC_GGA_XC_HCTH_120;
      psp_data.correlation = XC_GGA_XC_HCTH_120;
      break;
    case 23:
      psp_data.exchange = XC_GGA_X_WC;
      psp_data.correlation = XC_GGA_C_PBE;
      break;
    case 26:
      psp_data.exchange = XC_GGA_XC_HCTH_147;
      psp_data.correlation = XC_GGA_XC_HCTH_147;
      break;
    case 27:
      psp_data.exchange = XC_GGA_XC_HCTH_407;
      psp_data.correlation = XC_GGA_XC_HCTH_407;
      break;

// the following are not in libxc?
    case 3:
    case 8:
    case 20:
    case 22:
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
      return PSPIO_VALUE_ERROR;
  }

  return 0

}
