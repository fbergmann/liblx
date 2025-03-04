/**
 * @file    XMLOutputStream.h
 * @brief   XMLOutputStream
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
 * ---------------------------------------------------------------------- -->
 *
 * @class XMLOutputStream
 * @sbmlbrief{core} Interface to an XML output stream.
 *
 * @htmlinclude not-sbml-warning.html
 *
 * SBML content is serialized using XML; the resulting data can be stored and
 * read to/from a file or data stream.  Low-level XML parsers such as Xerces
 * provide facilities to read XML data.  To permit the use of different XML
 * parsers (Xerces, Expat or libxml2), libSBML implements an abstraction
 * layer.  XMLInputStream and XMLOutputStream are two parts of that
 * abstraction layer.
 *
 * XMLOutputStream provides a wrapper above output streams to facilitate
 * writing XML.  XMLOutputStream keeps track of start and end elements,
 * indentation, XML namespace prefixes, and more.  The interface provides
 * features for converting non-text data types into appropriate textual form;
 * this takes the form of overloaded <code>writeAttribute(...)</code> methods
 * that allow users to simply use the same method with any data type.  For
 * example, suppose an element @c testElement has two attributes, @c size and
 * @c id, and the attributes are variables in your code as follows:
@if cpp
@code{.cpp}
double size = 3.2;
std::string id = "id";
@endcode
@endif
@if java
@code
double size = 3.2;
String id = "id";
@endcode
@endif
@if python
@code
size = 3.2;
id = "id";
@endcode
@endif
  * Then, the element and the attributes can be written to the
  * standard output stream @ifnot cpp (provided as @c cout in the libSBML
  * language bindings)@endif@~ as follows:
@if cpp
@code{.cpp}
double size = 3.2;
std::string id = "id";

// Create an XMLOutputStream object that will write to the
// standard output stream:

XMLOutputStream xos = new XMLOutputStream(cout);

// Create the start element, write the attributes, and close
// the element.  The output will be written immediately as
// each method is called.

xos.startElement("testElement")
xos.writeAttribute("size", size)
xos.writeAttribute("id", id)
xos.endElement("testElement")
@endcode
@endif
@if java
@code{.java}
import org.sbml.libsbml.XMLOutputStream;
import org.sbml.libsbml.libsbml;

public class test
{
    public static void main (String[] args)
    {
        double size = 3.2;
        String id = "id";

        // Create an XMLOutputStream object that will write to the
        // standard output stream, which is provide in libSBML's
        // Java language interface as the object "libsbml.cout".

        XMLOutputStream xos = new XMLOutputStream(libsbml.cout);

        // Create the start element, write the attributes, and close
        // the element.  The output will be written immediately as
        // each method is called.

        xos.startElement("testElement");
        xos.writeAttribute("size", size);
        xos.writeAttribute("id", id);
        xos.endElement("testElement");
    }

    static
    {
        System.loadLibrary("sbmlj");
    }
}
@endcode
@endif
@if python
@code{.py}
from libsbml import *

size = 3.2;
id = "id";

# Create an XMLOutputStream object that will write to the standard
# output stream, which is provide in libSBML's Python language
# interface as the object "libsbml.cout".  Since we imported * from
# the libsbml module, we can simply refer to it as "cout" here:

output_stream = XMLOutputStream(cout)

# Create the start element, write the attributes, and close the
# element.  The output is written immediately by each method.

output_stream.startElement("testElement")
output_stream.writeAttribute("size", size)
output_stream.writeAttribute("id", id)
output_stream.endElement("testElement")
@endcode
@endif
 *
 * Other classes in SBML take XMLOutputStream objects as arguments, and use
 * that to write elements and attributes seamlessly to the XML output stream.
 *
 * It is also worth noting that unlike XMLInputStream, XMLOutputStream is
 * actually independent of the underlying XML parsers.  It does not use the
 * XML parser libraries at all.
 *
 * @note The convenience of the XMLInputStream and XMLOutputStream
 * abstraction may be useful for developers interested in creating parsers
 * for other XML formats besides SBML.  It can provide developers with a
 * layer above more basic XML parsers, as well as some useful programmatic
 * elements such as XMLToken, XMLError, etc.
 *
 * @see XMLInputStream
 */

/**
 * <!-- ~ ~ ~ ~ ~ Start of common documentation strings ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
 * The following text is used as common documentation blocks copied multiple
 * times elsewhere in this file.  The use of @class is a hack needed because
 * Doxygen's @copydetails command has limited functionality.  Symbols
 * beginning with "doc_" are marked as ignored in our Doxygen configuration.
 * ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  -->
 *
 * @class doc_xml_declaration
 *
 * @par
 * The XML declaration has the form
 * @verbatim
<?xml version="1.0" encoding="UTF-8"?>
@endverbatim
 * Note that the SBML specifications require the use of UTF-8 encoding and
 * version 1.0, so for SBML documents, the above is the standard XML
 * declaration.
 *
 * <!-- ------------------------------------------------------------------- -->
 * @class doc_programname_arguments
 *
 * @par
 * The functionality associated with the @p programName and @p
 * programVersion arguments concerns an optional comment that libSBML can
 * write at the beginning of the output stream.  The comment is intended
 * for human readers of the XML file, and has the following form:
 * @verbatim
<!-- Created by <program name> version <program version>
on yyyy-MM-dd HH:mm with libSBML version <libsbml version>. -->
@endverbatim
 *
 * This program information comment is a separate item from the XML
 * declaration that this method can also write to this output stream.  The
 * comment is also not mandated by any SBML specification.  This libSBML
 * functionality is provided for the convenience of calling programs, and to
 * help humans trace the origin of SBML files.
 *
 * <!-- ------------------------------------------------------------------- -->
 * @class doc_indentation
 *
 * LibSBML tries to produce human-readable XML output by automatically
 * indenting the bodies of elements.  Callers can manually control
 * indentation further by using the XMLOutputStream::upIndent()
 * and XMLOutputStream::downIndent() methods to increase and
 * decrease, respectively, the current level of indentation in the
 * XML output.
 */

#ifndef XMLOutputStream_h
#define XMLOutputStream_h

#include <liblx/xml/common/liblxfwd.h>
#ifdef __cplusplus

#include <iostream>
#include <limits>
#include <locale>
#include <string>
#include <time.h>
#include <liblx/xml/common/liblx-version.h>

#include <liblx/xml/common/extern.h>

#include <liblx/xml/sbmlMemoryStubs.h>

LIBLX_CPP_NAMESPACE_BEGIN

class XMLTriple;
class XMLNamespaces;

class LIBLX_EXTERN XMLOutputStream
{
public:

  /**
   * Creates a new XMLOutputStream that wraps the given @p stream.
   *
   * @copydetails doc_programname_arguments
   *
   * @copydetails doc_xml_declaration
   *
   * @param stream the input stream to wrap.
   *
   * @param encoding the XML encoding to declare in the output. This value
   * should be <code>"UTF-8"</code> for SBML documents.  The default value
   * is <code>"UTF-8"</code> if no value is supplied for this parameter.
   *
   * @param writeXMLDecl whether to write a standard XML declaration at
   * the beginning of the content written on @p stream.  The default is
   * @c true.
   *
   * @param programName an optional program name to write as a comment
   * in the output stream.
   *
   * @param programVersion an optional version identification string to write
   * as a comment in the output stream.
   *
   * @ifnot hasDefaultArgs @htmlinclude warn-default-args-in-docs.html @endif@~
   */
  XMLOutputStream (std::ostream&       stream
                   , const std::string  encoding       = "UTF-8"
                   , bool                writeXMLDecl   = true
                   , const std::string  programName    = ""
                   , const std::string  programVersion = "");


  /**
   * Destroys this XMLOutputStream.
   */
  virtual ~XMLOutputStream();


  /**
   * Writes the given XML end element name to this XMLOutputStream.
   *
   * @param name the name of the element.
   *
   * @param prefix an optional XML namespace prefix to write in front of the
   * @p element name.  (The result has the form
   * <code><em>prefix</em>:<em>name</em></code>.)
   *
   * @ifnot hasDefaultArgs @htmlinclude warn-default-args-in-docs.html @endif@~
   */
  void endElement (const std::string& name, const std::string prefix = "");


  /**
   * Writes the given element to the stream.
   *
   * @param triple the XML element to write.
   * @param text the text to put
   */
  void endElement (const XMLTriple& triple, bool text = false);


  /**
   * Turns automatic indentation on or off for this XMLOutputStream.
   *
   * @param indent if @c true, automatic indentation is turned on.
   */
  void setAutoIndent (bool indent);


  /**
   * Writes the given XML start element name to this XMLOutputStream.
   *
   * @param name the name of the element.
   *
   * @param prefix an optional XML namespace prefix to write in front of the
   * @p element name.  (The result has the form
   * <code><em>prefix</em>:<em>name</em></code>.)
   *
   * @ifnot hasDefaultArgs @htmlinclude warn-default-args-in-docs.html @endif@~
   */
  void startElement (const std::string& name, const std::string prefix = "");


  /**
   * Writes the given XML start element
   * <code><em>prefix</em>:<em>name</em></code> on this output stream.
   *
   * @param triple the start element to write.
   */
  void startElement (const XMLTriple& triple);


  /**
   * Writes the given XML start and end element name to this XMLOutputStream.
   *
   * @param name the name of the element.
   *
   * @param prefix an optional XML namespace prefix to write in front of the
   * @p element name.  (The result has the form
   * <code><em>prefix</em>:<em>name</em></code>.)
   *
   * @ifnot hasDefaultArgs @htmlinclude warn-default-args-in-docs.html @endif@~
   */
  void startEndElement (const std::string& name, const std::string prefix = "");


  /**
   * Writes the given start element to this output stream.
   *
   * @param triple the XML element to write.
   */
  void startEndElement (const XMLTriple& triple);


  /**
   * Writes the given attribute and value to this output stream.
   *
   * @param name the name of the attribute.
   *
   * @param value the value of the attribute.
   */
  void writeAttribute (const std::string& name, const std::string& value);


  /**
   * Writes the given namespace-prefixed attribute value to this output stream.
   *
   * @param name the name of the attribute.
   *
   * @param prefix an XML namespace prefix to write in front of the
   * @p element name.  (The result has the form
   * <code><em>prefix</em>:<em>name</em></code>.)  See other versions of
   * this method for a variant that does not require a prefix.
   *
   * @param value the value of the attribute.
   */
  void writeAttribute (const std::string& name, const std::string& prefix,
                       const std::string& value);


  /**
   * Writes the given attribute and value to this output stream.
   *
   * @param triple the attribute, in the form of an XMLTriple.
   *
   * @param value the value of the attribute.
   */
  void writeAttribute (const XMLTriple& triple, const std::string& value);


  /**
   * Writes the given attribute and value to this output stream.
   *
   * @param name the name of the attribute.
   *
   * @param value the value of the attribute.
   */
  void writeAttribute (const std::string& name, const char* value);


  /**
   * Writes the given namespace-prefixed attribute value to this output stream.
   *
   * @param name the name of the attribute.
   *
   * @param prefix an XML namespace prefix to write in front of the
   * @p element name.  (The result has the form
   * <code><em>prefix</em>:<em>name</em></code>.)  See other versions of
   * this method for a variant that does not require a prefix.
   *
   * @param value the value of the attribute.
   */
  void writeAttribute (const std::string& name, const std::string& prefix,
                       const char* value);


  /**
   * Writes the given attribute and value to this output stream.
   *
   * @param triple the attribute, in the form of an XMLTriple.
   *
   * @param value the value of the attribute.
   */
  void writeAttribute (const XMLTriple& triple, const char* value);


  /**
   * Writes the given attribute and value to this output stream.
   *
   * @param name the name of the attribute.
   *
   * @param value the value of the attribute.
   */
  void writeAttribute (const std::string& name, const bool& value);


  /**
   * Writes the given namespace-prefixed attribute value to this output stream.
   *
   * @param name the name of the attribute.
   *
   * @param prefix an XML namespace prefix to write in front of the
   * @p element name.  (The result has the form
   * <code><em>prefix</em>:<em>name</em></code>.)  See other versions of
   * this method for a variant that does not require a prefix.
   *
   * @param value the value of the attribute.
   */
  void writeAttribute (const std::string& name, const std::string &prefix,
                       const bool& value);


  /**
   * Writes the given attribute and value to this output stream.
   *
   * @param triple the attribute, in the form of an XMLTriple.
   *
   * @param value the value of the attribute.
   */
  void writeAttribute (const XMLTriple& triple, const bool& value);


  /**
   * Writes the given attribute and value to this output stream.
   *
   * @param name the name of the attribute.
   *
   * @param value the value of the attribute.
   */
  void writeAttribute (const std::string& name, const double& value);


  /**
   * Writes the given namespace-prefixed attribute value to this output stream.
   *
   * @param name the name of the attribute.
   *
   * @param prefix an XML namespace prefix to write in front of the
   * @p element name.  (The result has the form
   * <code><em>prefix</em>:<em>name</em></code>.)  See other versions of
   * this method for a variant that does not require a prefix.
   *
   * @param value the value of the attribute.
   */
  void writeAttribute (const std::string& name, const std::string& prefix,
                       const double& value);


  /**
   * Writes the given attribute and value to this output stream.
   *
   * @param triple the attribute, in the form of an XMLTriple.
   *
   * @param value the value of the attribute.
   */
  void writeAttribute (const XMLTriple& triple, const double& value);


  /**
   * Writes the given attribute and value to this output stream.
   *
   * @param name the name of the attribute.
   *
   * @param value the value of the attribute.
   */
  void writeAttribute (const std::string& name, const long& value);


  /**
   * Writes the given namespace-prefixed attribute value to this output stream.
   *
   * @param name the name of the attribute.
   *
   * @param prefix an XML namespace prefix to write in front of the
   * @p element name.  (The result has the form
   * <code><em>prefix</em>:<em>name</em></code>.)  See other versions of
   * this method for a variant that does not require a prefix.
   *
   * @param value the value of the attribute.
   */
  void writeAttribute (const std::string& name, const std::string& prefix,
                       const long& value);


  /**
   * Writes the given attribute and value to this output stream.
   *
   * @param triple the attribute, in the form of an XMLTriple.
   *
   * @param value the value of the attribute.
   */
  void writeAttribute (const XMLTriple& triple, const long& value);


  /**
   * Writes the given attribute and value to this output stream.
   *
   * @param name the name of the attribute.
   *
   * @param value the value of the attribute.
   */
  void writeAttribute (const std::string& name, const int& value);


  /**
   * Writes the given namespace-prefixed attribute value to this output stream.
   *
   * @param name the name of the attribute.
   *
   * @param prefix an XML namespace prefix to write in front of the
   * @p element name.  (The result has the form
   * <code><em>prefix</em>:<em>name</em></code>.)  See other versions of
   * this method for a variant that does not require a prefix.
   *
   * @param value the value of the attribute.
   */
  void writeAttribute (const std::string& name, const std::string& prefix,
                       const int& value);


  /**
   * Writes the given attribute and value to this output stream.
   *
   * @param triple the attribute, in the form of an XMLTriple.
   *
   * @param value the value of the attribute.
   */
  void writeAttribute (const XMLTriple& triple, const int& value);


  /**
   * Writes the given attribute and value to this output stream.
   *
   * @param name the name of the attribute.
   *
   * @param value the value of the attribute.
   */
  void writeAttribute (const std::string& name, const unsigned int& value);


  /**
   * Writes the given namespace-prefixed attribute value to this output stream.
   *
   * @param name the name of the attribute.
   *
   * @param prefix an XML namespace prefix to write in front of the
   * @p element name.  (The result has the form
   * <code><em>prefix</em>:<em>name</em></code>.)  See other versions of
   * this method for a variant that does not require a prefix.
   *
   * @param value the value of the attribute.
   */
  void writeAttribute (const std::string& name, const std::string& prefix,
                       const unsigned int& value);


  /**
   * Writes the given attribute and value to this output stream.
   *
   * @param triple the attribute, in the form of an XMLTriple.
   *
   * @param value the value of the attribute.
   */
  void writeAttribute (const XMLTriple& triple, const unsigned int& value);


  /**
   * Writes a standard XML declaration to this output stream.
   *
   * @copydetails doc_xml_declaration
   */
  void writeXMLDecl ();


  /**
   * Writes an XML comment with the name and version of this program.
   *
   * The XML comment has the following form:
   * @verbatim
<!-- Created by <program name> version <program version>
on yyyy-MM-dd HH:mm with libSBML version <libsbml version>. -->
@endverbatim
   *
   * See the class constructor for more information about this program
   * comment.
   *
   * @param programName an optional program name to write as a comment
   * in the output stream.
   *
   * @param programVersion an optional version identification string to write
   * as a comment in the output stream.
   *
   * @param writeTimestamp an optional flag indicating that a timestamp should
   * be written.
   */
  void writeComment (const std::string& programName,
                     const std::string& programVersion,
                     bool writeTimestamp = true);


  /**
   * Writes the given characters to the underlying stream.
   *
   * @param chars the characters to write.
   */
  XMLOutputStream& operator<< (const std::string& chars);


  /**
   * Writes the given double-type number to the underlying stream.
   *
   * @param value the value to write.
   */
  XMLOutputStream& operator<< (const double& value);


  /**
   * Outputs the given long-type number to the underlying stream.
   *
   * @param value the value to write.
   */
  XMLOutputStream& operator<< (const long& value);


  /**
   * Outputs a single character to the underlying stream.
   *
   * @param c the value to write.
   */
  XMLOutputStream& operator<< (const char& c);


  /**
   * Decreases the indentation level for this XMLOutputStream.
   *
   * @copydetails doc_indentation
   *
   * @see upIndent()
   */
  void downIndent ();


  /**
   * Increases the indentation level for this XMLOutputStream.
   *
   * @copydetails doc_indentation
   *
   * @see downIndent()
   */
  void upIndent ();


  /** @cond doxygenLibsbmlInternal */
  bool getStringStream();
  /** @endcond */


  /**
   * Returns the XMLNamespaces object attached to this output stream.
   *
   * @return the XMLNamespaces object, or @c NULL if none has been set.
   */
  XMLNamespaces * getXMLNamespaces();


  /**
   * Sets the XMLNamespaces object associated with this output stream.
   *
   * @param xmlns the namespace object.
   */
  void setXMLNamespaces(XMLNamespaces * xmlns);


  /**
   * @return a boolean, whether the output stream will write an XML
   * comment at the top of the file. (Enabled by default.)
   */
  static bool getWriteComment();
  /**
   * sets a flag, whether the output stream will write an XML
   * comment at the top of the file. (Enabled by default.)
   *
   * @param writeComment the flag.
   */
  static void setWriteComment(bool writeComment);

  /**
   * @return a boolean, whether the output stream will write an XML
   * comment with a timestamp at the top of the file. (Enabled by default.)
   */
  static bool getWriteTimestamp();
  /**
   * sets a flag, whether the output stream will write an XML
   * comment with a timestamp at the top of the file. (Enabled by default.)
   *
   * @param writeTimestamp the flag.
   */
  static void setWriteTimestamp(bool writeTimestamp);


  /**
   * @return the name of the library to be used in comments ('libSBML' by default).
   */
  static std::string getLibraryName();

  /**
   * sets the name of the library writing the XML

   * @param libraryName the name of the library to be used in comments.
   */
  static void setLibraryName(const std::string& libraryName);

  /**
   * @return a string representing the version of the library writing the output.
   *         This is the value of getLibSBMLDottedVersion() by default.
   */
  static std::string getLibraryVersion();

  /**
   * sets the name of the library writing the output
   *
   * @param libraryVersion the version information as string.
   */
  static void setLibraryVersion(const std::string& libraryVersion);

  /** @cond doxygenLibsbmlInternal */
  unsigned int getIndent();
  void setIndent(unsigned int indent);
  /** @endcond */

private:
  /** @cond doxygenLibsbmlInternal */
  /**
   * Copy Constructor, made private so as to notify users, that copying an input stream is not supported.
   *
   * @param other the instance to copy.
   */
  XMLOutputStream (const XMLOutputStream& other);


  /**
   * Assignment operator, made private so as to notify users, that copying an input stream is not supported.
   */
  XMLOutputStream& operator=(const XMLOutputStream& other);

  /** @endcond */


protected:
  /** @cond doxygenLibsbmlInternal */
  /**
   * Unitialized XMLOutputStreams may only be created by subclasses.
   */
  XMLOutputStream ();


  /**
   * Outputs the given characters to the underlying stream.
   */
  void writeChars (const std::string& name);


  /**
   * Outputs indentation whitespace.
   *
   * @ifnot hasDefaultArgs @htmlinclude warn-default-args-in-docs.html @endif@~
   */
  void writeIndent (bool isEnd = false);


  /**
   * Outputs name.
   */
  void writeName (const std::string& name, const std::string prefix = "");


  /**
   * Outputs prefix:name.
   */
  void writeName (const XMLTriple& triple);


  /**
   * Outputs value in quotes.
   */
  void writeValue (const std::string& value);


  /**
   * Outputs value in quotes.
   */
  void writeValue (const char* value);


  /**
   * Outputs "true" or "false" in quotes.
   */
  void writeValue (const bool& value);


  /**
   * Outputs the double value in quotes, or "INF", "-INF", or "NaN".
   */
  void writeValue (const double& value);


  /**
   * Outputs the long value in quotes.
   */
  void writeValue (const long& value);


  /**
   * Outputs the int value in quotes.
   */
  void writeValue (const int& value);


  /**
   * Outputs the int value in quotes.
   */
  void writeValue (const unsigned int& value);


  std::ostream& mStream;
  std::string   mEncoding;

  bool mInStart;
  bool mDoIndent;
  unsigned int mIndent;
  bool mInText;
  bool mSkipNextIndent;

  // this bool value is used to identify if the next character is '&'
  // for a character reference or predefined entity.
  bool mNextAmpersandIsRef;

  XMLNamespaces* mXMLns;

  // boolean indicating whether the comment on the top of the file is
  // written (enabled by default)
  static bool mWriteComment;

  // boolean indicating whether a timestamp will be generated at the time
  // of writing (enabled by default)
  static bool mWriteTimestamp;

  // the name of the library writing the file (i.e: libSBML)
  static std::string mLibraryName;

  // the version of the library writing the file
  static std::string mLibraryVersion;

  /* this is needed for the derived classes used to create the C wrapper */
  bool mStringStream;
  void setStringStream();
  void unsetStringStream();

  /** @endcond */
};


/** @cond doxygenLibsbmlInternal */
class LIBLX_EXTERN XMLOutputStringStream : public XMLOutputStream
{
public:

  /**
   * Creates a new XMLOutputStream that wraps stream.
   *
   * @ifnot hasDefaultArgs @htmlinclude warn-default-args-in-docs.html @endif@~
   */
  XMLOutputStringStream (  std::ostringstream& stream
                         , const std::string  encoding     = "UTF-8"
                         , bool                writeXMLDecl = true
                         , const std::string  programName  = ""
                         , const std::string  programVersion = "");

  std::ostringstream& getString();

protected:
  /** @cond doxygenLibsbmlInternal */
  std::ostringstream& mString;
  /** @endcond */
};
/** @endcond */

/** @cond doxygenLibsbmlInternal */
class LIBLX_EXTERN XMLOwningOutputStringStream : public XMLOutputStringStream
{
public:

  /**
   * Creates a new XMLOutputStream that wraps stream.
   *
   * @ifnot hasDefaultArgs @htmlinclude warn-default-args-in-docs.html @endif@~
   */
  XMLOwningOutputStringStream (  const std::string  encoding     = "UTF-8"
                               , bool                writeXMLDecl = true
                               , const std::string  programName  = ""
                               , const std::string  programVersion = "");

  virtual ~XMLOwningOutputStringStream();

};
/** @endcond */

/** @cond doxygenLibsbmlInternal */
class LIBLX_EXTERN XMLOutputFileStream : public XMLOutputStream
{
public:

  /**
   * Creates a new XMLOutputStream that wraps stream.
   *
   * @ifnot hasDefaultArgs @htmlinclude warn-default-args-in-docs.html @endif@~
   */
  XMLOutputFileStream (  std::ofstream&      stream
                       , const std::string  encoding     = "UTF-8"
                       , bool                writeXMLDecl = true
                       , const std::string  programName  = ""
                       , const std::string  programVersion = "");

};
/** @endcond */


/** @cond doxygenLibsbmlInternal */
class LIBLX_EXTERN XMLOwningOutputFileStream : public XMLOutputFileStream
{
public:

  /**
   * Creates a new XMLOutputStream that wraps stream.
   *
   * @ifnot hasDefaultArgs @htmlinclude warn-default-args-in-docs.html @endif@~
   */
  XMLOwningOutputFileStream (const std::string&  filename
                             , const std::string  encoding     = "UTF-8"
                             , bool                writeXMLDecl = true
                             , const std::string  programName  = ""
                             , const std::string  programVersion = "");

  virtual ~XMLOwningOutputFileStream();

};
/** @endcond */


LIBLX_CPP_NAMESPACE_END

#endif  /* __cplusplus */



#ifndef SWIG

LIBLX_CPP_NAMESPACE_BEGIN
BEGIN_C_DECLS


/**
 * Creates a new XMLOutputStream_t that wraps std output stream.
 *
 * @memberof XMLOutputStream_t
 */
LIBLX_EXTERN
XMLOutputStream_t *
XMLOutputStream_createAsStdout (const char * encoding, int writeXMLDecl);


/**
 * Creates a new XMLOutputStream_t instance for use as a standard output
 * stream (<em>stdout</em>).
 *
 * @copydetails doc_programname_arguments
 *
 * @copydetails doc_xml_declaration
 *
 * @param encoding the XML encoding to declare in the output. This value
 * should be <code>"UTF-8"</code> for SBML documents.  The default value
 * is <code>"UTF-8"</code> if no value is supplied for this parameter.
 *
 * @param writeXMLDecl whether to write a standard XML declaration at
 * the beginning of the content written on @p stream.  The default is
 * @c true.
 *
 * @param programName an optional program name to write as a comment
 * in the output stream.
 *
 * @param programVersion an optional version identification string to write
 * as a comment in the output stream.
 *
 * @memberof XMLOutputStream_t
 */
LIBLX_EXTERN
XMLOutputStream_t *
XMLOutputStream_createAsStdoutWithProgramInfo (const char * encoding,
                                               int writeXMLDecl,
                                               const char * programName,
                                               const char * programVersion);


/**
 * Creates a new XMLOutputStream_t instance for use as a string
 *
 * @memberof XMLOutputStream_t
 */
LIBLX_EXTERN
XMLOutputStream_t *
XMLOutputStream_createAsString (const char * encoding, int writeXMLDecl);


/**
 * Creates a new XMLOutputStream_t that wraps std string output stream
 * and adds program information as a comment.
 *
 * @memberof XMLOutputStream_t
 */
LIBLX_EXTERN
XMLOutputStream_t *
XMLOutputStream_createAsStringWithProgramInfo (const char * encoding,
                                               int writeXMLDecl,
                                               const char * programName,
                                               const char * programVersion);


/**
 * Creates a new XMLOutputStream_t that wraps std file output stream.
 *
 * @memberof XMLOutputStream_t
 */
LIBLX_EXTERN
XMLOutputStream_t *
XMLOutputStream_createFile (const char * filename, const char * encoding, int writeXMLDecl);


/**
 * Creates a new XMLOutputStream_t that wraps std file output stream
 * and adds program information as a comment.
 *
 * @memberof XMLOutputStream_t
 */
LIBLX_EXTERN
XMLOutputStream_t *
XMLOutputStream_createFileWithProgramInfo (const char * filename, const char * encoding,
                                           int writeXMLDecl, const char * programName,
                                           const char * programVersion);


/**
 * Deletes this XMLOutputStream_t.
 *
 * @memberof XMLOutputStream_t
 */
LIBLX_EXTERN
void
XMLOutputStream_free (XMLOutputStream_t *stream);


/**
 * Writes the XML declaration:
 * <?xml version="1.0" encoding="..."?>
 *
 * @memberof XMLOutputStream_t
 */
LIBLX_EXTERN
void
XMLOutputStream_writeXMLDecl (XMLOutputStream_t *stream);


/**
 * Increases the indentation level for this XMLOutputStream_t.
 *
 * @memberof XMLOutputStream_t
 */
LIBLX_EXTERN
void
XMLOutputStream_upIndent(XMLOutputStream_t *stream);


/**
 * Decreases the indentation level for this XMLOutputStream_t.
 *
 * @memberof XMLOutputStream_t
 */
LIBLX_EXTERN
void
XMLOutputStream_downIndent(XMLOutputStream_t *stream);


/**
 * Writes the given XML end element name to this XMLOutputStream_t.
 *
 * @memberof XMLOutputStream_t
 */
LIBLX_EXTERN
void
XMLOutputStream_endElement (XMLOutputStream_t *stream, const char* name);


/**
 * Writes the given XML end element 'prefix:name' to this
 * XMLOutputStream_t.
 *
 * @memberof XMLOutputStream_t
 */
LIBLX_EXTERN
void
XMLOutputStream_endElementTriple (XMLOutputStream_t *stream,
                                  const XMLTriple_t *triple);


/**
 * Turns automatic indentation on or off for this XMLOutputStream_t.
 *
 * @memberof XMLOutputStream_t
 */
LIBLX_EXTERN
void
XMLOutputStream_setAutoIndent (XMLOutputStream_t *stream, int indent);


/**
 * Writes the given XML start element name to this XMLOutputStream_t.
 *
 * @memberof XMLOutputStream_t
 */
LIBLX_EXTERN
void
XMLOutputStream_startElement (XMLOutputStream_t *stream, const char* name);


/**
 * Writes the given XML start element 'prefix:name' to this
 * XMLOutputStream_t.
 *
 * @memberof XMLOutputStream_t
 */
LIBLX_EXTERN
void
XMLOutputStream_startElementTriple (XMLOutputStream_t *stream,
                                    const XMLTriple_t *triple);


/**
 * Writes the given XML start and end element name to this XMLOutputStream_t.
 *
 * @memberof XMLOutputStream_t
 */
LIBLX_EXTERN
void
XMLOutputStream_startEndElement (XMLOutputStream_t *stream, const char* name);


/**
 * Writes the given XML start and end element 'prefix:name' to this
 * XMLOutputStream_t.
 *
 * @memberof XMLOutputStream_t
 */
LIBLX_EXTERN
void
XMLOutputStream_startEndElementTriple (XMLOutputStream_t *stream,
                                       const XMLTriple_t *triple);


/**
 * Writes the given attribute, name="value" to this XMLOutputStream_t.
 *
 * @memberof XMLOutputStream_t
 */
LIBLX_EXTERN
void
XMLOutputStream_writeAttributeChars (XMLOutputStream_t *stream,
                                     const char* name, const char* chars);


/**
 * Writes the given attribute, prefix:name="value" to this
 * XMLOutputStream_t.
 *
 * @memberof XMLOutputStream_t
 */
LIBLX_EXTERN
void
XMLOutputStream_writeAttributeCharsTriple (XMLOutputStream_t *stream,
                                           const XMLTriple_t *triple,
                                           const char* chars);

/**
 * Writes the given attribute, name="true" or name="false" to this
 * XMLOutputStream_t.
 *
 * @memberof XMLOutputStream_t
 */
LIBLX_EXTERN
void
XMLOutputStream_writeAttributeBool (XMLOutputStream_t *stream,
                                    const char* name,
                                    const int flag);

/**
 * Writes the given attribute, prefix:name="true" or prefix:name="false"
 * to this XMLOutputStream_t.
 *
 * @memberof XMLOutputStream_t
 */
LIBLX_EXTERN
void
XMLOutputStream_writeAttributeBoolTriple (XMLOutputStream_t *stream,
                                          const XMLTriple_t *triple,
                                          const int flag);


/**
 * Writes the given attribute, name="value" to this XMLOutputStream_t.
 *
 * @memberof XMLOutputStream_t
 */
LIBLX_EXTERN
void
XMLOutputStream_writeAttributeDouble (XMLOutputStream_t *stream,
                                      const char* name,
                                      const double value);


/**
 * Writes the given attribute, prefix:name="value" to this XMLOutputStream_t.
 *
 * @memberof XMLOutputStream_t
 */
LIBLX_EXTERN
void
XMLOutputStream_writeAttributeDoubleTriple (XMLOutputStream_t *stream,
                                            const XMLTriple_t *triple,
                                            const double value);


/**
 * Writes the given attribute, name="value" to this XMLOutputStream_t.
 *
 * @memberof XMLOutputStream_t
 */
LIBLX_EXTERN
void
XMLOutputStream_writeAttributeLong (XMLOutputStream_t *stream,
                                    const char* name,
                                    const long value);


/**
 * Writes the given attribute, prefix:name="value" to this XMLOutputStream_t.
 *
 * @memberof XMLOutputStream_t
 */
LIBLX_EXTERN
void
XMLOutputStream_writeAttributeLongTriple (XMLOutputStream_t *stream,
                                          const XMLTriple_t *triple,
                                          const long value);


/**
 * Writes the given attribute, name="value" to this XMLOutputStream_t.
 *
 * @memberof XMLOutputStream_t
 */
LIBLX_EXTERN
void
XMLOutputStream_writeAttributeInt (XMLOutputStream_t *stream,
                                   const char* name,
                                   const int value);


/**
 * Writes the given attribute, prefix:name="value" to this XMLOutputStream_t.
 *
 * @memberof XMLOutputStream_t
 */
LIBLX_EXTERN
void
XMLOutputStream_writeAttributeIntTriple (XMLOutputStream_t *stream,
                                         const XMLTriple_t *triple,
                                         const int value);


/**
 * Writes the given attribute, name="value" to this XMLOutputStream_t.
 *
 * @memberof XMLOutputStream_t
 */
LIBLX_EXTERN
void
XMLOutputStream_writeAttributeUInt (XMLOutputStream_t *stream,
                                    const char* name,
                                    const unsigned int value);


/**
 * Writes the given attribute, prefix:name="value" to this XMLOutputStream_t.
 *
 * @memberof XMLOutputStream_t
 */
LIBLX_EXTERN
void
XMLOutputStream_writeAttributeUIntTriple (XMLOutputStream_t *stream,
                                          const XMLTriple_t *triple,
                                          const unsigned int value);


/**
 * Outputs the given characters to the underlying stream.
 *
 * @memberof XMLOutputStream_t
 */
LIBLX_EXTERN
void
XMLOutputStream_writeChars (XMLOutputStream_t *stream, const char* chars);


/**
 * Outputs the given double to the underlying stream.
 *
 * @memberof XMLOutputStream_t
 */
LIBLX_EXTERN
void
XMLOutputStream_writeDouble (XMLOutputStream_t *stream, const double value);


/**
 * Outputs the given long to the underlying stream.
 *
 * @memberof XMLOutputStream_t
 */
LIBLX_EXTERN
void
XMLOutputStream_writeLong (XMLOutputStream_t *stream, const long value);


/**
 * Returns the given string associated to the underlying string stream.
 *
 * @memberof XMLOutputStream_t
 */
LIBLX_EXTERN
const char *
XMLOutputStream_getString(XMLOutputStream_t* stream);


END_C_DECLS
LIBLX_CPP_NAMESPACE_END

#endif  /* !SWIG */
#endif  /* XMLOutputStream_h */
