/*
 Copyright (C) 2011-2012 J. Alberdi, M. Oliveira, Y. Pouillon, and M. Verstraete

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation; either version 3 of the License, or 
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

*/

/** 
 * @file upf_tag.c
 * @brief functions to deal with UPF tags
 */
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
