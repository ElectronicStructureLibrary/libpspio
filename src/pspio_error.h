/**
 * Error codes
 */
#define PSPIO_SUCCESS  0 
#define PSPIO_ERROR   -1
#define PSPIO_NOFILE   1
#define PSPIO_IOERR    2
#define PSPIO_VALUE_ERROR 3

const char * pspio_errorstr (const int pspio_errorid);

