//#include "StdAfx.h"
#include "win_service.h"
#include "sc_manager.h"
#include "winsrv_helper.hpp"
#include <iostream>

namespace WinService
{


/************************************************************************
//  win_service
//  --------------------------------------------------------------------
//                                                                  
************************************************************************/


win_service::win_service(const std::string& svc_name,win_sc_manger& sc_mgr)
:svc_name_(svc_name),sc_mgr_(sc_mgr)
{
}
win_service::~win_service(void)
{
	//
}
void			win_service::copy_from(const win_service& rh)
{
	svc_name_=rh.svc_name_;
	cfg_=rh.cfg_;
	cfg_ex_=rh.cfg_ex_;
	proc_info_=rh.proc_info_;
	sc_mgr_=rh.sc_mgr_;
}
const svc_cfg_info&	win_service::config_info()const
{
	return cfg_;
}

svc_cfg_info&	win_service::config_info()
{
	return cfg_;
}

const svc_cfg_info_ex&		win_service::config_ex_info()const
{
	return cfg_ex_;
}
svc_cfg_info_ex&			win_service::config_ex_info()
{
	return cfg_ex_;
}
const srv_process_info&	win_service::proc_info()const
{
	return proc_info_;
}
srv_process_info&		win_service::proc_info()
{
	return proc_info_;
}

int		win_service::reload_cfg_info_all(bool stop_on_fail,std::string& what_wrong)
{
	int err = reload_general_cfg_info(what_wrong);
	if(sc_success_code() == err || !stop_on_fail) err = reload_more_cfg_info(stop_on_fail,what_wrong);
	return err;
}

int		win_service::reload_more_cfg_info(bool stop_on_fail,std::string& what_wrong)
{
	SC_HANDLE handle=NULL;
	int err=sc_mgr_.open_service(svc_name_,handle,what_wrong);
	if(sc_success_code() != err){
		what_wrong=get_sys_err_msg(err);
	}else{
		err = get_srv_cfg_delayed_auto_start_info(handle,cfg_ex_,what_wrong);
		if(sc_success_code() == err || !stop_on_fail) err = get_srv_cfg_description(handle,cfg_ex_,what_wrong);
		if(sc_success_code() == err || !stop_on_fail) err = get_srv_cfg_failure_action_list(handle,cfg_ex_,what_wrong);
		if(sc_success_code() == err || !stop_on_fail) err = get_srv_cfg_failure_actions_flag(handle,cfg_ex_,what_wrong);
		if(sc_success_code() == err || !stop_on_fail) err = get_srv_cfg_preshutdown_info(handle,cfg_ex_,what_wrong);
		if(sc_success_code() == err || !stop_on_fail) err = get_srv_cfg_required_privileges_info(handle,cfg_ex_,what_wrong);
		if(sc_success_code() == err || !stop_on_fail) err = get_srv_cfg_service_sid_info(handle,cfg_ex_,what_wrong);
		std::string igrone;
		close_svc_handle(handle,igrone);
	}
	return err;
}
int		win_service::reload_general_cfg_info(std::string& what_wrong)
{
	SC_HANDLE handle=NULL;
	int err=sc_mgr_.open_service(svc_name_,handle,what_wrong);
	if(sc_success_code() != err){
		what_wrong=get_sys_err_msg(err);
	}else{
		std::string igrone;
		err=get_srv_general_cfg(handle,cfg_,what_wrong);
		close_svc_handle(handle,igrone);
	}
	return err;
}


int		win_service::cfg_ex_call(SC_HANDLE handle,unsigned long catalog,void* ptr,unsigned long& bytes ,std::string& what_wrong)
{
	BOOL win_api_ok=FALSE;
	int err=0;
	if(ptr == NULL && bytes==0){
		win_api_ok = ::QueryServiceConfig2(handle,catalog,(unsigned char*)ptr,bytes,&bytes);
		err=GetLastError();
		if(!win_api_ok && err!=ERROR_INSUFFICIENT_BUFFER){
			what_wrong=get_sys_err_msg(err);
			return err;
		}else{
			return sc_success_code();
		}
	}

	win_api_ok = ::QueryServiceConfig2(handle,catalog,(unsigned char*)ptr,bytes,&bytes);
	if(!win_api_ok){
		err=GetLastError();
		what_wrong=get_sys_err_msg(err);
	}else{
		err=sc_success_code();
	}
	return err;
}
int		win_service::get_srv_cfg_delayed_auto_start_info(SC_HANDLE handle,svc_cfg_info_ex& saver,std::string& what_wrong)
{
	int err=0;
	unsigned long bytes_need=0;
	err=cfg_ex_call(handle,SERVICE_CONFIG_DELAYED_AUTO_START_INFO,NULL,bytes_need,what_wrong);

	buffer<unsigned char> svc_buf(bytes_need);
	if(err == sc_success_code()) {
		err=cfg_ex_call(handle,SERVICE_CONFIG_DELAYED_AUTO_START_INFO,svc_buf.ptr(),bytes_need,what_wrong);
	}
	if(err == sc_success_code()) {
		SERVICE_DELAYED_AUTO_START_INFO* info_ptr=((SERVICE_DELAYED_AUTO_START_INFO*)svc_buf.ptr());
		saver.DelayedAutostart=info_ptr->fDelayedAutostart;
	}

	return err;
}

int		win_service::get_srv_cfg_description(SC_HANDLE handle,svc_cfg_info_ex& saver,std::string& what_wrong)
{
	int err=0;
	unsigned long bytes_need=0;
	err=cfg_ex_call(handle,SERVICE_CONFIG_DESCRIPTION,NULL,bytes_need,what_wrong);

	buffer<unsigned char> svc_buf(bytes_need);
	if(err == sc_success_code()) {
		err=cfg_ex_call(handle,SERVICE_CONFIG_DESCRIPTION,svc_buf.ptr(),bytes_need,what_wrong);
	}
	if(err == sc_success_code()) {
		SERVICE_DESCRIPTION* info_ptr=((SERVICE_DESCRIPTION*)svc_buf.ptr());
		saver.Description=null_as_empty_str(info_ptr->lpDescription);
	}

	return err;
}
int		win_service::get_srv_cfg_failure_action_list(SC_HANDLE handle,svc_cfg_info_ex& saver,std::string& what_wrong)
{
	int err=0;
	unsigned long bytes_need=0;
	err=cfg_ex_call(handle,SERVICE_CONFIG_FAILURE_ACTIONS,NULL,bytes_need,what_wrong);

	buffer<unsigned char> svc_buf(bytes_need);
	if(err == sc_success_code()) {
		err=cfg_ex_call(handle,SERVICE_CONFIG_FAILURE_ACTIONS,svc_buf.ptr(),bytes_need,what_wrong);
	}
	if(err == sc_success_code()) {
		SERVICE_FAILURE_ACTIONS* info_ptr=((SERVICE_FAILURE_ACTIONS*)svc_buf.ptr());
		svc_fail_action::sfa_to_svc_fail_action(info_ptr,saver.FailAction);
	}

	return err;
}
int		win_service::get_srv_cfg_failure_actions_flag(SC_HANDLE handle,svc_cfg_info_ex& saver,std::string& what_wrong)
{
	int err=0;
	unsigned long bytes_need=0;
	err=cfg_ex_call(handle,SERVICE_CONFIG_FAILURE_ACTIONS_FLAG,NULL,bytes_need,what_wrong);

	buffer<unsigned char> svc_buf(bytes_need);
	if(err == sc_success_code()) {
		err=cfg_ex_call(handle,SERVICE_CONFIG_FAILURE_ACTIONS_FLAG,svc_buf.ptr(),bytes_need,what_wrong);
	}
	if(err == sc_success_code()) {
		SERVICE_FAILURE_ACTIONS_FLAG* info_ptr=((SERVICE_FAILURE_ACTIONS_FLAG*)svc_buf.ptr());
		saver.EnqueueFailActionWhenExitBadly=info_ptr->fFailureActionsOnNonCrashFailures;
	}

	return err;
}
int		win_service::get_srv_cfg_preshutdown_info(SC_HANDLE handle,svc_cfg_info_ex& saver,std::string& what_wrong)
{
	int err=0;
	unsigned long bytes_need=0;
	err=cfg_ex_call(handle,SERVICE_CONFIG_PRESHUTDOWN_INFO,NULL,bytes_need,what_wrong);

	buffer<unsigned char> svc_buf(bytes_need);
	if(err == sc_success_code()) {
		err=cfg_ex_call(handle,SERVICE_CONFIG_PRESHUTDOWN_INFO,svc_buf.ptr(),bytes_need,what_wrong);
	}
	if(err == sc_success_code()) {
		SERVICE_PRESHUTDOWN_INFO* info_ptr=((SERVICE_PRESHUTDOWN_INFO*)svc_buf.ptr());
		saver.PreshutdownTimeoutMS=info_ptr->dwPreshutdownTimeout;
	}

	return err;
}
int		win_service::get_srv_cfg_required_privileges_info(SC_HANDLE handle,svc_cfg_info_ex& saver,std::string& what_wrong)
{
	int err=0;
	unsigned long bytes_need=0;
	err=cfg_ex_call(handle,SERVICE_CONFIG_REQUIRED_PRIVILEGES_INFO,NULL,bytes_need,what_wrong);

	buffer<unsigned char> svc_buf(bytes_need);
	if(err == sc_success_code()) {
		err=cfg_ex_call(handle,SERVICE_CONFIG_REQUIRED_PRIVILEGES_INFO,svc_buf.ptr(),bytes_need,what_wrong);
	}
	if(err == sc_success_code()) {
		SERVICE_REQUIRED_PRIVILEGES_INFO* info_ptr=((SERVICE_REQUIRED_PRIVILEGES_INFO*)svc_buf.ptr());
		saver.RequiredPrivilegesOnCfg= null_as_empty_str(info_ptr->pmszRequiredPrivileges);
	}

	return err;
}
int		win_service::get_srv_cfg_service_sid_info(SC_HANDLE handle,svc_cfg_info_ex& saver,std::string& what_wrong)
{
	int err=0;
	unsigned long bytes_need=0;
	err=cfg_ex_call(handle,SERVICE_CONFIG_SERVICE_SID_INFO,NULL,bytes_need,what_wrong);

	buffer<unsigned char> svc_buf(bytes_need);
	if(err == sc_success_code()) {
		err=cfg_ex_call(handle,SERVICE_CONFIG_SERVICE_SID_INFO,svc_buf.ptr(),bytes_need,what_wrong);
	}
	if(err == sc_success_code()) {
		SERVICE_SID_INFO* info_ptr=((SERVICE_SID_INFO*)svc_buf.ptr());
		saver.ServiceSIDType=info_ptr->dwServiceSidType;
	}

	return err;
}
int		win_service::get_srv_general_cfg(SC_HANDLE handle,svc_cfg_info& saver,std::string& what_wrong)
{
	int err=0;

	unsigned long bytes_need=0;
	// check how many bytes needed.
	if(!::QueryServiceConfig(handle,NULL,0,&bytes_need)){
		err=GetLastError();
		if (err != ERROR_INSUFFICIENT_BUFFER){
			what_wrong=get_sys_err_msg(err);
			return err;
		}
	}

	buffer<unsigned char> svc_buf(bytes_need);
	QUERY_SERVICE_CONFIG* qsc=(QUERY_SERVICE_CONFIG*)(svc_buf.ptr());
	if(::QueryServiceConfig(handle,qsc,svc_buf.count(),&bytes_need)){
		svc_cfg_info::qsc_to_svc_cfg(qsc,saver);
		err=0;
	}else{
		err=GetLastError();
		what_wrong=get_sys_err_msg(err);
	}
	return err;
}


}//WinService
