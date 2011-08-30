#ifndef PSPIO_LIBXCCODES_H
#define PSPIO_LIBXCCODES_H

#ifdef HAVE_LIBXC
#include <xc_funcs.h> /* this should point to the locally installed version of libxc/include/xc_funcs.h */
#else
#include <xc_funcs.h> /* imported from libxc into pspio package - may need to be updated periodically */
#endif

/* addition to the present libxc standard - define value for no xc explicitly */
#define XC_NONE 0

#endif
