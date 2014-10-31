//#include "StdAfx.h"
#include "svc_infomation.h"
#include "winsrv_helper.hpp"
#include <iostream>
#include <algorithm>

namespace WinService
{


/************************************************************************
//  svc_cfg_info
//  --------------------------------------------------------------------
//                                                                  
************************************************************************/


svc_cfg_info::svc_cfg_info()
:ServiceType(0),
StartType(0),
ErrorControl(0),
TagId(0)
{
	//
}


svc_cfg_info::~svc_cfg_info(){
	//
}
void	svc_cfg_info::qsc_to_svc_cfg(const QUERY_SERVICE_CONFIG* qsc,svc_cfg_info& svc_cfg)
{
	svc_cfg.BinaryPathName=null_as_empty_str(qsc->lpBinaryPathName);
	svc_cfg.Dependencies=null_as_empty_str(qsc->lpDependencies);
	svc_cfg.DisplayName=null_as_empty_str(qsc->lpDisplayName);
	svc_cfg.ErrorControl=qsc->dwErrorControl;
	svc_cfg.LoadOrderGroup=null_as_empty_str(qsc->lpLoadOrderGroup);
	svc_cfg.ServiceStartName=null_as_empty_str(qsc->lpServiceStartName);
	svc_cfg.ServiceType=qsc->dwServiceType;
	svc_cfg.StartType=qsc->dwStartType;
	svc_cfg.TagId=qsc->dwTagId;
}

std::string svc_cfg_info::FmtServiceTypeStr(unsigned long val)
{
	switch(val){
		case SERVICE_FILE_SYSTEM_DRIVER:
			return std::string("SERVICE_FILE_SYSTEM_DRIVER");
		case SERVICE_KERNEL_DRIVER:
			return std::string("SERVICE_KERNEL_DRIVER");
		case SERVICE_WIN32_OWN_PROCESS:
			return std::string("SERVICE_WIN32_OWN_PROCESS");
		case SERVICE_WIN32_SHARE_PROCESS:
			return std::string("SERVICE_WIN32_SHARE_PROCESS");
		case SERVICE_INTERACTIVE_PROCESS:
			return std::string("SERVICE_INTERACTIVE_PROCESS");
		default:
			return std::string("unknow ServiceType code");
	}
}
std::string svc_cfg_info::FmtStartTypeStr(unsigned long val)
{

	switch(val){
		case SERVICE_AUTO_START:
			return std::string("SERVICE_AUTO_START");
		case SERVICE_BOOT_START:
			return std::string("SERVICE_BOOT_START");
		case SERVICE_DEMAND_START:
			return std::string("SERVICE_DEMAND_START");
		case SERVICE_DISABLED:
			return std::string("SERVICE_DISABLED");
		case SERVICE_SYSTEM_START:
			return std::string("SERVICE_SYSTEM_START");
		default:
			return std::string("unknow StartType code");
	}
}
std::string svc_cfg_info::FmtErrorControlStr(unsigned long val)
{

	switch(val){
		case SERVICE_ERROR_CRITICAL:
			return std::string("SERVICE_ERROR_CRITICAL");
		case SERVICE_ERROR_IGNORE:
			return std::string("SERVICE_ERROR_IGNORE");
		case SERVICE_ERROR_NORMAL:
			return std::string("SERVICE_ERROR_NORMAL");
		case SERVICE_ERROR_SEVERE:
			return std::string("SERVICE_ERROR_SEVERE");
		default:
			return std::string("unknow ErrorControl code");
	}
}
std::string svc_cfg_info::FmtTagIdStr(unsigned long val)
{
	char int_str[128]={0};
	sprintf_s(int_str,sizeof int_str,"%08d",val);
	return std::string(int_str);
}
std::string svc_cfg_info::DescribServiceTypeStr(unsigned long val)
{
	switch(val){
		case SERVICE_FILE_SYSTEM_DRIVER:
			return std::string("File system driver service");
		case SERVICE_KERNEL_DRIVER:
			return std::string("Driver service");
		case SERVICE_WIN32_OWN_PROCESS:
			return std::string("Service that runs in its own process");
		case SERVICE_WIN32_SHARE_PROCESS:
			return std::string("Service that shares a process with other services");
		case SERVICE_INTERACTIVE_PROCESS:
			return std::string("The service can interact with the desktop");
		default:
			return std::string("unknow ServiceType code");
	}
}
std::string svc_cfg_info::DescribStartTypeStr(unsigned long val)
{

	switch(val){
		case SERVICE_AUTO_START:
			return std::string("A service started automatically by the service control manager during system startup");
		case SERVICE_BOOT_START:
			return std::string("A device driver started by the system loader. This value is valid only for driver services");
		case SERVICE_DEMAND_START:
			return std::string("A service started by the service control manager when a process calls the StartService function");
		case SERVICE_DISABLED:
			return std::string("A service that cannot be started");
		case SERVICE_SYSTEM_START:
			return std::string("A device driver started by the IoInitSystem function. This value is valid only for driver services");
		default:
			return std::string("unknow StartType code");
	}
}
std::string svc_cfg_info::DescribErrorControlStr(unsigned long val)
{
	switch(val){
		case SERVICE_ERROR_CRITICAL:
			return std::string("The startup program logs the error in the event log, if possible. If the last-known good configuration is being started, the startup operation fails. Otherwise, the system is restarted with the last-known good configuration.");
		case SERVICE_ERROR_IGNORE:
			return std::string("The startup program ignores the error and continues the startup operation");
		case SERVICE_ERROR_NORMAL:
			return std::string("The startup program logs the error in the event log and continues the startup operation");
		case SERVICE_ERROR_SEVERE:
			return std::string("The startup program logs the error in the event log. If the last-known good configuration is being started, the startup operation continues. Otherwise, the system is restarted with the last-known-good configuration.");
		default:
			return std::string("unknow ErrorControl code");
	}

}
std::string svc_cfg_info::DescribTagIdStr(unsigned long val)
{
	switch(val){
		case 0:
			return std::string("The service has not been assigned a tag( only evaluated for SERVICE_KERNEL_DRIVER and SERVICE_FILE_SYSTEM_DRIVER type services )");
		default:
			char int_str[128]={0};
			sprintf_s(int_str,sizeof int_str,"%08d",val);
			return std::string(int_str);
	}
}

void		svc_cfg_info::print(const svc_cfg_info& info)
{
	svc_cfg_info::print(info,std::cout);
}
void		svc_cfg_info::print(const svc_cfg_info& info,std::ostream& os)
{
	os<<"BinaryPathName    :"<< info.BinaryPathName<<std::endl;
	os<<"DisplayName       :"<< info.DisplayName<<std::endl;
	os<<"ServiceType       :"<< info.ServiceType<<" "<< FmtServiceTypeStr(info.ServiceType)<<std::endl;
	os<<"StartType         :"<< info.StartType<<" "<< FmtStartTypeStr(info.StartType)<<std::endl;
	os<<"TagId             :"<< info.TagId<<" "<< FmtTagIdStr(info.TagId)<<std::endl;
	os<<"ErrorControl      :"<< info.ErrorControl<<" "<< FmtErrorControlStr(info.ErrorControl)<<std::endl;
	os<<"LoadOrderGroup    :"<< info.LoadOrderGroup<<std::endl;
	os<<"ServiceStartName  :"<< info.ServiceStartName<<std::endl;
	os<<"Dependencies      :"<< info.Dependencies<<std::endl;
}

/************************************************************************
//  svc_fail_action
//  --------------------------------------------------------------------
//                                                                  
************************************************************************/

svc_fail_action::svc_fail_action()
:ResetPeriod(0)
{
	//
}
svc_fail_action::~svc_fail_action()
{
	//
}
void	svc_fail_action::sfa_to_svc_fail_action(const SERVICE_FAILURE_ACTIONS* sfa,svc_fail_action& fail_ac)
{
	fail_ac.Command=null_as_empty_str(sfa->lpCommand);
	fail_ac.RebootMsg=null_as_empty_str(sfa->lpRebootMsg);
	fail_ac.ResetPeriod=sfa->dwResetPeriod;
	fail_ac.FailAactionList.clear();
	for (unsigned long i=0;i< sfa->cActions;++i){
		fail_ac.FailAactionList.push_back(svc_failure_action( sfa->lpsaActions[i].Type,sfa->lpsaActions[i].Delay));
	}
	
}


std::string svc_fail_action::FmtActionTypeStr(SC_ACTION_TYPE val)
{
	switch(val){
		case SC_ACTION_NONE:
			return std::string("SC_ACTION_NONE");
		case SC_ACTION_REBOOT:
			return std::string("SC_ACTION_REBOOT");
		case SC_ACTION_RESTART:
			return std::string("SC_ACTION_RESTART");
		case SC_ACTION_RUN_COMMAND:
			return std::string("SC_ACTION_RUN_COMMAND");
		default:
			return std::string("unknow ACTION");
	}
}
std::string svc_fail_action::DescribActionTypeStr(SC_ACTION_TYPE val)
{
	switch(val){
		case SC_ACTION_NONE:
			return std::string("No action");
		case SC_ACTION_REBOOT:
			return std::string("Reboot the computer");
		case SC_ACTION_RESTART:
			return std::string("Restart the service");
		case SC_ACTION_RUN_COMMAND:
			return std::string("Run a command");
		default:
			return std::string("unknow ACTION");
	}
}
/************************************************************************
//  svc_cfg_info_ex
//  --------------------------------------------------------------------
//                                                                  
************************************************************************/

svc_cfg_info_ex::svc_cfg_info_ex()
:EnqueueFailActionWhenExitBadly(false),
DelayedAutostart(false),
PreshutdownTimeoutMS(0),
ServiceSIDType(0)
{
	//
}
svc_cfg_info_ex::~svc_cfg_info_ex()
{
	//
}

std::string svc_cfg_info_ex::FmtServiceSIDTypeStr(unsigned long val)
{

	switch(val){
		case SERVICE_SID_TYPE_NONE:
			return std::string("SERVICE_SID_TYPE_NONE");
		case SERVICE_SID_TYPE_RESTRICTED:
			return std::string("SERVICE_SID_TYPE_RESTRICTED");
		case SERVICE_SID_TYPE_UNRESTRICTED:
			return std::string("SERVICE_SID_TYPE_UNRESTRICTED");
		default:
			return std::string("unknow ServiceSIDType");
	}
}

std::string svc_cfg_info_ex::DescribServiceSIDTypeStr(unsigned long val)
{

	switch(val){
		case SERVICE_SID_TYPE_NONE:
			return std::string("No information about this service security identifier");
		case SERVICE_SID_TYPE_RESTRICTED:
			return std::string("The service security identifier is a restricted type");
		case SERVICE_SID_TYPE_UNRESTRICTED:
			return std::string("The service security identifier is not a restricted type");
		default:
			return std::string("unknow ServiceSIDType");
	}
}

void		svc_cfg_info_ex::print(const svc_cfg_info_ex& info)
{

	svc_cfg_info_ex::print(info,std::cout);
}
void		svc_cfg_info_ex::print(const svc_cfg_info_ex& info,std::ostream& os)
{
	os<<"Description                     :"<< info.Description<<std::endl;
	os<<"ServiceSIDType                  :"<< FmtServiceSIDTypeStr(info.ServiceSIDType)<<std::endl;

	os<<"EnqueueFailActionWhenExitBadly  :"<< info.EnqueueFailActionWhenExitBadly<<std::endl;
	os<<"DelayedAutostart                :"<< info.DelayedAutostart<<std::endl;
	os<<"RequiredPrivilegesOnCfg         :"<< info.RequiredPrivilegesOnCfg<<std::endl;
	os<<"PreshutdownTimeoutMS            :"<< info.PreshutdownTimeoutMS<<std::endl;
	os<<"FailAction Count                :"<< info.FailAction.FailAactionList.size()<<std::endl;

	svc_fail_action::svc_failure_action_list::const_iterator it=info.FailAction.FailAactionList.begin();
	svc_fail_action::svc_failure_action_list::const_iterator it_end=info.FailAction.FailAactionList.end();

	while(it != it_end){
		os<<"FailAction                 :"<< svc_fail_action::FmtActionTypeStr(it->first) << " After " << it->second <<" milliseconds" <<std::endl;
		++it;
	}
}


/************************************************************************
//  srv_process_info
//  --------------------------------------------------------------------
//                                                                  
************************************************************************/



srv_process_info::srv_process_info()
:ServiceType(0),
CurrentState(0),
ControlsAccepted(0),
Win32ExitCode(0),
ServiceSpecificExitCode(0),
CheckPoint(0),
WaitHint(0),
ProcessId(0),
ServiceFlags(0)
{
	///
}
srv_process_info::~srv_process_info()
{
	//
}

void			srv_process_info::ssp_to_proc_info(const SERVICE_STATUS_PROCESS* ssp,srv_process_info& proc_info)
{
	proc_info.CheckPoint= ssp->dwCheckPoint;
	proc_info.ControlsAccepted= ssp->dwControlsAccepted;
	proc_info.CurrentState= ssp->dwCurrentState;
	proc_info.ProcessId= ssp->dwProcessId;
	proc_info.ServiceFlags= ssp->dwServiceFlags;
	proc_info.ServiceSpecificExitCode= ssp->dwServiceSpecificExitCode;
	proc_info.ServiceType= ssp->dwServiceType;
	proc_info.WaitHint= ssp->dwWaitHint;
	proc_info.Win32ExitCode= ssp->dwWin32ExitCode;
}
void			srv_process_info::print(const srv_process_info& info)
{
	srv_process_info::print(info,std::cout);
}
void			srv_process_info::print(const srv_process_info& info,std::ostream& os)
{
	ULongVector apt_codes=ExtractControlsAcceptedCode(info.ControlsAccepted);
	std::string apt_code_strs;
	for (size_t i=0;i<apt_codes.size();++i){
		apt_code_strs+=FmtControlsAcceptedStr(apt_codes[i])+" ";
	}
	os<<"ServiceType               :"<< info.ServiceType<<" "<< FmtServiceTypeStr(info.ServiceType)<<std::endl;
	os<<"CurrentState              :"<< info.CurrentState <<" "<< FmtCurrentStateStr(info.CurrentState)<<std::endl;
	os<<"ControlsAccepted          :"<< info.ControlsAccepted<<" "<< apt_code_strs<<std::endl;
	os<<"ProcessId                 :"<< info.ProcessId<<std::endl;
	os<<"ServiceFlags              :"<< info.ServiceFlags<<" "<< FmtServiceFlagsStr(info.ServiceFlags)<<std::endl;
	os<<"ServiceSpecificExitCode   :"<< info.ServiceSpecificExitCode<<std::endl;
	os<<"WaitHint                  :"<< info.WaitHint<<std::endl;
	os<<"Win32ExitCode             :"<< info.Win32ExitCode<<std::endl;
	os<<"CheckPoint                :"<< info.CheckPoint<<std::endl;
}


std::string srv_process_info::FmtServiceTypeStr(unsigned long val)
{
	return svc_cfg_info::FmtServiceTypeStr(val);
}
std::string srv_process_info::FmtCurrentStateStr(unsigned long val)
{
	switch(val){
		case SERVICE_CONTINUE_PENDING:
			return std::string("SERVICE_CONTINUE_PENDING");
		case SERVICE_PAUSE_PENDING:
			return std::string("SERVICE_PAUSE_PENDING");
		case SERVICE_PAUSED:
			return std::string("SERVICE_PAUSED");
		case SERVICE_RUNNING:
			return std::string("SERVICE_RUNNING");
		case SERVICE_START_PENDING:
			return std::string("SERVICE_START_PENDING");
		case SERVICE_STOP_PENDING:
			return std::string("SERVICE_STOP_PENDING");
		case SERVICE_STOPPED:
			return std::string("SERVICE_STOPPED");
		default:
			return std::string("unknow State");
	}
}
ULongVector srv_process_info::ExtractControlsAcceptedCode(unsigned long val)
{
	ULongVector vec;
	if((val & SERVICE_ACCEPT_NETBINDCHANGE) == SERVICE_ACCEPT_NETBINDCHANGE) vec.push_back(SERVICE_ACCEPT_NETBINDCHANGE);
	if((val & SERVICE_ACCEPT_PARAMCHANGE) == SERVICE_ACCEPT_PARAMCHANGE) vec.push_back(SERVICE_ACCEPT_PARAMCHANGE);
	if((val & SERVICE_ACCEPT_PAUSE_CONTINUE) == SERVICE_ACCEPT_PAUSE_CONTINUE) vec.push_back(SERVICE_ACCEPT_PAUSE_CONTINUE);
	if((val & SERVICE_ACCEPT_PRESHUTDOWN) == SERVICE_ACCEPT_PRESHUTDOWN) vec.push_back(SERVICE_ACCEPT_PRESHUTDOWN);
	if((val & SERVICE_ACCEPT_SHUTDOWN) == SERVICE_ACCEPT_SHUTDOWN) vec.push_back(SERVICE_ACCEPT_SHUTDOWN);
	if((val & SERVICE_ACCEPT_STOP) == SERVICE_ACCEPT_STOP) vec.push_back(SERVICE_ACCEPT_STOP);
	if((val & SERVICE_ACCEPT_HARDWAREPROFILECHANGE) == SERVICE_ACCEPT_HARDWAREPROFILECHANGE) vec.push_back(SERVICE_ACCEPT_HARDWAREPROFILECHANGE);
	if((val & SERVICE_ACCEPT_POWEREVENT) == SERVICE_ACCEPT_POWEREVENT) vec.push_back(SERVICE_ACCEPT_POWEREVENT);
	if((val & SERVICE_ACCEPT_SESSIONCHANGE) == SERVICE_ACCEPT_SESSIONCHANGE) vec.push_back(SERVICE_ACCEPT_SESSIONCHANGE);
	return vec;
}
std::string srv_process_info::FmtControlsAcceptedStr(unsigned long val)
{
	switch(val){
		case SERVICE_ACCEPT_NETBINDCHANGE:
			return std::string("SERVICE_ACCEPT_NETBINDCHANGE");
		case SERVICE_ACCEPT_PARAMCHANGE:
			return std::string("SERVICE_ACCEPT_PARAMCHANGE");
		case SERVICE_ACCEPT_PAUSE_CONTINUE:
			return std::string("SERVICE_ACCEPT_PAUSE_CONTINUE");
		case SERVICE_ACCEPT_PRESHUTDOWN:
			return std::string("SERVICE_ACCEPT_PRESHUTDOWN");
		case SERVICE_ACCEPT_SHUTDOWN:
			return std::string("SERVICE_ACCEPT_SHUTDOWN");
		case SERVICE_ACCEPT_STOP:
			return std::string("SERVICE_ACCEPT_STOP");
		case SERVICE_ACCEPT_HARDWAREPROFILECHANGE:
			return std::string("SERVICE_ACCEPT_HARDWAREPROFILECHANGE");
		case SERVICE_ACCEPT_POWEREVENT:
			return std::string("SERVICE_ACCEPT_POWEREVENT");
		case SERVICE_ACCEPT_SESSIONCHANGE:
			return std::string("SERVICE_ACCEPT_SESSIONCHANGE");

		default:
			return std::string("unknow ControlsAccepted code");
	}
}
std::string srv_process_info::FmtServiceFlagsStr(unsigned long val)
{
	switch(val){
		case 0:
			return std::string("N/A");
		case SERVICE_RUNS_IN_SYSTEM_PROCESS:
			return std::string("SERVICE_RUNS_IN_SYSTEM_PROCESS");
		default:
			return std::string("unknow ServiceFlag code");
	}
}
std::string srv_process_info::DescribServiceTypeStr(unsigned long val)
{
	return svc_cfg_info::DescribServiceTypeStr(val);
}
std::string srv_process_info::DescribCurrentStateStr(unsigned long val)
{
	switch(val){
		case SERVICE_CONTINUE_PENDING:
			return std::string("The service is about to continue");
		case SERVICE_PAUSE_PENDING:
			return std::string("The service is pausing");
		case SERVICE_PAUSED:
			return std::string("The service is paused");
		case SERVICE_RUNNING:
			return std::string("The service is running");
		case SERVICE_START_PENDING:
			return std::string("The service is starting");
		case SERVICE_STOP_PENDING:
			return std::string("The service is stopping");
		case SERVICE_STOPPED:
			return std::string("The service has stopped");
		default:
			return std::string("unknow State");
	}
}

std::string srv_process_info::DescribControlsAcceptedStr(unsigned long val)
{
	switch(val){
		case SERVICE_ACCEPT_NETBINDCHANGE:
			return std::string("The service is a network component that can accept changes in its binding without being stopped and restarted");
		case SERVICE_ACCEPT_PARAMCHANGE:
			return std::string("The service can reread its startup parameters without being stopped and restarted");
		case SERVICE_ACCEPT_PAUSE_CONTINUE:
			return std::string("The service can be paused and continued");
		case SERVICE_ACCEPT_PRESHUTDOWN:
			return std::string("The service can perform preshutdown tasks");
		case SERVICE_ACCEPT_SHUTDOWN:
			return std::string("The service is notified when system shutdown occurs");
		case SERVICE_ACCEPT_STOP:
			return std::string("The service can be stopped");
		case SERVICE_ACCEPT_HARDWAREPROFILECHANGE:
			return std::string("The service is notified when the computer's hardware profile has changed");
		case SERVICE_ACCEPT_POWEREVENT:
			return std::string("The service is notified when the computer's power status has changed");
		case SERVICE_ACCEPT_SESSIONCHANGE:
			return std::string("The service is notified when the computer's session status has changed");
		
		default:
			return std::string("unknow ControlsAccepted code");
	}
}
std::string srv_process_info::DescribServiceFlagsStr(unsigned long val)
{
	switch(val){
		case 0:
			return std::string("The service is running in a process that is not a system process, or it is not running");
		case SERVICE_RUNS_IN_SYSTEM_PROCESS:
			return std::string("The service runs in a system process that must always be running");
		default:
			return std::string("unknow ServiceFlag code");
	}
}

/************************************************************************
//  svc_create_parameter
//  --------------------------------------------------------------------
//                                                                  
************************************************************************/


svc_create_parameter::svc_create_parameter()
{
	//
}
svc_create_parameter::~svc_create_parameter()
{
	//
}
void		svc_create_parameter::make_simple_para(
							 const std::string& binary_path,
							 const std::string& svc_name,
							 const std::string& display_name,
							 unsigned long svc_type,
							 unsigned long start_type)
{
	ServiceName=svc_name;
	DisplayName=display_name;
	DesiredAccess=SC_MANAGER_CREATE_SERVICE ;// or SC_MANAGER_ALL_ACCESS 
	ServiceType=svc_type;
	StartType=start_type;
	ErrorControl=SERVICE_ERROR_NORMAL;
	BinaryPathName=binary_path;

	//Specify NULL or an empty string if the service does not belong to a group. 
	LoadOrderGroup="";
	// mark 0xFFFFFFFF to  pass NULL for TagId latter
	TagId=0xFFFFFFFF;

	// no dependencies,pass NULL or an empty can do it well 
	Dependencies="";
	// account who run this service.uses the LocalSystem account,will pass NULL. 
	ServiceStartName="";
	// passord for account who run this service. 
	// empty string if the account has no password or if the service runs in the LocalService
	Password="";
	if(BinaryPathName.find(' ')!= std::string::npos){
		if(BinaryPathName.c_str()[0] !='\"') BinaryPathName="\""+BinaryPathName;
		if(BinaryPathName.c_str()[BinaryPathName.length() - 1] !='\"') BinaryPathName=BinaryPathName+"\"";
	}
}
}//WinService
