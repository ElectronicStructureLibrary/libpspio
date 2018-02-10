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


#include <stdio.h>
#include <string.h>

#include "pspio_error.h"
#include "pspio_xc.h"

#if defined HAVE_CONFIG_H
#include "config.h"
#endif

int upf_to_libxc (const char xc_string[20], int *exchange, int *correlation)
{
  char exch[]  = "     ";
  char corr[]  = "     ";
  char gradx[] = "     ";
  char gradc[] = "     ";

  if (sscanf(xc_string, "%s %s %s %s", exch, corr, gradx, gradc) == 4) {
    /* All the following codes have been inferred from the pwscf source */

    /* Exchange: */
    if (strncmp(exch, "NOX", 3) == 0) {
      *exchange = XC_NONE;
    } else if (strncmp(exch, "SLA", 3) == 0) {
      if (strncmp(gradx, "NOGX", 4) == 0) {
        *exchange = XC_LDA_X;
      } else if (strncmp(gradx, "B88", 3) == 0) {
        *exchange = XC_GGA_X_B88;
      } else if (strncmp(gradx, "GGX", 3) == 0) {
	      *exchange = XC_GGA_X_PW91;
      } else if (strncmp(gradx, "PBX", 3) == 0) {
	      *exchange = XC_GGA_X_PBE;
      } else if (strncmp(gradx, "PSX", 3) == 0) {
	      *exchange = XC_GGA_X_PBE_SOL;
      } else if (strncmp(gradx, "RPB", 3) == 0) {
	      *exchange = XC_GGA_X_PBE_R;
      } else if (strncmp(gradx, "WCX", 3) == 0) {
	      *exchange = XC_GGA_X_WC;
      } else if (strncmp(gradx, "TPSS", 4) == 0) {
	      *exchange = XC_MGGA_X_TPSS;
      } else {
	      return PSPIO_EVALUE;
      }
    } else {
      return PSPIO_EVALUE;
    }

    /* Correlation: */
    if (strncmp(corr, "NOC", 3) == 0) {
      *correlation = XC_NONE;
    } else if ( (strncmp(corr, "PZ",  2) == 0) && (strncmp(gradc, "NOGC", 4) == 0) ) {
      *correlation = XC_LDA_C_PZ;
    } else if ( (strncmp(corr, "VWN", 3) == 0) && (strncmp(gradc, "NOGC", 4) == 0) ) {      
      *correlation = XC_LDA_C_VWN;
    } else if ( (strncmp(corr, "PW",  2) == 0) && (strncmp(gradc, "NOGC", 4) == 0) ) {      
      *correlation = XC_LDA_C_PW;
    } else if ( (strncmp(corr, "WIG", 2) == 0) && (strncmp(gradc, "NOGC", 4) == 0) ) {      
      *correlation = XC_LDA_C_WIGNER;
    } else if ( (strncmp(corr, "HL",  2) == 0) && (strncmp(gradc, "NOGC", 4) == 0) ) {      
      *correlation = XC_LDA_C_HL;
    } else if ( (strncmp(corr, "OBZ", 3) == 0) && (strncmp(gradc, "NOGC", 4) == 0) ) {      
      *correlation = XC_LDA_C_OB_PZ;
    } else if ( (strncmp(corr, "OBW", 3) == 0) && (strncmp(gradc, "NOGC", 4) == 0) ) {      
      *correlation = XC_LDA_C_OB_PW;
    } else if ( (strncmp(corr, "GL",  2) == 0) && (strncmp(gradc, "NOGC", 4) == 0) ) {      
      *correlation = XC_LDA_C_GL;
    } else if ( (strncmp(corr, "PW",  2) == 0) && (strncmp(gradc, "GGC",  4) == 0) ) {      
      *correlation = XC_GGA_C_PW91;
    } else if ( (strncmp(corr, "PW",  2) == 0) && (strncmp(gradc, "PBC",  4) == 0) ) {      
      *correlation = XC_GGA_C_PBE;
    } else if ( (strncmp(corr, "PZ",  2) == 0) && (strncmp(gradc, "P86",  4) == 0) ) {      
      *correlation = XC_GGA_C_P86;
    } else if ( (strncmp(corr, "PW",  2) == 0) && (strncmp(gradc, "PSC",  4) == 0) ) {      
      *correlation = XC_GGA_C_PBE_SOL;
    } else if ( (strncmp(corr, "PW",  2) == 0) && (strncmp(gradc, "TPSS", 4) == 0) ) {      
      *correlation = XC_MGGA_C_TPSS;
    } else {
      return PSPIO_EVALUE;
    }

  } else {
    return PSPIO_EVALUE;
  }

  return PSPIO_SUCCESS;
}

int libxc_to_upf (int exchange, int correlation, char longname[21],
		  char shortname[5])
{
  char exch[5], corr[4], gradx[6], gradc[5];

  /* Exchange: */
  if (exchange == 0) {
    strcpy(exch, "NOX ");
    strcpy(gradx,"NOGX ");
  } else if (exchange == XC_LDA_X) {
    strcpy(exch, "SLA ");
    strcpy(gradx,"NOGX ");
  } else if (exchange == XC_GGA_X_B88) {
    strcpy(exch, "SLA ");
    strcpy(gradx,"B88  ");
  } else if (exchange == XC_GGA_X_PW91) {
    strcpy(exch, "SLA ");
    strcpy(gradx,"GGX  ");
  } else if (exchange == XC_GGA_X_PBE) {
    strcpy(exch, "SLA ");
    strcpy(gradx,"PBX  ");
  } else if (exchange == XC_GGA_X_PBE_SOL) {
    strcpy(exch, "SLA ");
    strcpy(gradx,"PSX  ");
  } else if (exchange == XC_GGA_X_PBE_R) {
    strcpy(exch, "SLA ");
    strcpy(gradx,"RPB  ");
  } else if (exchange == XC_GGA_X_WC) {
    strcpy(exch, "SLA ");
    strcpy(gradx,"WCX  ");
  } else if (exchange == XC_MGGA_X_TPSS) {
    strcpy(exch, "SLA ");
    strcpy(gradx,"TPSS ");
  } else {
    strcpy(exch, "");
    strcpy(gradx,"");
  }

  /* Correlation: */
  if (correlation == 0) {
    strcpy(corr, "NOC");
    strcpy(gradc,"NOGC");
  } else if (correlation == XC_LDA_C_PZ) {
    strcpy(corr, "PZ ");
    strcpy(gradc,"NOGC");
  } else if (correlation == XC_LDA_C_VWN) {
    strcpy(corr, "VWN");
    strcpy(gradc,"NOGC");
  } else if (correlation == XC_LDA_C_PW) {
    strcpy(corr, "PW ");
    strcpy(gradc,"NOGC");
  } else if (correlation == XC_LDA_C_WIGNER) {
    strcpy(corr, "WIG");
    strcpy(gradc,"NOGC");
  } else if (correlation == XC_LDA_C_HL) {
    strcpy(corr, "HL ");
    strcpy(gradc,"NOGC");
  } else if (correlation == XC_LDA_C_OB_PZ) {
    strcpy(corr, "OPZ");
    strcpy(gradc,"NOGC");
  } else if (correlation == XC_LDA_C_OB_PW) {
    strcpy(corr, "OPW");
    strcpy(gradc,"NOGC");
  } else if (correlation == XC_LDA_C_GL) {
    strcpy(corr, "GL ");
    strcpy(gradc,"NOGC");
  } else if (correlation == XC_GGA_C_PW91) {
    strcpy(corr, "PW ");
    strcpy(gradc,"GGC ");
  } else if (correlation == XC_GGA_C_PBE) {
    strcpy(corr, "PW ");
    strcpy(gradc,"PBC ");
  } else if (correlation == XC_GGA_C_P86) {
    strcpy(corr, "PZ ");
    strcpy(gradc,"P86 ");
  } else if (correlation == XC_GGA_C_PBE_SOL) {
    strcpy(corr, "PW ");
    strcpy(gradc,"PSC ");
  } else if (correlation == XC_MGGA_C_TPSS) {
    strcpy(corr, "PW ");
    strcpy(gradc,"TPSS");
  } else {
    strcpy(corr, "");
    strcpy(gradc,"");
  }
  
  /* Create longname */
  sprintf(longname, "%4s %3s %4s %4s ", exch, corr, gradx, gradc);

  /* Create shortname */
  strcpy(shortname,"    ");
  if (exchange == XC_LDA_X) {
    strcpy(shortname, corr);
  } else if (exchange == XC_GGA_X_PBE && correlation == XC_GGA_C_PBE) {
    strcpy(shortname,"PBE");
  } else if (exchange == XC_GGA_X_B88 && correlation == XC_LDA_C_PZ) {
    strcpy(shortname,"B88");
  } else if (exchange == XC_GGA_X_B88 && correlation == XC_GGA_C_P86) {
    strcpy(shortname,"BP");
  } else if (exchange == XC_GGA_X_WC && correlation == XC_GGA_C_PBE) {
    strcpy(shortname,"WC");
  } else if (exchange == XC_MGGA_X_TPSS && correlation == XC_MGGA_C_TPSS) {
    strcpy(shortname,"TPSS");
  } else {
    strcpy(shortname,"");
  }

  return PSPIO_SUCCESS;
}
