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
 * @file pspio_pspinfo.h
 * @brief Pseudopotential general information
 */

#if !defined PSPIO_PSPINFO_H
#define PSPIO_PSPINFO_H

#include<time.h>


/**********************************************************************
 * Data structures                                                    *
 **********************************************************************/

/**
 * The pseudopotential information
 */
typedef struct{
  char *author;
  char *code;
  struct tm time;
  char *description; /**< descriptive string for content of file read in. Nothing should ever be assumed about its content. */
  char *scheme_name;
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
 * @param[in] code: pointer to code name
 * @return error code
 */
int pspio_pspinfo_set_code(pspio_pspinfo_t *pspinfo, const char *code);

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

/**
 * @param[in,out] pspinfo: pointer to pspinfo structure
 * @param[in] scheme_name: pointer to pseudopotential generation scheme name
 * @return error code
 */
int pspio_pspinfo_set_scheme_name(pspio_pspinfo_t *pspinfo, const char *scheme_name);


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
const char * pspio_pspinfo_get_code(const pspio_pspinfo_t *pspinfo);

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

/**
 * @param[in] pspdate: pointer to pspdata structure
 * @return pointer to scheme name.
 */
const char * pspio_pspinfo_get_scheme_name(const pspio_pspinfo_t *pspinfo);


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
