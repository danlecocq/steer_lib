/*----------------------------------------------------------------------------
  This header file contains routines and data structures for
  steerside WSRF, SOAP-based communication.

  (C) Copyright 2005, University of Manchester, United Kingdom,
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

  Authors........: Andrew Porter

---------------------------------------------------------------------------*/
#ifndef __REG_STEER_STEERSIDE_WSRF_H__
#define __REG_STEER_STEERSIDE_WSRF_H__

/** @file ReG_Steer_Steerside_WSRF.h
    @brief Header file for WSRF over SOAP communications for the steering client.
  */

/*-------------------------------------------------------------------*/

/** (For debugging.) Handler passed to gsoap to be called when get 
    unexpected tags 
    @internal */
int soapMismatchHandler(struct soap *soap, 
			const char  *tag);
/** Attach to a simulation 
    @internal */
int Sim_attach_wsrf (Sim_entry_type *sim, 
                     char           *SimID);
/** Send a control msg to a simulation 
    @internal */
int Send_control_msg_wsrf (Sim_entry_type *sim,
			   char           *buf);
/** Send a detach msg to a simulation */
int Send_detach_msg_wsrf (Sim_entry_type *sim);
/** Get the next status message from the simulation 
    @internal */
struct msg_struct *Get_status_msg_wsrf(Sim_entry_type *sim);
/** Retrieve the next stored message (if any) from the simulation.
    @return NULL if no stored msg otherwise ptr to next message.
    @internal */
struct msg_struct *Get_next_stored_msg(Sim_entry_type *sim);
/** Get the value of the specified resource property 
    @internal 
    @param sgs_info Pointer to struct holding info on SWS
    @param name     Name of the resource property to get
    @param pRP      If successful, ptr to array of char 
                    holding value of RP */
int Get_resource_property (SGS_info_type *sgs_info, 
			   char          *name,
			   char         **pRP);
/** Get the whole resource property document 
    @internal 
    @param sgs_info Pointer to struct holding info on SWS
    @param pRPDoc   If successful, ptr to array of char holding 
    contents of the ResourceProperty document */
int Get_resource_property_doc(SGS_info_type *sgs_info,
			      char         **pRPDoc);
/** Clean up a WSRF-based steering connection 
    @internal */
int Finalize_connection_wsrf (Sim_entry_type *sim);
/** Retrieve the full log of the parameter with the specified
    handle. Returns REG_SUCCESS or REG_FAILURE.  If successful
    then log data is stored in internal buffer.
    @internal */
int Get_param_log_wsrf(Sim_entry_type *sim,
		       int             handle);
/** Instruct the simulation to restart from the specified
    node in a checkpoint tree. 
    @internal
    The Restart method of the SWS, contacts the specified checkpoint
    node and obtains the information needed to identify the checkpoint
    files to the simulation. */
int Send_restart_msg_wsrf(Sim_entry_type *sim, char *chkGSH);

#endif