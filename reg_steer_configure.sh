#  File to set up environment for RealityGrid steering
#
#  (C) Copyright 2002, 2004, University of Manchester, United Kingdom,
#  all rights reserved.
#
#  This software is produced by the Supercomputing, Visualization and
#  e-Science Group, Manchester Computing, University of Manchester
#  as part of the RealityGrid project (http://www.realitygrid.org),
#  funded by the EPSRC under grants GR/R67699/01 and GR/R67699/02.
#
#  LICENCE TERMS
#
#  Redistribution and use in source and binary forms, with or without
#  modification, are permitted provided that the following conditions
#  are met:
#  1. Redistributions of source code must retain the above copyright
#     notice, this list of conditions and the following disclaimer.
#  2. Redistributions in binary form must reproduce the above copyright
#     notice, this list of conditions and the following disclaimer in the
#     documentation and/or other materials provided with the distribution.
#
#  THIS MATERIAL IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
#  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
#  LIMITED  TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
#  A PARTICULAR PURPOSE ARE DISCLAIMED. THE ENTIRE RISK AS TO THE QUALITY
#  AND PERFORMANCE OF THE PROGRAM IS WITH YOU.  SHOULD THE PROGRAM PROVE
#  DEFECTIVE, YOU ASSUME THE COST OF ALL NECESSARY SERVICING, REPAIR OR
#  CORRECTION.
#
#  Authors........: Andrew Porter, Robert Haines
#---------------------------------------------------------------------

# Set root directory for library installation
export REG_STEER_HOME=$HOME/RealityGrid/reg_steer_lib

# Set various paths needed for the wrappers
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$REG_STEER_HOME/wrappers/python
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$REG_STEER_HOME/wrappers/perl
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$REG_STEER_HOME/wrappers/java
export PYTHONPATH=$PYTHONPATH:$REG_STEER_HOME/wrappers/python
export PERL5LIB=$PERL5LIB:$REG_STEER_HOME/wrappers/perl
export CLASSPATH=$CLASSPATH:$REG_STEER_HOME/wrappers/java

# Set directory location to use for steering communication
export REG_STEER_DIRECTORY=/tmp

# Minimum time interval (integer no. of seconds) between checks on 
# whether a steering client has connected.  If set to zero then a 
# check is performed on every call to Steering_control.  If unset
# then a default value (set in ReG_Steer_types.h) is used.
export REG_APP_POLL_INTERVAL=5

# Set location of XML parser library and header files. On architectures
# (such as IBM PS2 and SGI Origin) that support both 32- and 64-bit
# compilation, the path to the library is generated by concatenating
# either "32" or "64" with REG_XML_LIBDIR. (This is done in the
# appropriate make/Makefile.<ARCH> file.)
export REG_XML_LIBDIR=/usr/lib
export REG_XML_INCDIR=/usr/include/libxml2

# Set which SSL libs (normally "-lssl -lcrypto") to link against and
# the location of OpenSSL libraries if not in standard place
export REG_SSL_LIBS=""
export REG_SSL_LIBDIR="."
export REG_SSL_INCDIR="."

#---------------------------------------------------------------
# This section only necessary if using sockets rather
# than file-based data transfer

# For socket connections - need to set port to that reported
# by library when application launched
export REG_CONNECTOR_HOSTNAME=vermont.mvc.mcc.ac.uk
export REG_CONNECTOR_PORT=2566
export GLOBUS_TCP_PORT_RANGE=2566,2666

#---------------------------------------------------------------
# This section needed when steering via SOAP and the Steering
# Grid Service (or SGS).

# Location of steering grid service that application will attach to
# This is an example - it will need to be set to the GSH of an
# application's SGS immediately before running the application.
export REG_SGS_ADDRESS=http://vermont.mvc.mcc.ac.uk:50000/1234567
