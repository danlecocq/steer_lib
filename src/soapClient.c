/* soapClient.c
   Generated by gSOAP 2.2.3b from SGS.h
   Copyright (C) 2001-2003 Genivia inc.
   All Rights Reserved.
*/
#include "soapH.h"
#ifdef __cplusplus
extern "C" {
#endif

SOAP_SOURCE_STAMP("@(#) soapClient.c ver 2.2.3b 2003-10-03 15:57:22 GMT")


SOAP_FMAC1 int SOAP_FMAC2 soap_call_tns__AppDetach(struct soap *soap, const char *URL, const char *action, struct tns__AppDetachResponse *out)
{
	struct tns__AppDetach soap_tmp_tns__AppDetach;
	if (!action)
		action = "";
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize_tns__AppDetach(soap, &soap_tmp_tns__AppDetach);
	soap_begin_count(soap);
	if (soap->mode & SOAP_IO_LENGTH)
	{	soap_envelope_begin_out(soap);
		soap_putheader(soap);
		soap_body_begin_out(soap);
		soap_put_tns__AppDetach(soap, &soap_tmp_tns__AppDetach, "tns:AppDetach", "");
		soap_body_end_out(soap);
		soap_envelope_end_out(soap);
	}
	if (soap_connect(soap, URL, action)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_tns__AppDetach(soap, &soap_tmp_tns__AppDetach, "tns:AppDetach", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_putattachments(soap)
	 || soap_end_send(soap))
		return soap->error;
	soap_default_tns__AppDetachResponse(soap, out);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap->error;
	soap_get_tns__AppDetachResponse(soap, out, "tns:AppDetachResponse", "tns:AppDetachResponse");
	if (soap->error)
	{	if (soap->error == SOAP_TAG_MISMATCH && soap->level == 2)
			soap_recv_fault(soap);
		return soap->error;
	}
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_getattachments(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap_closesock(soap);
	return SOAP_OK;
}

SOAP_FMAC1 int SOAP_FMAC2 soap_call_tns__Attach(struct soap *soap, const char *URL, const char *action, struct tns__AttachResponse *out)
{
	struct tns__Attach soap_tmp_tns__Attach;
	if (!action)
		action = "";
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize_tns__Attach(soap, &soap_tmp_tns__Attach);
	soap_begin_count(soap);
	if (soap->mode & SOAP_IO_LENGTH)
	{	soap_envelope_begin_out(soap);
		soap_putheader(soap);
		soap_body_begin_out(soap);
		soap_put_tns__Attach(soap, &soap_tmp_tns__Attach, "tns:Attach", "");
		soap_body_end_out(soap);
		soap_envelope_end_out(soap);
	}
	if (soap_connect(soap, URL, action)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_tns__Attach(soap, &soap_tmp_tns__Attach, "tns:Attach", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_putattachments(soap)
	 || soap_end_send(soap))
		return soap->error;
	soap_default_tns__AttachResponse(soap, out);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap->error;
	soap_get_tns__AttachResponse(soap, out, "tns:AttachResponse", "tns:AttachResponse");
	if (soap->error)
	{	if (soap->error == SOAP_TAG_MISMATCH && soap->level == 2)
			soap_recv_fault(soap);
		return soap->error;
	}
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_getattachments(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap_closesock(soap);
	return SOAP_OK;
}

SOAP_FMAC1 int SOAP_FMAC2 soap_call_tns__findServiceData(struct soap *soap, const char *URL, const char *action, char *input, struct tns__findServiceDataResponse *out)
{
	struct tns__findServiceData soap_tmp_tns__findServiceData;
	if (!action)
		action = "";
	soap_tmp_tns__findServiceData.input=input;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize_tns__findServiceData(soap, &soap_tmp_tns__findServiceData);
	soap_begin_count(soap);
	if (soap->mode & SOAP_IO_LENGTH)
	{	soap_envelope_begin_out(soap);
		soap_putheader(soap);
		soap_body_begin_out(soap);
		soap_put_tns__findServiceData(soap, &soap_tmp_tns__findServiceData, "tns:findServiceData", "");
		soap_body_end_out(soap);
		soap_envelope_end_out(soap);
	}
	if (soap_connect(soap, URL, action)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_tns__findServiceData(soap, &soap_tmp_tns__findServiceData, "tns:findServiceData", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_putattachments(soap)
	 || soap_end_send(soap))
		return soap->error;
	soap_default_tns__findServiceDataResponse(soap, out);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap->error;
	soap_get_tns__findServiceDataResponse(soap, out, "tns:findServiceDataResponse", "tns:findServiceDataResponse");
	if (soap->error)
	{	if (soap->error == SOAP_TAG_MISMATCH && soap->level == 2)
			soap_recv_fault(soap);
		return soap->error;
	}
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_getattachments(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap_closesock(soap);
	return SOAP_OK;
}

SOAP_FMAC1 int SOAP_FMAC2 soap_call_tns__AppRecordChkpoint(struct soap *soap, const char *URL, const char *action, char *chk_USCORE_meta_USCORE_data, char *node_USCORE_meta_USCORE_data, struct tns__AppRecordChkpointResponse *out)
{
	struct tns__AppRecordChkpoint soap_tmp_tns__AppRecordChkpoint;
	if (!action)
		action = "";
	soap_tmp_tns__AppRecordChkpoint.chk_USCORE_meta_USCORE_data=chk_USCORE_meta_USCORE_data;
	soap_tmp_tns__AppRecordChkpoint.node_USCORE_meta_USCORE_data=node_USCORE_meta_USCORE_data;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize_tns__AppRecordChkpoint(soap, &soap_tmp_tns__AppRecordChkpoint);
	soap_begin_count(soap);
	if (soap->mode & SOAP_IO_LENGTH)
	{	soap_envelope_begin_out(soap);
		soap_putheader(soap);
		soap_body_begin_out(soap);
		soap_put_tns__AppRecordChkpoint(soap, &soap_tmp_tns__AppRecordChkpoint, "tns:AppRecordChkpoint", "");
		soap_body_end_out(soap);
		soap_envelope_end_out(soap);
	}
	if (soap_connect(soap, URL, action)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_tns__AppRecordChkpoint(soap, &soap_tmp_tns__AppRecordChkpoint, "tns:AppRecordChkpoint", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_putattachments(soap)
	 || soap_end_send(soap))
		return soap->error;
	soap_default_tns__AppRecordChkpointResponse(soap, out);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap->error;
	soap_get_tns__AppRecordChkpointResponse(soap, out, "tns:AppRecordChkpointResponse", "tns:AppRecordChkpointResponse");
	if (soap->error)
	{	if (soap->error == SOAP_TAG_MISMATCH && soap->level == 2)
			soap_recv_fault(soap);
		return soap->error;
	}
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_getattachments(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap_closesock(soap);
	return SOAP_OK;
}

SOAP_FMAC1 int SOAP_FMAC2 soap_call_tns__setServiceData(struct soap *soap, const char *URL, const char *action, char *input, struct tns__setServiceDataResponse *out)
{
	struct tns__setServiceData soap_tmp_tns__setServiceData;
	if (!action)
		action = "";
	soap_tmp_tns__setServiceData.input=input;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize_tns__setServiceData(soap, &soap_tmp_tns__setServiceData);
	soap_begin_count(soap);
	if (soap->mode & SOAP_IO_LENGTH)
	{	soap_envelope_begin_out(soap);
		soap_putheader(soap);
		soap_body_begin_out(soap);
		soap_put_tns__setServiceData(soap, &soap_tmp_tns__setServiceData, "tns:setServiceData", "");
		soap_body_end_out(soap);
		soap_envelope_end_out(soap);
	}
	if (soap_connect(soap, URL, action)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_tns__setServiceData(soap, &soap_tmp_tns__setServiceData, "tns:setServiceData", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_putattachments(soap)
	 || soap_end_send(soap))
		return soap->error;
	soap_default_tns__setServiceDataResponse(soap, out);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap->error;
	soap_get_tns__setServiceDataResponse(soap, out, "tns:setServiceDataResponse", "tns:setServiceDataResponse");
	if (soap->error)
	{	if (soap->error == SOAP_TAG_MISMATCH && soap->level == 2)
			soap_recv_fault(soap);
		return soap->error;
	}
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_getattachments(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap_closesock(soap);
	return SOAP_OK;
}

SOAP_FMAC1 int SOAP_FMAC2 soap_call_tns__Detach(struct soap *soap, const char *URL, const char *action, struct tns__DetachResponse *out)
{
	struct tns__Detach soap_tmp_tns__Detach;
	if (!action)
		action = "";
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize_tns__Detach(soap, &soap_tmp_tns__Detach);
	soap_begin_count(soap);
	if (soap->mode & SOAP_IO_LENGTH)
	{	soap_envelope_begin_out(soap);
		soap_putheader(soap);
		soap_body_begin_out(soap);
		soap_put_tns__Detach(soap, &soap_tmp_tns__Detach, "tns:Detach", "");
		soap_body_end_out(soap);
		soap_envelope_end_out(soap);
	}
	if (soap_connect(soap, URL, action)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_tns__Detach(soap, &soap_tmp_tns__Detach, "tns:Detach", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_putattachments(soap)
	 || soap_end_send(soap))
		return soap->error;
	soap_default_tns__DetachResponse(soap, out);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap->error;
	soap_get_tns__DetachResponse(soap, out, "tns:DetachResponse", "tns:DetachResponse");
	if (soap->error)
	{	if (soap->error == SOAP_TAG_MISMATCH && soap->level == 2)
			soap_recv_fault(soap);
		return soap->error;
	}
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_getattachments(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap_closesock(soap);
	return SOAP_OK;
}

SOAP_FMAC1 int SOAP_FMAC2 soap_call_tns__GetStatus(struct soap *soap, const char *URL, const char *action, struct tns__GetStatusResponse *out)
{
	struct tns__GetStatus soap_tmp_tns__GetStatus;
	if (!action)
		action = "";
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize_tns__GetStatus(soap, &soap_tmp_tns__GetStatus);
	soap_begin_count(soap);
	if (soap->mode & SOAP_IO_LENGTH)
	{	soap_envelope_begin_out(soap);
		soap_putheader(soap);
		soap_body_begin_out(soap);
		soap_put_tns__GetStatus(soap, &soap_tmp_tns__GetStatus, "tns:GetStatus", "");
		soap_body_end_out(soap);
		soap_envelope_end_out(soap);
	}
	if (soap_connect(soap, URL, action)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_tns__GetStatus(soap, &soap_tmp_tns__GetStatus, "tns:GetStatus", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_putattachments(soap)
	 || soap_end_send(soap))
		return soap->error;
	soap_default_tns__GetStatusResponse(soap, out);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap->error;
	soap_get_tns__GetStatusResponse(soap, out, "tns:GetStatusResponse", "tns:GetStatusResponse");
	if (soap->error)
	{	if (soap->error == SOAP_TAG_MISMATCH && soap->level == 2)
			soap_recv_fault(soap);
		return soap->error;
	}
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_getattachments(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap_closesock(soap);
	return SOAP_OK;
}

SOAP_FMAC1 int SOAP_FMAC2 soap_call_tns__GetNthDataSource(struct soap *soap, const char *URL, const char *action, char *input, struct tns__GetNthDataSourceResponse *out)
{
	struct tns__GetNthDataSource soap_tmp_tns__GetNthDataSource;
	if (!action)
		action = "";
	soap_tmp_tns__GetNthDataSource.input=input;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize_tns__GetNthDataSource(soap, &soap_tmp_tns__GetNthDataSource);
	soap_begin_count(soap);
	if (soap->mode & SOAP_IO_LENGTH)
	{	soap_envelope_begin_out(soap);
		soap_putheader(soap);
		soap_body_begin_out(soap);
		soap_put_tns__GetNthDataSource(soap, &soap_tmp_tns__GetNthDataSource, "tns:GetNthDataSource", "");
		soap_body_end_out(soap);
		soap_envelope_end_out(soap);
	}
	if (soap_connect(soap, URL, action)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_tns__GetNthDataSource(soap, &soap_tmp_tns__GetNthDataSource, "tns:GetNthDataSource", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_putattachments(soap)
	 || soap_end_send(soap))
		return soap->error;
	soap_default_tns__GetNthDataSourceResponse(soap, out);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap->error;
	soap_get_tns__GetNthDataSourceResponse(soap, out, "tns:GetNthDataSourceResponse", "tns:GetNthDataSourceResponse");
	if (soap->error)
	{	if (soap->error == SOAP_TAG_MISMATCH && soap->level == 2)
			soap_recv_fault(soap);
		return soap->error;
	}
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_getattachments(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap_closesock(soap);
	return SOAP_OK;
}

SOAP_FMAC1 int SOAP_FMAC2 soap_call_tns__AppStop(struct soap *soap, const char *URL, const char *action, struct tns__AppStopResponse *out)
{
	struct tns__AppStop soap_tmp_tns__AppStop;
	if (!action)
		action = "";
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize_tns__AppStop(soap, &soap_tmp_tns__AppStop);
	soap_begin_count(soap);
	if (soap->mode & SOAP_IO_LENGTH)
	{	soap_envelope_begin_out(soap);
		soap_putheader(soap);
		soap_body_begin_out(soap);
		soap_put_tns__AppStop(soap, &soap_tmp_tns__AppStop, "tns:AppStop", "");
		soap_body_end_out(soap);
		soap_envelope_end_out(soap);
	}
	if (soap_connect(soap, URL, action)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_tns__AppStop(soap, &soap_tmp_tns__AppStop, "tns:AppStop", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_putattachments(soap)
	 || soap_end_send(soap))
		return soap->error;
	soap_default_tns__AppStopResponse(soap, out);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap->error;
	soap_get_tns__AppStopResponse(soap, out, "tns:AppStopResponse", "tns:AppStopResponse");
	if (soap->error)
	{	if (soap->error == SOAP_TAG_MISMATCH && soap->level == 2)
			soap_recv_fault(soap);
		return soap->error;
	}
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_getattachments(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap_closesock(soap);
	return SOAP_OK;
}

SOAP_FMAC1 int SOAP_FMAC2 soap_call_tns__Resume(struct soap *soap, const char *URL, const char *action, struct tns__ResumeResponse *out)
{
	struct tns__Resume soap_tmp_tns__Resume;
	if (!action)
		action = "";
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize_tns__Resume(soap, &soap_tmp_tns__Resume);
	soap_begin_count(soap);
	if (soap->mode & SOAP_IO_LENGTH)
	{	soap_envelope_begin_out(soap);
		soap_putheader(soap);
		soap_body_begin_out(soap);
		soap_put_tns__Resume(soap, &soap_tmp_tns__Resume, "tns:Resume", "");
		soap_body_end_out(soap);
		soap_envelope_end_out(soap);
	}
	if (soap_connect(soap, URL, action)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_tns__Resume(soap, &soap_tmp_tns__Resume, "tns:Resume", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_putattachments(soap)
	 || soap_end_send(soap))
		return soap->error;
	soap_default_tns__ResumeResponse(soap, out);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap->error;
	soap_get_tns__ResumeResponse(soap, out, "tns:ResumeResponse", "tns:ResumeResponse");
	if (soap->error)
	{	if (soap->error == SOAP_TAG_MISMATCH && soap->level == 2)
			soap_recv_fault(soap);
		return soap->error;
	}
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_getattachments(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap_closesock(soap);
	return SOAP_OK;
}

SOAP_FMAC1 int SOAP_FMAC2 soap_call_tns__GetControl(struct soap *soap, const char *URL, const char *action, struct tns__GetControlResponse *out)
{
	struct tns__GetControl soap_tmp_tns__GetControl;
	if (!action)
		action = "";
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize_tns__GetControl(soap, &soap_tmp_tns__GetControl);
	soap_begin_count(soap);
	if (soap->mode & SOAP_IO_LENGTH)
	{	soap_envelope_begin_out(soap);
		soap_putheader(soap);
		soap_body_begin_out(soap);
		soap_put_tns__GetControl(soap, &soap_tmp_tns__GetControl, "tns:GetControl", "");
		soap_body_end_out(soap);
		soap_envelope_end_out(soap);
	}
	if (soap_connect(soap, URL, action)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_tns__GetControl(soap, &soap_tmp_tns__GetControl, "tns:GetControl", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_putattachments(soap)
	 || soap_end_send(soap))
		return soap->error;
	soap_default_tns__GetControlResponse(soap, out);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap->error;
	soap_get_tns__GetControlResponse(soap, out, "tns:GetControlResponse", "tns:GetControlResponse");
	if (soap->error)
	{	if (soap->error == SOAP_TAG_MISMATCH && soap->level == 2)
			soap_recv_fault(soap);
		return soap->error;
	}
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_getattachments(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap_closesock(soap);
	return SOAP_OK;
}

SOAP_FMAC1 int SOAP_FMAC2 soap_call_tns__Stop(struct soap *soap, const char *URL, const char *action, struct tns__StopResponse *out)
{
	struct tns__Stop soap_tmp_tns__Stop;
	if (!action)
		action = "";
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize_tns__Stop(soap, &soap_tmp_tns__Stop);
	soap_begin_count(soap);
	if (soap->mode & SOAP_IO_LENGTH)
	{	soap_envelope_begin_out(soap);
		soap_putheader(soap);
		soap_body_begin_out(soap);
		soap_put_tns__Stop(soap, &soap_tmp_tns__Stop, "tns:Stop", "");
		soap_body_end_out(soap);
		soap_envelope_end_out(soap);
	}
	if (soap_connect(soap, URL, action)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_tns__Stop(soap, &soap_tmp_tns__Stop, "tns:Stop", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_putattachments(soap)
	 || soap_end_send(soap))
		return soap->error;
	soap_default_tns__StopResponse(soap, out);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap->error;
	soap_get_tns__StopResponse(soap, out, "tns:StopResponse", "tns:StopResponse");
	if (soap->error)
	{	if (soap->error == SOAP_TAG_MISMATCH && soap->level == 2)
			soap_recv_fault(soap);
		return soap->error;
	}
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_getattachments(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap_closesock(soap);
	return SOAP_OK;
}

SOAP_FMAC1 int SOAP_FMAC2 soap_call_tns__GetNotifications(struct soap *soap, const char *URL, const char *action, struct tns__GetNotificationsResponse *out)
{
	struct tns__GetNotifications soap_tmp_tns__GetNotifications;
	if (!action)
		action = "";
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize_tns__GetNotifications(soap, &soap_tmp_tns__GetNotifications);
	soap_begin_count(soap);
	if (soap->mode & SOAP_IO_LENGTH)
	{	soap_envelope_begin_out(soap);
		soap_putheader(soap);
		soap_body_begin_out(soap);
		soap_put_tns__GetNotifications(soap, &soap_tmp_tns__GetNotifications, "tns:GetNotifications", "");
		soap_body_end_out(soap);
		soap_envelope_end_out(soap);
	}
	if (soap_connect(soap, URL, action)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_tns__GetNotifications(soap, &soap_tmp_tns__GetNotifications, "tns:GetNotifications", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_putattachments(soap)
	 || soap_end_send(soap))
		return soap->error;
	soap_default_tns__GetNotificationsResponse(soap, out);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap->error;
	soap_get_tns__GetNotificationsResponse(soap, out, "tns:GetNotificationsResponse", "tns:GetNotificationsResponse");
	if (soap->error)
	{	if (soap->error == SOAP_TAG_MISMATCH && soap->level == 2)
			soap_recv_fault(soap);
		return soap->error;
	}
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_getattachments(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap_closesock(soap);
	return SOAP_OK;
}

SOAP_FMAC1 int SOAP_FMAC2 soap_call_tns__Pause(struct soap *soap, const char *URL, const char *action, struct tns__PauseResponse *out)
{
	struct tns__Pause soap_tmp_tns__Pause;
	if (!action)
		action = "";
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize_tns__Pause(soap, &soap_tmp_tns__Pause);
	soap_begin_count(soap);
	if (soap->mode & SOAP_IO_LENGTH)
	{	soap_envelope_begin_out(soap);
		soap_putheader(soap);
		soap_body_begin_out(soap);
		soap_put_tns__Pause(soap, &soap_tmp_tns__Pause, "tns:Pause", "");
		soap_body_end_out(soap);
		soap_envelope_end_out(soap);
	}
	if (soap_connect(soap, URL, action)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_tns__Pause(soap, &soap_tmp_tns__Pause, "tns:Pause", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_putattachments(soap)
	 || soap_end_send(soap))
		return soap->error;
	soap_default_tns__PauseResponse(soap, out);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap->error;
	soap_get_tns__PauseResponse(soap, out, "tns:PauseResponse", "tns:PauseResponse");
	if (soap->error)
	{	if (soap->error == SOAP_TAG_MISMATCH && soap->level == 2)
			soap_recv_fault(soap);
		return soap->error;
	}
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_getattachments(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap_closesock(soap);
	return SOAP_OK;
}

SOAP_FMAC1 int SOAP_FMAC2 soap_call_tns__PutStatus(struct soap *soap, const char *URL, const char *action, char *input, struct tns__PutStatusResponse *out)
{
	struct tns__PutStatus soap_tmp_tns__PutStatus;
	if (!action)
		action = "";
	soap_tmp_tns__PutStatus.input=input;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize_tns__PutStatus(soap, &soap_tmp_tns__PutStatus);
	soap_begin_count(soap);
	if (soap->mode & SOAP_IO_LENGTH)
	{	soap_envelope_begin_out(soap);
		soap_putheader(soap);
		soap_body_begin_out(soap);
		soap_put_tns__PutStatus(soap, &soap_tmp_tns__PutStatus, "tns:PutStatus", "");
		soap_body_end_out(soap);
		soap_envelope_end_out(soap);
	}
	if (soap_connect(soap, URL, action)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_tns__PutStatus(soap, &soap_tmp_tns__PutStatus, "tns:PutStatus", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_putattachments(soap)
	 || soap_end_send(soap))
		return soap->error;
	soap_default_tns__PutStatusResponse(soap, out);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap->error;
	soap_get_tns__PutStatusResponse(soap, out, "tns:PutStatusResponse", "tns:PutStatusResponse");
	if (soap->error)
	{	if (soap->error == SOAP_TAG_MISMATCH && soap->level == 2)
			soap_recv_fault(soap);
		return soap->error;
	}
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_getattachments(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap_closesock(soap);
	return SOAP_OK;
}

SOAP_FMAC1 int SOAP_FMAC2 soap_call_tns__AppStart(struct soap *soap, const char *URL, const char *action, struct tns__AppStartResponse *out)
{
	struct tns__AppStart soap_tmp_tns__AppStart;
	if (!action)
		action = "";
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize_tns__AppStart(soap, &soap_tmp_tns__AppStart);
	soap_begin_count(soap);
	if (soap->mode & SOAP_IO_LENGTH)
	{	soap_envelope_begin_out(soap);
		soap_putheader(soap);
		soap_body_begin_out(soap);
		soap_put_tns__AppStart(soap, &soap_tmp_tns__AppStart, "tns:AppStart", "");
		soap_body_end_out(soap);
		soap_envelope_end_out(soap);
	}
	if (soap_connect(soap, URL, action)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_tns__AppStart(soap, &soap_tmp_tns__AppStart, "tns:AppStart", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_putattachments(soap)
	 || soap_end_send(soap))
		return soap->error;
	soap_default_tns__AppStartResponse(soap, out);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap->error;
	soap_get_tns__AppStartResponse(soap, out, "tns:AppStartResponse", "tns:AppStartResponse");
	if (soap->error)
	{	if (soap->error == SOAP_TAG_MISMATCH && soap->level == 2)
			soap_recv_fault(soap);
		return soap->error;
	}
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_getattachments(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap_closesock(soap);
	return SOAP_OK;
}

SOAP_FMAC1 int SOAP_FMAC2 soap_call_tns__Destroy(struct soap *soap, const char *URL, const char *action, struct tns__DestroyResponse *out)
{
	struct tns__Destroy soap_tmp_tns__Destroy;
	if (!action)
		action = "";
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize_tns__Destroy(soap, &soap_tmp_tns__Destroy);
	soap_begin_count(soap);
	if (soap->mode & SOAP_IO_LENGTH)
	{	soap_envelope_begin_out(soap);
		soap_putheader(soap);
		soap_body_begin_out(soap);
		soap_put_tns__Destroy(soap, &soap_tmp_tns__Destroy, "tns:Destroy", "");
		soap_body_end_out(soap);
		soap_envelope_end_out(soap);
	}
	if (soap_connect(soap, URL, action)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_tns__Destroy(soap, &soap_tmp_tns__Destroy, "tns:Destroy", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_putattachments(soap)
	 || soap_end_send(soap))
		return soap->error;
	soap_default_tns__DestroyResponse(soap, out);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap->error;
	soap_get_tns__DestroyResponse(soap, out, "tns:DestroyResponse", "tns:DestroyResponse");
	if (soap->error)
	{	if (soap->error == SOAP_TAG_MISMATCH && soap->level == 2)
			soap_recv_fault(soap);
		return soap->error;
	}
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_getattachments(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap_closesock(soap);
	return SOAP_OK;
}

SOAP_FMAC1 int SOAP_FMAC2 soap_call_tns__PutControl(struct soap *soap, const char *URL, const char *action, char *input, struct tns__PutControlResponse *out)
{
	struct tns__PutControl soap_tmp_tns__PutControl;
	if (!action)
		action = "";
	soap_tmp_tns__PutControl.input=input;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize_tns__PutControl(soap, &soap_tmp_tns__PutControl);
	soap_begin_count(soap);
	if (soap->mode & SOAP_IO_LENGTH)
	{	soap_envelope_begin_out(soap);
		soap_putheader(soap);
		soap_body_begin_out(soap);
		soap_put_tns__PutControl(soap, &soap_tmp_tns__PutControl, "tns:PutControl", "");
		soap_body_end_out(soap);
		soap_envelope_end_out(soap);
	}
	if (soap_connect(soap, URL, action)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_tns__PutControl(soap, &soap_tmp_tns__PutControl, "tns:PutControl", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_putattachments(soap)
	 || soap_end_send(soap))
		return soap->error;
	soap_default_tns__PutControlResponse(soap, out);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap->error;
	soap_get_tns__PutControlResponse(soap, out, "tns:PutControlResponse", "tns:PutControlResponse");
	if (soap->error)
	{	if (soap->error == SOAP_TAG_MISMATCH && soap->level == 2)
			soap_recv_fault(soap);
		return soap->error;
	}
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_getattachments(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap_closesock(soap);
	return SOAP_OK;
}
#ifdef __cplusplus
}
#endif

/* end of soapClient.c */
