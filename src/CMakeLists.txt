###############################################################################
# Description       : CMake build script for libLX
# Original author(s): Frank Bergmann <fbergman@caltech.edu>
# Organization      : California Institute of Technology
#
# This file is part of libLX.  Please visit http://sbml.org for more
# information about LX, and the latest version of libLX.
#
# Copyright (C) 2013-2018 jointly by the following organizations:
#     1. California Institute of Technology, Pasadena, CA, USA
#     2. EMBL European Bioinformatics Institute (EMBL-EBI), Hinxton, UK
#     3. University of Heidelberg, Heidelberg, Germany
#
# Copyright (C) 2009-2013 jointly by the following organizations:
#     1. California Institute of Technology, Pasadena, CA, USA
#     2. EMBL European Bioinformatics Institute (EMBL-EBI), Hinxton, UK
#
# Copyright (C) 2006-2008 by the California Institute of Technology,
#     Pasadena, CA, USA
#
# Copyright (C) 2002-2005 jointly by the following organizations:
#     1. California Institute of Technology, Pasadena, CA, USA
#     2. Japan Science and Technology Agency, Japan
#
# This library is free software; you can redistribute it and/or modify it
# under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation.  A copy of the license agreement is provided
# in the file named "LICENSE.txt" included with this software distribution
# and also available online as http://sbml.org/software/libsbml/license.html
#
###############################################################################

# used to create CMake config files for projects using this library
if (${CMAKE_VERSION} VERSION_GREATER 2.8.7)
include(CMakePackageConfigHelpers)
endif()

###############################################################################
#
# Initialize LX sources
#
set(LIBLX_SOURCES)

###############################################################################
#
# create liblx-config-common.h
#
include(CheckIncludeFiles)
check_include_files (check.h HAVE_CHECK_H)
check_include_files (expat.h HAVE_EXPAT_H)
check_include_files (errno.h HAVE_ERRNO_H)
check_include_files (ieeefp.h HAVE_IEEEFP_H)
check_include_files (math.h HAVE_MATH_H)
check_include_files (sys/types.h HAVE_SYS_TYPES_H)
check_include_files (float.h STDC_HEADERS)
check_include_files (stdarg.h STDC_HEADERS)
check_include_files (stdlib.h STDC_HEADERS)
check_include_files (string.h STDC_HEADERS)

set(WORDS_BIGENDIAN)
if (UNIX)
  include (TestBigEndian)
  test_big_endian(WORDS_BIGENDIAN)
else()
  # test_big_endian seems to fail with nmake (VS 2010) on windows
  # however, windows is always little endian, so catch this here
  set(WORDS_BIGENDIAN FALSE)
endif()

# for whatever reason some of the values are not correctly picked up
# so we encode these values here
if (MSVC)
  set(HAVE_ERRNO_H TRUE)
  set(STDC_HEADERS TRUE)
  set(HAVE_MATH_H TRUE)
  set(HAVE_SYS_TYPES_H TRUE)
endif()


include(CheckLibraryExists)
check_library_exists(m sqrt "" HAVE_LIBM)

# generate configuration for this system
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/liblx/xml/common/libsbxml-config-common.h.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/liblx/xml/common/libsbxml-config-common.h
)

###############################################################################
#
# mark header files for installation
#

foreach(dir common compress xml)
	file(GLOB header_files "${CMAKE_CURRENT_SOURCE_DIR}/liblx/xml/${dir}/*.h")
	install(FILES ${header_files} DESTINATION include/liblx/xml/${dir})

endforeach(dir)


file(GLOB files "${CMAKE_CURRENT_SOURCE_DIR}/sbml/*.h")
install(FILES ${files} DESTINATION include/sbml)


###############################################################################
#
# copy build specific header files
#

file(GLOB additional_common "${CMAKE_CURRENT_BINARY_DIR}/liblx/xml/common/*.h")
install(FILES ${additional_common} DESTINATION include/liblx/xml/common)


###############################################################################
#
# specify include directories
#

include_directories(BEFORE ${LIBLX_ROOT_SOURCE_DIR}/src)
include_directories(BEFORE ${LIBLX_ROOT_SOURCE_DIR}/src/liblx)
include_directories(BEFORE ${LIBLX_ROOT_SOURCE_DIR}/src/liblx/xml)
include_directories(BEFORE ${CMAKE_CURRENT_BINARY_DIR})
include_directories(BEFORE ${CMAKE_CURRENT_BINARY_DIR}/src)
include_directories(BEFORE ${CMAKE_CURRENT_BINARY_DIR}/src/liblx)
include_directories(BEFORE ${CMAKE_CURRENT_BINARY_DIR}/src/liblx/xml)
if (EXTRA_INCLUDE_DIRS) 
 include_directories(${EXTRA_INCLUDE_DIRS})
endif(EXTRA_INCLUDE_DIRS)

###############################################################################
#
# Find all sources
#

macro(ADD_FUNCTION directory)

    set(prefix "liblx/xml/")

    if ("${directory}" STREQUAL "liblx/xml")
        set(prefix "")
    endif()

	
    file(GLOB temp
            ${prefix}${directory}/*.cpp
            ${prefix}${directory}/*.cxx
            ${prefix}${directory}/*.c
            ${prefix}${directory}/*.h)
    # create source group for IDEs
    source_group(${directory} FILES ${temp})

    # add to liblx sources
    set(LIBLX_SOURCES ${LIBLX_SOURCES} ${temp})

endmacro(ADD_FUNCTION)

foreach (directory common)

	ADD_FUNCTION(${directory})
				       
endforeach()

###############################################################################
# Find compression sources and adjust include and lib directory
#
set (COMPRESS_SOURCES)
set(COMPRESS_SOURCES ${COMPRESS_SOURCES}
    liblx/xml/compress/CompressCommon.h
    liblx/xml/compress/CompressCommon.cpp
    liblx/xml/compress/InputDecompressor.cpp
    liblx/xml/compress/InputDecompressor.h
    liblx/xml/compress/OutputCompressor.cpp
    liblx/xml/compress/OutputCompressor.h
    )

if(WITH_BZIP2)

  set(COMPRESS_SOURCES ${COMPRESS_SOURCES}
        liblx/xml/compress/bzfstream.h
        liblx/xml/compress/bzfstream.cpp
        )
  include_directories(${LIBBZ_INCLUDE_DIR})
  set(LIBLX_LIBS ${LIBLX_LIBS} ${LIBBZ_LIBRARY})

endif()

if(WITH_ZLIB)

set(COMPRESS_SOURCES ${COMPRESS_SOURCES}
        liblx/xml/compress/zip.c
        liblx/xml/compress/zip.h
        liblx/xml/compress/unzip.h
        liblx/xml/compress/unzip.c
        liblx/xml/compress/ioapi.h
        liblx/xml/compress/ioapi.c
        liblx/xml/compress/ioapi_mem.h
        liblx/xml/compress/ioapi_mem.c
        liblx/xml/compress/zfstream.h
        liblx/xml/compress/zfstream.cpp
        liblx/xml/compress/zipfstream.cpp
        liblx/xml/compress/zipfstream.h
    )

    if (WIN32)
        #
        # compress/iowin32.c contains only symbols on WIN32 if we include
        # it on other platforms a warning is issued by ranlib about
        # no symbols. So it will only be included on windows.
        #
        set(COMPRESS_SOURCES ${COMPRESS_SOURCES}
            liblx/xml/compress/iowin32.c
            liblx/xml/compress/iowin32.h
        )
    endif()

    include_directories(${LIBZ_INCLUDE_DIR})
    set(LIBLX_LIBS ${LIBLX_LIBS} ${LIBZ_LIBRARY})

endif()

source_group(compress FILES ${COMPRESS_SOURCES})
set(LIBLX_SOURCES ${LIBLX_SOURCES} ${COMPRESS_SOURCES})

###############################################################################
#
# Find xml sources and adjust include and lib directory
#

set(XML_SOURCES)
set(XML_SOURCES ${XML_SOURCES}
	liblx/xml/sbmlStubs.cpp
	)

set(XML_SOURCES ${XML_SOURCES}
	liblx/xml/common/liblx-version.cpp
	)
	
set(XML_SOURCES ${XML_SOURCES}

  liblx/xml/XMLAttributes.cpp
  liblx/xml/XMLBuffer.cpp
  liblx/xml/XMLConstructorException.cpp
  liblx/xml/XMLError.cpp
  liblx/xml/XMLErrorLog.cpp
  liblx/xml/XMLLogOverride.cpp
  liblx/xml/XMLFileBuffer.cpp
  liblx/xml/XMLHandler.cpp
  liblx/xml/XMLInputStream.cpp
  liblx/xml/XMLMemoryBuffer.cpp
  liblx/xml/XMLNamespaces.cpp
  liblx/xml/XMLNode.cpp
  liblx/xml/XMLOutputStream.cpp
  liblx/xml/XMLParser.cpp
  liblx/xml/XMLToken.cpp
  liblx/xml/XMLTokenizer.cpp
  liblx/xml/XMLTriple.cpp
  liblx/xml/XMLAttributes.h
  liblx/xml/XMLBuffer.h
  liblx/xml/XMLConstructorException.h
  liblx/xml/XMLError.h
  liblx/xml/XMLErrorLog.h
  liblx/xml/XMLLogOverride.h
  liblx/xml/XMLFileBuffer.h
  liblx/xml/XMLHandler.h
  liblx/xml/XMLInputStream.h
  liblx/xml/XMLMemoryBuffer.h
  liblx/xml/XMLNamespaces.h
  liblx/xml/XMLNode.h
  liblx/xml/XMLOutputStream.h
  liblx/xml/XMLParser.h
  liblx/xml/XMLToken.h
  liblx/xml/XMLTokenizer.h
  liblx/xml/XMLTriple.h
)

if(WITH_EXPAT)

    set(XML_SOURCES ${XML_SOURCES}
        liblx/xml/ExpatAttributes.cpp
        liblx/xml/ExpatHandler.cpp
        liblx/xml/ExpatParser.cpp
        liblx/xml/ExpatAttributes.h
        liblx/xml/ExpatHandler.h
        liblx/xml/ExpatParser.h
    )
    set(LIBLX_LIBS ${LIBLX_LIBS} ${LIBEXPAT_LIBRARY})
    include_directories(${LIBEXPAT_INCLUDE_DIR})

endif(WITH_EXPAT)

if(WITH_LIBXML)

    set(XML_SOURCES ${XML_SOURCES}
        liblx/xml/LibXMLAttributes.cpp
        liblx/xml/LibXMLHandler.cpp
        liblx/xml/LibXMLNamespaces.cpp
        liblx/xml/LibXMLParser.cpp
        liblx/xml/LibXMLTranscode.cpp
        liblx/xml/LibXMLAttributes.h
        liblx/xml/LibXMLHandler.h
        liblx/xml/LibXMLNamespaces.h
        liblx/xml/LibXMLParser.h
        liblx/xml/LibXMLTranscode.h
    )
    set(LIBLX_LIBS ${LIBLX_LIBS} ${LIBXML_LIBRARY})
    include_directories(${LIBXML_INCLUDE_DIR})

endif(WITH_LIBXML)

if(WITH_XERCES)

    set(XML_SOURCES ${XML_SOURCES}
        liblx/xml/XercesAttributes.cpp
        liblx/xml/XercesHandler.cpp
        liblx/xml/XercesNamespaces.cpp
        liblx/xml/XercesParser.cpp
        liblx/xml/XercesTranscode.cpp
        liblx/xml/XercesAttributes.h
        liblx/xml/XercesHandler.h
        liblx/xml/XercesNamespaces.h
        liblx/xml/XercesParser.h
        liblx/xml/XercesTranscode.h
    )

    set(LIBLX_LIBS ${LIBLX_LIBS} ${XERCES_LIBRARY})
    include_directories(${XERCES_INCLUDE_DIR})


endif(WITH_XERCES)

# Add LXNamespaces until the reserved namespace mechanism is replaced
#set(XML_SOURCES ${XML_SOURCES}
#	liblx/xml/LXNamespaces.cpp
#	)

source_group(xml FILES ${XML_SOURCES})
set(LIBLX_SOURCES ${LIBLX_SOURCES} ${XML_SOURCES})

###############################################################################
#
# this is a directory level operation!
#
if (MSVC)
if (WITH_STATIC_RUNTIME)
    foreach(flag_var
        CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
        CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO
        CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE
        CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO)

        if(${flag_var} MATCHES "/MD")
            string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
        endif(${flag_var} MATCHES "/MD")


    endforeach(flag_var)
    add_definitions( -D_MT)
endif(WITH_STATIC_RUNTIME)
endif(MSVC)

###############################################################################
#
# Build library
#
SET (INCLUDE_DESTINATION)
if (${CMAKE_VERSION} VERSION_GREATER "2.8.11")
  set(INCLUDE_DESTINATION INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
endif()

if (NOT LIBLX_SKIP_SHARED_LIBRARY)

add_library (${LIBLX_LIBRARY} SHARED ${LIBLX_SOURCES} )

if (LIBLX_SHARED_VERSION)
  SET_TARGET_PROPERTIES(${LIBLX_LIBRARY} PROPERTIES
                        SOVERSION ${LIBLX_VERSION_MAJOR}
                        VERSION ${LIBLX_VERSION_MAJOR}.${LIBLX_VERSION_MINOR}.${LIBLX_VERSION_PATCH})
endif()

target_link_libraries(${LIBLX_LIBRARY} ${LIBLX_LIBS} ${EXTRA_LIBS})

INSTALL(TARGETS ${LIBLX_LIBRARY} EXPORT ${LIBLX_LIBRARY}-config
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ${INCLUDE_DESTINATION}
)

# and install the exported target configuration
INSTALL(EXPORT ${LIBLX_LIBRARY}-config
        DESTINATION ${PACKAGE_CONFIG_DIR}
)

if (${CMAKE_VERSION} VERSION_GREATER 2.8.7)
WRITE_BASIC_PACKAGE_VERSION_FILE(
  ${CMAKE_CURRENT_BINARY_DIR}/${LIBLX_LIBRARY}-config-version.cmake 
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY AnyNewerVersion
)
  
INSTALL(
  FILES ${CMAKE_CURRENT_BINARY_DIR}/${LIBLX_LIBRARY}-config-version.cmake
  DESTINATION ${PACKAGE_CONFIG_DIR}
)
endif()

  
endif()


if (NOT LIBLX_SKIP_STATIC_LIBRARY)
add_library (${LIBLX_LIBRARY}-static STATIC ${LIBLX_SOURCES} )

if (WIN32 AND NOT CYGWIN)
    # don't decorate static library 
    set_target_properties(${LIBLX_LIBRARY}-static PROPERTIES COMPILE_DEFINITIONS "LIBLX_STATIC=1")
endif(WIN32 AND NOT CYGWIN)

target_link_libraries(${LIBLX_LIBRARY}-static ${LIBLX_LIBS} ${EXTRA_LIBS})

INSTALL(TARGETS ${LIBLX_LIBRARY}-static
        EXPORT  ${LIBLX_LIBRARY}-static-config
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ${INCLUDE_DESTINATION}
)

# and install the exported target configuration
INSTALL(EXPORT ${LIBLX_LIBRARY}-static-config 
        DESTINATION ${PACKAGE_CONFIG_DIR}
)

if (${CMAKE_VERSION} VERSION_GREATER 2.8.7)
WRITE_BASIC_PACKAGE_VERSION_FILE(
  ${CMAKE_CURRENT_BINARY_DIR}/${LIBLX_LIBRARY}-static-config-version.cmake
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY AnyNewerVersion
)

INSTALL(FILES 
  ${CMAKE_CURRENT_BINARY_DIR}/${LIBLX_LIBRARY}-static-config-version.cmake
  DESTINATION ${PACKAGE_CONFIG_DIR}
)
endif()

endif (NOT LIBLX_SKIP_STATIC_LIBRARY)

##############################################################################
#
# add native tests
#
add_subdirectory(liblx)
##############################################################################
#
# construct list of all header files to create dependency list for 
# language bindings
#

file(GLOB LIBLX_HEADER_FILES "${CMAKE_CURRENT_BINARY_DIR}/liblx/common/*.h")    
foreach(file ${LIBSBML_SOURCES})
   
   if ("${file}" MATCHES "h$" )
     get_filename_component(full_path ${file} ABSOLUTE)
     list(APPEND LIBLX_HEADER_FILES ${full_path})     
   endif()

endforeach()

##############################################################################
#
# create language bindings
#
add_subdirectory(bindings)
