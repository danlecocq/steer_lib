#
#  The RealityGrid Steering Library
#
#  Copyright (c) 2002-2010, University of Manchester, United Kingdom.
#  All rights reserved.
#
#  This software is produced by Research Computing Services, University
#  of Manchester as part of the RealityGrid project and associated
#  follow on projects, funded by the EPSRC under grants GR/R67699/01,
#  GR/R67699/02, GR/T27488/01, EP/C536452/1, EP/D500028/1,
#  EP/F00561X/1.
#
#  LICENCE TERMS
#
#  Redistribution and use in source and binary forms, with or without
#  modification, are permitted provided that the following conditions
#  are met:
#
#    * Redistributions of source code must retain the above copyright
#       notice, this list of conditions and the following disclaimer.
#
#    * Redistributions in binary form must reproduce the above
#      copyright notice, this list of conditions and the following
#      disclaimer in the documentation and/or other materials provided
#      with the distribution.
#
#    * Neither the name of The University of Manchester nor the names
#      of its contributors may be used to endorse or promote products
#      derived from this software without specific prior written
#      permission.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
#  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
#  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
#  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
#  COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
#  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
#  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
#  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
#  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
#  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
#  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
#  POSSIBILITY OF SUCH DAMAGE.
#
#  Author: Robert Haines

include(CheckIncludeFiles)
include(CheckSymbolExists)
include(CheckFunctionExists)
include(CheckLibraryExists)

# check where malloc and friends are defined
CHECK_SYMBOL_EXISTS(malloc stdlib.h MALLOC_IN_STDLIB)
if(NOT MALLOC_IN_STDLIB)
  CHECK_INCLUDE_FILES(malloc.h REG_NEED_MALLOC_H)
endif(NOT MALLOC_IN_STDLIB)

# check for supported signals
CHECK_SYMBOL_EXISTS(SIGXCPU signal.h REG_HAS_SIGXCPU)
CHECK_SYMBOL_EXISTS(SIGUSR2 signal.h REG_HAS_SIGUSR2)

#
# find the required external libraries and
# keep a track of them to help with configuring
# other projects that link against this one
#
if(UNIX)
  find_library(LIBM_LIB m)
  mark_as_advanced(LIBM_LIB)
  set(REG_EXTERNAL_LIBS ${REG_EXTERNAL_LIBS} ${LIBM_LIB})
endif(UNIX)

find_package(ZLIB REQUIRED)
include_directories(${ZLIB_INCLUDE_DIR})
set(REG_EXTERNAL_LIBS ${ZLIB_LIBRARY} ${REG_EXTERNAL_LIBS})

find_package(LibXml2 REQUIRED)
include_directories("${LIBXML2_INCLUDE_DIR}")
set(REG_EXTERNAL_LIBS ${LIBXML2_LIBRARIES} ${REG_EXTERNAL_LIBS})
set(CMAKE_REQUIRED_INCLUDES ${LIBXML2_INCLUDE_DIR})
set(CMAKE_REQUIRED_LIBRARIES ${LIBXML2_LIBRARIES})
CHECK_FUNCTION_EXISTS(xmlReadMemory REG_HAS_XMLREADMEMORY)

if(REG_DYNAMIC_MOD_LOADING)
  find_library(LIBDL_LIB dl)
  mark_as_advanced(LIBDL_LIB)
  set(REG_EXTERNAL_LIBS ${REG_EXTERNAL_LIBS} ${LIBDL_LIB})
endif(REG_DYNAMIC_MOD_LOADING)

# sockets library always needed due to xdr
find_package(Sockets)
if(LIBSOCKET_FOUND)
  set(REG_EXTERNAL_LIBS ${LIBSOCKET_LIBRARIES} ${REG_EXTERNAL_LIBS})
endif(LIBSOCKET_FOUND)

# we always need the xdr component of the Sun rpc tools
find_package(XDR REQUIRED)
if(XDR_FOUND)
  include_directories(${XDR_INCLUDE_DIR})
  if(NOT XDR_LIBRARY STREQUAL "XDR_LIBRARY-NOTFOUND")
    set(REG_EXTERNAL_LIBS ${XDR_LIBRARY} ${REG_EXTERNAL_LIBS})
  endif(NOT XDR_LIBRARY STREQUAL "XDR_LIBRARY-NOTFOUND")
endif(XDR_FOUND)

# Do specific checks for the example applications
include(deps/Examples)

#
# Go through the various "types" of module
# and do any specific checks for them.
#
# Files should be in the deps directory and named "Service.cmake"
# where Service is what the module provides, eg Sockets, Files
#
if(REG_BUILD_MODULAR_LIBS)
  foreach(service ${REG_MODULES_PROVIDES})
    if(EXISTS ${CMAKE_SOURCE_DIR}/CMake/deps/${service}.cmake)
      include(deps/${service})
    endif(EXISTS ${CMAKE_SOURCE_DIR}/CMake/deps/${service}.cmake)
  endforeach(service ${REG_MODULES_PROVIDES})
else(REG_BUILD_MODULAR_LIBS)
  foreach(type ${REG_MODULES_TYPES})
    set(default_mod ${REG_USE_MODULE_${type}})
    if(EXISTS ${CMAKE_SOURCE_DIR}/CMake/deps/${default_mod}.cmake)
      include(deps/${default_mod})
    endif(EXISTS ${CMAKE_SOURCE_DIR}/CMake/deps/${default_mod}.cmake)
  endforeach(type ${REG_MODULES_TYPES})
endif(REG_BUILD_MODULAR_LIBS)
