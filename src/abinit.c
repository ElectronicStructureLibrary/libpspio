/* Copyright (C) 2012-2016 Micael Oliveira <micael.oliveira@mpsd.mpg.de>
 *                         Yann Pouillon <notifications@materialsevolution.es>
 *
 * This file is part of Libpspio.
 *
 * Libpspio is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, version 3 of the License, or (at your option) any later
 * version.
 *
 * Libpspio is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Libpspio.  If not, see <http://www.gnu.org/licenses/> or write to
 * the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301  USA.
 */

/** 
 * @file pspio_abinit.c
 * @brief implementation to read and write Abinit files 
 */

#include <stdlib.h>
#include <string.h>

#include "fhi.h"
#include "oncv.h"
#include "abinit.h"

#if defined HAVE_CONFIG_H
#include "config.h"
#endif


int pspio_abinit_read(FILE *fp, pspio_pspdata_t *pspdata, int format)
{
  int ierr;

  assert(fp != NULL);
  assert(pspdata != NULL);

  ierr = abinit_read_header(fp, format, pspdata);

  switch (format) {
  case PSPIO_FMT_ABINIT_6:
    if (ierr == PSPIO_SUCCESS) {
      ierr = pspio_fhi_read(fp, pspdata);
    }
    break;
  case PSPIO_FMT_ABINIT_8:
    if (ierr == PSPIO_SUCCESS) {
      ierr = pspio_oncv_read(fp, pspdata);
    }
    break;
  /* Format 9 is XML and should never be read as an Abinit format */
  case PSPIO_FMT_ABINIT_9:
    ierr = PSPIO_EFILE_FORMAT;
    break;
  case PSPIO_FMT_ABINIT_1:
  case PSPIO_FMT_ABINIT_2:
  case PSPIO_FMT_ABINIT_3:
  case PSPIO_FMT_ABINIT_4:
  case PSPIO_FMT_ABINIT_5:
  case PSPIO_FMT_ABINIT_7:
  case PSPIO_FMT_ABINIT_10:
  case PSPIO_FMT_ABINIT_11:
  case PSPIO_FMT_ABINIT_17:
    ierr = PSPIO_ENOSUPPORT;
    break;
  default:
    ierr = PSPIO_EVALUE;
  }

  RETURN_WITH_ERROR( ierr );
}


int pspio_abinit_write(FILE *fp, const pspio_pspdata_t *pspdata, int format)
{
  int ierr = PSPIO_ERROR;

  assert(fp != NULL);
  assert(pspdata != NULL);

  switch (format) {
  case PSPIO_FMT_ABINIT_6:
    ierr = abinit_write_header(fp, format, pspdata);
    if (ierr == PSPIO_SUCCESS) {
      ierr = pspio_fhi_write(fp, pspdata);
    }
    break;
  case PSPIO_FMT_ABINIT_8:
    ierr = abinit_write_header(fp, format, pspdata);
    if (ierr == PSPIO_SUCCESS) {
      ierr = pspio_oncv_write(fp, pspdata);
    }
    break;
  case PSPIO_FMT_ABINIT_1:
  case PSPIO_FMT_ABINIT_2:
  case PSPIO_FMT_ABINIT_3:
  case PSPIO_FMT_ABINIT_4:
  case PSPIO_FMT_ABINIT_5:
  case PSPIO_FMT_ABINIT_7:
  case PSPIO_FMT_ABINIT_9:
  case PSPIO_FMT_ABINIT_10:
  case PSPIO_FMT_ABINIT_11:
  case PSPIO_FMT_ABINIT_17:
    ierr = PSPIO_ENOSUPPORT;
    break;
  default:
    ierr = PSPIO_EVALUE;
  }

  RETURN_WITH_ERROR( ierr );
}
