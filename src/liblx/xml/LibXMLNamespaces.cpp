/**
 * @file    LibXMLNamespaces.cpp
 * @brief   Extracts XML namespace declarations from LibXML prefix/URI pairs.
 * @author  Ben Bornstein
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
 * in the file named "LICENSE.txt" included with this software distribution and
 * also available online as http://sbml.org/software/libsbml/license.html
 * ---------------------------------------------------------------------- -->*/

#include <liblx/xml/LibXMLTranscode.h>
#include <liblx/xml/LibXMLNamespaces.h>

/** @cond doxygenIgnored */

using namespace std;

/** @endcond */

LIBLX_CPP_NAMESPACE_BEGIN

/** @cond doxygenLibsbmlInternal */

/**
 * Creates a new list of XML namespaces declarations from a "raw" LibXML
 * prefix/URI pairs.
 */
LibXMLNamespaces::LibXMLNamespaces (  const xmlChar**     namespaces
                                    , const unsigned int& size )
{
  mNamespaces.reserve(size);

  for (unsigned int n = 0; n < size; ++n)
  {
    const string prefix = LibXMLTranscode( namespaces[2 * n]     );
    const string uri    = LibXMLTranscode( namespaces[2 * n + 1], true );

    add(uri, prefix);
  }
}


/**
 * Destroys this list of XML namespace declarations.
 */
LibXMLNamespaces::~LibXMLNamespaces ()
{
}

/** @endcond */

LIBLX_CPP_NAMESPACE_END
