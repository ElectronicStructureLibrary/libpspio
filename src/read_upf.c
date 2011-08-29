/** @file read_upf.c
 * @brief subroutine to read in UPF files 
 */

#include <pspio.h>
#include <stdio.h>


int init_tag(int unit, char * string, int go_back);
int check_end_tag(char * tag);
int ps_upf_init(psp_data_t * ps_upf, char * filename);
int ps_upf_end(psp_data_t * ps_upf);
int ps_upf_file_read(int unit, psp_data_t * ps_upf);
int tag_isdef(int unit, char * string);

int ps_upf_cutoff_radii(psp_data_t * ps_upf);
/**
 * checks normalization of the pseudo wavefunctions
 */   
int ps_upf_check_rphi(psp_data_t * ps_upf);
