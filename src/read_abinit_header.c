/**< subroutine to read in abinit header which is generic to all psp files for abinit */

#include <pspio.h>
#include <stdio.h>


     read (tmp_unit, '(a)' )pspheads(ipsp)%title
     read (tmp_unit,*)pspheads(ipsp)%znuclpsp,pspheads(ipsp)%zionpsp,pspheads(ipsp)%pspdat
     read (tmp_unit,*)pspheads(ipsp)%pspcod,pspheads(ipsp)%pspxc,&
&     pspheads(ipsp)%lmax,idum,mmax

int read_abinit_header (){

psp_data 


}
