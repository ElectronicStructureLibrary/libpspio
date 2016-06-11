/* Copyright (C) 2016 Micael Oliveira <micaeljtoliveira@gmail.com>
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
 * @file pspio_pspinfo.c
 * @brief Pseudopspinfo general information
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "pspio_pspinfo.h"
#include "pspio_error.h"


/**********************************************************************
 * Global routines                                                    *
 **********************************************************************/

int pspio_pspinfo_alloc(pspio_pspinfo_t **pspinfo)
{
  assert(pspinfo != NULL);

  /* Memory allocation */
  *pspinfo = (pspio_pspinfo_t *) malloc (sizeof(pspio_pspinfo_t));
  FULFILL_OR_EXIT(*pspinfo != NULL, PSPIO_ENOMEM);

  /* Nullify pointers */
  (*pspinfo)->author = NULL;
  (*pspinfo)->code = NULL;
  (*pspinfo)->date = NULL;
  (*pspinfo)->description = NULL;

  return PSPIO_SUCCESS;
}

int pspio_pspinfo_init(pspio_pspinfo_t *pspinfo, const char *author, const char *code,
                       const char *date, const char *description)
{
  assert(pspinfo != NULL);

  if (author != NULL) {
    pspinfo->author = strdup(author);
    FULFILL_OR_RETURN(pspinfo->author != NULL, PSPIO_ENOMEM);
  }

  if (code != NULL) {
    pspinfo->code = strdup(code);
    FULFILL_OR_RETURN(pspinfo->code != NULL, PSPIO_ENOMEM);
  }

  if (date != NULL) {
    pspinfo->date = strdup(date);
    FULFILL_OR_RETURN(pspinfo->date != NULL, PSPIO_ENOMEM);
  }

  if (description != NULL) {
    pspinfo->description = strdup(description);
    FULFILL_OR_RETURN(pspinfo->description != NULL, PSPIO_ENOMEM);
  }

  return PSPIO_SUCCESS;
}

int pspio_pspinfo_copy(pspio_pspinfo_t **dst, const pspio_pspinfo_t *src) {

  assert(src != NULL);

  pspio_pspinfo_free(*dst);
  SUCCEED_OR_RETURN( pspio_pspinfo_alloc(dst) );

  if (src->author != NULL)
    pspio_pspinfo_set_author(*dst, src->author);

  if (src->code != NULL)
    pspio_pspinfo_set_code(*dst, src->code);

  if (src->date != NULL)
    pspio_pspinfo_set_date(*dst, src->date);

  if (src->description != NULL)
    pspio_pspinfo_set_description(*dst, src->description);

  return PSPIO_SUCCESS;
}

void pspio_pspinfo_free(pspio_pspinfo_t *pspinfo)
{
  if (pspinfo != NULL) {
    free(pspinfo->author);
    free(pspinfo->code);
    free(pspinfo->date);
    free(pspinfo->description);
    free(pspinfo);
  }
}


/**********************************************************************
 * Setters                                                            *
 **********************************************************************/

int pspio_pspinfo_set_author(pspio_pspinfo_t *pspinfo, const char *author)
{
  assert(pspinfo != NULL);

  FULFILL_OR_RETURN(author != NULL, PSPIO_EVALUE);

  free(pspinfo->author);
  pspinfo->author = strdup(author);
  FULFILL_OR_EXIT( pspinfo->author != NULL, PSPIO_ENOMEM );

  return PSPIO_SUCCESS;
}

int pspio_pspinfo_set_code(pspio_pspinfo_t *pspinfo, const char *code)
{
  assert(pspinfo != NULL);

  FULFILL_OR_RETURN(code != NULL, PSPIO_EVALUE);

  free(pspinfo->code);
  pspinfo->code = strdup(code);
  FULFILL_OR_EXIT( pspinfo->code != NULL, PSPIO_ENOMEM );

  return PSPIO_SUCCESS;
}

int pspio_pspinfo_set_date(pspio_pspinfo_t *pspinfo, const char *date)
{
  assert(pspinfo != NULL);

  FULFILL_OR_RETURN(date != NULL, PSPIO_EVALUE);

  free(pspinfo->date);
  pspinfo->date = strdup(date);
  FULFILL_OR_EXIT( pspinfo->date != NULL, PSPIO_ENOMEM );

  return PSPIO_SUCCESS;
}

int pspio_pspinfo_set_description(pspio_pspinfo_t *pspinfo, const char *description)
{
  assert(pspinfo != NULL);

  FULFILL_OR_RETURN(description != NULL, PSPIO_EVALUE);

  free(pspinfo->description);
  pspinfo->description = strdup(description);
  FULFILL_OR_EXIT( pspinfo->description != NULL, PSPIO_ENOMEM );

  return PSPIO_SUCCESS;
}

/**********************************************************************
 * Getters                                                            *
 **********************************************************************/

const char * pspio_pspinfo_get_author(const pspio_pspinfo_t *pspinfo)
{
  assert(pspinfo != NULL);

  return pspinfo->author;
}

const char * pspio_pspinfo_get_code(const pspio_pspinfo_t *pspinfo)
{
  assert(pspinfo != NULL);

  return pspinfo->code;
}

const char * pspio_pspinfo_get_date(const pspio_pspinfo_t *pspinfo)
{
  assert(pspinfo != NULL);

  return pspinfo->date;
}

const char * pspio_pspinfo_get_description(const pspio_pspinfo_t *pspinfo)
{
  assert(pspinfo != NULL);

  return pspinfo->description;
}


/**********************************************************************
 * Utility routines                                                   *
 **********************************************************************/

int pspio_pspinfo_cmp(const pspio_pspinfo_t *pspinfo1, const pspio_pspinfo_t *pspinfo2)
{
  assert(pspinfo1 != NULL);
  assert(pspinfo2 != NULL);

  if (strcmp(pspinfo1->author, pspinfo2->author) ||
      strcmp(pspinfo1->code, pspinfo2->code) ||
      strcmp(pspinfo1->date, pspinfo2->date) ||
      strcmp(pspinfo1->description, pspinfo2->description) ) {
    return PSPIO_DIFF;
  } else {
    return PSPIO_EQUAL;
  }
}
