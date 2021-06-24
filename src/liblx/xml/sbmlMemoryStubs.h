/*
 * sbmlMemoryStubs.h
 *
 *  Created on: 27 October 2020
 *      Author: Timothy Spain
 */

#ifndef SBMLMEMORYSTUBS_H
#define SBMLMEMORYSTUBS_H

#include <ctype.h>

#include <liblx/xml/XMLExtern.h>


LIBLX_CPP_NAMESPACE_BEGIN

BEGIN_C_DECLS

LIBLX_EXTERN
char* safe_strdup(const char* s);

LIBLX_EXTERN
void* safe_malloc (size_t size);

/**
 * Easier-to-read and NULL-friendly string comparison.
 */
LIBLX_EXTERN
unsigned int
streq (const char *s, const char *t);


LIBLX_EXTERN
void
safe_free (void * element);


END_C_DECLS
LIBLX_CPP_NAMESPACE_END


#endif /*ndef SBMLMEMORYSTUBS_H */
