/*----------------------------------------------------------------------------
    Header file defining public routines used in the construction of
    the steering interface of a steering component.

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

#include "ReG_Steer_types.h"

#ifdef REG_STR_CPP
  #define PREFIX "C"
#else
  #define PREFIX 
#endif

/*-------------- Steerer-side function prototypes -------------*/

/* Attempt to attach to the specified simulation - returns a 
   handle for this simulation if successful. */
extern PREFIX int Sim_attach(REG_SimIDType SimID,
			     int *SimHandle);

/* Detach from the specfied simulation.  Signals the simulation
   that steerer has detached and then cleans-up associated
   files and table entries. */
extern PREFIX int Sim_detach(int *SimHandle);

/* Looks for the next message from any attached simulations. If
   it finds one then it returns the handle of the originating
   simulation and the type of message. */
extern PREFIX int Get_next_message(int         *SimHandle,
				   REG_MsgType *msg_type);

/* Consume and store the parameter definitions that the 
   simulation referred to by SimHandle has emitted. */
extern PREFIX int Consume_param_defs(int SimHandle);

/* Consume and store the IO-type definitions that the 
   simulation referred to by SimHandle has emitted. */
extern PREFIX int Consume_IOType_defs(int SimHandle);

/* Consume a status message emitted by the simulation associated
   with SimHandle.  Returns that simulations current sequence no.
   and a list of any commands received (e.g. finish). */
extern PREFIX int Consume_status(int   SimHandle,
				 int  *SeqNum,
				 int  *NumCmds,
				 int  *Commands);

/* Emit a steering-control message to the simulation associated
   with SimHandle.  Emits the specified commands (if any) and
   automatically sends any (steerable) parameter values that have
   been edited since the last call to this routine. */
extern PREFIX int Emit_control(int    SimHandle,
			       int    NumCommands,
			       int   *SysCommands);

/* Initialise the internal tables etc. used by the steering library
   on the steering application sied.  Must be called before all other
   steering-library routines. */
extern PREFIX int Steerer_initialize();

/* Cleans up the internal tables etc. Must be called after all steering
   activity is complete. */
extern PREFIX int Steerer_finalize();

/* Deletes all data associated with the simulation with handle SimHandle.
   Used when a simulation detaches. */
extern PREFIX int Delete_sim_table_entry(int *SimHandle);

/* A debugging routine - writes the complete contents of the internal
   table holding information on all connected simulations to 
   ./sim_table.txt */
extern PREFIX int Dump_sim_table();

/* Gets the number of <steerable> parameters associated with the simulation 
   with handle sim_handle.  i.e. if steerable==TRUE then this returns the
   number of steerable parameters that the simulation has. */
extern PREFIX int Get_param_number(int  sim_handle,
				   int  steerable,
				   int *num_params);

/* Gets the first <num_params> <steerable> parameters assocaited with the
   simulation with handle sim_handle.  Returns the handles, labels and
   values (as strings) of these parameters. handles, labels and vals
   must all point to chunks of memory large enough to receive num_params
   entries. */
extern PREFIX int Get_param_values(int    sim_handle,
				   int    steerable,
				   int    num_params,
				   int   *handles,
				   char* *labels,
				   char* *vals);

/* Sets the values of the parameters with the specified handles for the
   simulation with handle sim_handle. Causes internal flags to be set
   to indicate that these parameter values have changed. */
extern PREFIX int Set_param_values(int    sim_handle,
				   int    num_params,
				   int   *handles,
				   char* *vals);

/* Gets the number of IO types associated with the simulation with
   handle <sim_handle>. */
extern PREFIX int Get_iotype_number(int sim_handle,
				    int *num_iotypes);

/* Gets the first <num_iotypes> IO types associated with the simulation
   with hand <sim_handle>. Returns the handle and label associated with
   each IO type. */
extern PREFIX int Get_iotypes(int    sim_handle,
			      int    num_iotypes,
			      int   *handles,
			      char* *labels);
