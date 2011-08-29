#include "pspio_error.h"

const char * pspio_errorstr (const int pspio_errorid){
  switch (pspio_errorid)
    {
    case PSPIO_SUCCESS:
      return "success" ;
    case PSPIO_ERROR:
      return "error" ;
    default:
      return "unknown error code" ;
    }
}
