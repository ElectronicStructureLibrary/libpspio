/* Copyright (C) 2016 Micael Oliveira <micael.oliveira@mpsd.mpg.de>
 *                    Yann Pouillon <notifications@materialsevolution.es>
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
  (*pspinfo)->description = NULL;
  (*pspinfo)->scheme_name = NULL;

  /* Initialize variables */
  (*pspinfo)->time.tm_sec = 0;
  (*pspinfo)->time.tm_min = 0;
  (*pspinfo)->time.tm_hour = 0;
  (*pspinfo)->time.tm_mday = 1;
  (*pspinfo)->time.tm_mon = 0;
  (*pspinfo)->time.tm_year = 0;
  (*pspinfo)->time.tm_wday = 0;
  (*pspinfo)->time.tm_yday = 0;
  (*pspinfo)->time.tm_isdst = 0;

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

  if (src->description != NULL)
    pspio_pspinfo_set_description(*dst, src->description);

  if (src->scheme_name != NULL)
    pspio_pspinfo_set_scheme_name(*dst, src->scheme_name);

  (*dst)->time = src->time;

  return PSPIO_SUCCESS;
}

void pspio_pspinfo_free(pspio_pspinfo_t *pspinfo)
{
  if (pspinfo != NULL) {
    free(pspinfo->author);
    free(pspinfo->code);
    free(pspinfo->description);
    free(pspinfo->scheme_name);
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

int pspio_pspinfo_set_generation_day(pspio_pspinfo_t *pspinfo, int day)
{
  assert(pspinfo != NULL);

  if (day < 32 && day > 0)
    pspinfo->time.tm_mday = day;
  else
    return PSPIO_EVALUE;

  return PSPIO_SUCCESS;
}

int pspio_pspinfo_set_generation_month(pspio_pspinfo_t *pspinfo, int month)
{
  assert(pspinfo != NULL);

  if (month < 13 && month > 0)
    pspinfo->time.tm_mon = month - 1;
  else
    return PSPIO_EVALUE;

  return PSPIO_SUCCESS;
}

int pspio_pspinfo_set_generation_year(pspio_pspinfo_t *pspinfo, int year)
{
  assert(pspinfo != NULL);

  if (year >= 0)
    pspinfo->time.tm_year = year - 1900;
  else
    return PSPIO_EVALUE;

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

int pspio_pspinfo_set_scheme_name(pspio_pspinfo_t *pspinfo, const char *scheme_name)
{
  assert(pspinfo != NULL);

  FULFILL_OR_RETURN(scheme_name != NULL, PSPIO_EVALUE);

  free(pspinfo->scheme_name);
  pspinfo->scheme_name = strdup(scheme_name);
  FULFILL_OR_EXIT( pspinfo->scheme_name != NULL, PSPIO_ENOMEM );

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

int pspio_pspinfo_get_generation_day(const pspio_pspinfo_t *pspinfo)
{
  assert(pspinfo != NULL);

  return pspinfo->time.tm_mday;
}

int pspio_pspinfo_get_generation_month(const pspio_pspinfo_t *pspinfo)
{
  assert(pspinfo != NULL);

  return pspinfo->time.tm_mon + 1;
}

int pspio_pspinfo_get_generation_year(const pspio_pspinfo_t *pspinfo)
{
  assert(pspinfo != NULL);

  return pspinfo->time.tm_year + 1900;
}

const char * pspio_pspinfo_get_description(const pspio_pspinfo_t *pspinfo)
{
  assert(pspinfo != NULL);

  return pspinfo->description;
}

const char * pspio_pspinfo_get_scheme_name(const pspio_pspinfo_t *pspinfo)
{
  assert(pspinfo != NULL);

  return pspinfo->scheme_name;
}


/**********************************************************************
 * Utility routines                                                   *
 **********************************************************************/

int pspio_pspinfo_cmp(const pspio_pspinfo_t *pspinfo1, const pspio_pspinfo_t *pspinfo2)
{
  struct tm time1 = pspinfo1->time, time2 = pspinfo2->time;

  assert(pspinfo1 != NULL);
  assert(pspinfo2 != NULL);

  if (strcmp(pspinfo1->author, pspinfo2->author) ||
      strcmp(pspinfo1->code, pspinfo2->code) ||
      difftime(mktime(&time1),mktime(&time2) ) ||
      strcmp(pspinfo1->scheme_name, pspinfo2->scheme_name) ||
      strcmp(pspinfo1->description, pspinfo2->description) ) {
    return PSPIO_DIFF;
  } else {
    return PSPIO_EQUAL;
  }
}
