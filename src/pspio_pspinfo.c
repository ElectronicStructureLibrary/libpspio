/* Copyright (C) 2016-2017 Micael Oliveira <micael.oliveira@mpsd.mpg.de>
 *                         Yann Pouillon <devops@materialsevolution.es>
 *
 * This file is part of Libpspio.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * version 2.0. If a copy of the MPL was not distributed with this file, You
 * can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Libpspio is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the Mozilla Public License version 2.0 for
 * more details.
 */

/**
 * @file pspio_pspinfo.c
 * @brief Pseudopspinfo general information
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <memory.h>

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

  /* Initialize variables */
  strcpy((*pspinfo)->author, "Unknown");
  strcpy((*pspinfo)->code_name, "Unknown");
  strcpy((*pspinfo)->code_version, "Unknown");
  strcpy((*pspinfo)->description, "");
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

  strcpy((*dst)->author, src->author);
  strcpy((*dst)->code_name, src->code_name);
  strcpy((*dst)->code_version, src->code_version);
  strcpy((*dst)->description, src->description);
  (*dst)->time = src->time;

  return PSPIO_SUCCESS;
}

void pspio_pspinfo_free(pspio_pspinfo_t *pspinfo)
{
  if (pspinfo != NULL) {
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

  if(strlen(author) >= PSPIO_STRLEN_LINE)
    return PSPIO_STRLEN_ERROR;
  else
    strcpy(pspinfo->author, author);

  return PSPIO_SUCCESS;
}

int pspio_pspinfo_set_code_name(pspio_pspinfo_t *pspinfo, const char *code_name)
{
  assert(pspinfo != NULL);

  FULFILL_OR_RETURN(code_name != NULL, PSPIO_EVALUE);

  if(strlen(code_name) >= PSPIO_STRLEN_LINE)
    return PSPIO_STRLEN_ERROR;
  else
    strcpy(pspinfo->code_name, code_name);

  return PSPIO_SUCCESS;
}

int pspio_pspinfo_set_code_version(pspio_pspinfo_t *pspinfo, const char *code_version)
{
  assert(pspinfo != NULL);

  FULFILL_OR_RETURN(code_version != NULL, PSPIO_EVALUE);

  if(strlen(code_version) >= PSPIO_STRLEN_LINE)
    return PSPIO_STRLEN_ERROR;
  else
    strcpy(pspinfo->code_version, code_version);

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

  if(strlen(description) >= PSPIO_STRLEN_DESCRIPTION)
    return PSPIO_STRLEN_ERROR;
  else
    strcpy(pspinfo->description, description);

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

const char * pspio_pspinfo_get_code_name(const pspio_pspinfo_t *pspinfo)
{
  assert(pspinfo != NULL);

  return pspinfo->code_name;
}

const char * pspio_pspinfo_get_code_version(const pspio_pspinfo_t *pspinfo)
{
  assert(pspinfo != NULL);

  return pspinfo->code_version;
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


/**********************************************************************
 * Utility routines                                                   *
 **********************************************************************/

int pspio_pspinfo_cmp(const pspio_pspinfo_t *pspinfo1, const pspio_pspinfo_t *pspinfo2)
{
  assert(pspinfo1 != NULL);
  assert(pspinfo2 != NULL);

  {
    struct tm time1 = pspinfo1->time, time2 = pspinfo2->time;

    if (strcmp(pspinfo1->author, pspinfo2->author) ||
        strcmp(pspinfo1->code_name, pspinfo2->code_name) ||
        strcmp(pspinfo1->code_version, pspinfo2->code_version) ||
        difftime(mktime(&time1), mktime(&time2)) ||
        strcmp(pspinfo1->description, pspinfo2->description) ) {
      return PSPIO_DIFF;
    } else {
      return PSPIO_EQUAL;
    }
  }
}
