/*----------------------------------------------------------------------------
    Base header file for the steering library - contains definitions
    of return values, message types and array sizes.

    (C)Copyright 2002 The University of Manchester, United Kingdom,
    all rights reserved.

    This software is produced by the Supercomputing, Visualization &
    e-Science Group, Manchester Computing, the Victoria University of
    Manchester as part of the RealityGrid project.

    This software has been tested with care but is not guaranteed for
    any particular purpose. Neither the authors, nor the University of
    Manchester offer any warranties or representations, nor do they
    accept any liabilities with respect to this software.

    This program must not be used for commmercial gain without the
    written permission of the authors.
    
    Supercomputing, Visualization & e-Science Group
    Manchester Computing
    University of Manchester
    Manchester M13 9PL

    email:  csar-advice@cfs.ac.uk.
    Tel:    +44 161 275 6824/5997
    Fax:    +44 161 275 6040    
    
    Date          Version    Updates                            Author
    ----          -------    -------                            ------
    23.7.2002       0.1                                         A Porter

---------------------------------------------------------------------------*/

#ifndef __REG_TYPES_INCLUDED

#define __REG_TYPES_INCLUDED

#include <stdio.h>
#include <stdlib.h>

/* Flag for debugging - set to one to retain all xml messages */
#define NO_FILE_DELETE 0

/* The namespace used for steering messages (which are in xml) */
#define REG_STEER_NAMESPACE "http://www.realitygrid.org/xml/steering"

/* Filename of lockfile indicating sim is steerable */
#define APP_STEERABLE_FILENAME "app_steerable"

/* Filename of lockfile to signify steerer has connected */
#define STR_CONNECTED_FILENAME  "steering_active"

/* Filename used by application to send data to steerer */
#define APP_TO_STR_FILENAME "status_info"

/* Filename used by steerer to send data to application */
#define STR_TO_APP_FILENAME "control_info"

/* Return values */

#define REG_SUCCESS  0
#define REG_FAILURE  1
#define REG_EOD      2
#define REG_MEM_FAIL 3

/* Limit on number of commands steerer can send at any one time */
#define REG_MAX_NUM_STR_CMDS 50

/* Limit on number of parameters steerer can modify at any one time */
#define REG_MAX_NUM_STR_PARAMS 100

/* Initial sizes for internal tables of registered IO types 
   and parameters */

#define REG_INITIAL_NUM_IOTYPES 20
#define REG_INITIAL_NUM_PARAMS  20

/* Initial limit on no. of registered supported commands */

#define REG_INITIAL_NUM_CMDS 20

/* Initial limit on no. of sims being steered at any one time */

#define REG_MAX_NUM_STEERED_SIM 10

/* Limit on number of files used in communication - filenames
   wrap back on themselves once this limit reached */

#define REG_MAX_NUM_FILES 100

/* Maximum length for any string */

#define REG_MAX_STRING_LENGTH 80

/* Steering commands */

#define REG_STR_STOP             1
#define REG_STR_PAUSE            2
#define REG_STR_RESUME           3
#define REG_STR_DETACH           4
#define REG_STR_SET_EMIT_FREQ    5
#define REG_STR_SET_CONSUME_FREQ 6

/* All generated IOtype handles must be >= this value because they
   will be interpreted as commands - this value must therefore be >
   than all of the commands #define'd above */

#define REG_MIN_IOTYPE_HANDLE 1000

/* Coding scheme for data types */

#define REG_INT   0
#define REG_FLOAT 1
#define REG_DBL   2
#define REG_CHAR  3

/* Type definitions */

#define FALSE 0
#define TRUE  1
#define false 0
#define true  1

/* Reserved handle values */

#define REG_SIM_HANDLE_NOTSET   -1
#define REG_IODEF_HANDLE_NOTSET -1
#define REG_PARAM_HANDLE_NOTSET -1
#define REG_SEQ_NUM_HANDLE      -100

/* Message tags */

#define MSG_NOTSET  0
#define SUPP_CMDS   1
#define IO_DEFS     2
#define PARAM_DEFS  3
#define STATUS      4
#define CONTROL     5

/* Some type definitions - handles are simply integers for
   the moment */

typedef int REG_MsgType;
typedef int REG_IOHandleType;
typedef int REG_SimIDType;


#endif /* __REG_TYPES_INCLUDED defined */
