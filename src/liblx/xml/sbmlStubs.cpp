/*
 * sbmlMemoryStubs.cpp
 *
 *  Created on: 23 October 2020
 *      Author: Timothy Spain
 */

#include "sbmlMemoryStubs.h"

#include <cstring>
#include <cstdlib>
#include <string>

LIBLX_EXTERN
char* safe_strdup(const char* s)
{
	  size_t  size;
	  char   *duplicate;

	  if (s == NULL) return NULL;

	  size      = strlen(s) + 1;
	  duplicate = (char *) safe_malloc(size * sizeof(char));


	  strncpy(duplicate, s, size);

	  return duplicate;

}

LIBLX_EXTERN
void* safe_malloc (size_t size)
{
	  void *p = (void *) malloc(size);


	  if (p == NULL)
	  {
	#ifdef EXIT_ON_ERROR
	    fprintf(stderr, MSG_OUT_OF_MEMORY);
	    exit(-1);
	#endif
	  }

	  return p;

}

LIBLX_EXTERN
unsigned int
streq (const char *s, const char *t)
{
  if (s == NULL)
    return t == NULL;
  else if (t == NULL)
    return 0;
  else
    return !strcmp(s, t);
}



LIBLX_EXTERN
void
safe_free (void * element)
{
  if (!element)
    return;

  free(element);
  element = NULL;
}
