/**
 * @cond doxygenLibsbmlInternal
 *
 * @file    ExpatAttributes.cpp
 * @brief   Creates new XMLAttributes from "raw" Expat attributes.
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

#include <liblx/xml/ExpatAttributes.h>

using namespace std;

LIBLX_CPP_NAMESPACE_BEGIN

/**
 * Creates a new XMLAttributes set from the given "raw" Expat attributes.
 * The Expat attribute names are assumed to be in namespace triplet form
 * separated by sepchar.
 */
ExpatAttributes::ExpatAttributes (const XML_Char** attrs,
				  const XML_Char* elementName,
				  const XML_Char sep)
{
  unsigned int size = 0;
  while (attrs[2 * size]) ++size;

  mNames .reserve(size);
  mValues.reserve(size);

  for (unsigned int n = 0; n < size; ++n)
  {
    mNames .push_back( XMLTriple( attrs[2 * n], sep ) );
    mValues.push_back( string   ( attrs[2 * n + 1]  ) );
  }

  mElementName = elementName;
}


/**
 * Destroys this Attribute set.
 */
ExpatAttributes::~ExpatAttributes ()
{
}

LIBLX_CPP_NAMESPACE_END
/** @endcond */
