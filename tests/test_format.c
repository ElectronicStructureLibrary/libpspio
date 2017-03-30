#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "pspio.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif


const size_t DIFF_SIZE = 4095;


int main(int argc, char **argv) {

  char *cmd = NULL;
  char *psp_rd = NULL, *psp_wr = NULL;
  char psp_diff[DIFF_SIZE+1];
  int ierr, psp_fmt;
  size_t cmd_size;
  FILE *fp;
  pspio_pspdata_t *data = NULL;

  /* Get format and input file */
  if ( argc == 3 ) {
    psp_fmt = atoi(argv[1]);
    psp_rd = argv[2];
  } else {
    fprintf(stderr, "Usage: test_format int(format) file\n");
    return 1;
  }

  /* Prepare data structure */
  ierr = pspio_pspdata_alloc(&data);
  if ( ierr != PSPIO_SUCCESS ) {
    fprintf(stderr, "Could not allocate pseudopotential data structure\n");
    return 2;
  }

  /* Load input data */
  ierr = pspio_pspdata_read(data, psp_fmt, psp_rd);
  if ( ierr != PSPIO_SUCCESS ) {
    pspio_error_show(ierr, __FILE__, __LINE__, "__MAIN__");
    return 10;
  }

  /* Save output data */
  psp_wr = (char *) malloc (13 * sizeof(char));
  sprintf(psp_wr, "pspio-%6.6d", getpid());
  ierr = pspio_pspdata_write(data, psp_fmt, psp_wr);
  if ( ierr != PSPIO_SUCCESS ) {
    pspio_error_show(ierr, __FILE__, __LINE__, "__MAIN__");
    return 20;
  }

  /* Compare input and output */
  cmd_size = (18 + strlen(psp_rd) + strlen(psp_wr)) * sizeof(char);
  cmd = (char *) malloc ((12 + strlen(psp_rd) + strlen(psp_wr)) * sizeof(char));
  memset(cmd, 0, cmd_size);
  memset(psp_diff, 0, DIFF_SIZE+1);
  sprintf(cmd, "diff -urN %s %s 1>&2", psp_rd, psp_wr);

  fprintf(stderr,
    "Format : %d\nFile   : %s\n>>> BEGIN DIFF <<<\n", psp_fmt, psp_rd);
  system(cmd);
  fprintf(stderr, ">>> END DIFF <<<\n");

  return 0;
}
