/** subroutine to read in abinit header which is generic to all psp files for abinit */

#include <stdio.h>
#include <string.h>

#include "pspio.h"
#include "libxccodes.h"

/**
* subroutine reads in full abinit header file and initializes psp_data structure with available stuff
*@param[in]  fname  file name to be read in 
*@param[out] psp_data  output: pseudopotential info is partially filled in present routine 
*/
int read_abinit_header (char *fname, psp_data_t *psp_data, int pspcod){

 /// local variables
  FILE* fp;
  int ierr;
  int idum;
  int ncharead = 1000;
  int pspxc; 
  char line[ncharead];
  char *testread;

 /// open file
  fp = fopen(fname, "r");
  if(fp == NULL) return PSPIO_NOFILE;

  /**< read in title */
  if(fgets(line, ncharead, fp) == NULL) return PSPIO_IOERR;
  psp_data.title = (*char) malloc (strlen(line));
  psp_data.title = line[0:strlen(line)-1];
 
  /**< read in atomic number, pseudopotential ion charge (= num of valence electrons), and abinit data flag (not used) */
  if(fgets(line, ncharead, fp) == NULL) return PSPIO_IOERR;
  narg = sscanf (line, "%f %f %d", &psp_data.z, &psp_data.zvalence, &idum);
  ///check narg is equal to 2
 
  /**< read in psp code and xc code*/
  if(fgets(line, ncharead, fp) == NULL) return PSPIO_IOERR;
  narg = sscanf (line, "%d %d %d %d %d", &pspcod, &pspxc, &psp_data.lmax, &idum, &psp_data.mesh.np);
  ///check narg is equal to 5

// inferred from abinit web site http://www.abinit.org/documentation/helpfiles/for-v6.8/users/abinit_help.html#5
/* NOTE: pspcod = 5 is used also for new psp generated with the Martins code, including 2 projectors for spin orbit coupling */ 
  switch(pspcod){
    case 1:
    case 4:
    case 5:
    case 6:
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
  switch(pspxc){
    case(0):
      psp_data.exchange = XC_NONE;
      psp_data.correlation = XC_NONE;
      break;
    case(1):
      psp_data.exchange = XC_LDA_XC_TETER93;
      psp_data.correlation = XC_LDA_XC_TETER93;
      break;
    case(2):
      psp_data.exchange = XC_LDA_X;
      psp_data.correlation = XC_LDA_C_PZ_MOD;
      break;
    case(4):
      psp_data.exchange = XC_LDA_X;
      psp_data.correlation = XC_LDA_C_WIGNER;
      break;
    case(5):
      psp_data.exchange = XC_LDA_X;
      psp_data.correlation = XC_LDA_C_HL;
      break;
    case(6):
      psp_data.exchange = XC_LDA_X;
      psp_data.correlation = XC_LDA_C_XALPHA;
      break;
    case(7):
      psp_data.exchange = XC_LDA_X;
      psp_data.correlation = XC_LDA_C_PW;
      break;
    case(9):
      psp_data.exchange = XC_LDA_X;
      psp_data.correlation = XC_LDA_C_PW_RPA;
      break;
    case(11):
      psp_data.exchange = XC_GGA_X_PBE;
      psp_data.correlation = XC_GGA_C_PBE;
      break;
    case(12):
      psp_data.exchange = XC_GGA_X_PBE;
      psp_data.correlation = XC_NONE;
      break;
    case(13):
      psp_data.exchange = XC_GGA_XC_LB;
      psp_data.correlation = XC_GGA_XC_LB;
      break;
    case(14):
      psp_data.exchange = XC_GGA_X_PBE_R;
      psp_data.correlation = XC_GGA_C_PBE;
      break;
    case(15):
      psp_data.exchange = XC_GGA_X_RPBE;
      psp_data.correlation = XC_GGA_C_PBE; // is this correct?
      break;
    case(16):
      psp_data.exchange = XC_GGA_XC_HCTH_93;
      psp_data.correlation = XC_GGA_XC_HCTH_93;
      break;
    case(17):
      psp_data.exchange = XC_GGA_XC_HCTH_120;
      psp_data.correlation = XC_GGA_XC_HCTH_120;
      break;
    case(23):
      psp_data.exchange = XC_GGA_X_WC;
      psp_data.correlation = XC_GGA_C_PBE;
      break;
    case(26):
      psp_data.exchange = XC_GGA_XC_HCTH_147;
      psp_data.correlation = XC_GGA_XC_HCTH_147;
      break;
    case(27):
      psp_data.exchange = XC_GGA_XC_HCTH_407;
      psp_data.correlation = XC_GGA_XC_HCTH_407;
      break;

// the following are not in libxc?
    case(3):
    case(8):
    case(20):
    case(22):
    case(22):
    case(24):

// the following are real errors
    case(10):
    case(18):
    case(19):
    case(25):
    case(28):

// unknown abinit code
    default:
      return PSPIO_VALUE_ERROR;
  }

 /// close file
  ierr = close(fp);
 ///check for ierr being non 0


return 0

}
