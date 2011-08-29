/** subroutine to read in abinit header which is generic to all psp files for abinit */

#include <stdio.h>
#include <string.h>
#include "pspio.h"

/**
* subroutine reads in full abinit header file and initializes psp_data structure with available stuff
*@param[in]  fname      file name to be read in 
*@param[out] psp_data   pseudopotential info is partially filled in present routine 
*@param[out] pspcod     pseudopotential file type (HGH GTH fhi etc...)
*/
int read_abinit_header (FILE *fp, psp_data_t *psp_data, int pspcod){

  /// local variables
  int ierr;
  int idum;
  int ncharead = 1000;
  int pspxc; 
  char line[ncharead];
  char *testread;

  /**< read in title */
  if(fgets(line, ncharead, fp) == NULL) return PSPIO_IOERR;
  psp_data.title = line[0:min(strlen(line),STRLEN_TITLE)-1];
 
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


  ierr = pspxc2libxc (pspxc, psp_data)
  if (ierr != 0) return ierr

  return 0;

}


