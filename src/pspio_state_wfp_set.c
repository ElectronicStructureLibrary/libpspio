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

 $Id: $
*/

#include <stdio.h>
#include <string.h>
#include "pspio_error.h"
#include "pspio_state.h"

int pspio_state_wfp_set(pspio_state_t state, const double *wfp) {
  int wfp_size;

  if ( state.wfp != NULL ) {
    return PSPIO_ERROR;
  }

  wfp_size = sizeof(wfp);

  state.wfp = (double *)malloc(wfp_size);
  if ( state.wfp == NULL ) {
    return PSPIO_ERROR;
  } else {
    memcpy(state.wfp,wfp,wfp_size);
  }

  return PSPIO_SUCCESS;
}
