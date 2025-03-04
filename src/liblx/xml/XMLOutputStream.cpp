/**
 * @cond doxygenLibsbmlInternal
 *
 * @file    XMLOutputStream.cpp
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
 * ---------------------------------------------------------------------- -->*/

#include <iostream>
#include <sstream>
#include <fstream>

#include <cstdio>
#include <liblx/xml/XMLTriple.h>
#include <liblx/xml/XMLOutputStream.h>
#include <liblx/xml/XMLAttributes.h>
#include <liblx/xml/XMLConstructorException.h>
#include <liblx/xml/XMLNamespaces.h>
#include <liblx/xml/sbmlMemoryStubs.h>
#include <liblx/xml/sbmlMemoryStubs.h>
#include <liblx/xml/common/common.h>
#include <liblx/xml/common/liblx-version.h>
#if defined CYGWIN
#include <string.h>
#endif

using namespace std;

LIBLX_CPP_NAMESPACE_BEGIN
#ifdef __cplusplus

/**
 * Checks if the given string has a character reference at index in the string.
 *
 * character reference is expressed as follows:
 *
 *  CharRef ::=  '&#' [0-9]+ ';' | '&#x' [0-9a-fA-F]+ ';'
 *
 * This function is internal implementation.
 */
bool hasCharacterReference(const std::string &chars, size_t index)
{
  const std::string decChars = "0123456789";
  const std::string hexChars = "0123456789abcdefABCDEF";

  if ((chars.length() - 1) <= index)
  {
    return false;
  }
  else if (chars.at(index) != '&')
  {
    return false;
  }
  else if (chars.at(index+1) != '#')
  {
    return false;
  }
  else if (chars.at(index+2) == 'x')
  {
    //
    // the character reference uses hex characters (e.g. &#x00A8; ) if any
    //
    size_t pos = chars.find_first_not_of(hexChars, index+3);
    
    if (pos == std::string::npos)
    {
      // ';' is not found
      return false;
    }    
    else if (pos < index + 4)
    {
      // hex characters are not found
      return false;
    }
    else if (chars.at(pos) != ';')
    {
      // ';' is not found immediately after hex characters
      return false;
    }
  }
  else
  {
    //
    // the character reference uses deciaml characters (e.g. &#0185; ) if any
    //
    size_t pos = chars.find_first_not_of(decChars, index+2);
    
    if (pos == std::string::npos)
    {
      // ';' is not found
      return false;
    }    
    else if (pos < index + 3)
    {
      // decimal characters are not found
      return false;
    }
    else if (chars.at(pos) != ';')
    {
      // ';' is not found immediately after decimal characters
      return false;
    }
  }

  return true;
}   


/**
 * Checks if the given string has a predefined entity at index in the string.
 *
 * predefined entities are expressed as follows:
 *
 *  1) &amp;
 *  2) &apos;
 *  3) &lt;
 *  4) &gt;
 *  5) &quot;
 *
 * This function is internal implementation.
 */
bool hasPredefinedEntity(const std::string &chars, size_t index)
{
  if ((chars.length() - 1) <= index)
  {
    return false;
  }

  if (chars.find("&amp;",index) == index)
  {
    return true;
  }
  else if (chars.find("&apos;",index) == index)
  {
    return true;
  }
  else if (chars.find("&lt;",index) == index)
  {
    return true;
  }
  else if (chars.find("&gt;",index) == index)
  {
    return true;
  }
  else if (chars.find("&quot;",index) == index)
  {
    return true;
  }
     
  return false;
}   


// boolean indicating whether the comment on the top of the file is
// written (enabled by default)
bool XMLOutputStream::mWriteComment = true;

// boolean indicating whether a timestamp will be generated at the time
// of writing (enabled by default)
bool XMLOutputStream::mWriteTimestamp = true;

// the name of the library writing the file (i.e: libSBML)
std::string XMLOutputStream::mLibraryName = "libSBML";

// the version of the library writing the file
// TODO: Replace with a libSBXML version string
std::string XMLOutputStream::mLibraryVersion = getLibLXDottedVersion();


/**
 * Copy Constructor, made private so as to notify users, that copying an input stream is not supported. 
 */
XMLOutputStream::XMLOutputStream (const XMLOutputStream& other)
  : mStream(other.mStream)
  , mEncoding(other.mEncoding)
  , mInStart (other.mInStart)
  , mDoIndent(other.mDoIndent)
  , mIndent(other.mIndent)
  , mInText(other.mInText)
  , mSkipNextIndent(other.mSkipNextIndent)
  , mNextAmpersandIsRef(other.mNextAmpersandIsRef)
  , mStringStream(other.mStringStream)
{
}


/**
 * Assignment operator, made private so as to notify users, that copying an input stream is not supported. 
 */
XMLOutputStream& XMLOutputStream::operator=(const XMLOutputStream& /*other*/)
{
  return *this;
}


/*
 * Creates a new XMLOutputStream that wraps stream.
 */
XMLOutputStream::XMLOutputStream (  std::ostream&       stream
                                  , const std::string  encoding
                                  , bool                writeXMLDecl
                                  , const std::string  programName
                                  , const std::string  programVersion) :
   mStream  ( stream   )
 , mEncoding( encoding )
 , mInStart ( false    )
 , mDoIndent( true     )
 , mIndent  ( 0        )
 , mInText  ( false    )
 , mSkipNextIndent ( false    )
 , mNextAmpersandIsRef( false )
 , mXMLns (NULL)
{

  unsetStringStream();
  mStream.imbue( locale::classic() );
  if (writeXMLDecl) this->writeXMLDecl();
  if (mWriteComment) this->writeComment(programName, programVersion, mWriteTimestamp);
}


/*
 * Writes the given XML end element name to this XMLOutputStream.
 */
void
XMLOutputStream::endElement (const std::string& name, const std::string prefix)
{

  if (mInStart)
  {
    mInStart = false;
    mStream << '/' << '>';
  }
  else if (mInText)
  {
    mInText = false;
    mSkipNextIndent = false;
    mStream << '<' << '/';
    writeName(name, prefix);
    mStream << '>';
  }
  else
  {
    downIndent();
    writeIndent(true); 

    mStream << '<' << '/';
    writeName(name, prefix);
    mStream << '>';
  }
}


/*
 * Writes the given XML end element 'prefix:name' to this
 * XMLOutputStream.
 */
void
XMLOutputStream::endElement (const XMLTriple& triple, bool text)
{

  if (mInStart)
  {
    mInStart = false;
    mStream << '/' << '>';
  }
  else if (mInText || text)
  {
    mInText = false;
    mSkipNextIndent = false;
    mStream << '<' << '/';
    writeName(triple);
    mStream << '>';
  }
  else
  {
    downIndent();
    writeIndent(true); 

    mStream << '<' << '/';
    writeName(triple);
    mStream << '>';
  }
}


/*
 * Turns automatic indentation on or off for this XMLOutputStream.
 */
void
XMLOutputStream::setAutoIndent (bool indent)
{
  mDoIndent = indent;
}


/*
 * Writes the given XML start element name to this XMLOutputStream.
 */
void
XMLOutputStream::startElement (const std::string& name, const std::string prefix)
{

  if (mInStart)
  {
    mStream << '>';
    upIndent();
  }

  mInStart = true;

  if (mInText && mSkipNextIndent)
  {
    mSkipNextIndent = false;
  }
  else
  {
    writeIndent();
  }

  mStream << '<';
  writeName(name, prefix);
}


/*
 * Writes the given XML start element 'prefix:name' to this
 * XMLOutputStream.
 */
void
XMLOutputStream::startElement (const XMLTriple& triple)
{

  if (mInStart)
  {
    mStream << '>';
    upIndent();
  }

  mInStart = true;

  if (mInText && mSkipNextIndent)
  {
    mSkipNextIndent = false;
  }
  else
  {
    writeIndent();
  }

  mStream << '<';
  writeName(triple);
}


/*
 * Writes the given XML start and end element name to this XMLOutputStream.
 */
void
XMLOutputStream::startEndElement (const std::string& name, const std::string prefix)
{

  if (mInStart)
  {
    mStream << '>';
    upIndent();
  }

  if (mSkipNextIndent)
    mSkipNextIndent = false;

  mInStart = false;

  if (mInText && mSkipNextIndent)
  {
    mSkipNextIndent = false;
  }
  else
  {
    writeIndent();
  }

  mStream << '<';
  writeName(name, prefix);
  mStream << '/' << '>';
}


/*
 * Writes the given XML start and end element 'prefix:name' to this
 * XMLOutputStream.
 */
void
XMLOutputStream::startEndElement (const XMLTriple& triple)
{

  if (mInStart)
  {
    mStream << '>';
    upIndent();
  }

  if (mSkipNextIndent)
    mSkipNextIndent = false;

  mInStart = false;

  if (mInText && mSkipNextIndent)
  {
    mSkipNextIndent = false;
  }
  else
  {
    writeIndent();
  }

  mStream << '<';
  writeName(triple);
  mStream << '/' << '>';
}


/*
 * Writes the given attribute, name="value" to this XMLOutputStream.
 */
void
XMLOutputStream::writeAttribute (const std::string& name, const std::string& value)
{
  if ( value.empty() ) return; 

  mStream << ' ';

  writeName ( name  );
  writeValue( value );
}


/*
 * Writes the given attribute, prefix:name="value" to this XMLOutputStream.
 */
void
XMLOutputStream::writeAttribute (const std::string& name, const std::string& prefix, const std::string& value)
{
  if ( value.empty() ) return;

  mStream << ' ';

  writeName ( name , prefix );
  writeValue( value );
}


/*
 * Writes the given attribute, prefix:name="value" to this
 * XMLOutputStream.
 */
void
XMLOutputStream::writeAttribute (const XMLTriple& triple, const std::string& value)
{
  mStream << ' ';

  writeName ( triple );
  writeValue( value  );
}


/*
 * Writes the given attribute, name="value" to this XMLOutputStream.
 */
void
XMLOutputStream::writeAttribute (const std::string& name, const char* value)
{
  if ( !value || strcmp(value,"") == 0) return;

  mStream << ' ';
  
  writeName ( name  );
  writeValue( value );
}


/*
 * Writes the given attribute, prefix:name="value" to this XMLOutputStream.
 */
void
XMLOutputStream::writeAttribute (const std::string& name, const std::string& prefix, const char* value)
{
  if ( !value || strcmp(value,"") == 0) return;

  mStream << ' ';

  writeName ( name , prefix );
  writeValue( value );
}


/*
 * Writes the given attribute, prefix:name="value" to this
 * XMLOutputStream.
 */
void
XMLOutputStream::writeAttribute (const XMLTriple& triple, const char* value)
{
  if ( !value || strcmp(value,"") == 0) return;

  mStream << ' ';

  writeName ( triple );
  writeValue( value  );
}


/*
 * Writes the given attribute, name="true" or name="false" to this
 * XMLOutputStream.
 */
void
XMLOutputStream::writeAttribute (const std::string& name, const bool& value)
{

  mStream << ' ';

  writeName ( name  );
  writeValue( value );
}


/*
 * Writes the given attribute, prefix:name="true" or prefix:name="false" to this
 * XMLOutputStream.
 */
void
XMLOutputStream::writeAttribute (const std::string& name, const std::string& prefix, const bool& value)
{
  mStream << ' ';

  writeName ( name , prefix );
  writeValue( value );
}


/*
 * Writes the given attribute, prefix:name="true" or prefix:name="false" to
 * this XMLOutputStream.
 */
void
XMLOutputStream::writeAttribute (const XMLTriple& triple, const bool& value)
{

  mStream << ' ';

  writeName ( triple );
  writeValue( value  );
}


/*
 * Writes the given attribute, name="value" to this XMLOutputStream.
 */
void
XMLOutputStream::writeAttribute (const std::string& name, const double& value)
{

  mStream << ' ';

  writeName ( name  );
  writeValue( value );
}


/*
 * Writes the given attribute, prefix:name="value" to this XMLOutputStream.
 */
void
XMLOutputStream::writeAttribute (const std::string& name, const std::string& prefix, const double& value)
{
  mStream << ' ';

  writeName ( name , prefix );
  writeValue( value );
}


/*
 * Writes the given attribute, prefix:name="value" to this XMLOutputStream.
 */
void
XMLOutputStream::writeAttribute (const XMLTriple& triple, const double& value)
{
  mStream << ' ';

  writeName ( triple );
  writeValue( value  );
}


/*
 * Writes the given attribute, name="value" to this XMLOutputStream.
 */
void
XMLOutputStream::writeAttribute (const std::string& name, const long& value)
{

  mStream << ' ';

  writeName ( name  );
  writeValue( value );
}


/*
 * Writes the given attribute, prefix:name="value" to this XMLOutputStream.
 */
void
XMLOutputStream::writeAttribute (const std::string& name, const std::string& prefix, const long& value)
{
  mStream << ' ';

  writeName ( name , prefix );
  writeValue( value );
}



/*
 * Writes the given attribute, prefix:name="value" to this XMLOutputStream.
 */
void
XMLOutputStream::writeAttribute (const XMLTriple& triple, const long& value)
{
  mStream << ' ';

  writeName ( triple );
  writeValue( value  );
}


/*
 * Writes the given attribute, name="value" to this XMLOutputStream.
 */
void
XMLOutputStream::writeAttribute (const std::string& name, const int& value)
{

  mStream << ' ';

  writeName ( name  );
  writeValue( value );
}


/*
 * Writes the given attribute, prefix:name="value" to this XMLOutputStream.
 */
void
XMLOutputStream::writeAttribute (const std::string& name, const std::string& prefix, const int& value)
{
  mStream << ' ';

  writeName ( name , prefix );
  writeValue( value );
}


/*
 * Writes the given attribute, prefix:name="value" to this
 * XMLOutputStream.
 */
void
XMLOutputStream::writeAttribute (const XMLTriple& triple, const int& value)
{

  mStream << ' ';

  writeName ( triple );
  writeValue( value  );
}


/*
 * Writes the given attribute, name="value" to this XMLOutputStream.
 */
void
XMLOutputStream::writeAttribute (const std::string& name, const unsigned int& value)
{

  mStream << ' ';

  writeName ( name  );
  writeValue( value );
}


/*
 * Writes the given attribute, prefix:name="value" to this XMLOutputStream.
 */
void
XMLOutputStream::writeAttribute (const std::string& name, const std::string& prefix, const unsigned int& value)
{
  mStream << ' ';

  writeName ( name , prefix );
  writeValue( value );
}


/*
 * Writes the given attribute, prefix:name="value" to this
 * XMLOutputStream.
 */
void
XMLOutputStream::writeAttribute (  const XMLTriple&     triple
                                 , const unsigned int&  value )
{

  mStream << ' ';

  writeName ( triple );
  writeValue( value  );
}


/*
 * Decreases the indentation level for this XMLOutputStream.
 */
void
XMLOutputStream::downIndent ()
{
  if (mDoIndent && mIndent) --mIndent;
}


/*
 * Increases the indentation level for this XMLOutputStream.
 */
void
XMLOutputStream::upIndent ()
{
  if (mDoIndent) ++mIndent;
}

bool XMLOutputStream::getStringStream()
{
  return mStringStream;
}


/*
 * Outputs indentation whitespace.
 */
void
XMLOutputStream::writeIndent (bool isEnd)
{
  if (mDoIndent)
  {
    if (mIndent > 0 || isEnd) mStream << endl;
    for (unsigned int n = 0; n < mIndent; ++n) mStream << ' ' << ' ';
  }
}


/*
 * Outputs the given characters to the underlying stream.
 */
void
XMLOutputStream::writeChars (const std::string& chars)
{
  for (size_t i=0; i < chars.length(); i++)
  {
    const char& c = chars.at(i);
    if ( c == '&' && 
        (LIBLX_CPP_NAMESPACE ::hasCharacterReference(chars, i) ||
         LIBLX_CPP_NAMESPACE ::hasPredefinedEntity(chars,i)) )
      mNextAmpersandIsRef = true;

    *this << c;
  }
}


/*
 * Outputs name.
 */
void
XMLOutputStream::writeName (const std::string& name, const std::string prefix)
{
  if ( !prefix.empty() )
  {
    writeChars( prefix );
    mStream << ':';
  }

  writeChars(name);
}


/*
 * Outputs prefix:name.
 */
void
XMLOutputStream::writeName (const XMLTriple& triple)
{
  if ( !triple.getPrefix().empty() )
  {
    writeChars( triple.getPrefix() );
    mStream << ':';
  }

  writeChars( triple.getName() );
}


/*
 * Outputs value in quotes.
 */
void
XMLOutputStream::writeValue (const std::string& value)
{
  mStream << '=' << '"';
  writeChars(value);
  mStream << '"';
}

/*
 * Outputs value in quotes.
 */
void
XMLOutputStream::writeValue (const char* value)
{
  mStream << '=' << '"';
  writeChars(value);
  mStream << '"';
}


/*
 * Outputs "true" or "false" in quotes.
 */
void
XMLOutputStream::writeValue (const bool& value)
{
  mStream << '=' << '"' << (value ? "true" : "false") << '"';
}


/*
 * Outputs the double value in quotes, or "INF", "-INF", or "NaN".
 */
void
XMLOutputStream::writeValue (const double& value)
{
  mStream << '=' << '"';

  if (value != value)
  {
    mStream << "NaN";
  }
  else if (value == numeric_limits<double>::infinity())
  {
    mStream << "INF";
  }
  else if (value == - numeric_limits<double>::infinity())
  {
    mStream << "-INF";
  }
  else
  {
    mStream.precision(LIBLX_DOUBLE_PRECISION);
    mStream <<   value;
  }

  mStream << '"';
}


/*
 * Outputs the long value in quotes.
 */
void
XMLOutputStream::writeValue (const long& value)
{
  mStream << '=' << '"' << value << '"';
}


/*
 * Outputs the int value in quotes.
 */
void
XMLOutputStream::writeValue (const int& value)
{
  mStream << '=' << '"' << value << '"';
}


/*
 * Outputs the int value in quotes.
 */
void
XMLOutputStream::writeValue (const unsigned int& value)
{
  mStream << '=' << '"' << value << '"';
}

void
XMLOutputStream::setStringStream()
{
    mStringStream = true;
}

void
XMLOutputStream::unsetStringStream()
{
    mStringStream = false;
}


/*
 * Writes the XML declaration:
 * <?xml version="1.0" encoding="..."?>
 */
void
XMLOutputStream::writeXMLDecl ()
{
  mStream << "<?xml version=\"1.0\"";

  if ( !mEncoding.empty() ) writeAttribute("encoding", mEncoding);

  mStream << "?>";
  mStream << endl;
}


/*
 * Writes the XML comment:
 *   <!-- Created by <program name> version <program version>
 *   on yyyy-MM-dd HH:mm with libsbml version <libsbml version>. -->
 */
void
XMLOutputStream::writeComment (const std::string& programName, 
                               const std::string& programVersion,
                               bool writeTimestamp)
{
  // don't write without program name
  if (programName.empty())
    return;

  mStream << "<!-- Created by " << programName;

  // only write program version if we have it
  if (!programVersion.empty())
  {
    mStream << " version " << programVersion;
  }

  // only compute timestamp if we need to
  if (writeTimestamp)
  {
    char formattedDateAndTime[17];
    time_t tim=time(NULL);
    tm *now=localtime(&tim);

    sprintf(formattedDateAndTime, "%d-%02d-%02d %02d:%02d",
            now->tm_year+1900, now->tm_mon+1, now->tm_mday,
            now->tm_hour, now->tm_min);
    mStream << " on " << formattedDateAndTime;
  }

  // write library information
  if (!mLibraryName.empty())
  {
    mStream << " with " << mLibraryName;

    if (!mLibraryVersion.empty())
    {
      mStream << " version " << mLibraryVersion;
    }
  }

  mStream << ". -->";
  mStream << endl;

}


/*
 * Outputs the given characters to the underlying stream.
 */
XMLOutputStream&
XMLOutputStream::operator<< (const std::string& chars)
{
  if (mInStart)
  {
    mInStart = false;
    mStream << '>';
  }

  writeChars(chars);
  mInText = true;
  mSkipNextIndent = true;

  return *this;
}


/*
 * Outputs the given double to the underlying stream.
 */
XMLOutputStream&
XMLOutputStream::operator<< (const double& value)
{
  if (mInStart)
  {
    mInStart = false;
    mStream << '>';
  }

  mStream << value;

  return *this;
}


/*
 * Outputs the given long to the underlying stream.
 */
XMLOutputStream&
XMLOutputStream::operator<< (const long& value)
{
  if (mInStart)
  {
    mInStart = false;
    mStream << '>';
  }

  mStream << value;

  return *this;
}


/**
 * Outputs a single character to the underlying stream.
 */
XMLOutputStream& 
XMLOutputStream::operator<< (const char& c)
{
  if (c == '&' && mNextAmpersandIsRef)
  {
    // outputs '&' as-is because the '&' is the first letter
    // of a character reference (e.g. &#0168; )
    mStream << c;
    mNextAmpersandIsRef = false;
    return *this;
  }
  
  switch (c)
  {
    case '&' : mStream << "&amp;" ; break;
    case '\'': mStream << "&apos;"; break;
    case '<' : mStream << "&lt;"  ; break;
    case '>' : mStream << "&gt;"  ; break;
    case '"' : mStream << "&quot;"; break;
    default  : mStream << c;        break;
  }

  return *this;
}


XMLNamespaces *
XMLOutputStream::getXMLNamespaces()
{
  return mXMLns;
}


void
XMLOutputStream::setXMLNamespaces(XMLNamespaces * xmlns)
{
  if (mXMLns  != NULL)
    delete mXMLns;
  
  if (xmlns != NULL)
    mXMLns = xmlns->clone();
  else
    mXMLns = NULL;
}

bool XMLOutputStream::getWriteComment()
{
  return mWriteComment;
}

void XMLOutputStream::setWriteComment(bool writeComment)
{
  mWriteComment = writeComment;
}

bool XMLOutputStream::getWriteTimestamp()
{
  return mWriteTimestamp;
}

void XMLOutputStream::setWriteTimestamp(bool writeTimestamp)
{
  mWriteTimestamp = writeTimestamp;
}

string XMLOutputStream::getLibraryName()
{
  return mLibraryName;
}

void XMLOutputStream::setLibraryName(const string& libraryName)
{
  mLibraryName = libraryName;
}

string XMLOutputStream::getLibraryVersion()
{
  return mLibraryVersion;
}

void XMLOutputStream::setLibraryVersion(const string& libraryVersion)
{
  mLibraryVersion = libraryVersion;
}

unsigned int XMLOutputStream::getIndent()
{
  return mIndent;
}

void XMLOutputStream::setIndent(unsigned int indent)
{
  mIndent = indent;
}

XMLOutputStream::~XMLOutputStream()
{
  if (mXMLns != NULL)
    delete mXMLns;
}



XMLOutputStringStream::XMLOutputStringStream (  std::ostringstream& stream
                   , const std::string  encoding
                   , bool                writeXMLDecl
                   , const std::string  programName
                   , const std::string  programVersion):
  XMLOutputStream(stream, encoding, writeXMLDecl, 
                    programName, programVersion)
    , mString(stream)

{
  setStringStream();
}

std::ostringstream &
XMLOutputStringStream::getString()
{
  return mString;
}

XMLOwningOutputStringStream::XMLOwningOutputStringStream (const std::string  encoding
                               , bool                writeXMLDecl
                               , const std::string  programName
                               , const std::string  programVersion)
  : XMLOutputStringStream(*(new std::ostringstream), encoding, writeXMLDecl, programName, programVersion)
{
  
}

XMLOwningOutputStringStream::~XMLOwningOutputStringStream()
{
  delete &mStream;
}


XMLOutputFileStream::XMLOutputFileStream (std::ofstream& stream
                   , const std::string  encoding
                   , bool                writeXMLDecl
                   , const std::string  programName
                   , const std::string  programVersion)
  : XMLOutputStream(stream, encoding, writeXMLDecl, 
                    programName, programVersion)
{
}

XMLOwningOutputFileStream::XMLOwningOutputFileStream (  
                               const std::string&  filename
                             , const std::string  encoding
                             , bool                writeXMLDecl
                             , const std::string  programName
                             , const std::string  programVersion)
  : XMLOutputFileStream( *(new std::ofstream(filename.c_str(), std::ios::out)), 
                         encoding, writeXMLDecl, programName, programVersion)
{
}

XMLOwningOutputFileStream::~XMLOwningOutputFileStream()
{
  delete &mStream;
}

#endif /* __cplusplus */


LIBLX_EXTERN
XMLOutputStream_t *
XMLOutputStream_createAsStdout (const char * encoding, int writeXMLDecl)
{
  if (encoding == NULL) return NULL;
  return new(nothrow) XMLOutputStream(std::cout, encoding, writeXMLDecl);
}


LIBLX_EXTERN
XMLOutputStream_t *
XMLOutputStream_createAsStdoutWithProgramInfo (const char * encoding,
        int writeXMLDecl, const char * programName, const char * programVersion)
{
  if (encoding == NULL) return NULL;
  return new(nothrow) XMLOutputStream(std::cout, encoding, writeXMLDecl,
                                 programName, programVersion);
}


LIBLX_EXTERN
XMLOutputStream_t *
XMLOutputStream_createAsString (const char * encoding, int writeXMLDecl)
{
  if (encoding == NULL) return NULL;

  return new(nothrow) XMLOwningOutputStringStream(encoding, writeXMLDecl);
}


LIBLX_EXTERN
XMLOutputStream_t *
XMLOutputStream_createAsStringWithProgramInfo (const char * encoding,
        int writeXMLDecl, const char * programName, const char * programVersion)
{
  if (encoding == NULL) return NULL;

  return new(nothrow) XMLOwningOutputStringStream(encoding, writeXMLDecl,
                             programName, programVersion);
}


LIBLX_EXTERN
XMLOutputStream_t *
XMLOutputStream_createFile (const char * filename, const char * encoding, 
                            int writeXMLDecl)
{
  if (filename == NULL || encoding == NULL) return NULL;

  XMLOutputStream_t * out = new(nothrow) 
                           XMLOwningOutputFileStream(filename, encoding, writeXMLDecl);

  return out;
}


LIBLX_EXTERN
XMLOutputStream_t *
XMLOutputStream_createFileWithProgramInfo (const char * filename, 
                                           const char * encoding, 
        int writeXMLDecl, const char * programName, const char * programVersion)
{
  if (filename == NULL || encoding == NULL) return NULL;

  XMLOutputStream_t * out = new(nothrow) 
                           XMLOwningOutputFileStream(filename, encoding, writeXMLDecl,
                           programName, programVersion);
  
  return out;
}


LIBLX_EXTERN
void
XMLOutputStream_free (XMLOutputStream_t *stream)
{
  if (stream == NULL) return;
  delete static_cast<XMLOutputStream*>(stream);
}  


LIBLX_EXTERN
void 
XMLOutputStream_writeXMLDecl (XMLOutputStream_t *stream)
{
  if (stream == NULL) return; 
  stream->writeXMLDecl();
}


LIBLX_EXTERN
void
XMLOutputStream_upIndent(XMLOutputStream_t *stream)
{
  if (stream == NULL) return; 
  stream->upIndent();
}


LIBLX_EXTERN
void
XMLOutputStream_downIndent(XMLOutputStream_t *stream)
{
  if (stream == NULL) return;
  stream->downIndent();
}


LIBLX_EXTERN
void 
XMLOutputStream_endElement (XMLOutputStream_t *stream, const char* name)
{
  if(stream == NULL) return;
  stream->endElement(name);
}


LIBLX_EXTERN
void 
XMLOutputStream_endElementTriple (XMLOutputStream_t *stream, 
                                  const XMLTriple_t *triple)
{
  if (stream == NULL || triple == NULL) return;
  stream->endElement(*triple);
}


LIBLX_EXTERN
void 
XMLOutputStream_setAutoIndent (XMLOutputStream_t *stream, int indent)
{
  if (stream == NULL) return; 
  stream->setAutoIndent(static_cast<bool>(indent));
}


LIBLX_EXTERN
void 
XMLOutputStream_startElement (XMLOutputStream_t *stream, const char* name)
{
  if (stream == NULL) return;
  stream->startElement(name);
}


LIBLX_EXTERN
void 
XMLOutputStream_startElementTriple (XMLOutputStream_t *stream, 
                                    const XMLTriple_t *triple)
{
  if (stream == NULL || triple == NULL) return;
  stream->startElement(*triple);
}


LIBLX_EXTERN
void 
XMLOutputStream_startEndElement (XMLOutputStream_t *stream, const char* name)
{
  if (stream == NULL) return;
  stream->startEndElement(name);
}


LIBLX_EXTERN
void 
XMLOutputStream_startEndElementTriple (XMLOutputStream_t *stream, 
                                       const XMLTriple_t *triple)
{
  if (stream == NULL || triple == NULL) return; 
  stream->startEndElement(*triple);
}


LIBLX_EXTERN
void 
XMLOutputStream_writeAttributeChars (XMLOutputStream_t *stream, 
                                     const char* name, const char* chars)
{
  if (stream == NULL) return;
  stream->writeAttribute(name, string(chars));
}


LIBLX_EXTERN
void 
XMLOutputStream_writeAttributeCharsTriple (XMLOutputStream_t *stream, 
                                           const XMLTriple_t *triple,
                                           const char* chars)
{
  if (stream == NULL || triple == NULL) return;
  stream->writeAttribute(*triple, string(chars));
}


LIBLX_EXTERN
void 
XMLOutputStream_writeAttributeBool (XMLOutputStream_t *stream, 
                                    const char* name,
                                    const int flag)
{
  if (stream == NULL) return; 
  stream->writeAttribute(name, static_cast<bool>(flag));
}


LIBLX_EXTERN
void 
XMLOutputStream_writeAttributeBoolTriple (XMLOutputStream_t *stream, 
                                          const XMLTriple_t *triple,
                                          const int flag)
{
  if (stream == NULL || triple == NULL) return;
  stream->writeAttribute(*triple, static_cast<bool>(flag));
}


LIBLX_EXTERN
void 
XMLOutputStream_writeAttributeDouble (XMLOutputStream_t *stream, 
                                      const char* name,
                                      const double value)
{
  if (stream == NULL) return;
  stream->writeAttribute(name, value);
}


LIBLX_EXTERN
void 
XMLOutputStream_writeAttributeDoubleTriple (XMLOutputStream_t *stream, 
                                            const XMLTriple_t *triple,
                                            const double value)
{
  if (stream == NULL || triple == NULL) return;
  stream->writeAttribute(*triple, value);
}


LIBLX_EXTERN
void 
XMLOutputStream_writeAttributeLong (XMLOutputStream_t *stream, 
                                    const char* name,
                                    const long value)
{
  if (stream == NULL) return; 
  stream->writeAttribute(name, value);
}


LIBLX_EXTERN
void 
XMLOutputStream_writeAttributeLongTriple (XMLOutputStream_t *stream, 
                                          const XMLTriple_t *triple,
                                          const long value)
{
  if (stream == NULL || triple == NULL) return; 
  stream->writeAttribute(*triple, value);
}


LIBLX_EXTERN
void 
XMLOutputStream_writeAttributeInt (XMLOutputStream_t *stream, 
                                   const char* name,
                                   const int value)
{
  if (stream == NULL) return;
  stream->writeAttribute(name, value);
}


LIBLX_EXTERN
void 
XMLOutputStream_writeAttributeIntTriple (XMLOutputStream_t *stream, 
                                         const XMLTriple_t *triple,
                                         const int value)
{
  if (stream == NULL || triple == NULL) return;
  stream->writeAttribute(*triple, value);
}


LIBLX_EXTERN
void 
XMLOutputStream_writeAttributeUInt (XMLOutputStream_t *stream, 
                                    const char* name,
                                    const unsigned int value)
{
  if (stream == NULL) return; 
  stream->writeAttribute(name, value);
}


LIBLX_EXTERN
void 
XMLOutputStream_writeAttributeUIntTriple (XMLOutputStream_t *stream, 
                                          const XMLTriple_t *triple,
                                          const unsigned int value)
{
  if (stream == NULL) return; 
  stream->writeAttribute(*triple, value);
}


LIBLX_EXTERN
void
XMLOutputStream_writeChars (XMLOutputStream_t *stream, const char* chars)
{
  if (stream == NULL || chars == NULL) return; 
  stream->operator <<(chars);
}


LIBLX_EXTERN
void
XMLOutputStream_writeDouble (XMLOutputStream_t *stream, const double value)
{
  if (stream == NULL) return; 
  stream->operator <<(value);
}


LIBLX_EXTERN
void
XMLOutputStream_writeLong (XMLOutputStream_t *stream, const long value)
{
  if (stream == NULL) return; 
  stream->operator <<(value);
}


LIBLX_EXTERN
const char *
XMLOutputStream_getString(XMLOutputStream_t* stream)
{
  if (stream == NULL) return NULL;

  if (stream->getStringStream())
  {
    std::string buffer = static_cast <XMLOutputStringStream*>
                                                  (stream)->getString().str();
    return safe_strdup(buffer.c_str());
  }
  else
    return "";
}


LIBLX_CPP_NAMESPACE_END
/** @endcond */
