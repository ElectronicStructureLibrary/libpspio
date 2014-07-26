/*
 Copyright (C) 2011-2012 J. Alberdi, M. Oliveira, Y. Pouillon, and M. Verstraete

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

*/

#include <string.h>
#include <assert.h>

#include "util.h"
#include "pspio_error.h"

#if defined HAVE_CONFIG_H
#include "config.h"
#endif


const char *symbols[] = {"H  ","He ","Li ","Be ","B  ","C  ","N  ","O  ","F  ","Ne ","Na ", 
			"Mg ","Al ","Si ","P  ","S  ","Cl ","Ar ","K  ","Ca ","Sc ","Ti ", 
			"V  ","Cr ","Mn ","Fe ","Co ","Ni ","Cu ","Zn ","Ga ","Ge ","As ", 
			"Se ","Br ","Kr ","Rb ","Sr ","Y  ","Zr ","Nb ","Mo ","Tc ","Ru ", 
			"Rh ","Pd ","Ag ","Cd ","In ","Sn ","Sb ","Te ","I  ","Xe ","Cs ", 
			"Ba ","La ","Ce ","Pr ","Nd ","Pm ","Sm ","Eu ","Gd ","Tb ","Dy ", 
			"Ho ","Er ","Tm ","Yb ","Lu ","Hf ","Ta ","W  ","Re ","Os ","Ir ", 
			"Pt ","Au ","Hg ","Tl ","Pb ","Bi ","Po ","At ","Rn ","Fr ","Ra ", 
			"Ac ","Th ","Pa ","U  ","Np ","Pu ","Am ","Cm ","Bk ","Cf ","Es ", 
			"Fm ","Md ","No ","Lr ","Rf ","Db ","Sg ","Bh ","Hs ","Mt ","Uun", 
			"Uuu","Uub"};


int symbol_to_z(const char *symbol, double *z) {
  int i;

  assert(symbol != NULL);

  for (i=0; i<112; i++) {
    if (strncmp(symbol, symbols[i], 2)) {
      *z = i + 1.0;
      return PSPIO_SUCCESS;
    }
  }

  return PSPIO_ERROR;
}


int z_to_symbol(const double z, char *symbol) {
  FULFILL_OR_RETURN(z < 113.0 && z > 0.0, PSPIO_EVALUE);

  strncpy(symbol, symbols[(int)z-1], 3);

  return PSPIO_SUCCESS;
}
  

double linear_extrapolation(const double x1, const double x2,
         const double f1, const double f2, const double x) {
  double mm, f;

  mm = (f2 - f1) / (x2 - x1);
  f = f1 + mm * (x - x1);

  return f;
}
