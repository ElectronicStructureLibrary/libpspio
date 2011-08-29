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

/** @file read_upf.c
 * @brief subroutine to read in UPF files 
 */

#include <pspio.h>
#include <stdio.h>


int init_tag(int unit, char * string, int go_back);
int check_end_tag(char * tag);
int ps_upf_init(pspio_pspdata_t * ps_upf, char * filename);
int ps_upf_end(pspio_pspdata_t * ps_upf);
int ps_upf_file_read(int unit, pspio_pspdata_t * ps_upf);
int tag_isdef(int unit, char * string);

int ps_upf_cutoff_radii(pspio_pspdata_t * ps_upf);
/**
 * checks normalization of the pseudo wavefunctions
 */   
int ps_upf_check_rphi(pspio_pspdata_t * ps_upf);
