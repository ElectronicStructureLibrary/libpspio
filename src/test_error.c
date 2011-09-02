#include <stdlib.h>
#include <stdio.h>

#include "pspio_error.h"

#if defined HAVE_CONFIG_H
#include "config.h"
#endif


int main(void) {
  int eid, a, b, c;
  char *info = NULL;

  /* Display basic information */
  printf("%s test\nReport bugs to %s\n\n", PACKAGE_STRING, PACKAGE_BUGREPORT);

  /* check empty error chain */
  printf("test_error: checking empty error chain\n");
  printf("test_error: at the beginning, status = %d, length = %d\n",
    eid, pspio_error_len());
  printf("test_error: BEGIN FLUSH\n");
  eid = pspio_error_flush();
  printf("test_error: END FLUSH\n");
  printf("test_error: after pspio_error_flush, status = %d, length = %d\n",
    eid, pspio_error_len());
  printf("\n");

  /* Check single error */
  printf("test_error: checking single error (EVALUE)\n");
  pspio_error_set(PSPIO_EVALUE);
  eid = pspio_error_add("test_1_1.c", 1234);
  printf("test_error: after pspio_error_add, status = %d, length = %d\n",
    eid, pspio_error_len());
  printf("test_error: BEGIN FLUSH\n");
  pspio_error_flush();
  printf("test_error: END FLUSH\n");
  printf("test_error: after pspio_error_flush, status = %d, length = %d\n",
    eid, pspio_error_len());
  printf("\n");

  /* Check double error */
  printf("test_error: checking double error (EGSL, ENOSUPPORT)\n");
  pspio_error_set(PSPIO_EGSL);
  pspio_error_add("test_2_1.c", 201);
  printf("test_error: after pspio_error_add, status = %d, length = %d\n",
    eid, pspio_error_len());
  pspio_error_set(PSPIO_ENOSUPPORT);
  pspio_error_add("test_2_2.c", 202);
  printf("test_error: after pspio_error_add, status = %d, length = %d\n",
    eid, pspio_error_len());
  printf("test_error: BEGIN FLUSH\n");
  pspio_error_flush();
  printf("test_error: END FLUSH\n");
  printf("test_error: after pspio_error_flush, status = %d, length = %d\n",
    eid, pspio_error_len());
  printf("\n");

  /* Check triple error */
  printf("test_error: checking triple error (EVALUE, ENOFILE, ERROR)\n");
  pspio_error_set(PSPIO_EVALUE);
  pspio_error_add("test_3_1.c", 311);
  printf("test_error: after pspio_error_add, status = %d, length = %d\n",
    eid, pspio_error_len());
  pspio_error_set(PSPIO_ENOFILE);
  pspio_error_add("test_3_2.c", 322);
  printf("test_error: after pspio_error_add, status = %d, length = %d\n",
    eid, pspio_error_len());
  pspio_error_set(PSPIO_ERROR);
  pspio_error_add("test_3_3.c", 333);
  printf("test_error: after pspio_error_add, status = %d, length = %d\n",
    eid, pspio_error_len());
  printf("test_error: BEGIN FLUSH\n");
  pspio_error_flush();
  printf("test_error: END FLUSH\n");
  printf("test_error: after pspio_error_flush, status = %d, length = %d\n",
    eid, pspio_error_len());
  printf("\n");

  return 0;
}
