/** @file pspio_errorstr.c
 * @brief returns a string describing an error
*/

#include "pspio_error.h"

const char * pspio_errorstr (const int pspio_errorid){
  switch (pspio_errorid)
    {
    case PSPIO_SUCCESS:
      return "success" ;
    case PSPIO_ERROR:
      return "error" ;
    case PSPIO_NOFILE:
      return "file does not exist" ;
    case PSPIO_IOERR:
      return "error in I/O" ;
    default:
      return "unknown error code" ;
    }
}
