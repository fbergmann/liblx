/**
 *@cond doxygenLibsbmlInternal
 **
 *
 * @file    CompressCommon.h
 * @brief   common classes/functions for compression/decompression I/O
 * @author  Akiya Jouraku
 *
 *
 * <!--------------------------------------------------------------------------
 * This file is part of libSBML.  Please visit http://sbml.org for more
 * information about SBML, and the latest version of libSBML.
 *
 * Copyright (C) 2019 jointly by the following organizations:
 *     1. California Institute of Technology, Pasadena, CA, USA
 *     2. University of Heidelberg, Heidelberg, Germany
 *
 * Copyright (C) 2013-2018 jointly by the following organizations:
 *     1. California Institute of Technology, Pasadena, CA, USA
 *     2. EMBL European Bioinformatics Institute (EMBL-EBI), Hinxton, UK
 *     3. University of Heidelberg, Heidelberg, Germany
 *
 * Copyright (C) 2009-2013 jointly by the following organizations:
 *     1. California Institute of Technology, Pasadena, CA, USA
 *     2. EMBL European Bioinformatics Institute (EMBL-EBI), Hinxton, UK
 *
 * Copyright (C) 2006-2008 by the California Institute of Technology,
 *     Pasadena, CA, USA
 *
 * Copyright (C) 2002-2005 jointly by the following organizations:
 *     1. California Institute of Technology, Pasadena, CA, USA
 *     2. Japan Science and Technology Agency, Japan
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation.  A copy of the license agreement is provided
 * in the file named "LICENSE.txt" included with this software distribution
 * and also available online as http://sbml.org/software/libsbml/license.html
 * ---------------------------------------------------------------------- -->*/

#ifndef CompressCommon_h
#define CompressCommon_h

#include <exception>
#include <liblx/xml/common/extern.h>


LIBLX_CPP_NAMESPACE_BEGIN

/**
 *
 *  This exception will be thrown if a function which depends on
 *  some library invoked and underlying libSBML is not linked with
 *  the library.
 *
 */
class LIBLX_EXTERN NotLinked : public std::exception
{
public:
   NotLinked() throw() { }
   virtual ~NotLinked() throw() {}
};


/**
 *
 * This exception will be thrown if a function which depends on
 * zlib invoked and underlying libSBML is not linked with zlib.
 *
 */
class LIBLX_EXTERN ZlibNotLinked : public NotLinked
{
public:
   ZlibNotLinked() throw() { }
   virtual ~ZlibNotLinked() throw() {}
};


/**
 *
 *  This exception will be thrown if a function which depends on
 *  bzip2 library invoked and underlying libSBML is not linked with
 *  bzip2.
 *
 */
class LIBLX_EXTERN Bzip2NotLinked : public NotLinked
{
public:
   Bzip2NotLinked() throw() { }
   virtual ~Bzip2NotLinked() throw() {}
};


/**
 * Predicate returning @c true or @c false depending on whether
 * underlying libSBML is linked with zlib.
 *
 * @return @c true if libSBML is linked with zlib, @c false otherwise.
 */
LIBLX_EXTERN
bool hasZlib();


/**
 * Predicate returning @c true or @c false depending on whether
 * underlying libSBML is linked with bzip2.
 *
 * @return @c true if libSBML is linked with bzip2, @c false otherwise.
 */
LIBLX_EXTERN
bool hasBzip2();

LIBLX_CPP_NAMESPACE_END

#endif //CompressCommon_h
/** @endcond */
