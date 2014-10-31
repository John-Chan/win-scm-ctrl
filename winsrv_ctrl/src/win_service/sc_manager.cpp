//#include "StdAfx.h"
#include "sc_manager.h"
#include "winsrv_helper.hpp"
#include <boost/foreach.hpp>

namespace WinService
{


/************************************************************************
//  win_sc_manger
//  --------------------------------------------------------------------
//                                                                  
************************************************************************/


win_sc_manger::win_sc_manger()
:hand_(NULL)
{
	//
}
win_sc_manger::~win_sc_manger()
{
	std::string igrone;
	close_scm(igrone);
}


bool	win_sc_manger::scm_has_open()const
{
	return !invalid_svc_handle(hand_);
}
int win_sc_manger::qry_service_list(SrvStatus stat,SrvType type,WinSvcList& lst,std::string& what_wrong )
{
	BOOL got=TRUE;
	int err=0;

	unsigned long bytes_need=0;
	unsigned long start_page=0;
	unsigned long got_pages=0;
	// check how many bytes needed.
	got=::EnumServicesStatusEx(hand_,SC_ENUM_PROCESS_INFO,type,stat,NULL,0,&bytes_need,&got_pages,&start_page,NULL);
	if(!got){
		err=::GetLastError();
		if(err != ERROR_MORE_DATA){
			what_wrong=get_sys_err_msg(err);
			return err;
		}else{
			// it not a error
			err=sc_success_code();
		}
	}
	buffer<unsigned char> svc_buf(bytes_need);

	// got those...
	bytes_need=0;start_page=0;got_pages=0;
	got=::EnumServicesStatusEx(hand_,SC_ENUM_PROCESS_INFO,type,stat,svc_buf.ptr(),svc_buf.count(),&bytes_need,&got_pages,&start_page,NULL);

	if(!got){
		err=::GetLastError();
		what_wrong=get_sys_err_msg(err);
		return err;
	}else{
		ENUM_SERVICE_STATUS_PROCESS* essp=(ENUM_SERVICE_STATUS_PROCESS*)(svc_buf.ptr()) ;
		for (unsigned long i=0;i<got_pages;++i){
			win_service svc(essp->lpServiceName,*this);
			srv_process_info::ssp_to_proc_info(&essp->ServiceStatusProcess,svc.proc_info());

			lst.push_back(svc);
			//
			++essp;
		}
		return err;
	}

}
int		win_sc_manger::qry_service(const std::string& svc_name,SrvStatus stat,SrvType type,win_service& svc_info,std::string& what_wrong )
{
	WinSvcList lst;
	int err = qry_service_list(stat,type,lst,what_wrong);
	if(err == sc_success_code()){
		WinSvcList::const_iterator it=lst.begin();
		WinSvcList::const_iterator it_end=lst.end();
		while(it != it_end){
			if(it->name() == svc_name){
				svc_info.copy_from(*it);
				return 0;
			}
			++it;
		}
	}
	err=ERROR_SERVICE_NOT_FOUND;
	what_wrong=get_sys_err_msg(err);
	return err;

}
int win_sc_manger::open_service(const std::string& svc_name,SC_HANDLE& svc,std::string& what_wrong)
{
	unsigned int err=0;
	svc= ::OpenService(hand_,svc_name.c_str(),SERVICE_ALL_ACCESS);
	if(invalid_svc_handle(svc)){
		err=::GetLastError();
		what_wrong=get_sys_err_msg(err);
	}
	return err;

}

int		win_sc_manger::open_scm(std::string& what_wrong)
{
	if(scm_has_open())
		return 0;
	unsigned int err=0;
	//SC_MANAGER_ALL_ACCESS (0xF003F)
	hand_=::OpenSCManager(NULL,SERVICES_ACTIVE_DATABASE,SC_MANAGER_ALL_ACCESS);
	if(invalid_svc_handle(hand_)){
		err=::GetLastError();
		what_wrong=get_sys_err_msg(err);
	}
	return err;
}
int		win_sc_manger::close_scm(std::string& what_wrong)
{
	int err=close_svc_handle(hand_,what_wrong);
	hand_=NULL;
	return err;
}

int		win_sc_manger::create_service( svc_create_parameter& para,std::string& what_wrong)
{
	int err=0;
	SC_HANDLE created=::CreateService(
		hand_,
		para.ServiceName.c_str(),
		para.DisplayName.c_str(),
		para.DesiredAccess,
		para.ServiceType,
		para.StartType,
		para.ErrorControl,
		para.BinaryPathName.c_str(),
		para.LoadOrderGroup.c_str(),
		(para.TagId== 0xFFFFFFFF) ? (NULL):(&para.TagId),
		para.Dependencies.c_str(),
		para.ServiceStartName.length()==0?NULL:para.ServiceStartName.c_str(),
		para.Password.c_str());
	if(invalid_svc_handle(created)){
		err=::GetLastError();
		what_wrong=get_sys_err_msg(err);
	}else{
		std::string igrone;
		close_svc_handle(hand_,igrone);
	}
	return err;
}
int		win_sc_manger::delete_service(const std::string& svc_name,std::string& what_wrong)
{

	SC_HANDLE svc_handle=NULL;
	int err=open_service(svc_name,svc_handle, what_wrong);
	if(err == sc_success_code()){
		if(!::DeleteService(svc_handle)){
			err = GetLastError();
			what_wrong=get_sys_err_msg(err);
		}

		std::string igrone;
		close_svc_handle(hand_,igrone);
	}
	return err;

}
int		win_sc_manger::start_service(const std::string& svc_name,const std::vector<std::string>& cmd_line,std::string& what_wrong)
{
	std::vector<const char*>	cmd_argv;
	unsigned long	argc_count=static_cast<unsigned long>(cmd_line.size());
	if(argc_count>0){
		// If there are no arguments, this parameter can be NULL. Otherwise, the first argument  
		// is the name of the service
		cmd_argv.push_back(svc_name.c_str());
		BOOST_FOREACH(const std::string& val,cmd_line){
			cmd_argv.push_back(val.c_str());
		}
	}

	SC_HANDLE svc_handle=NULL;
	int err=open_service(svc_name,svc_handle, what_wrong);
	if(err == sc_success_code()){
		const char ** argv_list=(argc_count==0)?NULL:(&(cmd_argv[0]));
		if(!::StartService(svc_handle,argc_count,argv_list)){
			err = GetLastError();
			what_wrong=get_sys_err_msg(err);
		}

		std::string igrone;
		close_svc_handle(hand_,igrone);
	}
	return err;

}
int		win_sc_manger::stop_service(const std::string& svc_name,std::string& what_wrong)
{
	SC_HANDLE svc_handle=NULL;
	int err=open_service(svc_name,svc_handle, what_wrong);
	if(err == sc_success_code()){
		post_ctrl_msg(svc_handle,kSvcDoStop,NULL,what_wrong,err);
		std::string igrone;
		close_svc_handle(hand_,igrone);
	}
	return err;
}


int		win_sc_manger::pause_service(const std::string& svc_name,std::string& what_wrong)
{
	SC_HANDLE svc_handle=NULL;
	int err=open_service(svc_name,svc_handle, what_wrong);
	if(err == sc_success_code()){
		post_ctrl_msg(svc_handle,kSvcDoPause,NULL,what_wrong,err);
		std::string igrone;
		close_svc_handle(hand_,igrone);
	}
	return err;
}
int		win_sc_manger::resume_service(const std::string& svc_name,std::string& what_wrong)
{
	SC_HANDLE svc_handle=NULL;
	int err=open_service(svc_name,svc_handle, what_wrong);
	if(err == sc_success_code()){
		post_ctrl_msg(svc_handle,kSvcDoResume,NULL,what_wrong,err);
		std::string igrone;
		close_svc_handle(hand_,igrone);
	}
	return err;
}
bool		win_sc_manger::post_ctrl_msg(SC_HANDLE svc_handle,SvcCtrlCode ctrl_code,void* para_ptr,std::string& what_wrong,int & stat_code)
{
	::SetLastError(sc_success_code());
	BOOL call_ok=::ControlServiceEx(svc_handle,ctrl_code,SERVICE_CONTROL_STATUS_REASON_INFO,para_ptr);
	stat_code = GetLastError();
	what_wrong=get_sys_err_msg(stat_code);
	return static_cast<bool>(call_ok);
	
}
}//WinService
