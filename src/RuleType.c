/**
 * Filename    : RuleType.c
 * Description : SBML RuleType enumeration
 * Author(s)   : SBML Development Group <sbml-team@caltech.edu>
 * Organization: JST ERATO Kitano Symbiotic Systems Project
 * Created     : 2002-11-26
 * Revision    : $Id$
 * Source      : $Source$
 *
 * Copyright 2002 California Institute of Technology and
 * Japan Science and Technology Corporation.
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation; either version 2.1 of the License, or
 * any later version.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY, WITHOUT EVEN THE IMPLIED WARRANTY OF
 * MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.  The software and
 * documentation provided hereunder is on an "as is" basis, and the
 * California Institute of Technology and Japan Science and Technology
 * Corporation have no obligations to provide maintenance, support,
 * updates, enhancements or modifications.  In no event shall the
 * California Institute of Technology or the Japan Science and Technology
 * Corporation be liable to any party for direct, indirect, special,
 * incidental or consequential damages, including lost profits, arising
 * out of the use of this software and its documentation, even if the
 * California Institute of Technology and/or Japan Science and Technology
 * Corporation have been advised of the possibility of such damage.  See
 * the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
 *
 * The original code contained here was initially developed by:
 *
 *     Ben Bornstein
 *     The Systems Biology Markup Language Development Group
 *     ERATO Kitano Symbiotic Systems Project
 *     Control and Dynamical Systems, MC 107-81
 *     California Institute of Technology
 *     Pasadena, CA, 91125, USA
 *
 *     http://www.cds.caltech.edu/erato
 *     mailto:sbml-team@caltech.edu
 *
 * Contributor(s):
 */


#include "sbml/common.h"
#include "sbml/RuleType.h"


const char* RULE_TYPE_STRINGS[] =
{
    "rate"
  , "scalar"
  , "(Invalid RuleType)"
};


/**
 * Returns the RuleType with the given name (case-insensitive).
 */
LIBSBML_EXTERN
RuleType_t
RuleType_forName (const char *name)
{
  RuleType_t rt = RULE_TYPE_INVALID;


  /* Proceed iff name is not NULL and name is not an empty string */
  if (name && *name)
  {
    /* Too few types to warrant a binary search as in UnitKind_forName(). */
    for (rt = RULE_TYPE_RATE; rt < RULE_TYPE_INVALID; rt++)
    {
      if ( !strcmp_insensitive(name, RULE_TYPE_STRINGS[rt]) )
      {
        break;
      }
    }
  }

  return rt;
}


/**
 * Returns the name of the given RuleType.  The caller does not own the
 * returned string and is therefore not allowed to modify it.
 */
LIBSBML_EXTERN
const char *
RuleType_toString (RuleType_t rt)
{
  if ( (rt < RULE_TYPE_RATE) || (rt > RULE_TYPE_INVALID) )
  {
    rt = RULE_TYPE_INVALID;
  }

  return RULE_TYPE_STRINGS[rt];
}
