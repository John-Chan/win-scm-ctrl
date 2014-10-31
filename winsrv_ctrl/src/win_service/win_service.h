#ifndef win_service_h__
#define win_service_h__

#include <Windows.h>
#include <string>
#include <list>

#include "svc_infomation.h"

namespace WinService
{


class win_sc_manger;

class win_service
{
public:
	win_service(const std::string& svc_name,win_sc_manger& sc_mgr);
	~win_service(void);
	const svc_cfg_info&		config_info()const;
	svc_cfg_info&			config_info();

	const svc_cfg_info_ex&		config_ex_info()const;
	svc_cfg_info_ex&			config_ex_info();

	const srv_process_info&	proc_info()const;
	srv_process_info&		proc_info();

	int		reload_cfg_info_all(bool stop_on_fail,std::string& what_wrong);
	int		reload_general_cfg_info(std::string& what_wrong);
	int		reload_more_cfg_info(bool stop_on_fail,std::string& what_wrong);

	const	std::string& name()const{return svc_name_;};

	static int		get_srv_general_cfg(SC_HANDLE handle,svc_cfg_info& saver,std::string& what_wrong);

	static int		get_srv_cfg_delayed_auto_start_info(SC_HANDLE handle,svc_cfg_info_ex& saver,std::string& what_wrong);
	static int		get_srv_cfg_description(SC_HANDLE handle,svc_cfg_info_ex& saver,std::string& what_wrong);
	static int		get_srv_cfg_failure_action_list(SC_HANDLE handle,svc_cfg_info_ex& saver,std::string& what_wrong);
	static int		get_srv_cfg_failure_actions_flag(SC_HANDLE handle,svc_cfg_info_ex& saver,std::string& what_wrong);
	static int		get_srv_cfg_preshutdown_info(SC_HANDLE handle,svc_cfg_info_ex& saver,std::string& what_wrong);
	static int		get_srv_cfg_required_privileges_info(SC_HANDLE handle,svc_cfg_info_ex& saver,std::string& what_wrong);
	static int		get_srv_cfg_service_sid_info(SC_HANDLE handle,svc_cfg_info_ex& saver,std::string& what_wrong);
	static int		cfg_ex_call(SC_HANDLE handle,unsigned long catalog,void* ptr,unsigned long& bytes ,std::string& what_wrong);

	void			copy_from(const win_service& rh);
private:
	std::string		svc_name_;
	svc_cfg_info	cfg_;
	svc_cfg_info_ex cfg_ex_;
	srv_process_info proc_info_;
	win_sc_manger&	sc_mgr_;
};

typedef	std::list<win_service>	WinSvcList;

}//WinService



#endif // win_service_h__
