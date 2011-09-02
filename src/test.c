#include <stdio.h>

#include "pspio.h"


void test_upf(char *file_name)
{
  int ierr;
  char *error_msg;
  pspio_pspdata_t *data;

  ierr = pspio_pspdata_init(data, file_name, UPF);
  printf("The %s input file has been read\n",file_name);
  if (ierr) {
    pspio_error_flush();
  } else {
    if (data->mesh->np != NULL)
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

  printf("The test has started\n");
  test_upf(argv[1]);
  printf("The test has finished\n");

  return 0;
}
