/*
 Copyright (C) 2011 J. Alberdi, M. Oliveira, Y. Pouillon, and M. Verstraete

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2, or (at your option)
 any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 02111-1307, USA.

 $Id: pspio.h 27 2011-08-29 15:32:25Z micael $
*/

#include "pspio_mesh.h"
#include <stdlib.h>

int pspio_mesh_set_type(pspio_mesh_t *m, int type){
  
  m->type = type;  

  return(PSPIO_SUCCESS);
}

int pspio_mesh_set_parameters(pspio_mesh_t *m, const double a, const double b){

  m->a = a;
  m->b = b;

  return(PSPIO_SUCCESS);
}

int pspio_mesh_set_points(pspio_mesh_t *m, const int np, double r[]){
  int i;

  m->np = np;
  m->r = (double *)malloc(np*sizeof(double));
  
  for(i=0; i<np; i++) m->r[i] = r[i];

  return(PSPIO_SUCCESS);
}

