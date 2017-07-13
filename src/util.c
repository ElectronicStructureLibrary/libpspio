/* Copyright (C) 2011-2016 Micael Oliveira <micael.oliveira@mpsd.mpg.de>
 *                         Yann Pouillon <notifications@materialsevolution.es>
 *
 * This file is part of Libpspio.
 *
 * Libpspio is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, version 3 of the License, or (at your option) any later
 * version.
 *
 * Libpspio is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Libpspio.  If not, see <http://www.gnu.org/licenses/> or write to
 * the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301  USA.
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


int symbol_to_z(const char *symbol, double *z)
{
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


int z_to_symbol(double z, char *symbol)
{
  FULFILL_OR_RETURN(z < 113.0 && z > 0.0, PSPIO_EVALUE);

  strncpy(symbol, symbols[(int)z-1], 3);

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
      return "Bachelet-Hamann-Schluter scheme";
    case PSPIO_SCM_GTH:
      return "Goedecker-Teter-Hutter scheme";
    case PSPIO_SCM_HAMANN:
      return "Hamann scheme";
    case PSPIO_SCM_HGH:
      return "Hartwigsen-Goedecker-Hutter scheme";
    case PSPIO_SCM_HSC:
      return "Hamann-Schluter-Chiang scheme";
    case PSPIO_SCM_KERKER:
      return "Kerker scheme";
    case PSPIO_SCM_MRPP:
      return "Multi-reference TM scheme";
    case PSPIO_SCM_ONCV:
      return "Hamann ONCV scheme";
    case PSPIO_SCM_RRKJ:
      return "Rappe-Rabe-Kaxiras-Joannopoulos scheme";
    case PSPIO_SCM_TM:
      return "Troullier-Martins scheme";
    case PSPIO_SCM_TM2:
      return "Troullier-Martins 2 scheme";
    case PSPIO_SCM_RTM:
      return "Relativistic Troullier-Martins scheme";
    case PSPIO_SCM_UNKNOWN:
    default:
      return "Unknown pseudopotential generation scheme";
  }
}

int strcmp_nullok(char *s1, char *s2)
{
  if ( (s1 == NULL) && (s2 == NULL) ) {
    return strcmp("A", "A");
  }

  if ( (s1 == NULL) || (s2 == NULL) ) {
    return strcmp("A", "B");
  }

  return strcmp(s1, s2);
}
