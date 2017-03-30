#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "pspio.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif


const size_t DIFF_SIZE = 4096;


int main(int argc, char **argv) {

  char *cmd = NULL;
  char *psp_inp = NULL, *psp_out = NULL;
  char psp_tmp[L_tmpnam];
  char psp_diff[DIFF_SIZE];
  int ierr, psp_fmt;
  size_t cmd_size;
  FILE *fpd;
  pspio_pspdata_t *data = NULL;

  /* Get format and input file */
  if ( argc == 3 ) {
    psp_fmt = atoi(argv[1]);
    psp_inp = argv[2];
  } else {
    fprintf(stderr, "Usage: test_format int(format) file\n");
    return 1;
  }

  printf("FMT = %d FILE = %s\n", psp_fmt, psp_inp);

  /* Prepare data structure */
  ierr = pspio_pspdata_alloc(&data);
  if ( ierr != PSPIO_SUCCESS ) {
    fprintf(stderr, "Could not allocate pseudopotential data structure\n");
    return 2;
  }

  printf("data allocated\n");

  /* Load input data */
  ierr = pspio_pspdata_read(data, psp_fmt, psp_inp);
  if ( ierr != PSPIO_SUCCESS ) {
    pspio_error_show(ierr, __FILE__, __LINE__, "__MAIN__");
    return 10;
  }

  printf("data loaded\n");

  /* Save output data */
  psp_out = tmpnam_r(psp_tmp);
  ierr = pspio_pspdata_write(data, psp_fmt, psp_tmp);
  if ( ierr != PSPIO_SUCCESS ) {
    pspio_error_show(ierr, __FILE__, __LINE__, "__MAIN__");
    return 20;
  }

  printf("data saved to %s\n", psp_tmp);

  /* Compare input and output */
  cmd_size = (12 + strlen(psp_inp) + strlen(psp_tmp)) * sizeof(char);
  cmd = (char *) malloc ((12 + strlen(psp_inp) + strlen(psp_tmp)) * sizeof(char));
  memset(cmd, 0, cmd_size);

  sprintf(cmd, "diff -urN %s %s", psp_inp, psp_tmp);
  printf("will run '%s'\n", cmd);
  fpd = popen(cmd, "r");
  fgets(psp_diff, DIFF_SIZE, fpd);
  if ( pclose(fpd) != 0 ) {
    fprintf(stderr, "Failed to close diff stream\n");
    return 30;
  }

  printf("data compared\n");

  printf("Format : %d\nFile   : %s\n>>> BEGIN DIFF <<<\n%s\n>>> END DIFF <<<\n",
    psp_fmt, psp_inp, psp_diff);

  return 0;
}
