#include <stdlib.h>
#include <stdio.h>

#include "pspio_error.h"

#if defined HAVE_CONFIG_H
#include "config.h"
#endif


void dump_error_len(void) {
  int n = 0;
  pspio_error_t *cursor = pspio_error_chain;

  while ( cursor != NULL ) {
    n++;
    cursor = cursor->next;
  }

  printf("test_error: error chain has length %d\n", n);
}


int main(void) {
  int eid;

  /* check empty error chain */
  printf("test_error: checking empty error chain\n");
  dump_error_len();
  pspio_error_flush();
  dump_error_len();

  /* Check single error */
  printf("test_error: checking single error\n");
  pspio_error_tmp_id = PSPIO_EVALUE;
  eid = pspio_error_add("test_1_1.c", 1234);
  printf("test_error: after pspio_error_add, status = %d\n", eid);
  dump_error_len();
  pspio_error_flush();
  dump_error_len();

  /* Check double error */
  printf("test_error: checking double error\n");
  pspio_error_tmp_id = PSPIO_EGSL;
  pspio_error_add("test_2_1.c", 201);
  printf("test_error: after pspio_error_add, status = %d\n", eid);
  dump_error_len();
  pspio_error_tmp_id = PSPIO_ENOSUPPORT;
  pspio_error_add("test_2_2.c", 202);
  printf("test_error: after pspio_error_add, status = %d\n", eid);
  dump_error_len();
  pspio_error_flush();
  dump_error_len();

  /* Check triple error */
  printf("test_error: checking triple error\n");
  pspio_error_tmp_id = PSPIO_EVALUE;
  pspio_error_add("test_3_1.c", 311);
  printf("test_error: after pspio_error_add, status = %d\n", eid);
  dump_error_len();
  pspio_error_tmp_id = PSPIO_ENOFILE;
  pspio_error_add("test_3_2.c", 322);
  printf("test_error: after pspio_error_add, status = %d\n", eid);
  dump_error_len();
  pspio_error_tmp_id = PSPIO_ERROR;
  pspio_error_add("test_3_3.c", 333);
  printf("test_error: after pspio_error_add, status = %d\n", eid);
  dump_error_len();
  pspio_error_flush();
  dump_error_len();

  return 0;
}
