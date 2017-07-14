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
  char *psp_rd = NULL, *psp_wr = NULL, *my_fmt = NULL;
  char psp_diff[DIFF_SIZE+1];
  int ierr, psp_fmt;
  size_t cmd_size;
  pspio_pspdata_t *data = NULL;

  /* Get format and input file */
  if ( argc == 3 ) {
    my_fmt = argv[1];
    psp_rd = argv[2];
  } else {
    fprintf(stderr, "Usage: test_format int(format) file\n");
    return 1;
  }

  /* Convert format */
  if ( strcmp(my_fmt, "abinit1") == 0 ) {
      psp_fmt = PSPIO_FMT_ABINIT_1;
  } else if ( strcmp(my_fmt, "abinit2") == 0 ) {
      psp_fmt = PSPIO_FMT_ABINIT_2;
  } else if ( strcmp(my_fmt, "abinit3") == 0 ) {
      psp_fmt = PSPIO_FMT_ABINIT_3;
  } else if ( strcmp(my_fmt, "abinit4") == 0 ) {
      psp_fmt = PSPIO_FMT_ABINIT_4;
  } else if ( strcmp(my_fmt, "abinit5") == 0 ) {
      psp_fmt = PSPIO_FMT_ABINIT_5;
  } else if ( strcmp(my_fmt, "abinit6") == 0 ) {
      psp_fmt = PSPIO_FMT_ABINIT_6;
  } else if ( strcmp(my_fmt, "abinit7") == 0 ) {
      psp_fmt = PSPIO_FMT_ABINIT_7;
  } else if ( strcmp(my_fmt, "abinit8") == 0 ) {
      psp_fmt = PSPIO_FMT_ABINIT_8;
  } else if ( strcmp(my_fmt, "abinit9") == 0 ) {
      psp_fmt = PSPIO_FMT_ABINIT_9;
  } else if ( strcmp(my_fmt, "abinit10") == 0 ) {
      psp_fmt = PSPIO_FMT_ABINIT_10;
  } else if ( strcmp(my_fmt, "abinit11") == 0 ) {
      psp_fmt = PSPIO_FMT_ABINIT_11;
  } else if ( strcmp(my_fmt, "abinit17") == 0 ) {
      psp_fmt = PSPIO_FMT_ABINIT_17;
  } else if ( strcmp(my_fmt, "atom") == 0 ) {
      psp_fmt = PSPIO_FMT_ATOM;
  } else if ( strcmp(my_fmt, "fhi98pp") == 0 ) {
      psp_fmt = PSPIO_FMT_FHI98PP;
  } else if ( strcmp(my_fmt, "octopus_hgh") == 0 ) {
      psp_fmt = PSPIO_FMT_OCTOPUS_HGH;
  } else if ( strcmp(my_fmt, "siesta") == 0 ) {
      psp_fmt = PSPIO_FMT_SIESTA;
  } else if ( strcmp(my_fmt, "upf") == 0 ) {
      psp_fmt = PSPIO_FMT_UPF;
  } else if ( strcmp(my_fmt, "xml") == 0 ) {
      psp_fmt = PSPIO_FMT_XML;
  } else {
    pspio_error_add(PSPIO_EFILE_FORMAT, __FILE__, __LINE__, "main");
    pspio_error_flush(stderr);
    return 2;
  }

  /* Prepare data structure */
  ierr = pspio_pspdata_alloc(&data);
  if ( ierr != PSPIO_SUCCESS ) {
    pspio_error_flush(stderr);
    return 3;
  }

  /* Load input data */
  ierr = pspio_pspdata_read(data, psp_fmt, psp_rd);
  if ( ierr != PSPIO_SUCCESS ) {
    pspio_error_flush(stderr);
    return 10;
  }
  fflush(stderr);

  /* Save output data */
  psp_wr = (char *) malloc (13 * sizeof(char));
  sprintf(psp_wr, "pspio-test-%6.6d", getpid());
  ierr = pspio_pspdata_write(data, psp_fmt, psp_wr);
  if ( ierr != PSPIO_SUCCESS ) {
    pspio_error_flush(stderr);
    return 20;
  }
  fflush(stderr);

  /* Compare input and output */
  cmd_size = (18 + strlen(psp_rd) + strlen(psp_wr)) * sizeof(char);
  cmd = (char *) malloc (cmd_size);
  memset(cmd, 0, cmd_size);
  memset(psp_diff, 0, DIFF_SIZE+1);
  sprintf(cmd, "diff -urN %s %s 1>&2", psp_rd, psp_wr);

  fprintf(stderr,
    "Format : %d\nFile   : %s\n>>> BEGIN DIFF <<<\n", psp_fmt, psp_rd);
  system(cmd);
  fprintf(stderr, ">>> END DIFF <<<\n");

  return 0;
}
