/*----------------------------------------------------------------------------
  This file contains routines and data structures for SOAP-based 
  steering communication.

  (C) Copyright 2006, University of Manchester, United Kingdom,
  all rights reserved.

  This software was developed by the RealityGrid project
  (http://www.realitygrid.org), funded by the EPSRC under grants
  GR/R67699/01 and GR/R67699/02.

  LICENCE TERMS

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:
  1. Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
  2. Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in the
     documentation and/or other materials provided with the distribution.

  THIS MATERIAL IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
  A PARTICULAR PURPOSE ARE DISCLAIMED. THE ENTIRE RISK AS TO THE QUALITY
  AND PERFORMANCE OF THE PROGRAM IS WITH YOU.  SHOULD THE PROGRAM PROVE
  DEFECTIVE, YOU ASSUME THE COST OF ALL NECESSARY SERVICING, REPAIR OR
  CORRECTION.

  Authors........: Andrew Porter, Robert Haines

---------------------------------------------------------------------------*/

/** @file ReG_Steer_Appside_Soap.c
    @brief Source file for Soap-related Appside routines
  */

#include "ReG_Steer_types.h"
#include "ReG_Steer_Appside_internal.h"
#include "ReG_Steer_Appside_Soap.h"
#include "ReG_Steer_Logging.h"

#ifndef WIN32
#else
#define snprintf _snprintf
#endif

/* Need access to these tables which are actually declared in 
   ReG_Steer_Appside_internal.h and ReG_Steer_Appside.c */
extern IOdef_table_type IOTypes_table;

extern Steerer_connection_table_type Steerer_connection;

extern Param_table_type Params_table;

extern Chk_log_type Chk_log;

/* Absolute path of directory we are executing in */
extern char ReG_CurrentDir[REG_MAX_STRING_LENGTH];

/* Hostname of machine we are executing on */
extern char ReG_Hostname[REG_MAX_STRING_LENGTH];

/* Name (and version) of the application that has called us */
extern char ReG_AppName[REG_MAX_STRING_LENGTH];

extern char Global_scratch_buffer[];

/* Soap-specific declarations */

/** The gSoap environment structure
    static struct soap soap;*/

/** Names of the SGS' service data elements - MUST match those
    used in SGS.pm (as launched by container) */
char *SUPPORTED_CMDS_SDE = "SGS:Supp_cmds";
/** @see SUPPORTED_CMDS_SDE */
char *PARAM_DEFS_SDE     = "SGS:Param_defs";
/** @see SUPPORTED_CMDS_SDE */
char *IOTYPE_DEFS_SDE    = "SGS:IOType_defs";
/** @see SUPPORTED_CMDS_SDE */
char *CHKTYPE_DEFS_SDE   = "SGS:ChkType_defs";
/** @see SUPPORTED_CMDS_SDE */
char *STEER_STATUS_SDE   = "SGS:Steerer_status";
/** @see SUPPORTED_CMDS_SDE */
char *MACHINE_ADDRESS_SDE= "SGS:Machine_address";
/** @see SUPPORTED_CMDS_SDE */
char *WORKING_DIR_SDE    = "SGS:Working_directory";
/** @see SUPPORTED_CMDS_SDE */
char *APP_NAME_SDE       = "SGS:Application_name";

/*-------------------------------------------------------------------------*/

int Initialize_steering_connection_soap(int  NumSupportedCmds,
					int *SupportedCmds)
{
  struct sgs__setServiceDataResponse setSDE_response;
  struct sgs__AppStartResponse       appStart_response;
  char                              *pchar;
  char                               query_buf[REG_MAX_MSG_SIZE];

  /* malloc memory for soap struct for this connection and then
     initialise it */
  if(!(Steerer_connection.SGS_info.soap = 
                (struct soap*)malloc(sizeof(struct soap)))){

    fprintf(stderr, "STEER: Initialize_steering_connection_soap: failed to "
	    "malloc memory for soap struct\n");
    return REG_FAILURE;
  }

  /* Set location of steering scratch directory */
  if(Set_steering_directory() != REG_SUCCESS){

    fprintf(stderr, "STEER: Initialize_steering_connection_soap: failed to set "
	    "steering scratch directory - checkpoint info. will be "
	    "written to ./\n");
  }

  /* Get the address of the SGS for this application from an environment
     variable */
  pchar = getenv("REG_SGS_ADDRESS");

  if(pchar){

    snprintf(Steerer_connection.SGS_info.address, REG_MAX_STRING_LENGTH, 
	     "%s", pchar);
#if REG_DEBUG
    fprintf(stderr, "STEER: Initialize_steering_connection_soap: SGS address = %s\n",
	    Steerer_connection.SGS_info.address);
#endif
  }
  else{

    fprintf(stderr, "STEER: Initialize_steering_connection_soap: REG_SGS_ADDRESS "
	    "environment variable not set\n");
    return REG_FAILURE;
  }

  /* Initialise the soap run-time environment */
  /* soap_init(&soap); */
  /* Use this form to turn-on keep-alive for both incoming and outgoing
     http connections */
  soap_init2(Steerer_connection.SGS_info.soap, SOAP_IO_KEEPALIVE, 
	     SOAP_IO_KEEPALIVE);

  /* Since we are using KEEPALIVE, we can also ask gSOAP to bind the 
     socket to a specific port on the local machine - only do this if 
     GLOBUS_TCP_PORT_RANGE is set. */
  if( (pchar = getenv("GLOBUS_TCP_PORT_RANGE")) ){

    if(sscanf(pchar, "%d,%d", 
	      &(Steerer_connection.SGS_info.soap->client_port_min), 
	      &(Steerer_connection.SGS_info.soap->client_port_max)) != 2){
      Steerer_connection.SGS_info.soap->client_port_min = 0;
      Steerer_connection.SGS_info.soap->client_port_max = 0;
    }
  }

  appStart_response._AppStartReturn = NULL;
  if (soap_call_sgs__AppStart(Steerer_connection.SGS_info.soap, 
			      Steerer_connection.SGS_info.address, "", 
			      NULL, &appStart_response)){

    fprintf(stderr, "STEER: Initialize_steering_connection_soap: failed to "
	    "attach to SGS: %s\n", Steerer_connection.SGS_info.address);
    soap_print_fault(Steerer_connection.SGS_info.soap, stderr);
    return REG_FAILURE;
  }

  if(!appStart_response._AppStartReturn || 
           strstr(appStart_response._AppStartReturn, REG_SGS_ERROR)){

    fprintf(stderr, "STEER: Initialize_steering_connection_soap: AppStart "
	    "returned error\n");
    return REG_FAILURE;
  }
  
  /* Create msg to send to SGS */
  Make_supp_cmds_msg(NumSupportedCmds, SupportedCmds, 
		     Steerer_connection.supp_cmds, REG_MAX_MSG_SIZE);

  setSDE_response._setServiceDataReturn = NULL;

  /* Strip off any xml version declaration */
  pchar = strstr(Steerer_connection.supp_cmds,"<ReG_steer_message");

  snprintf(query_buf, REG_MAX_MSG_SIZE, 
	   "<ogsi:setByServiceDataNames><%s>%s</%s></ogsi:setByServiceDataNames>", 
	  SUPPORTED_CMDS_SDE, pchar, SUPPORTED_CMDS_SDE);

  if (soap_call_sgs__setServiceData(Steerer_connection.SGS_info.soap, 
				    Steerer_connection.SGS_info.address, "", 
				    query_buf, 
				    &setSDE_response)){

    fprintf(stderr, "STEER: Initialize_steering_connection_soap: failure\n");
    soap_print_fault(Steerer_connection.SGS_info.soap, stderr);
    return REG_FAILURE;
  }

#if REG_DEBUG
  if(setSDE_response._setServiceDataReturn){
    fprintf(stderr, "STEER: Initialize_steering_connection_soap: setServiceData "
	    "returned: %s\n", setSDE_response._setServiceDataReturn);
  }
  else{
    fprintf(stderr, "STEER: Initialize_steering_connection_soap: setServiceData "
	    "returned null\n");
  }
#endif

  /* Publish our location: machine and working directory - these
     are set in Steering_initialize prior to calling us */
  snprintf(query_buf, REG_MAX_MSG_SIZE, 
	   "<ogsi:setByServiceDataNames><%s>%s</%s>"
	   "<%s>%s</%s><%s>%s</%s></ogsi:setByServiceDataNames>", 
	   WORKING_DIR_SDE, ReG_CurrentDir, WORKING_DIR_SDE,
	   MACHINE_ADDRESS_SDE, ReG_Hostname, MACHINE_ADDRESS_SDE,
	   APP_NAME_SDE, ReG_AppName, APP_NAME_SDE);

  if (soap_call_sgs__setServiceData(Steerer_connection.SGS_info.soap, 
				    Steerer_connection.SGS_info.address, "", 
				    query_buf, 
				    &setSDE_response)){

    fprintf(stderr, "STEER: Initialize_steering_connection_soap: failure\n");
    soap_print_fault(Steerer_connection.SGS_info.soap, stderr);
    return REG_FAILURE;
  }

#if REG_DEBUG
  if(setSDE_response._setServiceDataReturn){
    fprintf(stderr, "STEER: Initialize_steering_connection_soap: setServiceData "
	    "returned: %s\n", setSDE_response._setServiceDataReturn);
  }
  else{
    fprintf(stderr, "STEER: Initialize_steering_connection_soap: setServiceData "
	    "returned null\n");
  }
#endif

  return REG_SUCCESS;
}

/*-------------------------------------------------------------------------*/

int Detach_from_steerer_soap()
{
  struct sgs__AppDetachResponse appDetach_response;

  appDetach_response._AppDetachReturn = NULL;
  if(soap_call_sgs__AppDetach(Steerer_connection.SGS_info.soap, 
			      Steerer_connection.SGS_info.address, 
			      "", NULL, &appDetach_response )){

    fprintf(stderr, "STEER: Detach_from_steerer_soap: AppDetach failed:\n");
    soap_print_fault(Steerer_connection.SGS_info.soap, stderr);

    return REG_FAILURE;
  }

  if(appDetach_response._AppDetachReturn && 
     strstr(appDetach_response._AppDetachReturn, "SGS_SUCCESS")){

    return REG_SUCCESS;
  }

  return REG_FAILURE;
}

/*-------------------------------------------------------------------------*/

int Steerer_connected_soap()
{
  struct sgs__findServiceDataResponse  findServiceData_response;
  char                                 query_buf[REG_MAX_STRING_LENGTH];

  findServiceData_response._findServiceDataReturn = NULL;
  snprintf(query_buf, REG_MAX_STRING_LENGTH, 
	   "<ogsi:queryByServiceDataNames names=\"%s\"/>", 
	   STEER_STATUS_SDE );
  if(soap_call_sgs__findServiceData(Steerer_connection.SGS_info.soap, 
				    Steerer_connection.SGS_info.address, 
				    "", query_buf, 
				    &findServiceData_response )){

    fprintf(stderr, "STEER: Steerer_connected_soap: findServiceData failed:\n");
    soap_print_fault(Steerer_connection.SGS_info.soap, stderr);

    return REG_FAILURE;
  }

#if REG_DEBUG
  if(findServiceData_response._findServiceDataReturn){
    fprintf(stderr, "STEER: Steerer_connected_soap: findServiceData returned: %s\n", 
	    findServiceData_response._findServiceDataReturn);
  }
  else{
    fprintf(stderr, "STEER: Steerer_connected_soap: findServiceData returned null\n");
  }
#endif

  if(findServiceData_response._findServiceDataReturn && 
     strstr(findServiceData_response._findServiceDataReturn, "ATTACHED")){
    return REG_SUCCESS;
  }
  else if(findServiceData_response._findServiceDataReturn && 
	  strstr(findServiceData_response._findServiceDataReturn, "DETACHING")){
    /* Steerer has attached and detached without us noticing and thus SGS
       is in 'detaching' state.  We have to 'detach' properly now to reset.*/
    Detach_from_steerer_soap();

    /* But we still return 'failure' because no steerer is attached */
  }

  return REG_FAILURE;
}

/*-------------------------------------------------------------------------*/

int Send_status_msg_soap(char* msg)
{
  struct sgs__PutStatusResponse       putStatus_response;
  struct sgs__setServiceDataResponse  setSDE_response;
  char                               *sde_name;
  char                                query_buf[REG_MAX_MSG_SIZE];
  int                                 nbytes;
  int                                 new_size;
  int                                 status;
  char                               *pbuf;

  /* Status & log messages are both sent as 'status' messages */
  if(strstr(msg, "<App_status>") || strstr(msg, "<Steer_log>")){

    /* We loop until we have a clear-cut success or failure - i.e.
       if we have a deadlock we fall back to here and try again */
    while(1){
      putStatus_response._PutStatusReturn = NULL;
      if(soap_call_sgs__PutStatus(Steerer_connection.SGS_info.soap, 
				  Steerer_connection.SGS_info.address, 
				  "", msg, &putStatus_response )){
	soap_print_fault(Steerer_connection.SGS_info.soap, stderr);
	
	return REG_FAILURE;
      }

#if REG_DEBUG
      if(putStatus_response._PutStatusReturn){
	fprintf(stderr, "STEER: Send_status_msg_soap: PutStatus returned: %s\n", 
		putStatus_response._PutStatusReturn);
      }
      else{
	fprintf(stderr, "STEER: Send_status_msg_soap: PutStatus returned null\n");
      }
#endif

      if(!putStatus_response._PutStatusReturn ||
	 strstr(putStatus_response._PutStatusReturn, REG_SGS_ERROR)){
	return REG_FAILURE;
      }

      if(!strstr(putStatus_response._PutStatusReturn, REG_SGS_TIMEOUT))break;
    }
  }
  else{

    if(strstr(msg, "<Supported_commands>")){
      
      sde_name = SUPPORTED_CMDS_SDE;
    }
    else if(strstr(msg, "<Param_defs>")){

      sde_name = PARAM_DEFS_SDE;
    }
    else if(strstr(msg, "<IOType_defs>")){

      sde_name = IOTYPE_DEFS_SDE;
    }
    else if(strstr(msg, "<ChkType_defs>")){

      sde_name = CHKTYPE_DEFS_SDE;
    }
    else{
      fprintf(stderr, "STEER: Send_status_msg_soap: not a status or log msg and"
	      "no matching SDE name found either\n");
      return REG_FAILURE;
    }

    setSDE_response._setServiceDataReturn = NULL;
    nbytes = snprintf(query_buf, REG_MAX_MSG_SIZE, 
		      "<ogsi:setByServiceDataNames>" 
		      "<%s>%s</%s></ogsi:setByServiceDataNames>", 
		      sde_name, msg, sde_name);

    /* Check for truncation - if it occurs then malloc a bigger buffer
       and try again */
    if((nbytes >= (REG_MAX_MSG_SIZE-1)) || (nbytes < 1)){

      new_size = strlen(msg) + 512;
      if(!(pbuf = (char *)malloc(new_size)) ){

	fprintf(stderr, "STEER: Send_status_msg_soap: malloc failed\n");
	return REG_FAILURE;
      }

      nbytes = snprintf(pbuf, new_size, 
			"<ogsi:setByServiceDataNames>" 
			"<%s>%s</%s></ogsi:setByServiceDataNames>", 
			sde_name, msg, sde_name);

      if((nbytes >= (new_size-1)) || (nbytes < 1)){
      
	free(pbuf);
	pbuf = NULL;
	fprintf(stderr, "STEER: Send_status_msg_soap: ERROR - msg truncated\n");
	return REG_FAILURE;
      }
      status = soap_call_sgs__setServiceData(Steerer_connection.SGS_info.soap, 
					     Steerer_connection.SGS_info.address,
					     "", pbuf,  
					     &setSDE_response);
      free(pbuf);
      pbuf = NULL;
    }
    else {
      status = soap_call_sgs__setServiceData(Steerer_connection.SGS_info.soap, 
					     Steerer_connection.SGS_info.address,
					     "", query_buf,  
					     &setSDE_response);
    }

    if(status){
      fprintf(stderr, "STEER: Send_status_msg_soap: setServiceData failed:\n");
      soap_print_fault(Steerer_connection.SGS_info.soap,stderr);
      return REG_FAILURE;
    }

    if(!setSDE_response._setServiceDataReturn || 
       strstr(setSDE_response._setServiceDataReturn, REG_SGS_ERROR)){
      return REG_FAILURE;
    }
  }

  return REG_SUCCESS;
}

/*-------------------------------------------------------------------------*/

struct msg_struct *Get_control_msg_soap()
{
  struct sgs__GetControlResponse getControl_response;
  struct msg_struct *msg = NULL;

#if REG_DEBUG
    fprintf(stderr, "STEER: Get_control_msg_soap: address = %s\n", 
	    Steerer_connection.SGS_info.address);
#endif
    getControl_response._GetControlReturn = NULL;
  if(soap_call_sgs__GetControl(Steerer_connection.SGS_info.soap, 
			       Steerer_connection.SGS_info.address, 
			       "",  NULL, &getControl_response)){
    soap_print_fault(Steerer_connection.SGS_info.soap, stderr);
    return NULL;
  }

#if REG_DEBUG
  if(getControl_response._GetControlReturn){
    fprintf(stderr, "STEER: Get_control_msg_soap: GetControl returned: %s\n", 
	    getControl_response._GetControlReturn);
  }
  else{
    fprintf(stderr, "STEER: Get_control_msg_soap: GetControl returned null\n");
  }
#endif

  if(getControl_response._GetControlReturn && 
     !strstr(getControl_response._GetControlReturn, REG_SGS_ERROR)){
    msg = New_msg_struct();

    /* Pass NULL down here because this is app side so have no ptr
       to a Sim_entry struct */
    if(Parse_xml_buf(getControl_response._GetControlReturn, 
		     strlen(getControl_response._GetControlReturn), 
		     msg, NULL) != REG_SUCCESS){

      Delete_msg_struct(&msg);
    }
  }

  return msg;
}

/*-------------------------------------------------------------------------*/

int Finalize_steering_connection_soap()
{
  struct sgs__AppStopResponse appStop_response;
  int    return_status = REG_SUCCESS;

  /* Tell the SGS to die - could use Destroy here but that doesn't 
     provide any opportunites for clean-up */
  appStop_response._AppStopReturn = NULL;
  if(soap_call_sgs__AppStop(Steerer_connection.SGS_info.soap, 
			    Steerer_connection.SGS_info.address, 
			    "",  NULL, &appStop_response)){
    soap_print_fault(Steerer_connection.SGS_info.soap, stderr);
    return REG_FAILURE;
  }

  if(!(appStop_response._AppStopReturn && 
     !strstr(appStop_response._AppStopReturn, REG_SGS_ERROR))){
    return_status = REG_FAILURE;
  }

  soap_end(Steerer_connection.SGS_info.soap);
  free(Steerer_connection.SGS_info.soap);
  Steerer_connection.SGS_info.soap = NULL;

  return return_status;
}

/*-------------------------------------------------------------------------*/

int Get_data_source_address_soap(int   index, 
				 char *hostname,
				 unsigned short int  *port)
{
  char  *pchar;
  struct sgs__GetNthDataSourceResponse getNthDataSource_response;

  /* Port returned as zero on failure */
  *port = 0;

  getNthDataSource_response._GetNthDataSourceReturn = NULL;
  if(soap_call_sgs__GetNthDataSource(Steerer_connection.SGS_info.soap, 
				     Steerer_connection.SGS_info.address, 
				     "", (xsd__int)index,
				     &getNthDataSource_response)){
    fprintf(stderr, "STEER: Get_data_source_address_soap: soap call failed:\n");
    soap_print_fault(Steerer_connection.SGS_info.soap, stderr);
    return REG_FAILURE;
  }

#if REG_DEBUG
  if(getNthDataSource_response._GetNthDataSourceReturn){
    fprintf(stderr, "STEER: Get_data_source_address_soap: GetNthDataSource "
	    "(for n=%d)\nreturned: >>%s<<\n", 
	    index, getNthDataSource_response._GetNthDataSourceReturn);
  }
  else{
    fprintf(stderr, "STEER: Get_data_source_address_soap: GetNthDataSource "
	    "(for n=%d)\nreturned null\n", index);
  }
#endif

  if(getNthDataSource_response._GetNthDataSourceReturn && 
     !strstr(getNthDataSource_response._GetNthDataSourceReturn, REG_SGS_ERROR)){

    if( (pchar = strtok(getNthDataSource_response._GetNthDataSourceReturn, ":")) ){

      strcpy(hostname, pchar);
      if( (pchar = strtok(NULL, ":")) ){

	*port = (unsigned short int)atoi(pchar);
      }
    }
  }

  /* So long as soap call did return something we return success - even
     if we didn't actually get a valid address.  This consistent with
     polling a GS for valid address - success is indicated by non-zero
     port no. */
  return REG_SUCCESS;
}

/*----------------------------------------------------------------------*/

int Record_checkpoint_set_soap(char *chk_data,
			       char *node_data)
{
  struct sgs__AppRecordChkpointResponse AppRecordChkpoint_response;

  AppRecordChkpoint_response._AppRecordChkpointReturn = NULL;
  if(soap_call_sgs__AppRecordChkpoint(Steerer_connection.SGS_info.soap, 
				      Steerer_connection.SGS_info.address, 
				      "", chk_data, node_data,  
				      &AppRecordChkpoint_response)){
    fprintf(stderr, "STEER: Record_checkpoint_set_soap: soap call failed:\n");
    soap_print_fault(Steerer_connection.SGS_info.soap, stderr);
    return REG_FAILURE;
  }

#if REG_DEBUG
  if(AppRecordChkpoint_response._AppRecordChkpointReturn){
    fprintf(stderr, "STEER: Record_checkpoint_set_soap: "
	    "AppRecordChkpoint returned: >>%s<<\n", 
	    AppRecordChkpoint_response._AppRecordChkpointReturn);
  }
  else{
    fprintf(stderr, "STEER: Record_checkpoint_set_soap: AppRecordChkpoint "
	    "returned null\n");
  }
#endif

  if(AppRecordChkpoint_response._AppRecordChkpointReturn && 
     strstr(AppRecordChkpoint_response._AppRecordChkpointReturn, REG_SGS_SUCCESS)){

    return REG_SUCCESS;
  }

  return REG_FAILURE;
}

/*----------------------------------------------------------------------*/

int Save_log_soap(char *log_data)
{
  struct sgs__AppPutLogResponse AppPutLog_response;
  char *pmsg_buf;

  AppPutLog_response._AppPutLogReturn = NULL;

  if(strlen(log_data) > REG_SCRATCH_BUFFER_SIZE){

    fprintf(stderr, "STEER: Save_log_soap: log data exceeds scratch buffer "
	    "size of %d btes.  More code needed here...\n", 
	    REG_SCRATCH_BUFFER_SIZE);
    return REG_FAILURE;
  }

  pmsg_buf = Global_scratch_buffer;

  /* We just send the data as it comes and therefore have to wrap it
     with CDATA tag to stop the parser getting upset */
  pmsg_buf += sprintf(pmsg_buf, "<Steer_log><Raw_param_log><![CDATA[");
  strcpy(pmsg_buf, log_data);
  pmsg_buf += strlen(log_data);
  pmsg_buf += sprintf(pmsg_buf, "]]></Raw_param_log></Steer_log>");

  if(soap_call_sgs__AppPutLog(Steerer_connection.SGS_info.soap, 
			      Steerer_connection.SGS_info.address, 
			      "", Global_scratch_buffer,  
			      &AppPutLog_response)){
    fprintf(stderr, "STEER: Save_log_soap: soap call failed:\n");
    soap_print_fault(Steerer_connection.SGS_info.soap, stderr);
    return REG_FAILURE;
  }

  return REG_SUCCESS;
}
