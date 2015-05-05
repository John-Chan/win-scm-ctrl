#ifndef sc_manager_h__
#define sc_manager_h__
#include <Windows.h>
#include <string>
#include <vector>
#include "win_service.h"
#include "svc_infomation.h"

namespace WinService
{

enum SvcCtrlCode{
	kSvcDoStop=SERVICE_CONTROL_STOP,						//0x00000001
	kSvcDoPause=SERVICE_CONTROL_PAUSE,						//0x00000002
	kSvcDoResume=SERVICE_CONTROL_CONTINUE,					//0x00000003
	kSvcDoReportStat=SERVICE_CONTROL_INTERROGATE,			//0x00000004
															//where is 0x00000005 ?
	kSvcEvtChangeParameter=SERVICE_CONTROL_PARAMCHANGE,		//0x00000006
	kSvcEvtNewComponent=SERVICE_CONTROL_NETBINDADD,			//0x00000007 ,network service ONLY
	kSvcEvtComponentRemoved=SERVICE_CONTROL_NETBINDREMOVE,	//0x00000008 ,network service ONLY
	kSvcEvtComponentEnabled=SERVICE_CONTROL_NETBINDENABLE,	//0x00000009 ,network service ONLY
	kSvcEvtComponentDisabled=SERVICE_CONTROL_NETBINDDISABLE,//0x0000000a ,network service ONLY
};

class win_sc_manger
{
public:
	win_sc_manger();
	~win_sc_manger();
	int		qry_service_list(SrvStatus stat,SrvType type,WinSvcList& lst,std::string& what_wrong );

	int		qry_service(const std::string& svc_name,SrvStatus stat,SrvType type,win_service& svc_info,std::string& what_wrong );

	int		open_service(const std::string& svc_name,SC_HANDLE& svc,std::string& what_wrong);

	int		open_scm(std::string& what_wrong);
	int		close_scm(std::string& what_wrong);

	bool	scm_has_open()const;

	/// create a new service
	int		create_service(svc_create_parameter& para,std::string& what_wrong);
	/// delete a service
	int		delete_service(const std::string& svc_name,std::string& what_wrong);
	
	/// send a message  to startup a service
	/// you  need check service status by yourself(@see get_service)
	int		start_service(const std::string& svc_name,const std::vector<std::string>& cmd_line,std::string& what_wrong);

	/// send a message  to stop a service
	/// you  need check service status by yourself(@see get_service)
	/// @warn it will blocks for 30 seconds if any service is busy handling a control code
	int		stop_service(const std::string& svc_name,std::string& what_wrong);

	/// send a message  to pause a service
	/// you  need check service status by yourself(@see get_service)
	/// @warn it will blocks for 30 seconds if any service is busy handling a control code
	int		pause_service(const std::string& svc_name,std::string& what_wrong);

	/// send a message  to resume a service
	/// you  need check service status by yourself(@see get_service)
	/// @warn it will blocks for 30 seconds if any service is busy handling a control code
	int		resume_service(const std::string& svc_name,std::string& what_wrong);

private:

	/// Ask the Service Control Manager (SCM) to send the requested control code to the service
	/// @warn This func call ControlServiceEx(or ControlService ) ,so it will blocks for 30 seconds if any service is busy handling a control code
	/// if WINVER>=0x0600 this func invoke ControlServiceEx,ortherwise call ControlService instead,and para_ptr is not used
	bool		post_ctrl_msg(SC_HANDLE svc_handle,SvcCtrlCode ctrl_code,void* para_ptr,std::string& what_wrong,int & stat_code);
private:
	SC_HANDLE	hand_;
};

}//WinService

#endif // sc_manager_h__
