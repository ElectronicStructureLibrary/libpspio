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
#include "util.h"

#if defined HAVE_CONFIG_H
#include "config.h"
#endif


int upf_tag_init(FILE * fp, const char * tag, const int go_back) {
  char line[PSPIO_STRLEN_LINE];
  char * init_tag = NULL;
  char * read_string = NULL;
  int i;

  if ( go_back ) rewind(fp);
  
  // Prepare base string
  init_tag = (char *) malloc ((strlen(tag)+3) * sizeof(char));
  FULFILL_OR_EXIT(init_tag != NULL, PSPIO_ENOMEM);
  init_tag[0] = '\0';
  strcat(init_tag,"<");
  strncat(init_tag,tag,strlen(tag));
  strcat(init_tag,">");
  for (i=0;init_tag[i]; i++)
    init_tag[i] = tolower(init_tag[i]);

  while ( fgets(line, sizeof line, fp) != NULL ) {
    // Skip white spaces
    if ( line[0] == ' ' )
      read_string = strtok(line," ");
    else 
      read_string = line;
    // Lowercase line
    for (i=0;read_string[i]; i++)
      read_string[i] = tolower(read_string[i]);
    
    if ( strncmp(read_string,init_tag,strlen(init_tag))==0 ) {
      free(init_tag);
      return PSPIO_SUCCESS;
    }
  }

  free(init_tag);
  return PSPIO_EFILE_CORRUPT;
}


int upf_tag_check_end(FILE * fp, const char * tag) {
  char line[PSPIO_STRLEN_LINE];
  char * ending_tag = NULL;
  char * read_string = NULL;
  int i, status;
  
  // Prepare base string
  ending_tag = (char *) malloc ((strlen(tag)+4) * sizeof(char));
  FULFILL_OR_EXIT(ending_tag != NULL, PSPIO_ENOMEM);
  ending_tag[0] = '\0';
  strcat(ending_tag,"</");
  strncat(ending_tag,tag,strlen(tag));
  strcat(ending_tag,">");
  for (i=0;ending_tag[i]; i++)
    ending_tag[i] = tolower(ending_tag[i]);
  
  FULFILL_OR_RETURN( fgets(line, sizeof line, fp) != NULL, PSPIO_EIO );
  // Skip white spaces
  if (line[0] == ' ')
    read_string = strtok(line," ");
  else 
    read_string = line;
  // Lowercase line
  for (i=0;read_string[i]; i++)
    read_string[i] = tolower(read_string[i]);

  // Compare with the ending tag
  if ( strncmp(read_string,ending_tag,strlen(ending_tag)) == 0 ) { 
    status = PSPIO_SUCCESS;
  } else {
    status = PSPIO_EFILE_CORRUPT;
  }
  free(ending_tag);

  return status;
}


int upf_tag_isdef(FILE * fp, const char * tag) {
  char line[PSPIO_STRLEN_LINE];
  char * init_tag = NULL; 
  char * read_string = NULL;
  int i;
  
  // Go to the beginning of the buffer
  rewind(fp);
  
  // Prepare base string
  init_tag = (char *) malloc ((strlen(tag)+3) * sizeof(char));
  FULFILL_OR_EXIT(init_tag != NULL, PSPIO_ENOMEM);
  init_tag[0] = '\0';
  strcat(init_tag,"<");
  strncat(init_tag,tag,strlen(tag));
  strcat(init_tag,">");
  for (i=0;init_tag[i]; i++)
    init_tag[i] = tolower(init_tag[i]);

  while ( fgets(line, sizeof line, fp) != NULL ) {
    // Skip white spaces
    if ( line[0] == ' ' )
      read_string = strtok(line," ");
    else 
      read_string = line;
    // Lowercase line
    for (i=0;line[i]; i++)
      line[i] = tolower(line[i]);

    if ( strncmp(read_string,init_tag,strlen(init_tag))==0 ) {
      free(init_tag);
      return 1;
    }
  }

  free(init_tag);
  // End of the buffer reached; so return false
  return 0;
}
