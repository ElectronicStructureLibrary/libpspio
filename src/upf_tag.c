/* Copyright (C) 2012-2017 Micael Oliveira <micael.oliveira@mpsd.mpg.de>
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
 * @file upf_tag.c
 * @brief functions to deal with UPF tags
 */
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "upf.h"

#if defined HAVE_CONFIG_H
#include "config.h"
#endif


int upf_tag_init(FILE * fp, const char * tag, int go_back)
{
  char line[PSPIO_STRLEN_LINE];
  char init_tag[PSPIO_STRLEN_LINE];
  char * read_string = NULL;

  if ( go_back ) rewind(fp);
  
  /* Prepare base string */
  sprintf(init_tag, "<%s>", tag);

  while ( fgets(line, sizeof line, fp) != NULL ) {
    /* Skip white spaces */
    if ( line[0] == ' ' )
      read_string = strtok(line," ");
    else 
      read_string = line;

    if ( strncasecmp(read_string, init_tag, strlen(init_tag)) == 0 ) {
      return PSPIO_SUCCESS;
    }
  }

  return PSPIO_EFILE_CORRUPT;
}

int upf_tag_check_end(FILE *fp, const char *tag)
{
  char line[PSPIO_STRLEN_LINE];
  char end_tag[PSPIO_STRLEN_LINE];
  char * read_string = NULL;
  int status;

  /* Prepare base string */
  sprintf(end_tag, "</%s>", tag);

  FULFILL_OR_RETURN( fgets(line, sizeof line, fp) != NULL, PSPIO_EIO );
  /* Skip white spaces */
  if (line[0] == ' ')
    read_string = strtok(line," ");
  else
    read_string = line;

  /* Compare with the ending tag */
  if ( strncasecmp(read_string, end_tag, strlen(end_tag)) == 0 ) {
    status = PSPIO_SUCCESS;
  } else {
    status = PSPIO_EFILE_CORRUPT;
  }

  return status;
}


int upf_tag_isdef(FILE *fp, const char *tag)
{
  char line[PSPIO_STRLEN_LINE];
  char init_tag[PSPIO_STRLEN_LINE];
  char * read_string = NULL;

  /* Go to the beginning of the buffer */
  rewind(fp);
  
  /* Prepare base string */
  sprintf(init_tag, "<%s>", tag);

  while ( fgets(line, sizeof line, fp) != NULL ) {
    /* Skip white spaces */
    if ( line[0] == ' ' )
      read_string = strtok(line," ");
    else 
      read_string = line;

    if ( strncasecmp(read_string, init_tag, strlen(init_tag)) == 0 ) {
      return 1;
    }
  }

  /* End of the buffer reached; so return false */
  return 0;
}
