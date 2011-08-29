/** subroutine to read in abinit header which is generic to all psp files for abinit */

#include <stdio.h>
#include <pspio.h>
#include <string.h>
#include "libxccodes.h"

     read (tmp_unit, '(a)' )pspheads(ipsp)%title
     read (tmp_unit,*)pspheads(ipsp)%znuclpsp,pspheads(ipsp)%zionpsp,pspheads(ipsp)%pspdat
     read (tmp_unit,*)pspheads(ipsp)%pspcod,pspheads(ipsp)%pspxc,pspheads(ipsp)%lmax,idum,mmax

int read_abinit_header (fname, psp_data){

 /// input variables
  char *fname; /**< file name to be read in */
  psp_data_t psp_data; /**< output: pseudopotential info is partially filled in present routine */

 /// local variables
  FILE* fp;            /**< file pointer for reading in */
  int ierr;
  int ncharead = 1000;
  int pspcod;
  int pspxc; 
  char line[ncharead];

 /// open file
  fp = fopen(fname, "r");
  if(fp == NULL) return PSPIO_NOFILE;

  /**< read in title */
  getline(&line, &ncharead, fp);
  if(line == NULL) return PSPIO_IOERR;
  psp_data.title = (*char) malloc (strlen(line));
  psp_data.title = line[0:strlen(line)-1];
 
  /**< read in atomic number, pseudopotential ion charge (= num of valence electrons), and abinit data flag (not used) */
  getline(&line, &ncharead, fp);
  narg = sscanf (line, "%f %f", &psp_data.z, &psp_data.zvalence);
  ///check narg is equal to 2
 
  /**< read in psp code and xc code*/
  getline(&line, &ncharead, fp);
  narg = sscanf (line, "%d %d", &pspcod, &pspxc);
  ///check narg is equal to 2

// inferred from abinit web site http://www.abinit.org/documentation/helpfiles/for-v6.8/users/abinit_help.html#5
/* NOTE: pspcod = 5 is used also for new psp generated with the Martins code, including 2 projectors for spin orbit coupling */ 
  switch(pspcod){
    case 1:
      psp_data.scheme = TM2;
      break;
    case 2:
      psp_data.scheme = GTH;
      break;
    case 3:
      psp_data.scheme = HGH;
      break;
    default:
      return PSPIO_VALUE_ERROR;
  }

// inferred from abinit web site http://www.abinit.org/documentation/helpfiles/for-v6.8/input_variables/varbas.html#ixc
/*
    0=> NO xc;

    1=> LDA or LSD, Teter Pade parametrization (4/93, published in S. Goedecker, M. Teter, J. Huetter, Phys.Rev.B54, 1703 (1996)), which reproduces Perdew-Wang (which reproduces Ceperley-Alder!).
    2=> LDA, Perdew-Zunger-Ceperley-Alder (no spin-polarization)
    3=> LDA, old Teter rational polynomial parametrization (4/91) fit to Ceperley-Alder data (no spin-polarization)
    4=> LDA, Wigner functional (no spin-polarization)
    5=> LDA, Hedin-Lundqvist functional (no spin-polarization)
    6=> LDA, "X-alpha" functional (no spin-polarization)
    7=> LDA or LSD, Perdew-Wang 92 functional
    8=> LDA or LSD, x-only part of the Perdew-Wang 92 functional
    9=> LDA or LSD, x- and RPA correlation part of the Perdew-Wang 92 functional

    11=> GGA, Perdew-Burke-Ernzerhof GGA functional
    12=> GGA, x-only part of Perdew-Burke-Ernzerhof GGA functional
    13=> GGA potential of van Leeuwen-Baerends, while for energy, Perdew-Wang 92 functional
    14=> GGA, revPBE of Y. Zhang and W. Yang, Phys. Rev. Lett. 80, 890 (1998)
    15=> GGA, RPBE of B. Hammer, L.B. Hansen and J.K. Norskov, Phys. Rev. B 59, 7413 (1999)
    16=> GGA, HTCH93 of F.A. Hamprecht, A.J. Cohen, D.J. Tozer, N.C. Handy, J. Chem. Phys. 109, 6264 (1998)
    17=> GGA, HTCH120 of A.D. Boese, N.L. Doltsinis, N.C. Handy, and M. Sprik, J. Chem. Phys 112, 1670 (1998) - The usual HCTH functional.
    18=> (NOT AVAILABLE : used internally for GGA BLYP pseudopotentials from M. Krack, see Theor. Chem. Acc. 114, 145 (2005), available from the CP2K repository - use the LibXC instead, with ixc=-106131.
    19=> (NOT AVAILABLE : used internally for GGA BP86 pseudopotentials from M. Krack, see Theor. Chem. Acc. 114, 145 (2005), available from the CP2K repository - use the LibXC instead, with ixc=-106132.

    20=> Fermi-Amaldi xc ( -1/N Hartree energy, where N is the number of electrons per cell ; G=0 is not taken into account however), for TDDFT tests. No spin-pol. Does not work for RF.
    21=> same as 20, except that the xc-kernel is the LDA (ixc=1) one, for TDDFT tests.
    22=> same as 20, except that the xc-kernel is the Burke-Petersilka-Gross hybrid, for TDDFT tests.
    23=> GGA of Z. Wu and R.E. Cohen, Phys. Rev. 73, 235116 (2006).
    24=> GGA, C09x exchange of V. R. Cooper, PRB 81, 161104(R) (2010).
    26=> GGA, HTCH147 of A.D. Boese, N.L. Doltsinis, N.C. Handy, and M. Sprik, J. Chem. Phys 112, 1670 (1998).
    27=> GGA, HTCH407 of A.D. Boese, and N.C. Handy, J. Chem. Phys 114, 5497 (2001).
    28=> (NOT AVAILABLE : used internally for GGA OLYP pseudopotentials from M. Krack, see Theor. Chem. Acc. 114, 145 (2005), available from the CP2K repository - use the LibXC instead, with ixc=-110131.
*/
  switch(pspxc){
    case(0):
      break;
    case(1):
      break;
    case(2):
      break;
    case(3):
      break;
    case(4):
      break;
    case(5):
      break;
    case(6):
      break;
    case(7):
      break;
    case(8):
      break;
    case(9):
      break;
    case(11):
      break;
    case(12):
      break;
    case(13):
      break;
    case(14):
      break;
    case(15):
      break;
    case(16):
      break;
    case(17):
      break;
    case(18):
      break;
    case(19):
      break;
    case(20):
      break;
    case(21):
      break;
    case(22):
      break;
    case(23):
      break;
    case(24):
      break;
    case(25):
      break;
    case(26):
      break;
    case(27):
      psp_data. = XC_GGA_XC_HCTH_147;
      break;

    case(28):
    case(10):
    default:
      return PSPIO_VALUE_ERROR;
  }

 /// close file
  ierr = close(fp);
 ///check for ierr being non 0


return 0

}
