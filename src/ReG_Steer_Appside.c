/*----------------------------------------------------------------------------
    This file contains routines and data structures for the construction
    of an interface to a steering component (from an application
    component).

    (C)Copyright 2002 The University of Manchester, United Kingdom,
    all rights reserved.

    This software is produced by the Supercomputing, Visualization &
    e-Science Group, Manchester Computing, the Victoria University of
    Manchester as part of the RealityGrid project.

    This software has been tested with care but is not guaranteed for
    any particular purpose. Neither the copyright holder, nor the
    University of Manchester offer any warranties or representations,
    nor do they accept any liabilities with respect to this software.

    This software must not be used for commercial gain without the
    written permission of the authors.
    
    This software must not be redistributed without the written
    permission of the authors.

    Permission is granted to modify this software, provided any
    modifications are made freely available to the original authors.
 
    Supercomputing, Visualization & e-Science Group
    Manchester Computing
    University of Manchester
    Manchester M13 9PL
    
    WWW:    http://www.sve.man.ac.uk  
    email:  sve@man.ac.uk
    Tel:    +44 161 275 6095
    Fax:    +44 161 275 6800    

    Initial version by:   A Porter, 23.7.2002
---------------------------------------------------------------------------*/

#include "ReG_Steer_Appside.h"
#include "ReG_Steer_Appside_internal.h"
#include "ReG_Steer_Appside_Globus.h"
#include "ReG_Steer_Globus_io.h"

#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <rpc/rpc.h>
#include <math.h>

/* Allow value of 'DEBUG' to propagate down from Reg_steer_types.h if
   it has been set there */
#ifndef DEBUG
#define DEBUG 1
#endif

/*----------------------------------------------------------------*/

void Steering_enable(const int EnableSteer)
{
  /* Set global flag that controls whether steering is enabled or not */
  ReG_SteeringEnabled = EnableSteer;

  return;
}

/*----------------------------------------------------------------*/

int Steering_initialize(int  NumSupportedCmds,
			int *SupportedCmds)
{
  int   i, j;
  char *pchar;

  /* Actually defined in ReG_Steer_Common.c because both steerer
     and steered have a variable of this name */
  extern char ReG_Steer_Schema_Locn[REG_MAX_STRING_LENGTH];

  /* Don't do anything if steering is not enabled */
  if (!ReG_SteeringEnabled) return REG_SUCCESS;

  /* Set the location of the file containing the schema describing all 
     steering communication */

  pchar = getenv("REG_STEER_HOME");

  if(pchar){

    /* Check that path ends in '/' - if not then add one */

    i = strlen(pchar);
    if( pchar[i-1] != '/' ){

      sprintf(ReG_Steer_Schema_Locn, "%s/xml_schema/reg_steer_comm.xsd",
                                     pchar);
    }
    else{

      sprintf(ReG_Steer_Schema_Locn, "%sxml_schema/reg_steer_comm.xsd",
                                     pchar);
    }
  }
  else{

    fprintf(stderr, "Steering_initialize: failed to get schema location\n");
    return REG_FAILURE;
  }
  
  /* Allocate memory and initialise tables of IO types and 
     parameters */

  IOTypes_table.num_registered = 0;
  IOTypes_table.max_entries    = REG_INITIAL_NUM_IOTYPES;
  IOTypes_table.io_def         = (IOdef_entry *)
                                 malloc(IOTypes_table.max_entries
					*sizeof(IOdef_entry));
 
  if(IOTypes_table.io_def == NULL){
    
    fprintf(stderr, "Steering_initialize: failed to allocate memory "
	    "for IOType table\n");
    return REG_FAILURE;
  }

  for(i=0; i<IOTypes_table.max_entries; i++){

    IOTypes_table.io_def[i].handle = REG_IODEF_HANDLE_NOTSET;
  }

  /* Initialise table of open IO channels */

  for(i=0; i<REG_INITIAL_NUM_IOTYPES; i++){
    IO_channel[i].buffer = NULL;
  }

  /* Initialise table for registered checkpoint types */

  ChkTypes_table.num_registered = 0;
  ChkTypes_table.max_entries    = REG_INITIAL_NUM_IOTYPES;
  ChkTypes_table.io_def         = (IOdef_entry *)
                                   malloc(IOTypes_table.max_entries
				  	  *sizeof(IOdef_entry));
 
  if(ChkTypes_table.io_def == NULL){
    
    fprintf(stderr, "Steering_initialize: failed to allocate memory "
	    "for ChkType table\n");
    free(IOTypes_table.io_def);
    return REG_FAILURE;
  }

  for(i=0; i<ChkTypes_table.max_entries; i++){

    ChkTypes_table.io_def[i].handle = REG_IODEF_HANDLE_NOTSET;
  }

  /* Initialise log of checkpoints */

  Chk_log.num_entries = 0;
  Chk_log.max_entries = REG_INITIAL_CHK_LOG_SIZE;
  Chk_log.num_unsent  = 0;
  Chk_log.entry = (Chk_log_entry_type *)malloc(Chk_log.max_entries
					  *sizeof(Chk_log_entry_type));
  if(Chk_log.entry == NULL){

    fprintf(stderr, "Steering_initialize: failed to allocate memory "
	    "for checkpoint logging\n");
    free(IOTypes_table.io_def);
    free(ChkTypes_table.io_def);
    return REG_FAILURE;
  }

  for(i=0; i<Chk_log.max_entries; i++){

    Chk_log.entry[i].sent_to_steerer = TRUE;
    Chk_log.entry[i].num_param       = 0;

    for(j=0; j<REG_MAX_NUM_STR_PARAMS; j++){
      Chk_log.entry[i].param[j].handle = REG_PARAM_HANDLE_NOTSET;
    }
  }

  /* Set up table for registered parameters */

  Params_table.num_registered = 0;
  Params_table.max_entries    = REG_INITIAL_NUM_PARAMS;
  Params_table.next_handle    = 0;
  Params_table.param          = (param_entry *)malloc(Params_table.max_entries
					      *sizeof(param_entry));

  if(Params_table.param == NULL){

    fprintf(stderr, "Steering_initialize: failed to allocate memory "
	    "for param table\n");

    free(IOTypes_table.io_def);
    free(ChkTypes_table.io_def);
    free(Chk_log.entry);
    return REG_FAILURE;
  }

  /* Initialise parameter handles */

  for(i=0; i<Params_table.max_entries; i++){

    Params_table.param[i].handle = REG_PARAM_HANDLE_NOTSET;
  }

  /* 'Sequence number' is treated as a parameter */
  Params_table.param[0].ptr       = NULL;
  Params_table.param[0].type      = REG_INT;
  Params_table.param[0].handle    = REG_SEQ_NUM_HANDLE;
  Params_table.param[0].steerable = FALSE;
  Params_table.param[0].modified  = FALSE;
  Params_table.param[0].is_internal=FALSE;
  sprintf(Params_table.param[0].label, "REG_SEQ_NUM");
  sprintf(Params_table.param[0].value, "-1");
  Increment_param_registered(&Params_table);

  /* Parameter for monitoring CPU time per step */
  i = Params_table.num_registered;
  Params_table.param[i].ptr       = NULL;
  Params_table.param[i].type      = REG_FLOAT;
  Params_table.param[i].handle    = REG_STEP_TIME_HANDLE;
  Params_table.param[i].steerable = FALSE;
  Params_table.param[i].modified  = FALSE;
  Params_table.param[i].is_internal=FALSE;
  sprintf(Params_table.param[i].label, "CPU_TIME_PER_STEP");
  sprintf(Params_table.param[i].value, "-1.0");
  Increment_param_registered(&Params_table);

  /* Set-up/prepare for connection to steering client */
  if(Initialize_steering_connection(NumSupportedCmds, 
				    SupportedCmds) != REG_SUCCESS){

    free(IOTypes_table.io_def);
    free(Params_table.param);
    return REG_FAILURE;
  }

  /* Set up signal handler so can clean up if application 
     exits in a hurry */
  /* ctrl-c */
  signal(SIGINT, Steering_signal_handler);
  /* kill (note cannot (and should not) catch kill -9) */
  signal(SIGTERM, Steering_signal_handler);
  signal(SIGSEGV, Steering_signal_handler);
  signal(SIGILL, Steering_signal_handler);
  signal(SIGABRT, Steering_signal_handler);
  signal(SIGFPE, Steering_signal_handler);

  /* Flag that library has been successfully initialised */

  ReG_SteeringInit = TRUE;

  return REG_SUCCESS;
}

/*----------------------------------------------------------------*/

int Steering_finalize()
{
  int  commands[1];

  /* Can only call this function if steering lib initialised */

  if (!ReG_SteeringInit) return REG_FAILURE;

  /* Tell the steerer that we are done */

  if(ReG_SteeringActive){

    commands[0] = REG_STR_DETACH;
    Emit_status(0,
		0,
		NULL,
		1,
		commands);
  }

  /* Clean-up IOTypes table */

  if(IOTypes_table.io_def != NULL){
    /* cleanup transport mechanism */
    Finalize_IOType_transport();
    free(IOTypes_table.io_def);
    IOTypes_table.io_def = NULL;
  }

  IOTypes_table.num_registered = 0;
  IOTypes_table.max_entries = REG_INITIAL_NUM_IOTYPES;

  /* Clean-up ChkTypes table */

  if(ChkTypes_table.io_def != NULL){
    free(ChkTypes_table.io_def);
    ChkTypes_table.io_def = NULL;
  }

  ChkTypes_table.num_registered = 0;
  ChkTypes_table.max_entries = REG_INITIAL_NUM_IOTYPES;

  /* Clean-up log of checkpoints */

  if(Chk_log.entry != NULL){

    free(Chk_log.entry);
    Chk_log.entry = NULL;
  }
  Chk_log.num_entries = 0;
  Chk_log.max_entries = REG_INITIAL_CHK_LOG_SIZE;

  /* Clean-up parameters table */

  if(Params_table.param != NULL){
    free(Params_table.param);
    Params_table.param = NULL;
  }

  Params_table.num_registered = 0;
  Params_table.max_entries = REG_INITIAL_NUM_IOTYPES;

  /* Signal that component no-longer steerable */
  Finalize_steering_connection();

  /* Reset state of library */

  ReG_ParamsChanged  = FALSE;
  ReG_IOTypesChanged = FALSE;
  ReG_ChkTypesChanged = FALSE;
  ReG_SteeringActive = FALSE;

  /* Flag that library no-longer initialised */
  ReG_SteeringInit    = FALSE;

  return REG_SUCCESS;
}

/*----------------------------------------------------------------*/

int Register_IOTypes(int    NumTypes,
                     char* *IOLabel,
		     int   *direction,
		     int   *IOFrequency,
                     int   *IOType)
{
  int          i;
  int          current;
  int          new_size;
  IOdef_entry *dum_ptr;
  char*        iofreq_label;
  int          iofreq_strbl;
  int          iofreq_type;
  int          iparam;
  void        *ptr_array[1];
  int          return_status = REG_SUCCESS;
 

  /* Check that steering is enabled */

  if(!ReG_SteeringEnabled){

    for(i=0; i<NumTypes; i++){
      IOType[i] = REG_IODEF_HANDLE_NOTSET;
    }
    return REG_SUCCESS;
  }

  /* Can only call this function if steering lib initialised */

  if (!ReG_SteeringInit) return REG_FAILURE;

  /* IO types cannot be deleted so is safe to use num_registered to 
     get next free entry */
  current = IOTypes_table.num_registered;

  for(i=0; i<NumTypes; i++){

    strcpy(IOTypes_table.io_def[current].label, IOLabel[i]);

    /* Will need to check that IOLabel is something that the 
       component framework knows about */
  
    /* Currently have no way of looking up what filename to use so 
       hardwire... */

    if(IOLabel[i] == "VTK_STRUCTURED_POINTS_INPUT"){

      sprintf(IOTypes_table.io_def[current].filename, "data.vtk");
    }
    else{
      /* In the short term, use the label as the filename */
      strcpy(IOTypes_table.io_def[current].filename, IOLabel[i]);
    }

    /* Whether input or output (sample data) */

    IOTypes_table.io_def[current].direction = direction[i];

    /* Set variables required for registration of associated io
       frequency as a steerable parameter */

    iofreq_label = "IO_Frequency";
    iofreq_strbl = TRUE;
    iofreq_type  = REG_INT;
    IOTypes_table.io_def[current].frequency = IOFrequency[i];
    ptr_array[0] = (void *)&(IOTypes_table.io_def[current].frequency);

    Register_params(1,
		    &iofreq_label,
		    &iofreq_strbl,
		    ptr_array,
		    &iofreq_type);

    /* Store the handle given to this parameter - this line must
       immediately succeed the call to Register_params */

    IOTypes_table.io_def[current].freq_param_handle = 
	                               Params_table.next_handle - 1;

    /* Annotate the parameter table entry just created to flag that
       it is a parameter that is internal to the steering library */
    iparam = Param_index_from_handle(&Params_table, 
			      IOTypes_table.io_def[current].freq_param_handle);

    if(iparam != -1){
      Params_table.param[iparam].is_internal = TRUE;
    }
    else{

#if DEBUG
      fprintf(stderr, "Register_IOTypes: failed to get handle for param\n");
#endif
      return_status = REG_FAILURE;
    }
      
    IOTypes_table.io_def[current].buffer = NULL;
    IOTypes_table.io_def[current].use_xdr = FALSE;

    /* set up transport for sample data - eg sockets */
    return_status = Initialize_IOType_transport(direction[i], current);

    /* Create, store and return a handle for this IOType */
    IOTypes_table.io_def[current].handle = Next_IO_Chk_handle++;
    IOType[i] = IOTypes_table.io_def[current].handle;

    current++;

    if(current == IOTypes_table.max_entries){

      new_size = IOTypes_table.max_entries + REG_INITIAL_NUM_IOTYPES;

      dum_ptr = (IOdef_entry*)realloc((void *)(IOTypes_table.io_def),
		                      new_size*sizeof(IOdef_entry));

      if(dum_ptr == NULL){

        fprintf(stderr, "Register_IOTypes: failed to allocate memory\n");
	return REG_FAILURE;
      }
      else{

	IOTypes_table.io_def = dum_ptr;
      }

      IOTypes_table.max_entries += REG_INITIAL_NUM_IOTYPES;
    }
  }

  IOTypes_table.num_registered = current;

  /* Flag that the registered IO Types have changed */
  ReG_IOTypesChanged = TRUE;

  return return_status;
}

/*----------------------------------------------------------------*/

int Register_ChkTypes(int    NumTypes,
		      char* *ChkLabel,
		      int   *direction,
		      int   *ChkFrequency,
		      int   *ChkType)
{
  int 	       i;
  int 	       current;
  int 	       iparam;
  int          new_size;
  char*        chkfreq_label;
  int          chkfreq_strbl;
  int          chkfreq_type;
  void        *ptr_array[1];
  IOdef_entry *dum_ptr;
  int          return_status = REG_SUCCESS;

  /* Check that steering is enabled */

  if(!ReG_SteeringEnabled){

    for(i=0; i<NumTypes; i++){
      ChkType[i] = REG_IODEF_HANDLE_NOTSET;
    }
    return REG_SUCCESS;
  }

  /* Can only call this function if steering lib initialised */

  if (!ReG_SteeringInit) return REG_FAILURE;

  /* Chk types cannot be deleted so is safe to use num_registered to 
     get next free entry */
  current = ChkTypes_table.num_registered;

  for(i=0; i<NumTypes; i++){

    strcpy(ChkTypes_table.io_def[current].label, ChkLabel[i]);

    /* filename not used currently */
    sprintf(ChkTypes_table.io_def[current].filename, "NOT_SET");

    /* Whether input or output */
    ChkTypes_table.io_def[current].direction = direction[i];

    /* Set variables required for registration of associated io
       frequency as a steerable parameter (but only if checkpoint is
       to be emitted) */
    if(direction[i] != REG_IO_IN){

      /* Set variables required for registration of associated io
         frequency as a steerable parameter */

      chkfreq_label = "Chk_Frequency";
      chkfreq_strbl = TRUE;
      chkfreq_type  = REG_INT;
      ChkTypes_table.io_def[current].frequency = ChkFrequency[i];
      ptr_array[0] = (void *)&(ChkTypes_table.io_def[current].frequency);

      Register_params(1,
		      &chkfreq_label,
		      &chkfreq_strbl,
		      ptr_array,
		      &chkfreq_type);

      /* Store the handle given to this parameter - this line MUST
         immediately succeed the call to Register_params */

      ChkTypes_table.io_def[current].freq_param_handle = 
	                                 Params_table.next_handle - 1;

#if DEBUG
      fprintf(stderr, "Register_ChkTypes: handle of freq. param = %d\n",
	      ChkTypes_table.io_def[current].freq_param_handle);
#endif

      /* Annotate the parameter table entry just created to flag that
         it is a parameter that is internal to the steering library */
      iparam = Param_index_from_handle(&Params_table, 
			    ChkTypes_table.io_def[current].freq_param_handle);
      if(iparam != -1){
        Params_table.param[iparam].is_internal = TRUE;
      }
      else{
#if DEBUG
        fprintf(stderr, "Register_ChkTypes: failed to get handle for param\n");
#endif
        return_status = REG_FAILURE;
      }
    }
    else{
      /* Auto consume is senseless for checkpoints so set frequency-related
	 elements to 'null' values */
      ChkTypes_table.io_def[current].freq_param_handle = REG_PARAM_HANDLE_NOTSET;
      ChkTypes_table.io_def[current].frequency = 0;
    }

    /* Create, store and return a handle for this ChkType */
    ChkTypes_table.io_def[current].handle = Next_IO_Chk_handle++;
    ChkType[i] = ChkTypes_table.io_def[current].handle;

    /* Check whether we need to allocate more storage */
    current++;
    if(current == ChkTypes_table.max_entries){

      new_size = ChkTypes_table.max_entries + REG_INITIAL_NUM_IOTYPES;

      dum_ptr = (IOdef_entry*)realloc((void *)(ChkTypes_table.io_def),
		                      new_size*sizeof(IOdef_entry));

      if(dum_ptr == NULL){

        fprintf(stderr, "Register_ChkTypes: failed to allocate memory\n");
	return REG_FAILURE;
      }
      else{

	ChkTypes_table.io_def = dum_ptr;
      }

      ChkTypes_table.max_entries += REG_INITIAL_NUM_IOTYPES;
    }

  } /* End of loop over Chk types to register */

  ChkTypes_table.num_registered = current;

  /* Flag that the registered Chk Types have changed */
  ReG_ChkTypesChanged = TRUE;

  return return_status;
}

/*----------------------------------------------------------------*/

int Record_Chkpt(int   ChkType,
		 char *ChkTag)
{
  int   j;
  int   index;
  int   count;
  int   new_size;
  void *ptr;

  /* Can only call this function if steering lib initialised */

  if (!ReG_SteeringInit) return REG_FAILURE;

  /* Check that we have enough storage space */

  if(Chk_log.num_entries == Chk_log.max_entries){

    new_size = Chk_log.max_entries + REG_INITIAL_CHK_LOG_SIZE;
    ptr = realloc((void *)(Chk_log.entry), 
		  new_size*sizeof(Chk_log_entry_type));

    if(!ptr){

      fprintf(stderr, "Record_Chkpt: failed to allocate memory to extend"
	      " chkpt log\n");
      return REG_FAILURE;
    }

    Chk_log.entry = (Chk_log_entry_type *)ptr;

    for(index=Chk_log.num_entries; index<Chk_log.max_entries; index++){

      Chk_log.entry[index].sent_to_steerer = TRUE;
      Chk_log.entry[index].num_param       = 0;

      for(j=0; j<REG_MAX_NUM_STR_PARAMS; j++){
	Chk_log.entry[index].param[j].handle = REG_PARAM_HANDLE_NOTSET;
      }
    }
  }

  Chk_log.entry[Chk_log.num_entries].key = Chk_log.primary_key_value++;
  strcpy(Chk_log.entry[Chk_log.num_entries].chk_tag, ChkTag);
  Chk_log.entry[Chk_log.num_entries].chk_handle      = ChkType;
  Chk_log.entry[Chk_log.num_entries].sent_to_steerer = FALSE;

  /* Store the values of all registered parameters at this point (so
     long as they're not internal to the library */
  count = 0;
  for(index = 0; index<Params_table.max_entries; index++){

    if(Params_table.param[index].handle == REG_PARAM_HANDLE_NOTSET ||
       Params_table.param[index].is_internal == TRUE){
      continue;
    }

    /* This is one we want - store its handle and current value */
    Chk_log.entry[Chk_log.num_entries].param[count].handle = 
                                           Params_table.param[index].handle;
    strcpy(Chk_log.entry[Chk_log.num_entries].param[count].value,
           Params_table.param[index].value);
    
    /* Storage for params associated with log entry is static */
    if(++count >= REG_MAX_NUM_STR_PARAMS)break;  
  }

  /* Store the no. of params this entry has */
  Chk_log.entry[Chk_log.num_entries].num_param = count;

  /* Keep a count of entries that we have yet to send to steerer */
  Chk_log.num_unsent++;

  Chk_log.num_entries++;

  return REG_SUCCESS;
}

/*----------------------------------------------------------------*/

int Consume_start(int  IOType,
		  int *IOTypeIndex)
{
  /* Check that steering is enabled */
  if(!ReG_SteeringEnabled) return REG_SUCCESS;

  /* Can only call this function if steering lib initialised */
  if (!ReG_SteeringInit) return REG_FAILURE;

  /* Find corresponding entry in table of IOtypes */
  *IOTypeIndex = IOdef_index_from_handle(&IOTypes_table, IOType);
  if(*IOTypeIndex == REG_IODEF_HANDLE_NOTSET){
    fprintf(stderr, "Consume_start: failed to find matching IOType, "
	    "handle = %d\n", IOType);
    return REG_FAILURE;
  }

  return Consume_start_data_check(*IOTypeIndex);
}

/*----------------------------------------------------------------*/

int Consume_stop(int *IOTypeIndex)
{
  int             return_status = REG_SUCCESS;

  /* Check that steering is enabled */
  if(!ReG_SteeringEnabled) return REG_SUCCESS;

  /* Can only call this function if steering lib initialised */
  if (!ReG_SteeringInit) return REG_FAILURE;

  /* Free memory associated with channel */
  if( IOTypes_table.io_def[*IOTypeIndex].buffer ){
    free(IOTypes_table.io_def[*IOTypeIndex].buffer);
    IOTypes_table.io_def[*IOTypeIndex].buffer = NULL;
    IOTypes_table.io_def[*IOTypeIndex].buffer_bytes = 0;
  }

  /* Reset handle associated with channel */
  *IOTypeIndex = REG_IODEF_HANDLE_NOTSET;

  return return_status;
}

/*----------------------------------------------------------------*/

int Consume_data_slice_header(int  IOTypeIndex,
			      int *DataType,
			      int *Count)
{
  int status;

  status = Consume_iotype_msg_header(IOTypeIndex,
				     DataType,
				     Count);

  if(status != REG_SUCCESS) return REG_FAILURE;

  /* Use of XDR is internal to library so make sure user doesn't
     get confused.  use_xdr flag set here for use in subsequent call
     to consume_data_slice */
  switch(*DataType){

  case REG_XDR_INT:
    IOTypes_table.io_def[IOTypeIndex].use_xdr = TRUE;
    *DataType = REG_INT;
    break;

  case REG_XDR_FLOAT:
    IOTypes_table.io_def[IOTypeIndex].use_xdr = TRUE;
    *DataType = REG_FLOAT;
    break;

  case REG_XDR_DOUBLE:
    IOTypes_table.io_def[IOTypeIndex].use_xdr = TRUE;
    *DataType = REG_DBL;
    break;

  default:
    break;
  }

  return REG_SUCCESS;
}

/*----------------------------------------------------------------*/

int Consume_data_slice(int    IOTypeIndex,
		       int    DataType,
		       int    Count,
		       void  *pData)
{
  int              i;
  int              return_status = REG_SUCCESS;
  size_t	   num_bytes_to_read;

  XDR     xdrs;
  int    *pint;
  float  *pfloat;
  double *pdouble;
  void   *tmp_ptr;

  /* Calculate how many bytes to expect */
  switch(DataType){

  case REG_INT:
    if(IOTypes_table.io_def[IOTypeIndex].use_xdr){
      num_bytes_to_read = Count*REG_SIZEOF_XDR_INT;
    }
    else{
      num_bytes_to_read = Count*sizeof(int);
    }
    break;

  case REG_FLOAT:
    if(IOTypes_table.io_def[IOTypeIndex].use_xdr){
      num_bytes_to_read = Count*REG_SIZEOF_XDR_FLOAT;
    }
    else{
      num_bytes_to_read = Count*sizeof(float);
    }
    break;

  case REG_DBL:
    if(IOTypes_table.io_def[IOTypeIndex].use_xdr){
      num_bytes_to_read = Count*REG_SIZEOF_XDR_DOUBLE;
    }
    else{
      num_bytes_to_read = Count*sizeof(double);
    }
    break;

  case REG_CHAR:
    num_bytes_to_read = Count*sizeof(char);
    break;

  default:
    fprintf(stderr, "Consume_data_slice: Unrecognised data type specified "
	    "in slice header\n");

    /* Reset use_xdr flag set as only valid on a per-slice basis */
    IOTypes_table.io_def[IOTypeIndex].use_xdr = FALSE;

    return REG_FAILURE;
    break;
  }

  /* Check that input buffer is large enough (only an issue if have XDR-
     encoded data) */
  if(IOTypes_table.io_def[IOTypeIndex].use_xdr){
    if(IOTypes_table.io_def[IOTypeIndex].buffer_bytes < num_bytes_to_read){

      tmp_ptr = realloc(IOTypes_table.io_def[IOTypeIndex].buffer, 
			(size_t)num_bytes_to_read);
            
      if(!tmp_ptr){
	fprintf(stderr, "Consume_data_slice: failed to realloc input "
		"buffer - consume failed\n");

	/* Reset use_xdr flag set as only valid on a per-slice basis */
	IOTypes_table.io_def[IOTypeIndex].use_xdr = FALSE;

	return REG_FAILURE;
      }

      IOTypes_table.io_def[IOTypeIndex].buffer_bytes = num_bytes_to_read;
      IOTypes_table.io_def[IOTypeIndex].buffer = tmp_ptr;
    }
  }

  /* Read this number of bytes */

  if (Consume_data_read(IOTypeIndex,
			DataType,
			num_bytes_to_read,
			pData) != REG_SUCCESS)
    return REG_FAILURE;


  if(IOTypes_table.io_def[IOTypeIndex].use_xdr){

#if DEBUG
    fprintf(stderr, "Consume_data_slice: doing XDR decode\n");
#endif

    xdrmem_create(&xdrs, 
		  IOTypes_table.io_def[IOTypeIndex].buffer,
		  num_bytes_to_read,
		  XDR_DECODE);

    switch(DataType){

    case REG_INT:
      
      pint = (int *)pData;

      for(i=0; i<Count; i++){

	if (1!=xdr_int(&xdrs, pint++)) {
	  fprintf(stderr, "Consume_data_slice: error decoding datum %d\n",i);
	  return_status = REG_FAILURE;
	  break;
	}
      }
      break;

    case REG_FLOAT:
      
      pfloat = (float *)pData;

      for(i=0; i<Count; i++){

	if (1!=xdr_float(&xdrs, pfloat++)) {
	  fprintf(stderr, "Consume_data_slice: error decoding datum %d\n",i);
	  return_status = REG_FAILURE;
	  break;
	}
      }
      break;

    case REG_DBL:
      
      pdouble = (double *)pData;

      for(i=0; i<Count; i++){

	if (1!=xdr_double(&xdrs, pdouble++)) {
	  fprintf(stderr, "Consume_data_slice: error decoding datum %d\n",i);
	  return_status = REG_FAILURE;
	  break;
	}
      }
      break;

    default:
      fprintf(stderr, "Consume_data_slice: unexpected datatype\n");
      return_status = REG_FAILURE;
      break;
    }

    xdr_destroy(&xdrs);
  }

#if DEBUG
  fprintf(stderr, "Consume_data_slice: done XDR decode\n");
#endif

  /* Reset use_xdr flag set as only valid on a per-slice basis */
  IOTypes_table.io_def[IOTypeIndex].use_xdr = FALSE;

  return return_status;
}


/*----------------------------------------------------------------*/

int Consume_data_slice_old(REG_IOHandleType  IOHandle,
		       int              *DataType,
		       int              *Count,
		       void            **pData)
{
  int return_status = REG_SUCCESS;
  int index;

  /* ARPDBG - for testing */
  static int test_count_call = 0;
  int   i, j, k, ivar;
  int   nx = 16;
  int   ny = 16;
  int   nz = 16;
  char *pdata;
  float value;
  double sum;
  XDR   xdrs;
  /* ...end of test variables */

  index = (int)(IOHandle);

  if( IO_channel[index].buffer == NULL ){

    fprintf(stderr, "Consume_data_slice: io channel not open\n");
    fprintf(stderr, "                    IOHandle = %d\n", index);
    return REG_FAILURE;
  }

  /* ARPDGB For testing purposes ONLY */
  fprintf(stderr, "Consume_data_slice: test_count_call = %d\n", 
	  test_count_call);

  if(test_count_call == 0){

    *DataType = REG_CHAR;

    pdata = (char *)(IO_channel[index].buffer);
    pdata += sprintf(pdata, REG_PACKET_FORMAT, "# AVS field file\n");
    pdata += sprintf(pdata, REG_PACKET_FORMAT, "ndim=3\n");
    pdata += sprintf(pdata, REG_PACKET_FORMAT, "dim1= 16\n");
    pdata += sprintf(pdata, REG_PACKET_FORMAT, "dim2= 16\n");
    pdata += sprintf(pdata, REG_PACKET_FORMAT, "dim3= 16\n");
    pdata += sprintf(pdata, REG_PACKET_FORMAT, "nspace=3\n");
    pdata += sprintf(pdata, REG_PACKET_FORMAT, "field=uniform\n");
    pdata += sprintf(pdata, REG_PACKET_FORMAT, "veclen=2\n");
    pdata += sprintf(pdata, REG_PACKET_FORMAT, "data=xdr_float\n");
    pdata += sprintf(pdata, REG_PACKET_FORMAT, "variable 1 filetype=binary "
    		   "skip=0000000 stride=2\n");
    pdata += sprintf(pdata, REG_PACKET_FORMAT, "variable 2 filetype=binary "
    		   "skip=0000008 stride=2\n");
    pdata += sprintf(pdata, REG_PACKET_FORMAT, "END_OF_HEADER\n");

    *Count = strlen((char *)(IO_channel[index].buffer));

    *pData = IO_channel[index].buffer;
  }
  else if(test_count_call == 1){

    fprintf(stderr, "Consume_data_slice: Creating xdr data buffer...\n");

    xdrmem_create(&xdrs, IO_channel[index].buffer, REG_IO_BUFSIZE, 
		  XDR_ENCODE);

    sum = 0.0;
    for(i=0; i<nx/2; i++){
      for(j=0; j<ny; j++){
	for(k=0; k<nz; k++){
	  for(ivar=0; ivar<2; ivar++){

	    value = (float)sqrt((double)(i*i*(ivar+1)*0.1 + j*j*0.1 +k*k*0.1));

	    sum += value;
	    if( xdr_float(&xdrs, &(value)) != 1){
	      fprintf(stderr, "Failed to write xdr datum no. %d\n", i);
	      break;
	    }
	  }
	}
      }
    }

    xdr_destroy(&xdrs);

    *DataType = REG_FLOAT;
    *Count = nx*ny*nz;
    *pData = IO_channel[index].buffer;
  }
  else if(test_count_call == 2){

    xdrmem_create(&xdrs, IO_channel[index].buffer, REG_IO_BUFSIZE, 
		  XDR_ENCODE);

    for(i=nx/2; i<nx; i++){
      for(j=0; j<ny; j++){
	for(k=0; k<nz; k++){
	  for(ivar=0; ivar<2; ivar++){

	    value = sqrt((float)(i*i*(ivar+1)*0.1 + j*j*0.1 +k*k*0.1));

	    if( xdr_float(&xdrs, &(value)) != 1){
	      fprintf(stderr, "Failed to write xdr datum no. %d\n", i);
	      break;
	    }
	  }
	}
      }

    }

    xdr_destroy(&xdrs);

    *DataType = REG_FLOAT;
    *Count = nx*ny*nz;
    /**pData = &(((double *)IO_channel[index].buffer)[nx*ny*nz]);
    *pData = &(((char *)IO_channel[index].buffer)[nx*ny*nz*8]);*/
    *pData = IO_channel[index].buffer;
  }
  else{

    return_status = REG_EOD;
  }

  /* ARPDBG */
  test_count_call++;

  return return_status;
}

/*----------------------------------------------------------------*/

int Emit_start(int  IOType,
	       int  SeqNum,
	       int  UseXDR,
	       int *IOTypeIndex)
{

  /* Check that steering is enabled */
  if(!ReG_SteeringEnabled) return REG_SUCCESS;

  /* Can only call this function if steering lib initialised */
  if (!ReG_SteeringInit) return REG_FAILURE;

  /* Find corresponding entry in table of IOtypes */
  *IOTypeIndex = IOdef_index_from_handle(&IOTypes_table, IOType);
  if(*IOTypeIndex == REG_IODEF_HANDLE_NOTSET){

    fprintf(stderr, "Emit_start: failed to find matching IOType\n");
    return REG_FAILURE;
  }

  /* Set whether or not to encode as XDR */
  IOTypes_table.io_def[*IOTypeIndex].use_xdr = UseXDR;

  /* We'll need additional memory to perform conversion to XDR */
  if(UseXDR){

    IOTypes_table.io_def[*IOTypeIndex].buffer = (void *)malloc(REG_IO_BUFSIZE);

    if(!IOTypes_table.io_def[*IOTypeIndex].buffer){

      return REG_FAILURE; 
    }

  }

  if (Emit_header(*IOTypeIndex) == REG_SUCCESS)
    return REG_SUCCESS;

  else {

    /* free up memory as no guarantee Emit_stop will be called */
    if(UseXDR && IOTypes_table.io_def[*IOTypeIndex].buffer){
      free(IOTypes_table.io_def[*IOTypeIndex].buffer);
      IOTypes_table.io_def[*IOTypeIndex].buffer = NULL;
    }

    return REG_FAILURE;
  }

}

/*----------------------------------------------------------------*/

int Emit_stop(int *IOTypeIndex)
{
  char            buffer[REG_PACKET_SIZE];
  char            fmt[16];
  int             return_status = REG_SUCCESS;

  /* Check that steering is enabled */
  if(!ReG_SteeringEnabled) return REG_SUCCESS;

  /* Can only call this function if steering lib initialised */
  if (!ReG_SteeringInit) return REG_FAILURE;

  /* Send footer */
  sprintf(fmt, "%s%ds", "%-", REG_PACKET_SIZE);
  sprintf(buffer, fmt, REG_DATA_FOOTER);

  return_status = Emit_footer(*IOTypeIndex, buffer);

  /* Free associated memory (will have been allocated if conversion to
     XDR performed) */
  if(IOTypes_table.io_def[*IOTypeIndex].buffer){

    free(IOTypes_table.io_def[*IOTypeIndex].buffer);
    IOTypes_table.io_def[*IOTypeIndex].buffer = NULL;
  }

  *IOTypeIndex = REG_IODEF_HANDLE_NOTSET;

  return return_status;
}

/*----------------------------------------------------------------*/

int Emit_data_slice(int		      IOTypeIndex,
		    int               DataType,
		    int               Count,
		    void             *pData)
{
  int              datatype;
  int              i;
  size_t	   num_bytes_to_send;

  XDR              xdrs;
  int             *iptr;
  float           *fptr;
  double          *dptr;

  /* Check that steering is enabled */
  if(!ReG_SteeringEnabled) return REG_SUCCESS;

  /* Can only call this function if steering lib initialised */
  if (!ReG_SteeringInit) return REG_FAILURE;

  /* check comms connection has been made */
  if (Get_communication_status(IOTypeIndex) !=  REG_SUCCESS)
    return REG_FAILURE;

  /* Check data type, calculate number of bytes to send and convert
     to XDR if required */
  switch(DataType){

  case REG_INT:
    if(IOTypes_table.io_def[IOTypeIndex].use_xdr){
      datatype = REG_XDR_INT;
      num_bytes_to_send = Count*REG_SIZEOF_XDR_INT;

      xdrmem_create(&xdrs, 
		    IOTypes_table.io_def[IOTypeIndex].buffer,
		    num_bytes_to_send,
		    XDR_ENCODE);

      iptr = (int *)pData;

      for(i=0; i<Count; i++){

	xdr_int(&xdrs, iptr++);
      }

      xdr_destroy(&xdrs);
    }
    else{
      datatype = DataType;
      num_bytes_to_send = Count*sizeof(int);
    }
    break;

  case REG_FLOAT:
    if(IOTypes_table.io_def[IOTypeIndex].use_xdr){
      datatype = REG_XDR_FLOAT;
      num_bytes_to_send = Count*REG_SIZEOF_XDR_FLOAT;

      xdrmem_create(&xdrs, 
		    IOTypes_table.io_def[IOTypeIndex].buffer,
		    num_bytes_to_send,
		    XDR_ENCODE);

      fptr = (float *)pData;

      for(i=0; i<Count; i++){

	xdr_float(&xdrs, fptr++);
      }

      xdr_destroy(&xdrs);
    }
    else{
      datatype = DataType;
      num_bytes_to_send = Count*sizeof(float);
    }
    break;

  case REG_DBL:
    if(IOTypes_table.io_def[IOTypeIndex].use_xdr){
      datatype = REG_XDR_DOUBLE;
      num_bytes_to_send = Count*REG_SIZEOF_XDR_DOUBLE;

      xdrmem_create(&xdrs, 
		    IOTypes_table.io_def[IOTypeIndex].buffer,
		    num_bytes_to_send,
		    XDR_ENCODE);

      dptr = (double *)pData;

      for(i=0; i<Count; i++){

	xdr_double(&xdrs, dptr++);
      }

      xdr_destroy(&xdrs);
    }
    else{
      datatype = DataType;
      num_bytes_to_send = Count*sizeof(double);
    }
    break;

  case REG_CHAR:
    datatype = DataType;
    num_bytes_to_send = Count*sizeof(char);
    break;

  default:
    fprintf(stderr, "Emit_data_slice: Unrecognised data type\n");
    return REG_FAILURE;
    break;
  }
 
  /* Send ReG-specific header */

  if( Emit_iotype_msg_header(IOTypeIndex,
			     datatype,
			     Count) != REG_SUCCESS){

    return REG_FAILURE;
  }

  /* Send data */
  return Emit_data(IOTypeIndex,
		   datatype,
		   num_bytes_to_send,
		   pData);

}

/*----------------------------------------------------------------*/

int Register_params(int    NumParams,
		    char* *ParamLabels,
		    int   *ParamSteerable,
		    void **ParamPtrs,
		    int   *ParamTypes)
{
  int i;
  int current;
  
  /* Check that steering is enabled */

  if(!ReG_SteeringEnabled) return REG_SUCCESS;

  /* Can only call this function if steering lib initialised */

  if (!ReG_SteeringInit) return REG_FAILURE;

  for(i=0; i<NumParams; i++){

    /* Find next free entry - allocates more memory if required */
    current = Next_free_param_index(&Params_table);

    if(current == -1){

      fprintf(stderr, "Register_params: failed to get find free "
	      "param entry\n");
      return REG_FAILURE;
    }

    /* Store label */
    strcpy(Params_table.param[current].label, ParamLabels[i]);

    /* Store 'steerable' */
    Params_table.param[current].steerable = ParamSteerable[i];
    
    /* Store pointer */
    Params_table.param[current].ptr = ParamPtrs[i];

    /* Store type */
    Params_table.param[current].type = ParamTypes[i];

    /* This set to TRUE external to this routine if this param.
       has been created by the steering library itself */
    Params_table.param[current].is_internal = FALSE;

    /* Create handle for this parameter */
    Params_table.param[current].handle = Params_table.next_handle++;

    Params_table.num_registered++;
  }

  /* Flag that the registered parameters have changed */
  ReG_ParamsChanged = TRUE;

  return REG_SUCCESS;
}

/*----------------------------------------------------------------*/

int Steering_control(int     SeqNum,
		     int    *NumSteerParams,
		     char  **SteerParamLabels,
		     int    *NumSteerCommands,
		     int    *SteerCommands,
		     char  **SteerCmdParams)
{
  int    i;
  int    status;
  int    count;
  int    detached;
  int    return_status;
  int    num_commands;
  int    commands[REG_MAX_NUM_STR_CMDS];
  int    param_handles[REG_MAX_NUM_STR_PARAMS];
  char*  param_labels[REG_MAX_NUM_STR_PARAMS];

  /* Variables for timing */
  float          time_per_step;
  clock_t        new_time;
  static clock_t previous_time = 0;
  static int     first_time    = TRUE;

  return_status     = REG_SUCCESS;
  *NumSteerParams   = 0;
  *NumSteerCommands = 0;

  /* Check that steering is enabled */

  if(!ReG_SteeringEnabled) return REG_SUCCESS;

  /* Can only call this function if steering lib initialised */

  if (!ReG_SteeringInit) return REG_FAILURE;

  /* Check to see if a steerer is trying to get control */

  if(!ReG_SteeringActive){

    if(Steerer_connected() == REG_SUCCESS){

      ReG_SteeringActive = TRUE;
      first_time = TRUE;
#if DEBUG
      fprintf(stderr, "Steering_control: steerer has connected\n");
#endif
    }
  }

  /* If we're being steered then... */

  if(ReG_SteeringActive){

    /* Update any library-controlled monitored variables */
    i = Param_index_from_handle(&(Params_table), REG_SEQ_NUM_HANDLE);
    if(i != -1){
      sprintf(Params_table.param[i].value, "%d", SeqNum);
      Params_table.param[i].modified = TRUE;
    }

    i = Param_index_from_handle(&(Params_table), REG_STEP_TIME_HANDLE);
    if(i != -1){

      new_time = clock();
      time_per_step = (float)(new_time - previous_time)/(float)CLOCKS_PER_SEC;
      previous_time = new_time;

      /* First value we get will be rubbish because need two passes 
	 through to get a valid difference... */
      if(!first_time){
	sprintf(Params_table.param[i].value, "%.3f", time_per_step);
        Params_table.param[i].modified = TRUE;
      }
      else{
	first_time = FALSE;
      }    
    }

    /* If registered params have changed since the last time then
       tell the steerer about the current set */
    if(ReG_ParamsChanged){
      
      Emit_param_defs();

#if DEBUG
      fprintf(stderr, "Steering_control: done Emit_param_defs\n");
#endif

      ReG_ParamsChanged  = FALSE;
    }

    /* If the registered IO types have changed since the last time
       then tell the steerer about the current set */
    if(ReG_IOTypesChanged){

      Emit_IOType_defs();

#if DEBUG
      fprintf(stderr, "Steering_control: done Emit_IOType_defs\n");
#endif

      ReG_IOTypesChanged = FALSE;
    }

    /* If the registered Chk types have changed since the last time
       then tell the steerer about the current set */
    if(ReG_ChkTypesChanged){

      Emit_ChkType_defs();

#if DEBUG
      fprintf(stderr, "Steering_control: done Emit_ChkType_defs\n");
#endif

      ReG_ChkTypesChanged = FALSE;
    }


    /* Emit logging info. */
    Emit_log();


    /* Read anything that the steerer has sent to us */
    if( Consume_control(&num_commands,
			commands,
			SteerCmdParams,
			NumSteerParams,
			param_handles,
			param_labels) != REG_SUCCESS ){

      return_status = REG_FAILURE;

#if DEBUG
      fprintf(stderr, "Steering_control: call to Consume_control failed\n");
#endif
    }

    /* Set array holding labels of changed params - pass back strings 
       rather than pointers to strings */

    for(i=0; i<(*NumSteerParams); i++){

      strcpy(SteerParamLabels[i], param_labels[i]);
    }

#if DEBUG
    fprintf(stderr, "Steering_control: done Consume_control\n");
#endif

    /* Parse list of commands for any that we can handle ourselves */

    count = 0;
    i     = 0;
    detached = FALSE;

    while(i<num_commands){

      switch(commands[i]){

      case REG_STR_DETACH:

#if DEBUG
        fprintf(stderr, "Steering_control: got detach command\n");
#endif

	if( Detach_from_steerer() != REG_SUCCESS){

	  return_status = REG_FAILURE;
	}

	/* Confirm that we have received the detach command */
	commands[0] = REG_STR_DETACH;
	Emit_status(SeqNum,
		    0,   
		    NULL,
		    1,
		    commands);

        detached = TRUE;
	break;

      default:

#if DEBUG
        fprintf(stderr, "Steering_control: got command %d\n", commands[i]);
#endif

        SteerCommands[count] = commands[i];
	strcpy(SteerCmdParams[count], SteerCmdParams[i]);
	count++;

	/* If we've received a stop command then do just that - don't
	   mess about */

	if(commands[i] == REG_STR_STOP){

	  if( Detach_from_steerer() != REG_SUCCESS){

	    return_status = REG_FAILURE;
	  }

	  /* Confirm that we have received the stop command */
	  commands[0] = REG_STR_STOP;
          Emit_status(SeqNum,
		      0,   
		      NULL,
		      1,
		      commands);

	  detached = TRUE;
	}

	break;
      }

      /* If we get a 'detach' command then don't process anything
         else */
      if(detached)break;

      i++;
    }

    /* Tell the steerer what we've been doing */
    if( !detached ){

      /* Currently don't support returning a copy of the data just 
	 received from the steerer - hence NULL's below */
      status = Emit_status(SeqNum,
			   0,    /* *NumSteerParams, */
			   NULL, /* param_handles,   */
			   *NumSteerCommands,
			   commands);

      if(status != REG_SUCCESS){

	fprintf(stderr, "Steering_control: call to Emit_status failed\n");
	return_status = REG_FAILURE;
      }

      /* Deal with automatic emission/consumption of data */
      Auto_generate_steer_cmds(SeqNum, &count, SteerCommands, SteerCmdParams);

    }

    /* Record how many commands we're going to pass back to caller */
    *NumSteerCommands = count;

  } /* End if steering active */

  return return_status;
}

/*----------------------------------------------------------------*/

int Auto_generate_steer_cmds(int    SeqNum,
			     int   *posn, 
			     int   *SteerCommands, 
			     char **SteerCmdParams)
{
  int i;
  int return_status = REG_SUCCESS;

  /* IOTypes cannot be deleted so the num_registered entries that
     we have will be contiguous within the table */
  for(i=0; i<IOTypes_table.num_registered; i++){

    /* A freq. of zero indicates no automatic emit/consume */
    if(IOTypes_table.io_def[i].frequency == 0) continue;

    if(fmod(SeqNum, IOTypes_table.io_def[i].frequency) >= REG_TOL_ZERO)continue;

    if( *posn >= REG_MAX_NUM_STR_CMDS ){
      fprintf(stderr, "Auto_generate_steer_cmds: WARNING: discarding "
	      "steering cmds as max number (%d) exceeded\n", 
	      REG_MAX_NUM_STR_CMDS);

      return_status = REG_FAILURE;
      break;
    }
 
    /* Add command to list to send back to caller */
    SteerCommands[*posn]  = IOTypes_table.io_def[i].handle;

    switch(IOTypes_table.io_def[i].direction){

    case REG_IO_IN:
      sprintf(SteerCmdParams[*posn], "IN");
      break;

    case REG_IO_OUT:
    case REG_IO_INOUT:
      sprintf(SteerCmdParams[*posn], "OUT");
      break;

    default:
      sprintf(SteerCmdParams[*posn], " ");
      break;
    }
    (*posn)++;
  }

  /* Repeat for Chk types */
  for(i=0; i<ChkTypes_table.num_registered; i++){

    /* A freq. of zero indicates no automatic emit/consume */
    if(ChkTypes_table.io_def[i].frequency == 0) continue;

    if(fmod(SeqNum, ChkTypes_table.io_def[i].frequency) >= REG_TOL_ZERO)continue;

    if( *posn >= REG_MAX_NUM_STR_CMDS ){
      fprintf(stderr, "Auto_generate_steer_cmds: WARNING: discarding "
	      "steering cmds as max number (%d) exceeded\n", 
	      REG_MAX_NUM_STR_CMDS);

      return_status = REG_FAILURE;
      break;
    }
 
    /* Add command to list to send back to caller */
    SteerCommands[*posn]  = ChkTypes_table.io_def[i].handle;

    /* We only ever instruct the app. to emit checkpoints since to consume
       a checkpoint implies a restart */
    sprintf(SteerCmdParams[*posn], "OUT");

    (*posn)++;
  }

  return return_status;
}

/*----------------------------------------------------------------*/

int Steering_pause(int   *NumSteerParams,
		   char **SteerParamLabels,
		   int   *NumCommands,
		   int   *SteerCommands,
		   char **SteerCmdParams)
{
  int    paused        = TRUE;
  int    return_status = REG_SUCCESS;
  int    i, j, index;
  int    seqnum;
  int    num_commands;
  int    commands[REG_MAX_NUM_STR_CMDS];
  int    param_handles[REG_MAX_NUM_STR_PARAMS];
  char*  param_labels[REG_MAX_NUM_STR_PARAMS];
  int    tot_num_params = 0;

  /* Can only call this function if steering lib initialised */

  if (!ReG_SteeringInit) return REG_FAILURE;

  /* Pause the application by waiting for a 'resume' or 'detach'
     (failsafe) command from the steerer.  If comms link goes 
     down then could remain paused indefinitely? */

  while(paused){

    sleep(1);

    /* Read anything that the steerer has sent to us */

    if( Consume_control(&num_commands,
			commands,
			SteerCmdParams,
			NumSteerParams,
			param_handles,
			param_labels) != REG_SUCCESS ){

      return_status = REG_FAILURE;
      paused = FALSE;
#if DEBUG
      fprintf(stderr, "Steering_pause: call to Consume_control failed\n");
#endif
    }
    else{

#if DEBUG
      fprintf(stderr,"Steering_pause: got %d cmds and %d params\n", 
	      num_commands,
	      *NumSteerParams);
#endif

      /* Add to array holding labels of changed params - pass back  
	 strings rather than pointers */

      for(j=0; j<(*NumSteerParams); j++){

	if(tot_num_params < REG_MAX_NUM_STR_PARAMS){
	  strcpy(SteerParamLabels[tot_num_params], param_labels[j]);
	  tot_num_params++;
	}
	else{

	  fprintf(stderr, "Steering_pause: no. of parameters edited "
	          "exceeds %d\n", REG_MAX_NUM_STR_PARAMS);
	  fprintf(stderr, "                Only returning the first %d\n",
		  REG_MAX_NUM_STR_PARAMS);
        }
      }

      /* Check for a resume command - any other commands are
	 ignored (although Consume_control will have updated the
         parameter tables & the associated simulation variables) */

      for(i=0; i<num_commands; i++){

	if(commands[i] == REG_STR_RESUME){

	  paused = FALSE;

	  /* Return all commands that follow the resume command */

	  *NumCommands = num_commands - i - 1;
	  for(j=0; j<*NumCommands; j++){
	    SteerCommands[j] = commands[i + 1 + j];
	    strcpy(SteerCmdParams[j], SteerCmdParams[i + 1 + j]);
	  }

	  break;
	}
	else if(commands[i] == REG_STR_DETACH){

	  paused = FALSE;
	  return_status = Detach_from_steerer();

	  /* Confirm that we have received the detach command */

	  index = Param_index_from_handle(&(Params_table), REG_SEQ_NUM_HANDLE);
	  if(index != -1){
	    sscanf(Params_table.param[index].value, "%d", &seqnum);
	  }
	  else{
	    seqnum = -1;
	  }

	  commands[0] = REG_STR_DETACH;
	  Emit_status(seqnum,
		      0,   
		      NULL,
		      1,
		      commands);

	  *NumCommands  = 0;
	  break;
	}
	else if(commands[i] == REG_STR_STOP){

	  paused = FALSE;
	  return_status = Detach_from_steerer();

	  /* Confirm that we have received the stop command */

	  index = Param_index_from_handle(&(Params_table), REG_SEQ_NUM_HANDLE);
	  if(index != -1){
	    sscanf(Params_table.param[index].value, "%d", &seqnum);
	  }
	  else{
	    seqnum = -1;
	  }
	  commands[0] = REG_STR_STOP;
          Emit_status(seqnum,
		      0,   
		      NULL,
		      1,
		      commands);

	  /* Return the stop command so app can act on it */
	  *NumCommands = 1;
	  SteerCommands[0] = REG_STR_STOP;
	  break;
	}
      }
    }
  }

  /* Return the total no. of parameters that have been edited
     while the application was paused */
  *NumSteerParams = tot_num_params;

  return return_status;
}

/*----------------------------------------------------------------
              Low-level steering routines
----------------------------------------------------------------*/

int Emit_param_defs(){

  int   i;
  char  buf[REG_MAX_MSG_SIZE];
  char *pbuf;

  /* Check to see that we do actually have something to emit */
  if (Params_table.num_registered == 0) return REG_SUCCESS;

  /* Emit all currently registered parameters */
  
  pbuf = buf;
  Write_xml_header(&pbuf);

  pbuf += sprintf(pbuf, "<Param_defs>\n");
  
  for(i=0; i<Params_table.max_entries; i++){

    /* Check handle because if a parameter is deleted then this is
  	 flagged by unsetting its handle */
  
    if(Params_table.param[i].handle != REG_PARAM_HANDLE_NOTSET){
  
      /* Update the 'value' part of this parameter's table entry */
      if(Get_ptr_value(&(Params_table.param[i])) == REG_SUCCESS){
    	 
	pbuf += sprintf(pbuf, "<Param>\n"
                              "<Label>%s</Label>\n"
			      "<Steerable>%d</Steerable>\n"
			      "<Type>%d</Type>\n"
			      "<Handle>%d</Handle>\n"
			      "<Value>%s</Value>\n", 
			Params_table.param[i].label, 
			Params_table.param[i].steerable,
			Params_table.param[i].type,
			Params_table.param[i].handle, 
			Params_table.param[i].value);

	if(Params_table.param[i].is_internal == TRUE){

	  pbuf += sprintf(pbuf, "<Is_internal>TRUE</Is_internal>\n"
			        "</Param>\n");
	}
	else{

	  pbuf += sprintf(pbuf, "<Is_internal>FALSE</Is_internal>\n"
			        "</Param>\n");
	}
      }
    }
  }
  
  pbuf += sprintf(pbuf, "</Param_defs>\n");
  Write_xml_footer(&pbuf);

  /* Physically send the message */
  return Send_status_msg(buf);
}

/*----------------------------------------------------------------*/

int Emit_IOType_defs(){

  int   i;
  char  buf[REG_MAX_MSG_SIZE];
  char *pbuf;

  /* Check that we do actually have something to emit */
  if (IOTypes_table.num_registered == 0) return REG_SUCCESS;

  /* Emit all currently registered IOTypes */
  
  pbuf = buf;
  Write_xml_header(&pbuf);

  pbuf += sprintf(pbuf, "<IOType_defs>\n");
  
  for(i=0; i<IOTypes_table.max_entries; i++){
  
    if(IOTypes_table.io_def[i].handle != REG_IODEF_HANDLE_NOTSET){
  
      pbuf += sprintf(pbuf,"<IOType>\n"
		           "<Label>%s</Label>\n"
		           "<Handle>%d</Handle>\n", 
		      IOTypes_table.io_def[i].label, 
		      IOTypes_table.io_def[i].handle);

      switch(IOTypes_table.io_def[i].direction){

      case REG_IO_IN:
        pbuf += sprintf(pbuf,"<Direction>IN</Direction>\n");
	break;

      case REG_IO_OUT:
        pbuf += sprintf(pbuf,"<Direction>OUT</Direction>\n");
	break;

      default:
#if DEBUG
	fprintf(stderr, 
		"Emit_IOType_defs: Unrecognised IOType direction\n");
#endif
	return REG_FAILURE;
      }

      pbuf += sprintf(pbuf,"<Freq_handle>%d</Freq_handle>\n"
                           "</IOType>\n",
	      IOTypes_table.io_def[i].freq_param_handle);
    }
  }
  
  pbuf += sprintf(pbuf,"</IOType_defs>\n");
  Write_xml_footer(&pbuf);

  /* Physically send message */
  return Send_status_msg(buf);
}

/*----------------------------------------------------------------*/

int Emit_ChkType_defs(){

  int   i;
  char  buf[REG_MAX_MSG_SIZE];
  char *pbuf;

  /* Check that we do actually have something to emit */
  if (ChkTypes_table.num_registered == 0) return REG_SUCCESS;

  /* Emit all currently registered ChkTypes */
  
  pbuf = buf;
  Write_xml_header(&pbuf);

  pbuf += sprintf(pbuf, "<ChkType_defs>\n");
  
  for(i=0; i<ChkTypes_table.max_entries; i++){
  
    if(ChkTypes_table.io_def[i].handle != REG_IODEF_HANDLE_NOTSET){
  
      pbuf += sprintf(pbuf,"<ChkType>\n"
		           "<Label>%s</Label>\n"
		           "<Handle>%d</Handle>\n", 
		      ChkTypes_table.io_def[i].label, 
		      ChkTypes_table.io_def[i].handle);

      switch(ChkTypes_table.io_def[i].direction){

      case REG_IO_IN:
        pbuf += sprintf(pbuf,"<Direction>IN</Direction>\n");
	break;

      case REG_IO_OUT:
        pbuf += sprintf(pbuf,"<Direction>OUT</Direction>\n");
	break;

      case REG_IO_INOUT:
	pbuf += sprintf(pbuf,"<Direction>INOUT</Direction>\n");
	break;

      default:
#if DEBUG
	fprintf(stderr, 
		"Emit_ChkType_defs: Unrecognised ChkType direction\n");
#endif
	return REG_FAILURE;
      }

      pbuf += sprintf(pbuf,"<Freq_handle>%d</Freq_handle>\n"
		           "</ChkType>\n",
	      ChkTypes_table.io_def[i].freq_param_handle);
    }
  }
  
  pbuf += sprintf(pbuf,"</ChkType_defs>\n");
  Write_xml_footer(&pbuf);

  /* Physically send message */
  return Send_status_msg(buf);
}

/*----------------------------------------------------------------*/

int Emit_log()
{
  int   i, j;
  char  buf[REG_MAX_MSG_SIZE];
  char *pbuf;
  FILE* fp;

  if (Chk_log.num_unsent == 0) return REG_SUCCESS;

  pbuf = buf;
  Write_xml_header(&pbuf);

  pbuf += sprintf(pbuf, "<Steer_log>\n");

  for(i=0; i<Chk_log.num_entries; i++){

    /* Check to see whether steerer already has this entry */
    if (Chk_log.entry[i].sent_to_steerer == TRUE) continue;

    pbuf += sprintf(pbuf, "<Log_entry>\n"
		          "<Key>%d</Key>\n"
		          "<Chk_handle>%d</Chk_handle>\n"
		          "<Chk_tag>%s</Chk_tag>\n", 
		    Chk_log.entry[i].key, 
		    Chk_log.entry[i].chk_handle, 
		    Chk_log.entry[i].chk_tag);

    /* Associated parameters are stored contiguously so need only
       loop over the no. of params that this entry has */
    for(j=0; j<Chk_log.entry[i].num_param; j++){

      pbuf += sprintf(pbuf, "<Param>\n"
		            "<Handle>%d</Handle>\n" 
		            "<Value>%s</Value>\n"
		            "</Param>\n", 
		      Chk_log.entry[i].param[j].handle,
		      Chk_log.entry[i].param[j].value);
    }
    pbuf += sprintf(pbuf, "</Log_entry>\n");

    /* Flag this entry as having been sent to steerer */
    Chk_log.entry[i].sent_to_steerer = TRUE;
  }

  pbuf += sprintf(pbuf, "</Steer_log>\n");

  /* Zero counter since we're just about to tell steerer all about any log
     entries it hasn't got */
  Chk_log.num_unsent = 0;

  /* Complete the xml message */
  Write_xml_footer(&pbuf);

  /* ARPDBG */
  fp = fopen("SteeringLog.xml", "a");
  if(fp){
    fprintf(fp, "%s", buf);
    fclose(fp);
  }

  /* Physically send message */
  return Send_status_msg(buf);
}

/*----------------------------------------------------------------*/

int Consume_control(int    *NumCommands,
		    int    *Commands,
		    char  **CommandParams,
		    int    *NumSteerParams,
		    int    *SteerParamHandles,
		    char  **SteerParamLabels){

  int                  j;
  int                  count;
  char                *ptr;
  struct msg_struct   *msg;
  struct cmd_struct   *cmd;
  struct param_struct *param;
  int                  handle;
  int                  return_status = REG_SUCCESS;
  
  /* Read the file produced by the steerer - may contain commands and/or
     new parameter values */

  if((msg = Get_control_msg()) != NULL){

    if(msg->control){

      cmd   = msg->control->first_cmd;
      count = 0;

      while(cmd){

	sscanf((char *)(cmd->id), "%d", &(Commands[count]));

	if(cmd->first_param){

	  param = cmd->first_param;
	  ptr   = CommandParams[count];
	  
	  while(param){

	    if(param->value){
	      sprintf(ptr, "%s ", (char *)(param->value));
	      ptr += strlen((char *)param->value) + 1;
	    }

	    param = param->next;
	  }
	}
	else{

	  sprintf(CommandParams[count], " ");
	}
#if DEBUG
	fprintf(stderr, "Consume_control: cmd[%d] = %d\n", count,
		Commands[count]);
	fprintf(stderr, "                 params  = %s\n", 
		CommandParams[count]);
#endif
	count++;

	if(count >= REG_MAX_NUM_STR_CMDS){

	  fprintf(stderr, 
		  "Consume_control: WARNING: truncating list of commands\n");
	  break;
	}

	cmd = cmd->next;
      }

      *NumCommands = count;

#if DEBUG
      fprintf(stderr, "Consume_control: received %d commands\n", 
	      *NumCommands);
#endif

      param = msg->control->first_param;
      count = 0;

      while(param){

	sscanf((char *)(param->handle), "%d", &handle);

	for(j=0; j<Params_table.max_entries; j++){
  
	  if(Params_table.param[j].handle == handle){
	  
	    break;
	  }
	}

	if(j == Params_table.max_entries){
  
	  fprintf(stderr, "Consume_control: failed to match param "
		  "handles\n");
	  return_status = REG_FAILURE;
	}
	else{

	  /* Store char representation of new parameter value */
	  if(param->value){

	    strcpy(Params_table.param[j].value, (char *)(param->value));

	    /* Update value associated with pointer */
	    Update_ptr_value(&(Params_table.param[j]));

	    if( !(Params_table.param[j].is_internal) ){

	      SteerParamHandles[count] = handle;
	      SteerParamLabels[count]  = Params_table.param[j].label;
	      count++;
	    }
	  }
	  else{
	    fprintf(stderr, "Consume_control: empty parameter value "
		    "field\n");
	  }
	}

	param = param->next;
      }

      /* Update the number of parameters received to allow for fact that
	 some may be internal and are not passed up to the calling routine */
      *NumSteerParams = count;

#if DEBUG
	fprintf(stderr, "Consume_control: received %d params\n", 
		*NumSteerParams);
#endif
    }
    else{
      fprintf(stderr, "Consume_control: error, no control data\n");
      *NumSteerParams = 0;
      *NumCommands    = 0;
      return_status   = REG_FAILURE;
    }

    /* free up msg memory */
    Delete_msg_struct(msg);
    msg = NULL;

  }
  else{

#if DEBUG
    fprintf(stderr, "Consume_control: no message from steerer\n");
#endif

    /* No message found */

    *NumSteerParams = 0;
    *NumCommands = 0;
  }

  return return_status;
}

/*----------------------------------------------------------------*/

int Generate_status_filename(char* filename)
{

#ifdef UNICORE_DEMO

  /* Always just output <path>/steer_status for UNICORE demo */
  sprintf(filename, "%ssteer_status", Steerer_connection.file_root);

#else /* Not UNICORE demo - use full, indexed filenames */

  static int output_file_index = 0;

  /* Generate next filename in sequence for sending data to
     steerer & increment counter */

  sprintf(filename, "%s%s_%d", Steerer_connection.file_root, 
	  APP_TO_STR_FILENAME, output_file_index++);

  /* Wrap counter if no. of distinct files exceeded */

  if(output_file_index == REG_MAX_NUM_FILES) output_file_index = 0;

#endif /* UNICORE_DEMO */

  return REG_SUCCESS;
}

/*----------------------------------------------------------------*/

int Detach_from_steerer()
{
  char  filename[REG_MAX_STRING_LENGTH];

  /* Remove lock file that indicates app is being steered */

  sprintf(filename, "%s%s", Steerer_connection.file_root, 
	  STR_CONNECTED_FILENAME);
  remove(filename);

  /* Remove any files that steerer has produced that we won't
     now be consuming */

  sprintf(filename, "%s%s", Steerer_connection.file_root, 
	  STR_TO_APP_FILENAME);
  Remove_files(filename);
  
  ReG_SteeringActive = FALSE;
  ReG_IOTypesChanged = TRUE;
  ReG_ChkTypesChanged= TRUE;
  ReG_ParamsChanged  = TRUE;

  return REG_SUCCESS;
}

/*----------------------------------------------------------------*/

int Emit_status(int   SeqNum,
		int   NumParams,
		int  *ParamHandles,
		int   NumCommands,
		int  *Commands)
{
  int   i;
  int   pcount = 0;
  int   tot_pcount = 0;
  int   ccount = 0;
  int   num_param;
  int   cmddone   = FALSE;
  int   paramdone = FALSE;
  char  buf[REG_MAX_MSG_SIZE];
  char *pbuf;

  /* Emit a status report - this is complicated because we must ensure we
     don't write too many params or commands to a single file (self-imposed
     limits to make it easier for user to supply arrays to receive results) */

  /* Count how many monitoring parameters there are */

  for(i=0; i<Params_table.max_entries; i++){
    
    if(Params_table.param[i].handle != REG_PARAM_HANDLE_NOTSET) 
	/* Want to output ALL params now - not just steerable ones */
	/* && (!Params_table.param[i].steerable) ) */  pcount++;
  }
  num_param = pcount;
  pcount = 0;

  /* If we are sending a 'detach' command then don't send any
     parameter values */
  if(NumCommands > 0){

    if(Commands[0] == REG_STR_DETACH){

      paramdone = TRUE;
    }
  }

  if(NumCommands == 0) cmddone = TRUE;
  if(num_param == 0) paramdone = TRUE;

  /* Loop until all params and commands have been emitted */

  while(!paramdone || !cmddone){

    pbuf = buf;

    Write_xml_header(&pbuf);
    pbuf += sprintf(pbuf, "<App_status>\n");

    /* Parameter values section */

    if(!paramdone){

      /* Loop over max. no. of params to write to any given file */

      for(i=0; i<REG_MAX_NUM_STR_PARAMS; i++){
  
    	/* Handle value used to indicate whether entry is valid */
    	if(Params_table.param[tot_pcount].handle != REG_PARAM_HANDLE_NOTSET){

 	  /* Update the 'value' part of this parameter's table entry
	     - Get_ptr_value checks to make sure parameter is not library-
	     controlled (& hence has valid ptr to get value from) */
 	  Get_ptr_value(&(Params_table.param[tot_pcount]));

 	  pbuf += sprintf(pbuf, "<Param>\n"
			        "<Handle>%d</Handle>\n"
			        "<Value>%s</Value>\n"
			        "</Param>\n", 
 		  Params_table.param[tot_pcount].handle, 
		  Params_table.param[tot_pcount].value);
  
 	  pcount++;
    	}
  
	/* Cumulative counter to move us through param table */
	tot_pcount++;

    	if(pcount >= num_param){
 	  paramdone = TRUE;
 	  break;
    	}
      }
    }

    /* Commands section */

    if(!cmddone){

#if DEBUG
      fprintf(stderr, "Emit_status: NumCommands = %d, ccount = %d\n", 
	      NumCommands, ccount);
#endif

      for(i=0; i<REG_MAX_NUM_STR_CMDS; i++){
  
    	pbuf += sprintf(pbuf, "<Command>\n"
			      "<Cmd_id>%d</Cmd_id>\n"
			      "</Command>\n", 
			Commands[ccount]);
    	ccount++;
  
    	if(ccount >= NumCommands){
 	  cmddone = TRUE;
 	  break;
    	}
      }
    }

    pbuf += sprintf(pbuf, "</App_status>\n");

    Write_xml_footer(&pbuf);

    /* Physically send the status message */
    Send_status_msg(buf);
  }

  return REG_SUCCESS;
}

/*----------------------------------------------------------------*/

int Update_ptr_value(param_entry *param)
{

  switch(param->type){

  case REG_INT:
    sscanf(param->value, "%d", (int *)(param->ptr));
    break;

  case REG_FLOAT:
    sscanf(param->value, "%f", (float *)(param->ptr));
    break;

  case REG_DBL:
    sscanf(param->value, "%lf", (double *)(param->ptr));
    break;

  case REG_CHAR:
    strcpy((char *)(param->ptr), param->value);
    break;

  default:
    fprintf(stderr, "Update_ptr_value: unrecognised parameter type\n");
    return REG_FAILURE;
  }

  return REG_SUCCESS;
}

/*----------------------------------------------------------------*/

int Get_ptr_value(param_entry *param)
{
  int return_status;

  /* Retrieve the value of the variable pointed to by this parameter's
     registered pointer and store it in the 'value' character string
     of the table entry */

  return_status = REG_SUCCESS;
  
  /* If this is a special parameter then its pointer isn't used */
  if(param->handle == REG_SEQ_NUM_HANDLE || 
     param->handle == REG_STEP_TIME_HANDLE){

    return REG_SUCCESS;
  }

  switch(param->type){
  
  case REG_INT:
    sprintf(param->value,"%d", *((int *)(param->ptr)));
    break;
  
  case REG_FLOAT:
    sprintf(param->value,"%f", *((float *)(param->ptr)));
    break;
  
  case REG_DBL:
    sprintf(param->value,"%lf", *((double *)(param->ptr)));
    break;
  
  case REG_CHAR:
    strcpy(param->value, (char *)(param->ptr));
    break;
  
  default:
    fprintf(stderr, "Get_ptr_value: unrecognised parameter type\n");
    fprintf(stderr, "Param type   = %d\n", param->type);
    fprintf(stderr, "Param handle = %d\n", param->handle);
    fprintf(stderr, "Param label  = %s\n", param->label);

    return_status = REG_FAILURE;
    break;
  }

  return return_status;
}

/*----------------------------------------------------------------*/

void Steering_signal_handler(int aSignal)
{
  
  /* caught one signal - ignore all others now as going to quit and do not
     want the quit process to be interrupted and restarted... */
  signal(SIGINT, SIG_IGN); 
  signal(SIGTERM, SIG_IGN);
  signal(SIGSEGV, SIG_IGN);
  signal(SIGILL, SIG_IGN);
  signal(SIGABRT, SIG_IGN);
  signal(SIGFPE, SIG_IGN);

  switch(aSignal){

    case SIGINT:
      fprintf(stderr, "Interrupt signal received (signal %d)\n", aSignal);
      break;
      
    case SIGTERM:
      fprintf(stderr, "Kill signal received (signal %d)\n", aSignal);
      break;
      
    case SIGSEGV:
      fprintf(stderr, "Illegal Access caught (signal %d)\n", aSignal);
      break;

    case  SIGILL:
      fprintf(stderr, "Illegal Exception caught (signal %d)\n", aSignal);
      break;

      /* note: abort called if exception not caught (and hence calls 
	 terminate) */
    case SIGABRT:
      fprintf(stderr, "Abort signal caught (signal %d)\n", aSignal);
      break;

    case SIGFPE:
      fprintf(stderr, "Arithmetic Exception caught (signal %d)\n", aSignal);
      break;

    default:
      fprintf(stderr, "Signal caught (signal %d)\n", aSignal);

  }

  fprintf(stderr, "Steering library quitting...\n");

  if (Steering_finalize() != REG_SUCCESS){
    fprintf(stderr, "Steering_signal_handler: Steerer_finalize failed");
  }

  exit(0);
}

/*--------------------------------------------------------------------*/

int Make_vtk_buffer(int    nx,
		    int    ny,
		    int    nz,
		    int    veclen,
		    double a,
		    double b,
		    double c,
		    float *array)
{
  int    i, j, k;
  int    count;
  double a2, b2, c2;
  float  mag;
  float  sum;
  float *fptr;

  a2 = a*a;
  b2 = b*b;
  c2 = c*c;

  /* Make an array of data */
  fptr = array;

  sum = 0.0;
  count = 0;

  if(veclen == 1){

    for(i=-nx/2; i<nx/2; i++){
      for(j=-ny/2; j<ny/2; j++){
	for(k=-nz/2; k<nz/2; k++){
          *fptr = (float)sqrt((double)(i*i*a2 + j*j*b2 + k*k*c2));
	  sum += *(fptr++);

	  count++;
	}
      }
    }
  }
  else if(veclen == 2){

    for(i=-nx/2; i<nx/2; i++){
      for(j=-ny/2; j<ny/2; j++){
	for(k=-nz/2; k<nz/2; k++){

	  mag = 2.0/(1.0 + (float)sqrt((double)(i*i*a2 + j*j*b2 + k*k*c2)));
	  *fptr = mag*(float)(i*k);
	  sum += *(fptr++);
	  *fptr = mag*(float)(j*k);
	  sum += *(fptr++);

	  count = count+2;
	}
      }
    }
  }
  else if(veclen == 3){

    for(i=-nx/2; i<nx/2; i++){
      for(j=-ny/2; j<ny/2; j++){
	for(k=-nz/2; k<nz/2; k++){
	  /*
	  mag = 2.0/(1.0 + (float)sqrt((double)(i*i*a2 + j*j*b2 + k*k*c2)));
	  *fptr = mag*(float)i;
	  sum += *(fptr++);
	  *fptr = mag*(float)j;
	  sum += *(fptr++);
	  *fptr = mag*(float)k;
	  sum += *(fptr++);
	  */
	  mag = 1.0;
	  *fptr = mag*(float)i;
	  sum += *(fptr++);
	  *fptr = mag*(float)j;
	  sum += *(fptr++);
	  *fptr = mag*(float)k;
	  sum += *(fptr++);

	  count = count+3;
	}
      }
    }
  }
  else{
    fprintf(stderr, "Make_vtk_buffer: error, only  1 <= veclen <= 3 "
	    "supported\n");
    return REG_FAILURE;
  }

  fprintf(stderr, "Make_vtk_buffer: checksum = %f\n", sum/((float) count));

  /*
  fptr = array;
  fprintf(stderr, "Array is: \n");
  for(i=0; i<nx; i++){

    fprintf(stderr, "%.3f %.3f %.3f\n", *(fptr++), *(fptr++), *(fptr++));
  }
  */
  return REG_SUCCESS;
}

/*--------------------------------------------------------------------
  Make ASCII header to describe data to vtk
*/
int Make_vtk_header(char  *header,
		    char  *title,
		    int    nx,
		    int    ny,
		    int    nz,
		    int    veclen,
		    int    type)
{
  char *pchar = header;
  char  type_text[8];

  /* Flag to switch between AVS- and vtk-style headers 
     - for testing */
  const int AVS_STYLE = 1;

  if (!pchar) return REG_FAILURE;

  if(veclen != 1 && veclen != 3){

    fprintf(stderr, "Make_vtk_header: only veclen of 1 or 3 supported\n");
    return REG_FAILURE;
  }


  if(AVS_STYLE){

    pchar += sprintf(pchar, "# AVS field file\n");
    pchar += sprintf(pchar, "ndim=3\n");
    pchar += sprintf(pchar, "dim1= %d\n", nx);
    pchar += sprintf(pchar, "dim2= %d\n", ny);
    pchar += sprintf(pchar, "dim3= %d\n", nz);
    pchar += sprintf(pchar, "nspace=3\n");
    pchar += sprintf(pchar, "field=uniform\n");
    pchar += sprintf(pchar, "veclen= %d\n", veclen);

    if(type == REG_DBL){
      sprintf(type_text, "double");
    }
    else if(type == REG_FLOAT){

      sprintf(type_text, "float");
    }
    else if(type == REG_INT){

      sprintf(type_text, "integer");
    }
    else{

      fprintf(stderr, "Make_vtk_header: Unrecognised data type\n");
      return REG_FAILURE;
    }
    pchar += sprintf(pchar, "data=%s\n", type_text);

    /* Use 'filetype=stream' because this is _not_ standard AVS because
       the way we interpret 'skip' at the other end of a socket is
       not standard either - we use it as the number of objects (floats,
       ints etc.) to skip rather than the no. of bytes or lines. */
    if(veclen == 1){
      pchar += sprintf(pchar, "variable 1 filetype=stream "
		       "skip=0000000 stride=1\n");
    }
    else if(veclen == 2){
      pchar += sprintf(pchar, "variable 1 filetype=stream "
		     "skip=0000000 stride=2\n");
      pchar += sprintf(pchar, "variable 2 filetype=stream "
		     "skip=0000001 stride=2\n");

    }
    else if(veclen == 3){
      pchar += sprintf(pchar, "variable 1 filetype=stream "
		       "skip=0000000 stride=3\n");
      pchar += sprintf(pchar, "variable 2 filetype=stream "
		       "skip=0000001 stride=3\n");
      pchar += sprintf(pchar, "variable 3 filetype=stream "
		       "skip=0000002 stride=3\n");
    }
    pchar += sprintf(pchar, "END_OF_HEADER\n");
  }
  else{ /* Make a vtk-style header */

    pchar += sprintf(pchar, "# vtk DataFile Version 2.1\n");
    pchar += sprintf(pchar, "%s\n", title);
    pchar += sprintf(pchar, "BINARY\n");
    pchar += sprintf(pchar, "DATASET STRUCTURED_POINTS\n");
    pchar += sprintf(pchar, "DIMENSIONS %d %d %d\n", nx, ny, nz);
    pchar += sprintf(pchar, "ORIGIN  0.000   0.000   0.000\n");
    pchar += sprintf(pchar, "SPACING  1  1  1\n");
    pchar += sprintf(pchar, "POINT_DATA %d\n", nx*ny*nz);

    if(type == REG_DBL){
      sprintf(type_text, "double");
    }
    else if(type == REG_FLOAT){

      sprintf(type_text, "float");
    }
    else if(type == REG_INT){

      sprintf(type_text, "int");
    }
    else{

      fprintf(stderr, "Make_vtk_header: Unrecognised data type\n");
      return REG_FAILURE;
    }

    if(veclen == 1){

      pchar += sprintf(pchar, "SCALARS scalars %s\n", type_text);
    }
    else if(veclen == 3){

      pchar += sprintf(pchar, "VECTORS vectors %s\n", type_text);
    }
    else{

      fprintf(stderr, "Make_vtk_header: invalid veclen value: %d\n", veclen);
      return REG_FAILURE;
    }

    pchar += sprintf(pchar, "LOOKUP_TABLE default\n");
  }

  return REG_SUCCESS;
}

/*--------------------------------------------------------------------*/

int Steerer_connected()
{

#if REG_GLOBUS_STEERING

  return Steerer_connected_globus();
#else

  return Steerer_connected_file();
#endif
}

/*--------------------------------------------------------------------*/

int Steerer_connected_file()
{
  char   filename[REG_MAX_STRING_LENGTH];
  FILE  *fp;

  sprintf(filename, "%s%s", Steerer_connection.file_root, 
	  STR_CONNECTED_FILENAME);

  if( (fp = fopen(filename, "r")) ){

      fclose(fp);
      return REG_SUCCESS;
  }

  return REG_FAILURE;
}

/*-------------------------------------------------------------------*/

int Send_status_msg(char *buf)
{
#if DEBUG
  fprintf(stderr, "Send_status_msg: sending:\n>>%s<<\n", buf);
#endif

#if REG_GLOBUS_STEERING

  return Send_status_msg_globus(buf);
#else

  return Send_status_msg_file(buf);
#endif
}

/*-------------------------------------------------------------------*/

int Send_status_msg_file(char *buf)
{
  FILE *fp;
  char  filename[REG_MAX_STRING_LENGTH];

  Generate_status_filename(filename);

  if( (fp = fopen(filename, "w")) == NULL){

    fprintf(stderr, "Send_status_msg: failed to open file\n");
    return REG_FAILURE;
  }

  fprintf(fp, "%s", buf);
  fclose(fp);

  Create_lock_file(filename);

  return REG_SUCCESS;
}

/*-------------------------------------------------------------------*/

struct msg_struct *Get_control_msg()
{

#if REG_GLOBUS_STEERING

  return Get_control_msg_globus();

#else

  return Get_control_msg_file();

#endif

}

/*-------------------------------------------------------------------*/

struct msg_struct *Get_control_msg_file()
{
  struct msg_struct   *msg = NULL;
  FILE                *fp;
  char                 filename[REG_MAX_STRING_LENGTH];

  sprintf(filename, "%s%s", Steerer_connection.file_root, STR_TO_APP_FILENAME);

  if( (fp = Open_next_file(filename)) != NULL){

    fclose(fp);

    msg = New_msg_struct();

    if(Parse_xml_file(filename, msg) != REG_SUCCESS){

      fprintf(stderr, "Read_control: failed to parse <%s>\n", filename);
      Delete_msg_struct(msg);
      msg = NULL;
    }

    /* Delete the file once we've read it */
    if( Delete_file(filename) != REG_SUCCESS){

      fprintf(stderr, "Read_control: failed to delete %s\n",filename);
    }
  }

  return msg;
}

/*-------------------------------------------------------------------*/

int Initialize_steering_connection(int  NumSupportedCmds,
				   int *SupportedCmds)
{

#if REG_GLOBUS_STEERING

  return Initialize_steering_connection_globus(NumSupportedCmds,
					       SupportedCmds);
#else

  return Initialize_steering_connection_file(NumSupportedCmds,
					     SupportedCmds);
#endif

}

/*-------------------------------------------------------------------*/

int Initialize_steering_connection_file(int  NumSupportedCmds,
					int *SupportedCmds)
{
  FILE *fp;
  char *pchar;
  char  buf[REG_MAX_MSG_SIZE];
  char  filename[REG_MAX_STRING_LENGTH];
  int   i;

  /* Set location of all comms files */

  pchar = getenv("REG_STEER_DIRECTORY");

  if(pchar){

    /* Check that path ends in '/' - if not then add one */

    i = strlen(pchar);
    if( pchar[i-1] != '/' ){

      sprintf(Steerer_connection.file_root, "%s/", pchar);
    }
    else{

      strcpy(Steerer_connection.file_root, pchar);
    }

    if(Directory_valid(Steerer_connection.file_root) != REG_SUCCESS){

      fprintf(stderr, "Initialize_steering_connection_file: invalid dir for "
	      "steering messages: %s\n", Steerer_connection.file_root);
      return REG_FAILURE;
    }
    else{
      fprintf(stderr, "Using following dir for steering messages: %s\n", 
	     Steerer_connection.file_root);
    }
  }
  else{
    fprintf(stderr, "Initialize_steering_connection_file: failed to get "
	    "scratch directory\n");
    return REG_FAILURE;
  }

  /* Clean up any old files... */

  /* ...file indicating a steerer is connected (which it can't be since we've
     only just begun) */ 
  sprintf(filename, "%s%s", Steerer_connection.file_root, 
	  STR_CONNECTED_FILENAME);
  fp = fopen(filename, "w");
  if(fp != NULL){

    fclose(fp);
    if(remove(filename)){

      fprintf(stderr, "Initialize_steering_connection_file: failed to "
	      "remove %s\n",filename);
    }
#if DEBUG
    else{
      fprintf(stderr, "Initialize_steering_connection_file: removed "
	      "%s\n", filename);
    }
#endif
  }

  /* ...files containing messages from a steerer */
  sprintf(filename, "%s%s", Steerer_connection.file_root, 
	  STR_TO_APP_FILENAME);

  Remove_files(filename);

  /* Signal that component is available to be steered */

  sprintf(filename, "%s%s", Steerer_connection.file_root, 
	                    APP_STEERABLE_FILENAME);
  fp = fopen(filename,"w");

  if(fp == NULL){

    fprintf(stderr, "Initialize_steering_connection_file: failed to open %s\n",
	    filename);
    return REG_FAILURE;
  }

#if DEBUG
  fprintf(stderr, "Initialize_steering_connection_file: writing file: %s\n", 
	  filename);
#endif

  Make_supp_cmds_msg(NumSupportedCmds, SupportedCmds, buf);

  fprintf(fp, "%s", buf);
  fclose(fp);

  return REG_SUCCESS;
}

/*-------------------------------------------------------------------*/

int Finalize_steering_connection()
{

#if REG_GLOBUS_STEERING

  return Finalize_steering_connection_globus();
#else

  return Finalize_steering_connection_file();
#endif

}

/*-------------------------------------------------------------------*/

int Finalize_steering_connection_file()
{
  int  max, max1;
  char sys_command[REG_MAX_STRING_LENGTH];

  max = strlen(APP_STEERABLE_FILENAME);
  max1 = strlen(STR_CONNECTED_FILENAME);

  if(max1 > max) max=max1;
  
  max += strlen(Steerer_connection.file_root);
  if(max > REG_MAX_STRING_LENGTH ){

    fprintf(stderr, "Finalize_steering_connection: WARNING: truncating "
	    "filename\n");
  }

  /* Delete the lock file that indicates we are steerable */
  sprintf(sys_command, "%s%s", Steerer_connection.file_root,
	  APP_STEERABLE_FILENAME);
  if(remove(sys_command)){

    fprintf(stderr, "Finalize_steering_connection: failed to remove "
	    "%s\n", sys_command);
  }

  /* Delete the lock file that indicates we are being steered */
  if(ReG_SteeringActive){

    sprintf(sys_command, "%s%s", Steerer_connection.file_root,
	    STR_CONNECTED_FILENAME);
    if(remove(sys_command)){

      fprintf(stderr, "Finalize_steering_connection: failed to remove "
	      "%s\n", sys_command);
    }
  }

  /* Delete any files we'd have consumed if we'd lived longer */
  sprintf(sys_command, "%s%s", Steerer_connection.file_root, 
	  STR_TO_APP_FILENAME);
  Remove_files(sys_command);

  return REG_SUCCESS;
}
/*---------------------------------------------------*/

int Make_supp_cmds_msg(int   NumSupportedCmds,
		       int  *SupportedCmds, 
                       char *msg)
{
  char *pchar;
  int   i;

  pchar = msg;

  Write_xml_header(&pchar);

  pchar += sprintf(pchar, "<Supported_commands>\n");

  for(i=0; i<NumSupportedCmds; i++){
    pchar += sprintf(pchar, "<Command>\n");
    pchar += sprintf(pchar, "<Cmd_id>%d</Cmd_id>\n", SupportedCmds[i]);
    pchar += sprintf(pchar, "</Command>\n");
  }

  pchar += sprintf(pchar, "</Supported_commands>\n");

  Write_xml_footer(&pchar);

  return REG_SUCCESS;
}

/*---------------------------------------------------*/

int Initialize_IOType_transport(const int direction,
				const int index)
{

#if REG_GLOBUS_SAMPLES

  return Initialize_IOType_transport_globus(direction, index);
#else

  return REG_SUCCESS;
#endif

}

/*---------------------------------------------------*/

void Finalize_IOType_transport()
{

#if REG_GLOBUS_SAMPLES

  Finalize_IOType_transport_globus();

#endif

}

/*---------------------------------------------------*/

int Consume_start_data_check(const int index)
{

#if REG_GLOBUS_SAMPLES

  return Consume_start_data_check_globus(index);
#else

  return REG_FAILURE;
#endif

}

/*---------------------------------------------------*/

int Consume_data_read(const int		index,  
		      const int		datatype,
		      const size_t	num_bytes_to_read, 
		      void		*pData)
{

#if REG_GLOBUS_SAMPLES

  return Consume_data_read_globus(index,
				  datatype,
				  num_bytes_to_read,
				  pData);
#else

  return REG_FAILURE;
#endif

}

/*---------------------------------------------------*/

int Emit_header(const int index)
{
#if REG_GLOBUS_SAMPLES

  return Emit_header_globus(index);
#else

  return REG_FAILURE;
#endif

}

/*---------------------------------------------------*/

int Emit_footer(const int index,
		const char * const buffer)
{
#if REG_GLOBUS_SAMPLES

  return Emit_footer_globus(index, buffer);
#else

  return REG_FAILURE;
#endif

}

/*---------------------------------------------------*/

int Emit_data(const int		index,  
	      const int		datatype,
	      const size_t	num_bytes_to_send,
	      void		*pData )
{
#if REG_GLOBUS_SAMPLES

  return Emit_data_globus(index, 
			  datatype,
			  num_bytes_to_send,
			  pData);
#else

  return REG_FAILURE;
#endif

}

/*---------------------------------------------------*/

int Get_communication_status(const int	index)
{

#if REG_GLOBUS_SAMPLES

  return Get_communication_status_globus(index);
#else

  return REG_FAILURE;
#endif

}

/*---------------------------------------------------*/

int Consume_iotype_msg_header(int IOTypeIndex, /*socket_io_type *sock_info,*/
			      int *DataType,
			      int *Count)
{
  
#if REG_GLOBUS_SAMPLES
  return Consume_msg_header_globus(&(IOTypes_table.io_def[IOTypeIndex].socket_info), /*sock_info,*/
				   DataType,
				   Count);
#else

  return REG_FAILURE;
#endif

}

/*----------------------------------------------------------------*/

int Emit_iotype_msg_header(int IOTypeIndex,
			   int DataType,
			   int Count)
{

#if REG_GLOBUS_SAMPLES
  return Emit_msg_header_globus(&(IOTypes_table.io_def[IOTypeIndex].socket_info),
				DataType,
				Count);
#else

  return REG_FAILURE;
#endif

}
