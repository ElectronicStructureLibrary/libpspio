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

 $Id: ab2libxc.c 237 2011-09-06 13:41:55Z micael $
*/


#include <stdio.h>
#include <string.h>

#include "pspio_error.h"
#include "libxccodes.h"

#if defined HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_LIBXC
#  include <xc.h>
#  include <xc_funcs.h>
#else
#  include "xc_funcs_pspio.h"
#endif

/**
* subroutine converts pwcf xc string to libxc codes
*@param[in]  xc_string: pwscf string
*@param[out] exchange: libxc code for exchange
*@param[out] correlation: libxc code for correlation
*/
int pwscf2libxc (const char *xc_string, int exchange, int correlation){

// inferred from ???

  if ( strncmp(xc_string, "SLA  PZ  NOGX  NOGC", 19) ) {
    exchange = XC_LDA_X;
    correlation = XC_LDA_C_PZ;

  } else if ( strncmp(xc_string, "SLA  PW  NOGX  NOGC", 19) ) {
    exchange = XC_LDA_X;
    correlation = XC_LDA_C_PW;
  } else if ( strncmp(xc_string, "SLA  PW  PBX  PBC", 17) ) {
    exchange = XC_GGA_X_PBE;
    correlation = XC_GGA_C_PBE;
  } else {
    // unknown xc_string
    return PSPIO_EVALUE;
  }

  return PSPIO_SUCCESS;
}
