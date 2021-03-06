/*
  The RealityGrid Steering Library

  Copyright (c) 2002-2010, University of Manchester, United Kingdom.
  All rights reserved.

  This software is produced by Research Computing Services, University
  of Manchester as part of the RealityGrid project and associated
  follow on projects, funded by the EPSRC under grants GR/R67699/01,
  GR/R67699/02, GR/T27488/01, EP/C536452/1, EP/D500028/1,
  EP/F00561X/1.

  LICENCE TERMS

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:

    * Redistributions of source code must retain the above copyright
       notice, this list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following
      disclaimer in the documentation and/or other materials provided
      with the distribution.

    * Neither the name of The University of Manchester nor the names
      of its contributors may be used to endorse or promote products
      derived from this software without specific prior written
      permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
  COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE.

  Author: Andrew Porter
          Robert Haines
 */

/** @internal
    @file ReG_Steer_Logging.c
    @brief Implementation of logging functionality

    This file contains routines and data structures for the construction
    of an interface to a steering component (from an application
    component).
    @author Andrew Porter */

#include "ReG_Steer_Config.h"
#include "ReG_Steer_types.h"
#include "ReG_Steer_Common.h"
#include "ReG_Steer_Logging.h"
#include "ReG_Steer_Appside_internal.h"
#include "ReG_Steer_Steering_Transport_API.h"

/** Basic library config - declared in ReG_Steer_Common */
extern Steer_lib_config_type Steer_lib_config;

/** Log of checkpoints taken - declared in ReG_Steer_Appside.c  */
extern Chk_log_type Chk_log;

/** Log of values of parameters for which logging has
    been requested - declared in ReG_Steer_Appside.c */
extern Chk_log_type Param_log;

extern Param_table_type Params_table;

/** Whether or not a client is currently attached - declared in
    ReG_Steer_Appside.c */
extern int ReG_SteeringActive;

/*----------------------------------------------------------------*/

int Initialize_log(Chk_log_type *log, log_type_type log_type)
{
  int i, j;

  log->log_type         = log_type;
  log->num_entries 	= 0;
  log->max_entries 	= REG_INITIAL_CHK_LOG_SIZE;
  log->num_unsent  	= 0;
  log->emit_in_progress = REG_FALSE;
  log->file_content     = NULL;
  log->send_all    	= REG_TRUE;
  if(log->log_type == PARAM){
    for(i=0; i<REG_MAX_NUM_STR_PARAMS; i++){
      log->param_send_all[i] = REG_TRUE;
    }

    Initialize_log_impl(log);

    /* We delete any existing parameter log file rather than
       append to it */
    Delete_log_file(log);
  }

  Set_log_primary_key(log);

  if( Open_log_file(log) != REG_SUCCESS ){
    fprintf(stderr, "STEER: Initialize_log: failed to create log file, "
	    "log will not be saved to file\n");
  }

  log->entry = (Chk_log_entry_type *)malloc(Chk_log.max_entries
					  *sizeof(Chk_log_entry_type));

  log->pSteer_cmds = (char *)malloc(REG_MAX_MSG_SIZE*sizeof(char));
  log->steer_cmds_bytes = REG_MAX_MSG_SIZE;

  if(!(log->entry) || !(log->pSteer_cmds) ){

    fprintf(stderr, "STEER: Initialize_log: failed to allocate memory "
	    "for checkpoint logging\n");
    return REG_FAILURE;
  }

  log->pSteer_cmds[0] = '\0';
  /* pSteer_cmds_slot points to next free space in the buffer */
  log->pSteer_cmds_slot = log->pSteer_cmds;

  for(i=0; i<log->max_entries; i++){

    log->entry[i].sent_to_steerer = REG_TRUE;
    log->entry[i].num_param       = 0;

    for(j=0; j<REG_MAX_NUM_STR_PARAMS; j++){
      log->entry[i].param[j].handle = REG_PARAM_HANDLE_NOTSET;
    }
  }
  return REG_SUCCESS;
}

/*----------------------------------------------------------------*/

int Finalize_log(Chk_log_type *log)
{
  if(log->entry != NULL){

    free(log->entry);
    log->entry = NULL;
  }
  log->num_entries = 0;
  log->max_entries = REG_INITIAL_CHK_LOG_SIZE;
  /* Buffer for logged commands */
  free(log->pSteer_cmds);
  log->pSteer_cmds = NULL;
  log->steer_cmds_bytes = 0;

  return REG_SUCCESS;
}

/*----------------------------------------------------------------*/

int Delete_log_file(Chk_log_type *log)
{
  char *pchar;
  char *backup_suffix = ".backup";
  int   len, status;

  if(log->file_ptr){
    fclose(log->file_ptr);
  }
  len = strlen(log->filename) + strlen(backup_suffix) + 1;

  pchar = (char*)malloc(len);
  if(!pchar)return REG_FAILURE;

  strcpy(pchar, log->filename);
  strcat(pchar, backup_suffix);

  /* We don't actually delete the file - just move it to one side */
  status = rename(log->filename, pchar);

  free(pchar);
  pchar = NULL;

  if(status == 0)return REG_SUCCESS;
  return REG_FAILURE;
}

/*----------------------------------------------------------------*/

int Open_log_file(Chk_log_type *log)
{
  if(log->file_ptr){
    fclose(log->file_ptr);
  }

  log->file_ptr = fopen(log->filename, "a");

  return REG_SUCCESS;
}

/*----------------------------------------------------------------*/

int Close_log_file(Chk_log_type *log)
{
  if(log->file_ptr){
    fclose(log->file_ptr);
    log->file_ptr = NULL;
  }

  return REG_SUCCESS;
}

/*----------------------------------------------------------------*/

int Save_log(Chk_log_type *log)
{
  char *buf = NULL;
  int   len;
  int   status;

  if(!log->file_ptr){

    return REG_FAILURE;
  }

  if(log->num_entries > 0){
    if(log->log_type == PARAM){

      status = Log_to_columns(log, &buf, &len, REG_FALSE);
    }
    else{

       /* 5th argument says we want all entries - not just those that
	  haven't already been sent to the steerer.  2nd (handle)
          argument isn't used for checkpoint logs */
      status = Log_to_xml(log, 0, &buf, &len, REG_FALSE);
    }
    if(status == REG_SUCCESS){
      Save_log_impl(log->file_ptr, buf);
    }
  }

  log->num_entries = 0;

  if(buf){
    free(buf);
    buf = NULL;
  }

  return REG_SUCCESS;
}

/*----------------------------------------------------------------*/

int Log_to_xml(Chk_log_type *log, int handle, char **pchar,
	       int *count, const int not_sent_only)
{
  int   size = BUFSIZ;

  if( !(*pchar = (char *)malloc(size*sizeof(char))) ){

    fprintf(stderr, "STEER: Log_to_xml: malloc failed\n");
    return REG_FAILURE;
  }

  *count = 0;

  if(log->log_type == PARAM){
    return Param_log_to_xml(log, handle, pchar, count, not_sent_only);
  }
  else if(log->log_type == CHKPT){
    return Chk_log_to_xml(log, pchar, count, not_sent_only);
  }
  else{
    free(*pchar);
    *pchar = NULL;
    fprintf(stderr, "STEER: ERROR: Log_to_xml: unknown log type\n");
    return REG_FAILURE;
  }
}

/*----------------------------------------------------------------*/

int Chk_log_to_xml(Chk_log_type *log, char **pchar, int *count,
		   const int not_sent_only)
{
  int   i, j, len;
  char  entry[BUFSIZ];
  char *pentry;
  char *pbuf;
  void *ptr;
  int   size = BUFSIZ;
  int   nbytes = 0;
  int   bytes_left;

  pbuf = *pchar;

  for(i=0; i<log->num_entries; i++){

    /* Check to see whether steerer already has this entry */
    if (not_sent_only && (log->entry[i].sent_to_steerer == REG_TRUE)) continue;

    bytes_left = BUFSIZ;
    pentry = entry;
    nbytes = snprintf(pentry, bytes_left, "<Log_entry>\n"
		      "<Key>%d</Key>\n"
		      "<Chk_log_entry>"
		      "<Chk_handle>%d</Chk_handle>\n"
		      "<Chk_tag>%s</Chk_tag>\n",
		      log->entry[i].key,
		      log->entry[i].chk_handle,
		      log->entry[i].chk_tag);

#ifdef REG_DEBUG
    /* Check for truncation of message */
    if((nbytes >= (bytes_left-1)) || (nbytes < 1)){
      fprintf(stderr, "STEER: Chk_log_to_xml: message size exceeds BUFSIZ (%d)\n",
	      BUFSIZ);
      free(*pchar);
      *pchar = NULL;
      return REG_FAILURE;
    }
#endif
    bytes_left -= nbytes;
    pentry += nbytes;

    /* Associated parameters are stored contiguously so need only
       loop over the no. of params that this entry has */
    for(j=0; j<log->entry[i].num_param; j++){

      nbytes = snprintf(pentry, bytes_left, "<Param>\n"
			"<Handle>%d</Handle>\n"
			"<Value>%s</Value>\n"
			"</Param>\n",
		        log->entry[i].param[j].handle,
		        log->entry[i].param[j].value);

#ifdef REG_DEBUG
      /* Check for truncation of message */
      if((nbytes >= (bytes_left-1)) || (nbytes < 1)){
	fprintf(stderr, "STEER: Chk_log_to_xml: message size exceeds BUFSIZ (%d)\n",
		BUFSIZ);
	free(*pchar);
	*pchar = NULL;
	return REG_FAILURE;
      }
#endif
      bytes_left -= nbytes;
      pentry += nbytes;
    }

    nbytes = snprintf(pentry, bytes_left, "</Chk_log_entry>\n"
		                          "</Log_entry>\n");
#ifdef REG_DEBUG
    /* Check for truncation of message */
    if((nbytes >= (bytes_left-1)) || (nbytes < 1)){
      fprintf(stderr, "STEER: Chk_log_to_xml: message size exceeds BUFSIZ (%d)\n",
	      BUFSIZ);
      free(*pchar);
      *pchar = NULL;
      return REG_FAILURE;
    }
#endif
    bytes_left -= nbytes;
    pentry += nbytes;

    len = strlen(entry);

    if( (size - *count) < len){

      size += ((len/BUFSIZ) + 1)*BUFSIZ;
      if( !(ptr = realloc(*pchar, size*sizeof(char))) ){

	fprintf(stderr, "STEER: Chk_log_to_xml: realloc failed\n");
	free(*pchar);
	*pchar = NULL;
	return REG_FAILURE;
      }

      *pchar = (char *)ptr;
      pbuf = &((*pchar)[*count]);
    }

    strncpy(pbuf, entry, len);
    pbuf += len;
    *count += len;

    /* Flag this entry as having been sent to steerer */
    log->entry[i].sent_to_steerer = REG_TRUE;
  }

  *pbuf = 0;

  return REG_SUCCESS;
}

/*----------------------------------------------------------------*/

int Param_log_to_xml(Chk_log_type *log, int handle, char **pchar,
		     int *count, const int not_sent_only)
{
  int   i, j, len;
  char  entry[BUFSIZ];
  char *pentry;
  char *pbuf;
  void *ptr;
  int   size = BUFSIZ;
  int   nbytes = 0;
  int   bytes_left;

  pbuf = *pchar;

  for(i=0; i<log->num_entries; i++){

    /* Check to see whether steerer already has this entry */
    if (not_sent_only && (log->entry[i].sent_to_steerer == REG_TRUE)) continue;

    bytes_left = BUFSIZ;
    pentry = entry;
    nbytes = snprintf(pentry, bytes_left, "<Log_entry>\n"
		      "<Key>%d</Key>\n",
		      log->entry[i].key);

#ifdef REG_DEBUG
    /* Check for truncation of message */
    if((nbytes >= (bytes_left-1)) || (nbytes < 1)){
      fprintf(stderr, "STEER: Param_log_to_xml: message size exceeds BUFSIZ (%d)\n",
	      BUFSIZ);
      free(*pchar);
      *pchar = NULL;
      return REG_FAILURE;
    }
#endif
    bytes_left -= nbytes;
    pentry += nbytes;

    /* Associated parameters are stored contiguously so need only
       loop over the no. of params that this entry has */
    for(j=0; j<log->entry[i].num_param; j++){

      if(log->entry[i].param[j].handle == handle){

	nbytes = snprintf(pentry, bytes_left, "<Param>"
			  "<Handle>%d</Handle><Value>%s</Value>"
			  "</Param>\n",
			  log->entry[i].param[j].handle,
			  log->entry[i].param[j].value);

#ifdef REG_DEBUG
	/* Check for truncation of message */
	if((nbytes >= (bytes_left-1)) || (nbytes < 1)){
	  fprintf(stderr, "STEER: Param_log_to_xml: message size "
		  "exceeds BUFSIZ (%d)\n", BUFSIZ);
	  free(*pchar);
	  *pchar = NULL;
	  return REG_FAILURE;
	}
#endif
	bytes_left -= nbytes;
	pentry += nbytes;

	/* We only want the parameter with the specified handle */
	break;
      }
    }

    nbytes = snprintf(pentry, bytes_left, "</Log_entry>\n");
#ifdef REG_DEBUG
    /* Check for truncation of message */
    if((nbytes >= (bytes_left-1)) || (nbytes < 1)){
      fprintf(stderr, "STEER: Param_log_to_xml: message size "
	      "exceeds BUFSIZ (%d)\n", BUFSIZ);
      free(*pchar);
      *pchar = NULL;
      return REG_FAILURE;
    }
#endif
    bytes_left -= nbytes;
    pentry += nbytes;

    len = strlen(entry);

    if( (size - *count) < len){

      size += ((len/BUFSIZ) + 1)*BUFSIZ;
      if( !(ptr = realloc(*pchar, size*sizeof(char))) ){

	fprintf(stderr, "STEER: Param_log_to_xml: realloc failed\n");
	free(*pchar);
	*pchar = NULL;
	return REG_FAILURE;
      }

      *pchar = (char *)ptr;
      pbuf = &((*pchar)[*count]);
    }

    strncpy(pbuf, entry, len);
    pbuf += len;
    *count += len;

    /* Flag this entry as having been sent to steerer */
    log->entry[i].sent_to_steerer = REG_TRUE;
  }

  *pbuf = 0;

  return REG_SUCCESS;
}

/*----------------------------------------------------------------*/

int Log_to_columns(Chk_log_type *log, char **pchar, int *count,
		   const int not_sent_only)
{
  int   i, j, len;
  int   size = BUFSIZ;
  int   bytes_left;
  int   nbytes;
  char *pentry;
  char *pbuf;
  char  entry[BUFSIZ];
  void *ptr;

  if( !(*pchar = (char *)malloc(size*sizeof(char))) ){

    fprintf(stderr, "STEER: Log_to_columns: malloc failed\n");
    return REG_FAILURE;
  }

  *count = 0;
  pbuf = *pchar;

  for(i=0; i<log->num_entries; i++){

    /* Check to see whether steerer already has this entry */
    if (not_sent_only && (log->entry[i].sent_to_steerer == REG_TRUE)) continue;
    bytes_left = BUFSIZ;
    pentry = entry;
    nbytes = snprintf(pentry, bytes_left, "%d", log->entry[i].key);

#ifdef REG_DEBUG
    /* Check for truncation of message */
    if((nbytes >= (bytes_left-1)) || (nbytes < 1)){
      fprintf(stderr, "STEER: Log_to_columns: message size exceeds BUFSIZ (%d)\n",
	      BUFSIZ);
      free(*pchar);
      *pchar = NULL;
      return REG_FAILURE;
    }
#endif
    bytes_left -= nbytes;
    pentry += nbytes;

    /* Associated parameters are stored contiguously so need only
       loop over the no. of params that this entry has */
    for(j=0; j<log->entry[i].num_param; j++){

      nbytes = snprintf(pentry, bytes_left, " %d %s",
		        log->entry[i].param[j].handle,
		        log->entry[i].param[j].value);

#ifdef REG_DEBUG
      /* Check for truncation of message */
      if((nbytes >= (bytes_left-1)) || (nbytes < 1)){
	fprintf(stderr, "STEER: Log_to_columns: message size exceeds BUFSIZ (%d)\n",
		BUFSIZ);
	free(*pchar);
	*pchar = NULL;
	return REG_FAILURE;
      }
#endif
      bytes_left -= nbytes;
      pentry += nbytes;
    }

    nbytes = snprintf(pentry, bytes_left, "\n");
#ifdef REG_DEBUG
    /* Check for truncation of message */
    if((nbytes >= (bytes_left-1)) || (nbytes < 1)){
      fprintf(stderr, "STEER: Log_to_columns: message size exceeds BUFSIZ (%d)\n",
	      BUFSIZ);
      free(*pchar);
      *pchar = NULL;
      return REG_FAILURE;
    }
#endif
    bytes_left -= nbytes;
    pentry += nbytes;

    len = strlen(entry);

    if( (size - *count) < len){

      size += ((len/BUFSIZ) + 1)*BUFSIZ;
      if( !(ptr = realloc(*pchar, size*sizeof(char))) ){

	fprintf(stderr, "STEER: Log_to_columns: realloc failed\n");
	free(*pchar);
	*pchar = NULL;
	return REG_FAILURE;
      }

      *pchar = (char *)ptr;
      pbuf = &((*pchar)[*count]);
    }

    strncpy(pbuf, entry, len);
    pbuf += len;
    *count += len;

    /* Flag this entry as having been sent to steerer */
    log->entry[i].sent_to_steerer = REG_TRUE;
  }
  *pbuf = 0;

  return REG_SUCCESS;
}

/*----------------------------------------------------------------*/

int Set_log_primary_key(Chk_log_type *log)
{
  int   size;
  int   return_status = REG_SUCCESS;
  char *pbuf = NULL;
  char *ptr = NULL;
  char *old_ptr = NULL;

  Close_log_file(log);

  /* Read the log file and get back contents in buffer pointed
     to by pbuf.  We must free() this once we're done. */

  if(Read_file(log->filename, &pbuf, &size, REG_TRUE) != REG_SUCCESS){

    log->primary_key_value = 0;
    if(pbuf)free(pbuf);
    return REG_SUCCESS;
  }

  if(size == 0){

    /* Log file existed but was empty */
    log->primary_key_value = 0;
    if(pbuf)free(pbuf);
    return REG_SUCCESS;
  }

  ptr = pbuf;

  /* Check whether we've got xml or columns... */
  if(!strstr(pbuf, "<Key>")){
    /* We have columns of data, first column holds key values... */

    while( (ptr = strstr((ptr+1), "\n")) ){

      if(*(ptr+1) == '\0')break;
      old_ptr = ptr;
    }

#ifdef REG_DEBUG
    fprintf(stderr, "STEER: Set_log_primary_key: last chunk = >>%s<<\n", old_ptr);
#endif

    if(2 != sscanf(old_ptr, "%d %s", &(log->primary_key_value),
		   Steer_lib_config.scratch_buffer)){
      log->primary_key_value = 0;
      return_status = REG_FAILURE;
    }
    else{
      log->primary_key_value++;
    }
  }
  else{
    /* We have xml...*/

    while( (ptr = strstr((ptr+1), "<Key>")) ){
      old_ptr = ptr;
    }

#ifdef REG_DEBUG
    fprintf(stderr, "STEER: Set_log_primary_key: last chunk = >>%s<<\n", old_ptr);
#endif

    if( 1 != sscanf(old_ptr, "<Key>%d</Key>", &(log->primary_key_value))){

      log->primary_key_value = 0;
      return_status = REG_FAILURE;
    }
    else{
      log->primary_key_value++;
    }
  }

  free(pbuf);
  Open_log_file(log);

  return return_status;
}

/*----------------------------------------------------------------*/

int Log_param_values()
{
  int index;
  int count;

  if(Params_table.log_all == REG_FALSE)return REG_SUCCESS;

  /* Check that we have enough storage space - if not then store
     current entries on disk (rather than continually grab more
     memory) */

  if(Param_log.num_entries == Param_log.max_entries){

    /* Save_log also resets Param_log.num_entries to zero */
    if(Save_log(&Param_log) != REG_SUCCESS){

      fprintf(stderr, "STEER: Log_param_values: Save_log failed\n");
      return REG_FAILURE;
    }
  }

  Param_log.entry[Param_log.num_entries].key = Param_log.primary_key_value++;
  /* This isn't a checkpoint so put meaningless values in next
     two fields */
  sprintf(Param_log.entry[Param_log.num_entries].chk_tag, "REG_PARAM_LOG");
  Param_log.entry[Param_log.num_entries].chk_handle = -99;
  count = 0;

  for(index = 0; index<Params_table.max_entries; index++){

    if(Params_table.param[index].handle == REG_PARAM_HANDLE_NOTSET)
      continue;

    if(Params_table.param[index].is_internal == REG_TRUE)continue;

    if(Params_table.param[index].logging_on == REG_FALSE)continue;

    /* We do not log 'raw binary' parameters */
    if(Params_table.param[index].type == REG_BIN)continue;

    /* This is one we want - store its handle and current value */
    Param_log.entry[Param_log.num_entries].param[count].handle =
                                           Params_table.param[index].handle;

    /* Update value associated with pointer */
    Get_ptr_value(&(Params_table.param[index]));

    /* Save this value */
    strcpy(Param_log.entry[Param_log.num_entries].param[count].value,
           Params_table.param[index].value);

    /* Storage for params associated with log entry is static */
    if(++count >= REG_MAX_NUM_STR_PARAMS)break;
  }

  /* Store the no. of params this entry has */
  Param_log.entry[Param_log.num_entries].num_param = count;

  /* Since we send status messages containing parameter values to
     attached client, we need only flag an entry as not being sent
     to steerer if no steerer is currently attached */
  Param_log.entry[Param_log.num_entries].sent_to_steerer = ReG_SteeringActive;

    /* Keep a count of entries that we have yet to send to steerer */
  if(ReG_SteeringActive != REG_TRUE)Param_log.num_unsent++;

  Param_log.num_entries++;

  return REG_SUCCESS;
}

/*----------------------------------------------------------------*/

int Emit_log(Chk_log_type *log, int handle)
{
  int   size = 0;
  int   index = 0;
  int   return_status = REG_SUCCESS;

  if(log->log_type == PARAM){
    index = Param_index_from_handle(&Params_table, handle);
    if(index == REG_PARAM_HANDLE_NOTSET)return REG_FAILURE;
  }

  if(log->emit_in_progress == REG_TRUE){

    if(Emit_log_entries(log, log->file_content, handle) == REG_UNFINISHED){
      return return_status;
    }
    log->emit_in_progress = REG_FALSE;

    free(log->file_content);
    log->file_content = NULL;

    /* Now send the entries that we have stored in memory -
       need to send all current log entries to the steerer */
    if(Log_to_xml(log, handle, &(log->file_content), &size,
		  REG_FALSE) != REG_SUCCESS){

      return REG_FAILURE;
    }
  }
  else if((log->log_type == CHKPT && log->send_all == REG_TRUE)||
	  (log->log_type == PARAM && log->param_send_all[index] == REG_TRUE)){

#ifdef REG_DEBUG_FULL
    fprintf(stderr, "STEER: Emit_log: sending all saved log entries...\n");
#endif
    /* Then we have to send any entries we've saved to file too... */
    Close_log_file(log);

    /* Read the log file and get back contents in buffer pointed
       to by pbuf.  We must free() this once we're done. */
    if(log->log_type == CHKPT){
      return_status = Read_file(log->filename, &(log->file_content),
				&size, REG_FALSE);
    }
    else if(log->log_type == PARAM){
      return_status = Read_file(log->filename, &(log->file_content),
				&size, REG_TRUE);
    }

    if(return_status != REG_SUCCESS){
      if(!log->file_content)free(log->file_content);
      log->file_content = NULL;
      return REG_FAILURE;
    }

    if (size > 0) Emit_log_entries(log, log->file_content, handle);

    /* Re-open log-file for future buffering */
    if( Open_log_file(log) != REG_SUCCESS){

#ifdef REG_DEBUG
      fprintf(stderr, "STEER: Emit_log: Open_log_file failed\n");
#endif
    }

    if(log->emit_in_progress == REG_TRUE){
      /* Don't set log->send_all if going to continue to Log_to_xml
	 as will affect what log entries are returned */
      log->send_all = REG_FALSE;
      return REG_SUCCESS;
    }

    free(log->file_content);
    log->file_content = NULL;

    /* End of sending buffered entries */

    /* Now send the entries that we have stored in memory -
       need to send all current log entries to the steerer */
    if(Log_to_xml(log, handle, &(log->file_content), &size,
		  REG_FALSE) != REG_SUCCESS){

      return REG_FAILURE;
    }

    if(log->log_type == PARAM){
      log->param_send_all[index] = REG_FALSE;
    }
    else{
      log->send_all = REG_FALSE;
    }
  }
  else{

#ifdef REG_DEBUG_FULL
    fprintf(stderr, "STEER: Emit_log: sending unsent log entries...\n");
#endif
    /* Fifth argument specifies that we only want those entries that haven't
       already been sent to the steerer */
    if(Log_to_xml(log, handle, &(log->file_content),
		  &size, REG_TRUE) != REG_SUCCESS){
      return REG_FAILURE;
    }
  }

#ifdef REG_DEBUG_FULL
  fprintf(stderr, "STEER: Emit_log: calling Emit_log_entries...\n");
#endif

  /* Pull the entries out of the buffer returned by Log_to_xml and
     send them to the steerer */
  if(size > 0){
    if( (return_status = Emit_log_entries(log, log->file_content,
					  handle)) == REG_UNFINISHED){
      return REG_SUCCESS;
    }
  }
  free(log->file_content);
  log->file_content = NULL;

#ifdef REG_DEBUG_FULL
  fprintf(stderr, "STEER: Emit_log: sending logged steering commands...\n");
#endif

  /* Send log of steering commands */
  if(strlen(log->pSteer_cmds) > 0){

    Emit_log_entries(log, log->pSteer_cmds, handle);
    log->pSteer_cmds[0]='\0';
    log->pSteer_cmds_slot = log->pSteer_cmds;
  }

  if(return_status == REG_SUCCESS){

    /* Zero counter since we've just told steerer all about any log
       entries it hadn't got */
    log->num_unsent = 0;
  }

  return return_status;
}

/*----------------------------------------------------------------*/

int Emit_log_entries(Chk_log_type *log, char *buf, int handle)
{
  static char *pXmlBuf = NULL;
  static char *pmsg_buf = NULL;
  int          status;

  /* Zero the count of messages sent this time around */
  log->num_sent = 0;

  if(log->emit_in_progress == REG_TRUE){
    if(Pack_send_log_entries(&pXmlBuf, &(log->num_sent)) == REG_UNFINISHED){

      return REG_UNFINISHED;
    }
    /* Check to see if we're all done or whether there's still
       raw log data to process */
    if(!pmsg_buf){

      return REG_SUCCESS;
    }
  }
  else{
    pmsg_buf = buf;
  }

  if(!strstr(pmsg_buf, "<Log_entry>")){

    while(1){
      pXmlBuf = Steer_lib_config.scratch_buffer;

      status = Log_columns_to_xml(&pmsg_buf, Steer_lib_config.scratch_buffer,
				  REG_SCRATCH_BUFFER_SIZE, handle);
      if(status == REG_FAILURE){
	return REG_FAILURE;
      }
      else if(status == REG_EOD){
	log->emit_in_progress = REG_FALSE;
	pmsg_buf = NULL;
      }

      while(Pack_send_log_entries(&pXmlBuf, &(log->num_sent))
	    == REG_UNFINISHED){
	log->num_sent = 0;
	usleep(500000);
      }

      log->num_sent = 0;
      if(status == REG_EOD) break;
    }
  }
  else{
    Pack_send_log_entries(&pmsg_buf, &(log->num_sent));
  }

  return REG_SUCCESS;
}

/*----------------------------------------------------------------*/

int Pack_send_log_entries(char **pBuf, int *msg_count)
{
  char *msg_buf, *pmsg_buf;
  char *plast = NULL;
  char *pbuf1 = NULL;
  char *pbuf2 = NULL;
  char *pbuf3 = NULL;
  void *pdum;
  int   msg_buf_size;
  int   tot_len = 0;
  int   len;
  int   nbytes;
  int   rewind;
  int   return_status = REG_SUCCESS;
  int   bytes_left;

  msg_buf_size = REG_MAX_MSG_SIZE;
  if(!(msg_buf = (char *)malloc(msg_buf_size))){

    fprintf(stderr, "STEER: Pack_send_log_entries: malloc failed\n");
    return REG_FAILURE;
  }

  /* Pull each log entry out of the buffer and pack them into
     messages to the steerer */
  pbuf1 = strstr(*pBuf, "<Log_entry>");
  pbuf2 = pbuf1;
  if(pbuf2){
    if( (pbuf3 = strstr(pbuf2, "</Log_entry>")) ){
      /* Increment ptr so as to include all of the "</Log_entry>" */
      pbuf3 += 12;
    }
  }

  while(pbuf3){

    if(*msg_count > REG_MAX_NUM_LOG_MSG){
      *pBuf = pbuf3;
      free(msg_buf);
      msg_buf = NULL;
      return REG_UNFINISHED;
    }

    if(tot_len == 0){
      /* Begin the first message */
      bytes_left = msg_buf_size;
      pmsg_buf = msg_buf;
      Write_xml_header(&pmsg_buf, &bytes_left);
      pmsg_buf += sprintf(pmsg_buf, "<Steer_log>\n");
      tot_len = (int)(pmsg_buf - msg_buf);
    }
    len = (int)(pbuf3 - pbuf2);

    /* 35 = strlen("</Steer_log>\n</ReG_steer_message>\n"); */
    if((tot_len + len) < (msg_buf_size - 35)){

      /* Buffer has enough free space so add this entry to it */
      plast = pmsg_buf;
      strncpy(pmsg_buf, pbuf2, len);
      pmsg_buf += len;
      tot_len += len;
    }
    else if(!plast){

      /* We've not managed to fit a single entry into the buffer
	 - time for a realloc... */
      msg_buf_size *= 2;
      if( !(pdum = realloc(msg_buf, msg_buf_size)) ){

	free(msg_buf);
	msg_buf = NULL;
	fprintf(stderr, "STEER: Pack_send_log_entries: realloc failed\n");
	return REG_FAILURE;
      }
      /* Allow for fact that realloc can return a ptr to a diff't
	 chunk of memory (with any content intact though) */
      pmsg_buf += (char *)pdum - msg_buf;
      msg_buf = (char *)pdum;
      continue;
    }
    else{
      /* Complete the xml message */
      nbytes = snprintf(pmsg_buf, (msg_buf_size-tot_len),
			"</Steer_log>\n");

      rewind = (nbytes >= (msg_buf_size-tot_len-1) || (nbytes < 1));

      if(!rewind){
	pmsg_buf += nbytes;
	tot_len += nbytes;

	if(Write_xml_footer(&pmsg_buf, (msg_buf_size-tot_len))
	                                             != REG_SUCCESS){
	  rewind = 1;
	}
      }

      if(rewind){

	/* Can't fit the footer in remaining space in buffer - go
	   back to last complete entry */
	if(*plast == '\n')plast++;
	tot_len = (int)(plast - msg_buf);
	pmsg_buf = plast;
	pmsg_buf += sprintf(pmsg_buf, "</Steer_log>\n");
	tot_len = (int)(pmsg_buf - msg_buf);
	pbuf3 = pbuf2;
	pbuf2 = pbuf1;
	len = (int)(pbuf3 - pbuf2);

	if(Write_xml_footer(&pmsg_buf, (msg_buf_size-tot_len))
	                                             != REG_SUCCESS){
	  fprintf(stderr, "STEER: Pack_send_log_entries: error writing footer\n");
	  free(msg_buf);
	  msg_buf = NULL;
	  return REG_FAILURE;
	}
      }

      if(Send_status_msg(msg_buf) != REG_SUCCESS){
	*pBuf = pbuf2;
	free(msg_buf);
	msg_buf = NULL;
	return REG_UNFINISHED;
      }
      (*msg_count)++;

      /* Begin the next message */
      plast = NULL;
      pmsg_buf = msg_buf;
      bytes_left = msg_buf_size;
      Write_xml_header(&pmsg_buf, &bytes_left);
      pmsg_buf += sprintf(pmsg_buf, "<Steer_log>\n");

      strncpy(pmsg_buf, pbuf2, len);
      pmsg_buf += len;
      tot_len = (int)(pmsg_buf - msg_buf);
    }

    /* Look for next entry */
    if(*pbuf3 == '\n')pbuf3++;
    pbuf1 = pbuf2;
    pbuf2 = pbuf3;
    if( (pbuf3 = strstr(pbuf2, "</Log_entry>")) ){
      /* Increment ptr so as to include all of the "</Log_entry>" */
      pbuf3 += 12;
    }
  }

  /* Complete the xml message */
  if(tot_len > 0){
    pmsg_buf += sprintf(pmsg_buf, "</Steer_log>\n");
    tot_len = (int)(pmsg_buf - msg_buf);

    if(Write_xml_footer(&pmsg_buf, (msg_buf_size-tot_len))
                                                   == REG_SUCCESS){

      return_status = Send_status_msg(msg_buf);
      if(return_status != REG_SUCCESS){
	/* pbuf3 is NULL here 'cos we've broken out of while loop*/
	*pBuf = pbuf1;
	free(msg_buf);
	msg_buf = NULL;
	return REG_UNFINISHED;
      }
      (*msg_count)++;
    }
    else{
      fprintf(stderr, "STEER: Pack_send_log_entries: error writing final footer\n");
      return_status = REG_FAILURE;
    }
  }

  free(msg_buf);
  msg_buf = NULL;
  return return_status;
}

/*----------------------------------------------------------------*/

int Log_columns_to_xml(char **buf, char* out_buf, int out_buf_size,
		       int handle)
{
  /* We have contents of log file as:
     key   <handle 0> <value 0> <handle 1> <value 1>... \n
     key++ <handle 0> <value 0> <handle 1> <value 1>... \n
     etc.
  */
  /* '2*' to allow for handle & value of each parameter and '+1'
     for key of each log entry */
#define max_num_fields  (2*REG_MAX_NUM_STR_PARAMS + 1)
#define max_field_length  32
  char *fields[max_num_fields];
  char *pbuf;
  char *ptr1;
  char *ptr2;
  char *ptr3;
  char *ptr4;
  int   i, count;
  int   nbytes = 0;
  int   bytes_left = out_buf_size;
  char  handle_str[16];

  if(!out_buf){
    fprintf(stderr, "STEER: Log_columns_to_xml: ERROR: supplied ptr to "
	    "output buffer is NULL\n");
    return REG_FAILURE;
  }

  sprintf(handle_str, "%d", handle);

  fields[0] = malloc(max_num_fields*max_field_length*sizeof(char));
  if(!fields[0]){
    return REG_FAILURE;
  }

  for(i=1;i<max_num_fields;i++){
    fields[i] = (char *)(fields[0] + i*max_field_length);
  }

  /* Set pointer to buffer to take output. ASSUME that this is
     pre-allocated by caller and has room for out_buf_size bytes. */
  pbuf = out_buf;

  ptr1 = *buf;
  while( (ptr2 = strstr(ptr1, "\n")) ){

    ptr3 = ptr1;
    count = 0;
    while( (count < max_num_fields) &&
	   ((ptr4 = strstr(ptr3, " ")) && ((void*)ptr4 < (void*)ptr2)) ){
      if( (i = (int)(ptr4 - ptr3)) > max_field_length){
	/* So we can output the problematic field */
	strncpy(fields[0], ptr3, i);
	fields[0][i] = '\0';
	fprintf(stderr, "STEER: Log_columns_to_xml: ERROR: field "
		">>%s<< exceeds maximum width of %d characters\n",
		 fields[0], max_field_length);
	free(fields[0]);
	return REG_FAILURE;
      }
      memcpy(fields[count], ptr3, i);
      (fields[count])[i] = '\0';
      ptr3 = ptr4;
      while(*(++ptr3) == ' '){}/* Cope with multiple blank spaces */
      count++;
    }
    memcpy(fields[count], ptr3, ptr2-ptr3);
    (fields[count])[ptr2-ptr3] = '\0';
    count++;

    nbytes = snprintf(pbuf, bytes_left, "<Log_entry><Key>%s</Key><Param>",
		      fields[0]);
    if((nbytes >= (bytes_left-1)) || (nbytes < 1)){
      *buf = ptr1;
      free(fields[0]);
      return REG_SUCCESS;
    }
    pbuf += nbytes;
    bytes_left -= nbytes;

    for(i=1;i<count;i+=2){
      /* Only pull out the parameter with the requested handle */
      if(strcmp(handle_str, fields[i]) == 0){
	nbytes = snprintf(pbuf, bytes_left,
			  "<Handle>%s</Handle><Value>%s</Value>",
			  fields[i], fields[i+1]);
	if((nbytes >= (bytes_left-1)) || (nbytes < 1)){
	  *buf = ptr1;
	  free(fields[0]);
	  return REG_SUCCESS;
	}
	pbuf += nbytes;
	bytes_left -= nbytes;
	break;
      }
    }
    nbytes = snprintf(pbuf, bytes_left, "</Param></Log_entry>\n");
    if((nbytes >= (bytes_left-1)) || (nbytes < 1)){
      *buf = ptr1;
      free(fields[0]);
      return REG_SUCCESS;
    }
    pbuf += nbytes;
    bytes_left -= nbytes;

    ptr1 = ptr2 + 1;
  }

  pbuf = '\0';
  free(fields[0]);

  return REG_EOD;
}

/*----------------------------------------------------------------------*/

int Log_control_msg(struct control_struct *control)
{
  char      *pbuf;
  static int seq_num_index   = -1;
  int        nbytes;
  int        bytes_free;
  void      *pdum;
  struct cmd_struct   *ctrl;
  struct param_struct *param;

  /* If this is the first time we've been called then calculate
     the index of the Sequence No. in the table of parameters */
  if(seq_num_index == -1){
    seq_num_index = Param_index_from_handle(&(Params_table),
					    REG_SEQ_NUM_HANDLE);
    if(seq_num_index == -1){
      fprintf(stderr, "STEER: Log_control_msg: failed to find "
	      "index of sequence no.\n");
      return REG_FAILURE;
    }
  }

  pbuf = Chk_log.pSteer_cmds_slot;
  bytes_free = Chk_log.steer_cmds_bytes - (int)(pbuf - Chk_log.pSteer_cmds);

  nbytes = snprintf(pbuf, bytes_free, "<Log_entry>\n"
		    "<Seq_num>%s</Seq_num>\n"
		    "<Steer_log_entry>\n",
		    Params_table.param[seq_num_index].value);

  if(nbytes >= (REG_MAX_STRING_LENGTH-1) || (nbytes < 1)){

    if(!(pdum = realloc(Chk_log.pSteer_cmds, 2*Chk_log.steer_cmds_bytes))){

      fprintf(stderr, "STEER: Log_control_msg: failed to realloc log buffer\n");
      /* Terminate buffer at end of last complete entry */
      *(Chk_log.pSteer_cmds_slot) = '\0';
      return REG_FAILURE;
    }
    else{

      Chk_log.steer_cmds_bytes *= 2;
      bytes_free += Chk_log.steer_cmds_bytes;
      Chk_log.pSteer_cmds = (char *)pdum;
      pbuf = Chk_log.pSteer_cmds_slot;

      nbytes = snprintf(pbuf, bytes_free, "<Log_entry>\n"
			"<Seq_num>%s</Seq_num>\n"
			"<Steer_log_entry>\n",
			Params_table.param[seq_num_index].value);
    }
  }
  pbuf += nbytes;
  bytes_free -= nbytes;

  if(control->first_cmd){
    ctrl = control->first_cmd;
    while(ctrl){
      if(ctrl->id){
	nbytes = snprintf(pbuf, bytes_free, "<Command><Cmd_id>%s</Cmd_id>",
			  (char*)(ctrl->id));
	pbuf += nbytes;
	bytes_free -= nbytes;
      }
      else if(ctrl->name){
	nbytes = snprintf(pbuf, bytes_free, "<Command><Cmd_name>%s</Cmd_name>",
			  (char*)(ctrl->name));
	pbuf += nbytes;
	bytes_free -= nbytes;
      }
      if(ctrl->first_param){
	param = ctrl->first_param;
        while(param){
	   nbytes = snprintf(pbuf, bytes_free, "<Cmd_param><Handle>"
			     "%s</Handle><Value>%s</Value></Cmd_param>",
			     (char*)(param->handle),
			     (char*)(param->value));
	   pbuf += nbytes;
	   bytes_free -= nbytes;
	   /* ARPDBG - check for buffer overrun */
	   param = param->next;
	}
      }
      nbytes = snprintf(pbuf, bytes_free, "</Command>");
      pbuf += nbytes;
      bytes_free -= nbytes;
      ctrl = ctrl->next;
    }
  }

  if(control->first_param){
    param = control->first_param;
    while(param){
      nbytes = snprintf(pbuf, bytes_free, "<Param><Handle>"
			"%s</Handle><Value>%s</Value></Param>",
			(char*)(param->handle), (char*)(param->value));
      bytes_free -= nbytes;
      pbuf += nbytes;
      param = param->next;
    }
  }

  pbuf += sprintf(pbuf, "</Steer_log_entry>\n"
		  "</Log_entry>\n");

  /* Point to next free space in buffer */
  Chk_log.pSteer_cmds_slot = pbuf;

  return REG_SUCCESS;
}

/*----------------------------------------------------------------------*/
