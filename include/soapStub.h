/* soapStub.h
   Generated by gSOAP 2.2.3b from new.h
   Copyright (C) 2001-2003 Genivia inc.
   All Rights Reserved.
*/
#ifndef soapStub_H
#define soapStub_H
#ifdef __cplusplus
extern "C" {
#endif

/* Types With Custom (De)serializers: */

/* Enumerations */

/* Classes and Structs */

#ifndef _SOAP_sgs__GetStatusResponse
#define _SOAP_sgs__GetStatusResponse
struct sgs__GetStatusResponse
{
	char *_GetStatusReturn;
};
#endif

#ifndef _SOAP_sgs__PauseResponse
#define _SOAP_sgs__PauseResponse
struct sgs__PauseResponse
{
	char *_PauseReturn;
};
#endif

#ifndef _SOAP_sgs__AppStartResponse
#define _SOAP_sgs__AppStartResponse
struct sgs__AppStartResponse
{
	char *_AppStartReturn;
};
#endif

#ifndef _SOAP_sgs__RestartResponse
#define _SOAP_sgs__RestartResponse
struct sgs__RestartResponse
{
	char *_RestartReturn;
};
#endif

#ifndef _SOAP_sgs__AppRecordChkpointResponse
#define _SOAP_sgs__AppRecordChkpointResponse
struct sgs__AppRecordChkpointResponse
{
	char *_AppRecordChkpointReturn;
};
#endif

#ifndef _SOAP_sgs__AppDetachResponse
#define _SOAP_sgs__AppDetachResponse
struct sgs__AppDetachResponse
{
	char *_AppDetachReturn;
};
#endif

#ifndef _SOAP_sgs__GetNthDataSourceResponse
#define _SOAP_sgs__GetNthDataSourceResponse
struct sgs__GetNthDataSourceResponse
{
	char *_GetNthDataSourceReturn;
};
#endif

#ifndef _SOAP_sgs__PutControlResponse
#define _SOAP_sgs__PutControlResponse
struct sgs__PutControlResponse
{
	char *_PutControlReturn;
};
#endif

#ifndef _SOAP_sgs__StopResponse
#define _SOAP_sgs__StopResponse
struct sgs__StopResponse
{
	char *_StopReturn;
};
#endif

#ifndef _SOAP_sgs__AppStopResponse
#define _SOAP_sgs__AppStopResponse
struct sgs__AppStopResponse
{
	char *_AppStopReturn;
};
#endif

#ifndef _SOAP_sgs__destroyResponse
#define _SOAP_sgs__destroyResponse
struct sgs__destroyResponse
{
};
#endif

#ifndef _SOAP_sgs__PutStatusResponse
#define _SOAP_sgs__PutStatusResponse
struct sgs__PutStatusResponse
{
	char *_PutStatusReturn;
};
#endif

#ifndef _SOAP_sgs__DetachResponse
#define _SOAP_sgs__DetachResponse
struct sgs__DetachResponse
{
	char *_DetachReturn;
};
#endif

#ifndef _SOAP_sgs__ClearStatusMsgQueueResponse
#define _SOAP_sgs__ClearStatusMsgQueueResponse
struct sgs__ClearStatusMsgQueueResponse
{
	char *_ClearStatusMsgQueueReturn;
};
#endif

#ifndef _SOAP_sgs__GetNotificationsResponse
#define _SOAP_sgs__GetNotificationsResponse
struct sgs__GetNotificationsResponse
{
	char *_GetNotificationsReturn;
};
#endif

#ifndef _SOAP_sgs__GetControlResponse
#define _SOAP_sgs__GetControlResponse
struct sgs__GetControlResponse
{
	char *_GetControlReturn;
};
#endif

#ifndef _SOAP_sgs__AttachResponse
#define _SOAP_sgs__AttachResponse
struct sgs__AttachResponse
{
	char *_AttachReturn;
};
#endif

#ifndef _SOAP_sgs__ResumeResponse
#define _SOAP_sgs__ResumeResponse
struct sgs__ResumeResponse
{
	char *_ResumeReturn;
};
#endif

#ifndef _SOAP_sgs__setServiceDataResponse
#define _SOAP_sgs__setServiceDataResponse
struct sgs__setServiceDataResponse
{
	char *_setServiceDataReturn;
};
#endif

#ifndef _SOAP_sgs__findServiceDataResponse
#define _SOAP_sgs__findServiceDataResponse
struct sgs__findServiceDataResponse
{
	char *_findServiceDataReturn;
};
#endif

#ifndef _SOAP_sgs__setServiceData
#define _SOAP_sgs__setServiceData
struct sgs__setServiceData
{
	char *in0;
};
#endif

#ifndef _SOAP_sgs__destroy
#define _SOAP_sgs__destroy
struct sgs__destroy
{
};
#endif

#ifndef _SOAP_sgs__AppDetach
#define _SOAP_sgs__AppDetach
struct sgs__AppDetach
{
};
#endif

#ifndef _SOAP_sgs__PutStatus
#define _SOAP_sgs__PutStatus
struct sgs__PutStatus
{
	char *in0;
};
#endif

#ifndef _SOAP_sgs__AppStop
#define _SOAP_sgs__AppStop
struct sgs__AppStop
{
};
#endif

#ifndef _SOAP_sgs__GetControl
#define _SOAP_sgs__GetControl
struct sgs__GetControl
{
};
#endif

#ifndef _SOAP_sgs__GetStatus
#define _SOAP_sgs__GetStatus
struct sgs__GetStatus
{
};
#endif

#ifndef _SOAP_sgs__Restart
#define _SOAP_sgs__Restart
struct sgs__Restart
{
	char *in0;
};
#endif

#ifndef _SOAP_sgs__Attach
#define _SOAP_sgs__Attach
struct sgs__Attach
{
};
#endif

#ifndef _SOAP_sgs__GetNthDataSource
#define _SOAP_sgs__GetNthDataSource
struct sgs__GetNthDataSource
{
	int in0;
};
#endif

#ifndef _SOAP_sgs__Stop
#define _SOAP_sgs__Stop
struct sgs__Stop
{
};
#endif

#ifndef _SOAP_sgs__Detach
#define _SOAP_sgs__Detach
struct sgs__Detach
{
};
#endif

#ifndef _SOAP_sgs__PutControl
#define _SOAP_sgs__PutControl
struct sgs__PutControl
{
	char *in0;
};
#endif

#ifndef _SOAP_sgs__findServiceData
#define _SOAP_sgs__findServiceData
struct sgs__findServiceData
{
	char *in0;
};
#endif

#ifndef _SOAP_sgs__AppRecordChkpoint
#define _SOAP_sgs__AppRecordChkpoint
struct sgs__AppRecordChkpoint
{
	char *in0;
	char *in1;
};
#endif

#ifndef _SOAP_sgs__Pause
#define _SOAP_sgs__Pause
struct sgs__Pause
{
};
#endif

#ifndef _SOAP_sgs__GetNotifications
#define _SOAP_sgs__GetNotifications
struct sgs__GetNotifications
{
};
#endif

#ifndef _SOAP_sgs__AppStart
#define _SOAP_sgs__AppStart
struct sgs__AppStart
{
};
#endif

#ifndef _SOAP_sgs__Resume
#define _SOAP_sgs__Resume
struct sgs__Resume
{
};
#endif

#ifndef _SOAP_sgs__ClearStatusMsgQueue
#define _SOAP_sgs__ClearStatusMsgQueue
struct sgs__ClearStatusMsgQueue
{
};
#endif

#ifndef _SOAP_sgsf__createServiceResponse
#define _SOAP_sgsf__createServiceResponse
struct sgsf__createServiceResponse
{
	char *_createServiceReturn;
};
#endif

#ifndef _SOAP_sgsf__destroyResponse
#define _SOAP_sgsf__destroyResponse
struct sgsf__destroyResponse
{
};
#endif

#ifndef _SOAP_sgsf__registerSelfResponse
#define _SOAP_sgsf__registerSelfResponse
struct sgsf__registerSelfResponse
{
	char *_registerSelfReturn;
};
#endif

#ifndef _SOAP_sgsf__registerSelf
#define _SOAP_sgsf__registerSelf
struct sgsf__registerSelf
{
	char *in0;
	char *in1;
};
#endif

#ifndef _SOAP_sgsf__destroy
#define _SOAP_sgsf__destroy
struct sgsf__destroy
{
};
#endif

#ifndef _SOAP_sgsf__createService
#define _SOAP_sgsf__createService
struct sgsf__createService
{
	char *in0;
	char *in1;
};
#endif

#ifndef _SOAP_sgr__findServiceDataResponse
#define _SOAP_sgr__findServiceDataResponse
struct sgr__findServiceDataResponse
{
	char *_findServiceDataReturn;
};
#endif

#ifndef _SOAP_ArrayOf_USCORE_xsd_USCORE_string
#define _SOAP_ArrayOf_USCORE_xsd_USCORE_string
/* Array of xsd:string schema type: */
struct ArrayOf_USCORE_xsd_USCORE_string
{
	char **__ptr;
	int __size;
	int __offset;
};
#endif

#ifndef _SOAP_sgr__addResponse
#define _SOAP_sgr__addResponse
struct sgr__addResponse
{
	struct ArrayOf_USCORE_xsd_USCORE_string *_addReturn;
};
#endif

#ifndef _SOAP_sgr__removeResponse
#define _SOAP_sgr__removeResponse
struct sgr__removeResponse
{
};
#endif

#ifndef _SOAP_sgr__remove
#define _SOAP_sgr__remove
struct sgr__remove
{
	char *in0;
};
#endif

#ifndef _SOAP_sgr__add
#define _SOAP_sgr__add
struct sgr__add
{
	char *in0;
	char *in1;
	char *in2;
};
#endif

#ifndef _SOAP_sgr__findServiceData
#define _SOAP_sgr__findServiceData
struct sgr__findServiceData
{
	char *in0;
};
#endif

#ifndef _SOAP_rgtf__requestTerminationBeforeResponse
#define _SOAP_rgtf__requestTerminationBeforeResponse
struct rgtf__requestTerminationBeforeResponse
{
	char *_requestTerminationBeforeReturn;
};
#endif

#ifndef _SOAP_rgtf__findServiceDataResponse
#define _SOAP_rgtf__findServiceDataResponse
struct rgtf__findServiceDataResponse
{
	char *_findServiceDataReturn;
};
#endif

#ifndef _SOAP_rgtf__findByHandleResponse
#define _SOAP_rgtf__findByHandleResponse
struct rgtf__findByHandleResponse
{
	char *_findByHandleReturn;
};
#endif

#ifndef _SOAP_rgtf__getActiveTreesResponse
#define _SOAP_rgtf__getActiveTreesResponse
struct rgtf__getActiveTreesResponse
{
	char *_getActiveTreesReturn;
};
#endif

#ifndef _SOAP_rgtf__createNewTreeResponse
#define _SOAP_rgtf__createNewTreeResponse
struct rgtf__createNewTreeResponse
{
	char *_createNewTreeReturn;
};
#endif

#ifndef _SOAP_rgtf__requestTerminationAfterResponse
#define _SOAP_rgtf__requestTerminationAfterResponse
struct rgtf__requestTerminationAfterResponse
{
	char *_requestTerminationAfterReturn;
};
#endif

#ifndef _SOAP_rgtf__destroyResponse
#define _SOAP_rgtf__destroyResponse
struct rgtf__destroyResponse
{
};
#endif

#ifndef _SOAP_rgtf__createNewTree
#define _SOAP_rgtf__createNewTree
struct rgtf__createNewTree
{
	char *in0;
	char *in1;
	char *in2;
	char *in3;
};
#endif

#ifndef _SOAP_rgtf__findByHandle
#define _SOAP_rgtf__findByHandle
struct rgtf__findByHandle
{
	char *in0;
	char *in1;
};
#endif

#ifndef _SOAP_rgtf__destroy
#define _SOAP_rgtf__destroy
struct rgtf__destroy
{
};
#endif

#ifndef _SOAP_rgtf__findServiceData
#define _SOAP_rgtf__findServiceData
struct rgtf__findServiceData
{
	char *in0;
};
#endif

#ifndef _SOAP_rgtf__getActiveTrees
#define _SOAP_rgtf__getActiveTrees
struct rgtf__getActiveTrees
{
};
#endif

#ifndef _SOAP_rgtf__requestTerminationBefore
#define _SOAP_rgtf__requestTerminationBefore
struct rgtf__requestTerminationBefore
{
	char *in0;
};
#endif

#ifndef _SOAP_rgtf__requestTerminationAfter
#define _SOAP_rgtf__requestTerminationAfter
struct rgtf__requestTerminationAfter
{
	char *in0;
};
#endif

#ifndef _SOAP_rgt__getParentNodeResponse
#define _SOAP_rgt__getParentNodeResponse
struct rgt__getParentNodeResponse
{
	char *_getParentNodeReturn;
};
#endif

#ifndef _SOAP_rgt__requestTerminationAfterResponse
#define _SOAP_rgt__requestTerminationAfterResponse
struct rgt__requestTerminationAfterResponse
{
	char *_requestTerminationAfterReturn;
};
#endif

#ifndef _SOAP_rgt__addNodeResponse
#define _SOAP_rgt__addNodeResponse
struct rgt__addNodeResponse
{
	char *_addNodeReturn;
};
#endif

#ifndef _SOAP_rgt__getCheckPointDataResponse
#define _SOAP_rgt__getCheckPointDataResponse
struct rgt__getCheckPointDataResponse
{
	char *_getCheckPointDataReturn;
};
#endif

#ifndef _SOAP_rgt__findServiceDataResponse
#define _SOAP_rgt__findServiceDataResponse
struct rgt__findServiceDataResponse
{
	char *_findServiceDataReturn;
};
#endif

#ifndef _SOAP_rgt__setCheckPointDataResponse
#define _SOAP_rgt__setCheckPointDataResponse
struct rgt__setCheckPointDataResponse
{
};
#endif

#ifndef _SOAP_rgt__getChildNodesResponse
#define _SOAP_rgt__getChildNodesResponse
struct rgt__getChildNodesResponse
{
	char *_getChildNodesReturn;
};
#endif

#ifndef _SOAP_rgt__requestTerminationBeforeResponse
#define _SOAP_rgt__requestTerminationBeforeResponse
struct rgt__requestTerminationBeforeResponse
{
	char *_requestTerminationBeforeReturn;
};
#endif

#ifndef _SOAP_rgt__getSteeringCommandsResponse
#define _SOAP_rgt__getSteeringCommandsResponse
struct rgt__getSteeringCommandsResponse
{
	char *_getSteeringCommandsReturn;
};
#endif

#ifndef _SOAP_rgt__getInputFileResponse
#define _SOAP_rgt__getInputFileResponse
struct rgt__getInputFileResponse
{
	char *_getInputFileReturn;
};
#endif

#ifndef _SOAP_rgt__destroyResponse
#define _SOAP_rgt__destroyResponse
struct rgt__destroyResponse
{
};
#endif

#ifndef _SOAP_rgt__requestTerminationAfter
#define _SOAP_rgt__requestTerminationAfter
struct rgt__requestTerminationAfter
{
	char *in0;
};
#endif

#ifndef _SOAP_rgt__getChildNodes
#define _SOAP_rgt__getChildNodes
struct rgt__getChildNodes
{
};
#endif

#ifndef _SOAP_rgt__getInputFile
#define _SOAP_rgt__getInputFile
struct rgt__getInputFile
{
};
#endif

#ifndef _SOAP_rgt__findServiceData
#define _SOAP_rgt__findServiceData
struct rgt__findServiceData
{
	char *in0;
};
#endif

#ifndef _SOAP_rgt__setCheckPointData
#define _SOAP_rgt__setCheckPointData
struct rgt__setCheckPointData
{
};
#endif

#ifndef _SOAP_rgt__destroy
#define _SOAP_rgt__destroy
struct rgt__destroy
{
};
#endif

#ifndef _SOAP_rgt__getSteeringCommands
#define _SOAP_rgt__getSteeringCommands
struct rgt__getSteeringCommands
{
};
#endif

#ifndef _SOAP_rgt__requestTerminationBefore
#define _SOAP_rgt__requestTerminationBefore
struct rgt__requestTerminationBefore
{
	char *in0;
};
#endif

#ifndef _SOAP_rgt__getCheckPointData
#define _SOAP_rgt__getCheckPointData
struct rgt__getCheckPointData
{
};
#endif

#ifndef _SOAP_rgt__getParentNode
#define _SOAP_rgt__getParentNode
struct rgt__getParentNode
{
};
#endif

#ifndef _SOAP_rgt__addNode
#define _SOAP_rgt__addNode
struct rgt__addNode
{
	char *in0;
	char *in1;
	char *in2;
	char *in3;
	char *in4;
};
#endif

#ifndef _SOAP_SOAP_ENV__Header
#define _SOAP_SOAP_ENV__Header
/* SOAP Header: */
struct SOAP_ENV__Header
{
	void *dummy;	/* transient */
};
#endif

#ifndef _SOAP_SOAP_ENV__Code
#define _SOAP_SOAP_ENV__Code
/* SOAP Fault Code: */
struct SOAP_ENV__Code
{
	char *SOAP_ENV__Value;
	char *SOAP_ENV__Node;
	char *SOAP_ENV__Role;
};
#endif

#ifndef _SOAP_SOAP_ENV__Fault
#define _SOAP_SOAP_ENV__Fault
/* SOAP Fault: */
struct SOAP_ENV__Fault
{
	char *faultcode;
	char *faultstring;
	char *faultactor;
	char *detail;
	struct SOAP_ENV__Code *SOAP_ENV__Code;
	char *SOAP_ENV__Reason;
	char *SOAP_ENV__Detail;
};
#endif

/* Typedefs */
typedef char *xsd__string;
typedef int xsd__int;

/* Variables */

/* Remote Methods */

SOAP_FMAC1 int SOAP_FMAC2 sgs__setServiceData(struct soap*, char *, struct sgs__setServiceDataResponse *);

SOAP_FMAC1 int SOAP_FMAC2 sgs__destroy(struct soap*, struct sgs__destroyResponse *);

SOAP_FMAC1 int SOAP_FMAC2 sgs__AppDetach(struct soap*, struct sgs__AppDetachResponse *);

SOAP_FMAC1 int SOAP_FMAC2 sgs__PutStatus(struct soap*, char *, struct sgs__PutStatusResponse *);

SOAP_FMAC1 int SOAP_FMAC2 sgs__AppStop(struct soap*, struct sgs__AppStopResponse *);

SOAP_FMAC1 int SOAP_FMAC2 sgs__GetControl(struct soap*, struct sgs__GetControlResponse *);

SOAP_FMAC1 int SOAP_FMAC2 sgs__GetStatus(struct soap*, struct sgs__GetStatusResponse *);

SOAP_FMAC1 int SOAP_FMAC2 sgs__Restart(struct soap*, char *, struct sgs__RestartResponse *);

SOAP_FMAC1 int SOAP_FMAC2 sgs__Attach(struct soap*, struct sgs__AttachResponse *);

SOAP_FMAC1 int SOAP_FMAC2 sgs__GetNthDataSource(struct soap*, int, struct sgs__GetNthDataSourceResponse *);

SOAP_FMAC1 int SOAP_FMAC2 sgs__Stop(struct soap*, struct sgs__StopResponse *);

SOAP_FMAC1 int SOAP_FMAC2 sgs__Detach(struct soap*, struct sgs__DetachResponse *);

SOAP_FMAC1 int SOAP_FMAC2 sgs__PutControl(struct soap*, char *, struct sgs__PutControlResponse *);

SOAP_FMAC1 int SOAP_FMAC2 sgs__findServiceData(struct soap*, char *, struct sgs__findServiceDataResponse *);

SOAP_FMAC1 int SOAP_FMAC2 sgs__AppRecordChkpoint(struct soap*, char *, char *, struct sgs__AppRecordChkpointResponse *);

SOAP_FMAC1 int SOAP_FMAC2 sgs__Pause(struct soap*, struct sgs__PauseResponse *);

SOAP_FMAC1 int SOAP_FMAC2 sgs__GetNotifications(struct soap*, struct sgs__GetNotificationsResponse *);

SOAP_FMAC1 int SOAP_FMAC2 sgs__AppStart(struct soap*, struct sgs__AppStartResponse *);

SOAP_FMAC1 int SOAP_FMAC2 sgs__Resume(struct soap*, struct sgs__ResumeResponse *);

SOAP_FMAC1 int SOAP_FMAC2 sgs__ClearStatusMsgQueue(struct soap*, struct sgs__ClearStatusMsgQueueResponse *);

SOAP_FMAC1 int SOAP_FMAC2 sgsf__registerSelf(struct soap*, char *, char *, struct sgsf__registerSelfResponse *);

SOAP_FMAC1 int SOAP_FMAC2 sgsf__destroy(struct soap*, struct sgsf__destroyResponse *);

SOAP_FMAC1 int SOAP_FMAC2 sgsf__createService(struct soap*, char *, char *, struct sgsf__createServiceResponse *);

SOAP_FMAC1 int SOAP_FMAC2 sgr__remove(struct soap*, char *, struct sgr__removeResponse *);

SOAP_FMAC1 int SOAP_FMAC2 sgr__add(struct soap*, char *, char *, char *, struct sgr__addResponse *);

SOAP_FMAC1 int SOAP_FMAC2 sgr__findServiceData(struct soap*, char *, struct sgr__findServiceDataResponse *);

SOAP_FMAC1 int SOAP_FMAC2 rgtf__createNewTree(struct soap*, char *, char *, char *, char *, struct rgtf__createNewTreeResponse *);

SOAP_FMAC1 int SOAP_FMAC2 rgtf__findByHandle(struct soap*, char *, char *, struct rgtf__findByHandleResponse *);

SOAP_FMAC1 int SOAP_FMAC2 rgtf__destroy(struct soap*, struct rgtf__destroyResponse *);

SOAP_FMAC1 int SOAP_FMAC2 rgtf__findServiceData(struct soap*, char *, struct rgtf__findServiceDataResponse *);

SOAP_FMAC1 int SOAP_FMAC2 rgtf__getActiveTrees(struct soap*, struct rgtf__getActiveTreesResponse *);

SOAP_FMAC1 int SOAP_FMAC2 rgtf__requestTerminationBefore(struct soap*, char *, struct rgtf__requestTerminationBeforeResponse *);

SOAP_FMAC1 int SOAP_FMAC2 rgtf__requestTerminationAfter(struct soap*, char *, struct rgtf__requestTerminationAfterResponse *);

SOAP_FMAC1 int SOAP_FMAC2 rgt__requestTerminationAfter(struct soap*, char *, struct rgt__requestTerminationAfterResponse *);

SOAP_FMAC1 int SOAP_FMAC2 rgt__getChildNodes(struct soap*, struct rgt__getChildNodesResponse *);

SOAP_FMAC1 int SOAP_FMAC2 rgt__getInputFile(struct soap*, struct rgt__getInputFileResponse *);

SOAP_FMAC1 int SOAP_FMAC2 rgt__findServiceData(struct soap*, char *, struct rgt__findServiceDataResponse *);

SOAP_FMAC1 int SOAP_FMAC2 rgt__setCheckPointData(struct soap*, struct rgt__setCheckPointDataResponse *);

SOAP_FMAC1 int SOAP_FMAC2 rgt__destroy(struct soap*, struct rgt__destroyResponse *);

SOAP_FMAC1 int SOAP_FMAC2 rgt__getSteeringCommands(struct soap*, struct rgt__getSteeringCommandsResponse *);

SOAP_FMAC1 int SOAP_FMAC2 rgt__requestTerminationBefore(struct soap*, char *, struct rgt__requestTerminationBeforeResponse *);

SOAP_FMAC1 int SOAP_FMAC2 rgt__getCheckPointData(struct soap*, struct rgt__getCheckPointDataResponse *);

SOAP_FMAC1 int SOAP_FMAC2 rgt__getParentNode(struct soap*, struct rgt__getParentNodeResponse *);

SOAP_FMAC1 int SOAP_FMAC2 rgt__addNode(struct soap*, char *, char *, char *, char *, char *, struct rgt__addNodeResponse *);

/* Stubs */

SOAP_FMAC1 int SOAP_FMAC2 soap_call_sgs__setServiceData(struct soap*, const char*, const char*, char *, struct sgs__setServiceDataResponse *);

SOAP_FMAC1 int SOAP_FMAC2 soap_call_sgs__destroy(struct soap*, const char*, const char*, struct sgs__destroyResponse *);

SOAP_FMAC1 int SOAP_FMAC2 soap_call_sgs__AppDetach(struct soap*, const char*, const char*, struct sgs__AppDetachResponse *);

SOAP_FMAC1 int SOAP_FMAC2 soap_call_sgs__PutStatus(struct soap*, const char*, const char*, char *, struct sgs__PutStatusResponse *);

SOAP_FMAC1 int SOAP_FMAC2 soap_call_sgs__AppStop(struct soap*, const char*, const char*, struct sgs__AppStopResponse *);

SOAP_FMAC1 int SOAP_FMAC2 soap_call_sgs__GetControl(struct soap*, const char*, const char*, struct sgs__GetControlResponse *);

SOAP_FMAC1 int SOAP_FMAC2 soap_call_sgs__GetStatus(struct soap*, const char*, const char*, struct sgs__GetStatusResponse *);

SOAP_FMAC1 int SOAP_FMAC2 soap_call_sgs__Restart(struct soap*, const char*, const char*, char *, struct sgs__RestartResponse *);

SOAP_FMAC1 int SOAP_FMAC2 soap_call_sgs__Attach(struct soap*, const char*, const char*, struct sgs__AttachResponse *);

SOAP_FMAC1 int SOAP_FMAC2 soap_call_sgs__GetNthDataSource(struct soap*, const char*, const char*, int, struct sgs__GetNthDataSourceResponse *);

SOAP_FMAC1 int SOAP_FMAC2 soap_call_sgs__Stop(struct soap*, const char*, const char*, struct sgs__StopResponse *);

SOAP_FMAC1 int SOAP_FMAC2 soap_call_sgs__Detach(struct soap*, const char*, const char*, struct sgs__DetachResponse *);

SOAP_FMAC1 int SOAP_FMAC2 soap_call_sgs__PutControl(struct soap*, const char*, const char*, char *, struct sgs__PutControlResponse *);

SOAP_FMAC1 int SOAP_FMAC2 soap_call_sgs__findServiceData(struct soap*, const char*, const char*, char *, struct sgs__findServiceDataResponse *);

SOAP_FMAC1 int SOAP_FMAC2 soap_call_sgs__AppRecordChkpoint(struct soap*, const char*, const char*, char *, char *, struct sgs__AppRecordChkpointResponse *);

SOAP_FMAC1 int SOAP_FMAC2 soap_call_sgs__Pause(struct soap*, const char*, const char*, struct sgs__PauseResponse *);

SOAP_FMAC1 int SOAP_FMAC2 soap_call_sgs__GetNotifications(struct soap*, const char*, const char*, struct sgs__GetNotificationsResponse *);

SOAP_FMAC1 int SOAP_FMAC2 soap_call_sgs__AppStart(struct soap*, const char*, const char*, struct sgs__AppStartResponse *);

SOAP_FMAC1 int SOAP_FMAC2 soap_call_sgs__Resume(struct soap*, const char*, const char*, struct sgs__ResumeResponse *);

SOAP_FMAC1 int SOAP_FMAC2 soap_call_sgs__ClearStatusMsgQueue(struct soap*, const char*, const char*, struct sgs__ClearStatusMsgQueueResponse *);

SOAP_FMAC1 int SOAP_FMAC2 soap_call_sgsf__registerSelf(struct soap*, const char*, const char*, char *, char *, struct sgsf__registerSelfResponse *);

SOAP_FMAC1 int SOAP_FMAC2 soap_call_sgsf__destroy(struct soap*, const char*, const char*, struct sgsf__destroyResponse *);

SOAP_FMAC1 int SOAP_FMAC2 soap_call_sgsf__createService(struct soap*, const char*, const char*, char *, char *, struct sgsf__createServiceResponse *);

SOAP_FMAC1 int SOAP_FMAC2 soap_call_sgr__remove(struct soap*, const char*, const char*, char *, struct sgr__removeResponse *);

SOAP_FMAC1 int SOAP_FMAC2 soap_call_sgr__add(struct soap*, const char*, const char*, char *, char *, char *, struct sgr__addResponse *);

SOAP_FMAC1 int SOAP_FMAC2 soap_call_sgr__findServiceData(struct soap*, const char*, const char*, char *, struct sgr__findServiceDataResponse *);

SOAP_FMAC1 int SOAP_FMAC2 soap_call_rgtf__createNewTree(struct soap*, const char*, const char*, char *, char *, char *, char *, struct rgtf__createNewTreeResponse *);

SOAP_FMAC1 int SOAP_FMAC2 soap_call_rgtf__findByHandle(struct soap*, const char*, const char*, char *, char *, struct rgtf__findByHandleResponse *);

SOAP_FMAC1 int SOAP_FMAC2 soap_call_rgtf__destroy(struct soap*, const char*, const char*, struct rgtf__destroyResponse *);

SOAP_FMAC1 int SOAP_FMAC2 soap_call_rgtf__findServiceData(struct soap*, const char*, const char*, char *, struct rgtf__findServiceDataResponse *);

SOAP_FMAC1 int SOAP_FMAC2 soap_call_rgtf__getActiveTrees(struct soap*, const char*, const char*, struct rgtf__getActiveTreesResponse *);

SOAP_FMAC1 int SOAP_FMAC2 soap_call_rgtf__requestTerminationBefore(struct soap*, const char*, const char*, char *, struct rgtf__requestTerminationBeforeResponse *);

SOAP_FMAC1 int SOAP_FMAC2 soap_call_rgtf__requestTerminationAfter(struct soap*, const char*, const char*, char *, struct rgtf__requestTerminationAfterResponse *);

SOAP_FMAC1 int SOAP_FMAC2 soap_call_rgt__requestTerminationAfter(struct soap*, const char*, const char*, char *, struct rgt__requestTerminationAfterResponse *);

SOAP_FMAC1 int SOAP_FMAC2 soap_call_rgt__getChildNodes(struct soap*, const char*, const char*, struct rgt__getChildNodesResponse *);

SOAP_FMAC1 int SOAP_FMAC2 soap_call_rgt__getInputFile(struct soap*, const char*, const char*, struct rgt__getInputFileResponse *);

SOAP_FMAC1 int SOAP_FMAC2 soap_call_rgt__findServiceData(struct soap*, const char*, const char*, char *, struct rgt__findServiceDataResponse *);

SOAP_FMAC1 int SOAP_FMAC2 soap_call_rgt__setCheckPointData(struct soap*, const char*, const char*, struct rgt__setCheckPointDataResponse *);

SOAP_FMAC1 int SOAP_FMAC2 soap_call_rgt__destroy(struct soap*, const char*, const char*, struct rgt__destroyResponse *);

SOAP_FMAC1 int SOAP_FMAC2 soap_call_rgt__getSteeringCommands(struct soap*, const char*, const char*, struct rgt__getSteeringCommandsResponse *);

SOAP_FMAC1 int SOAP_FMAC2 soap_call_rgt__requestTerminationBefore(struct soap*, const char*, const char*, char *, struct rgt__requestTerminationBeforeResponse *);

SOAP_FMAC1 int SOAP_FMAC2 soap_call_rgt__getCheckPointData(struct soap*, const char*, const char*, struct rgt__getCheckPointDataResponse *);

SOAP_FMAC1 int SOAP_FMAC2 soap_call_rgt__getParentNode(struct soap*, const char*, const char*, struct rgt__getParentNodeResponse *);

SOAP_FMAC1 int SOAP_FMAC2 soap_call_rgt__addNode(struct soap*, const char*, const char*, char *, char *, char *, char *, char *, struct rgt__addNodeResponse *);

/* Skeletons */

SOAP_FMAC1 int SOAP_FMAC2 soap_serve(struct soap*);

SOAP_FMAC1 int SOAP_FMAC2 soap_serve_sgs__setServiceData(struct soap*);

SOAP_FMAC1 int SOAP_FMAC2 soap_serve_sgs__destroy(struct soap*);

SOAP_FMAC1 int SOAP_FMAC2 soap_serve_sgs__AppDetach(struct soap*);

SOAP_FMAC1 int SOAP_FMAC2 soap_serve_sgs__PutStatus(struct soap*);

SOAP_FMAC1 int SOAP_FMAC2 soap_serve_sgs__AppStop(struct soap*);

SOAP_FMAC1 int SOAP_FMAC2 soap_serve_sgs__GetControl(struct soap*);

SOAP_FMAC1 int SOAP_FMAC2 soap_serve_sgs__GetStatus(struct soap*);

SOAP_FMAC1 int SOAP_FMAC2 soap_serve_sgs__Restart(struct soap*);

SOAP_FMAC1 int SOAP_FMAC2 soap_serve_sgs__Attach(struct soap*);

SOAP_FMAC1 int SOAP_FMAC2 soap_serve_sgs__GetNthDataSource(struct soap*);

SOAP_FMAC1 int SOAP_FMAC2 soap_serve_sgs__Stop(struct soap*);

SOAP_FMAC1 int SOAP_FMAC2 soap_serve_sgs__Detach(struct soap*);

SOAP_FMAC1 int SOAP_FMAC2 soap_serve_sgs__PutControl(struct soap*);

SOAP_FMAC1 int SOAP_FMAC2 soap_serve_sgs__findServiceData(struct soap*);

SOAP_FMAC1 int SOAP_FMAC2 soap_serve_sgs__AppRecordChkpoint(struct soap*);

SOAP_FMAC1 int SOAP_FMAC2 soap_serve_sgs__Pause(struct soap*);

SOAP_FMAC1 int SOAP_FMAC2 soap_serve_sgs__GetNotifications(struct soap*);

SOAP_FMAC1 int SOAP_FMAC2 soap_serve_sgs__AppStart(struct soap*);

SOAP_FMAC1 int SOAP_FMAC2 soap_serve_sgs__Resume(struct soap*);

SOAP_FMAC1 int SOAP_FMAC2 soap_serve_sgs__ClearStatusMsgQueue(struct soap*);

SOAP_FMAC1 int SOAP_FMAC2 soap_serve_sgsf__registerSelf(struct soap*);

SOAP_FMAC1 int SOAP_FMAC2 soap_serve_sgsf__destroy(struct soap*);

SOAP_FMAC1 int SOAP_FMAC2 soap_serve_sgsf__createService(struct soap*);

SOAP_FMAC1 int SOAP_FMAC2 soap_serve_sgr__remove(struct soap*);

SOAP_FMAC1 int SOAP_FMAC2 soap_serve_sgr__add(struct soap*);

SOAP_FMAC1 int SOAP_FMAC2 soap_serve_sgr__findServiceData(struct soap*);

SOAP_FMAC1 int SOAP_FMAC2 soap_serve_rgtf__createNewTree(struct soap*);

SOAP_FMAC1 int SOAP_FMAC2 soap_serve_rgtf__findByHandle(struct soap*);

SOAP_FMAC1 int SOAP_FMAC2 soap_serve_rgtf__destroy(struct soap*);

SOAP_FMAC1 int SOAP_FMAC2 soap_serve_rgtf__findServiceData(struct soap*);

SOAP_FMAC1 int SOAP_FMAC2 soap_serve_rgtf__getActiveTrees(struct soap*);

SOAP_FMAC1 int SOAP_FMAC2 soap_serve_rgtf__requestTerminationBefore(struct soap*);

SOAP_FMAC1 int SOAP_FMAC2 soap_serve_rgtf__requestTerminationAfter(struct soap*);

SOAP_FMAC1 int SOAP_FMAC2 soap_serve_rgt__requestTerminationAfter(struct soap*);

SOAP_FMAC1 int SOAP_FMAC2 soap_serve_rgt__getChildNodes(struct soap*);

SOAP_FMAC1 int SOAP_FMAC2 soap_serve_rgt__getInputFile(struct soap*);

SOAP_FMAC1 int SOAP_FMAC2 soap_serve_rgt__findServiceData(struct soap*);

SOAP_FMAC1 int SOAP_FMAC2 soap_serve_rgt__setCheckPointData(struct soap*);

SOAP_FMAC1 int SOAP_FMAC2 soap_serve_rgt__destroy(struct soap*);

SOAP_FMAC1 int SOAP_FMAC2 soap_serve_rgt__getSteeringCommands(struct soap*);

SOAP_FMAC1 int SOAP_FMAC2 soap_serve_rgt__requestTerminationBefore(struct soap*);

SOAP_FMAC1 int SOAP_FMAC2 soap_serve_rgt__getCheckPointData(struct soap*);

SOAP_FMAC1 int SOAP_FMAC2 soap_serve_rgt__getParentNode(struct soap*);

SOAP_FMAC1 int SOAP_FMAC2 soap_serve_rgt__addNode(struct soap*);
#ifdef __cplusplus
}
#endif
#endif

/* end of soapStub.h */
