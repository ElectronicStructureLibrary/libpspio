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
 * @file pspio_pspinfo.h
 * @brief Pseudopotential general information
 */

#if !defined PSPIO_PSPINFO_H
#define PSPIO_PSPINFO_H

#include<time.h>
#include "pspio_common.h"


/**********************************************************************
 * Data structures                                                    *
 **********************************************************************/

/**
 * The pseudopotential information
 */
typedef struct{
  char author[PSPIO_STRLEN_LINE];
  char code_name[PSPIO_STRLEN_LINE];
  char code_version[PSPIO_STRLEN_LINE];
  struct tm time;
  char description[PSPIO_STRLEN_DESCRIPTION]; /**< descriptive string for content of file read in. Nothing should ever be assumed about its content. */
} pspio_pspinfo_t;


/**********************************************************************
 * Global routines                                                    *
 **********************************************************************/

/**
 * Allocates memory and preset pspinfo structure
 *
 * @param[in,out] pspinfo: pspinfo structure
 * @return error code
 */
int pspio_pspinfo_alloc(pspio_pspinfo_t **pspinfo);

/**
 * Duplicates a pspinfo structure.
 * @param[out] dst: destination pspinfo structure pointer
 * @param[in] src: source pspinfo structure pointer 
 * @return error code
 * @note The src pointer has to be allocated first with the pspio_pspinfo_alloc 
 *       method.
 * @note The dst pointer might or might not be allocated first. If it is not,
 *        then it is allocated here.
 */
int pspio_pspinfo_copy(pspio_pspinfo_t **dst, const pspio_pspinfo_t *src);

/**
 * Frees all memory associated with pspinfo structure
 * @param[in,out] pspinfo: pointer to pspinfo structure to be free
 */
void pspio_pspinfo_free(pspio_pspinfo_t *pspinfo);


/**********************************************************************
 * Setters                                                            *
 **********************************************************************/

/**
 * @param[in,out] pspinfo: pointer to pspinfo structure
 * @param[in] author: pointer to the author name
 * @return error code
 */
int pspio_pspinfo_set_author(pspio_pspinfo_t *pspinfo, const char *author);

/**
 * @param[in,out] pspinfo: pointer to pspinfo structure
 * @param[in] code_name: pointer to code name
 * @return error code
 */
int pspio_pspinfo_set_code_name(pspio_pspinfo_t *pspinfo, const char *code_name);

/**
 * @param[in,out] pspinfo: pointer to pspinfo structure
 * @param[in] code_version: pointer to code version
 * @return error code
 */
int pspio_pspinfo_set_code_version(pspio_pspinfo_t *pspinfo, const char *code_version);

/**
 * @param[in,out] pspinfo: pointer to pspinfo structure
 * @param[in] generation_day: day when the pseudo was generated (range 1 to 31)
 * @return error code
 */
int pspio_pspinfo_set_generation_day(pspio_pspinfo_t *pspinfo, int day);

/**
 * @param[in,out] pspinfo: pointer to pspinfo structure
 * @param[in] generation_month: month when the pseudo was generated (range 1 to 12)
 * @return error code
 */
int pspio_pspinfo_set_generation_month(pspio_pspinfo_t *pspinfo, int month);

/**
 * @param[in,out] pspinfo: pointer to pspinfo structure
 * @param[in] generation_year: year when the pseudo was generated
 * @return error code
 */
int pspio_pspinfo_set_generation_year(pspio_pspinfo_t *pspinfo, int year);

/**
 * @param[in,out] pspinfo: pointer to pspinfo structure
 * @param[in] description: pointer to a description of the pseudopotential file
 * @return error code
 */
int pspio_pspinfo_set_description(pspio_pspinfo_t *pspinfo, const char *description);


/**********************************************************************
 * Getters                                                            *
 **********************************************************************/

/**
 * @param[in] pspinfo: pointer to pspinfo structure
 * @return pointer to author name
 */
const char * pspio_pspinfo_get_author(const pspio_pspinfo_t *pspinfo);

/**
 * @param[in] pspinfo: pointer to pspinfo structure
 * @return pointer to code name
 */
const char * pspio_pspinfo_get_code_name(const pspio_pspinfo_t *pspinfo);

/**
 * @param[in] pspinfo: pointer to pspinfo structure
 * @return pointer to code version
 */
const char * pspio_pspinfo_get_code_version(const pspio_pspinfo_t *pspinfo);

/**
 * @param[in] pspinfo: pointer to pspinfo structure
 * @return day of month (range 1 to 31)
 */
int pspio_pspinfo_get_generation_day(const pspio_pspinfo_t *pspinfo);

/**
 * @param[in] pspinfo: pointer to pspinfo structure
 * @return month (range 1 to 12)
 */
int pspio_pspinfo_get_generation_month(const pspio_pspinfo_t *pspinfo);

/**
 * @param[in] pspinfo: pointer to pspinfo structure
 * @return year
 */
int pspio_pspinfo_get_generation_year(const pspio_pspinfo_t *pspinfo);

/**
 * @param[in] pspinfo: pointer to pspinfo structure
 * @return pointer to a description of the pseudopotential file
 */
const char * pspio_pspinfo_get_description(const pspio_pspinfo_t *pspinfo);


/**********************************************************************
 * Utility routines                                                   *
 **********************************************************************/

/**
 * Compares two pspinfos.
 * @param[in] pspinfo1: first pspinfo to compare
 * @param[in] pspinfo2: second pspinfo to compare
 * @return PSPIO_EQUAL when equal, PSPIO_DIFF when different, PSPIO_ERROR if a
 * problem occured.
 */
int pspio_pspinfo_cmp(const pspio_pspinfo_t *pspinfo1, const pspio_pspinfo_t *pspinfo2);

#endif
