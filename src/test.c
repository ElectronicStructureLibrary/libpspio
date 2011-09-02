#include <stdio.h>

#include "pspio.h"


void test_upf(char *file_name)
{
  int ierr;
  char *error_msg;
  pspio_pspdata_t *data;

  ierr = pspio_pspdata_init(data, file_name, UPF);
  if (ierr) {
    pspio_error_flush();
  } else {
    printf("Mesh number of point: %d\n", data->mesh->np);
  }

  pspio_pspdata_free(data);
}


/*----------------------------------------------------------*/
int main(int argc, char *argv[])
{
  if(argc != 2){
    printf("Usage:\n%s file\n", argv[0]);
    return 1;
  }

  test_upf(argv[1]);

  return 0;
}
