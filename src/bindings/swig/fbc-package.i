/**
 * Filename    : fbc-package.i
 * Description : fbc swig file for bindings.
 *
 * <!--------------------------------------------------------------------------
 * This file is part of libSBML.  Please visit http://sbml.org for more
 * information about SBML, and the latest version of libSBML.
 *
 * Copyright (C) 2013-2015 jointly by the following organizations:
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
 * ---------------------------------------------------------------------- -->
 */

#ifdef USE_FBC

%newobject removeAssociation;

%template(FbcPkgNamespaces) SBMLExtensionNamespaces<FbcExtension>;

%include <sbml/packages/fbc/extension/FbcExtension.h>
%include <sbml/packages/fbc/extension/FbcModelPlugin.h>
%include <sbml/packages/fbc/extension/FbcSpeciesPlugin.h>

%include <sbml/packages/fbc/util/CobraToFbcConverter.h>
%include <sbml/packages/fbc/util/FbcToCobraConverter.h>

%include <sbml/packages/fbc/sbml/Association.h>
%include <sbml/packages/fbc/sbml/FluxBound.h>
%include <sbml/packages/fbc/sbml/FluxObjective.h>
%include <sbml/packages/fbc/sbml/GeneAssociation.h>
%include <sbml/packages/fbc/sbml/Objective.h>

%include <sbml/packages/fbc/validator/FbcSBMLError.h>

#endif /* USE_FBC */
