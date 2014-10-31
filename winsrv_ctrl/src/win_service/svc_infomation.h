#ifndef svc_infomation_h__
#define svc_infomation_h__

#include <string>
#include <vector>
#include <Windows.h>
namespace WinService
{


enum	SrvStatus{
	kStatActive=SERVICE_ACTIVE,			//0x00000001
	kStatInActive=SERVICE_INACTIVE,		//0x00000002
	kStatAll=SERVICE_STATE_ALL			//0x00000003
};

enum	SrvType{
	kSvcWin32=SERVICE_WIN32,			//0x00000030
	kSvcDriver=SERVICE_DRIVER			//0x0000000B
};



class svc_create_parameter
{
public:
	svc_create_parameter();
	~svc_create_parameter();

	void		make_simple_para(
		const std::string& binary_path,
		const std::string& display_name,
		const std::string& svc_name,
		unsigned long svc_type=SERVICE_WIN32_OWN_PROCESS,
		unsigned long start_type=SERVICE_DEMAND_START);

	/************************************************************************
	//  a short word:
	//  --------------------------------------------------------------------
	//     
	//     
	/// ServiceType can be:
	/// SERVICE_FILE_SYSTEM_DRIVER(0x00000002) - File system driver service.
	/// SERVICE_KERNEL_DRIVER(0x00000001) - Driver service.
	/// SERVICE_WIN32_OWN_PROCESS(0x00000010) - Service that runs in its own process.
	/// SERVICE_WIN32_SHARE_PROCESS(0x00000020) - Service that shares a process with one or more other services.
	/// SERVICE_INTERACTIVE_PROCESS(0x00000100) - The service can interact with the desktop
	/// 
	/// StartType can be:
	/// SERVICE_AUTO_START(0x00000002) - A service started automatically by the service control manager during system startup
	/// SERVICE_BOOT_START(0x00000000) - A device driver started by the system loader. This value is valid only for driver services.
	/// SERVICE_DEMAND_START(0x00000003) - A service started by the service control manager when a process calls the StartService function
	/// SERVICE_DISABLED(0x00000004) - A service that cannot be started. Attempts to start the service result in the error code ERROR_SERVICE_DISABLED.
	/// SERVICE_SYSTEM_START(0x00000001) - A device driver started by the IoInitSystem function. This value is valid only for driver services.
    /// 
	/// ErrorControl can be:
	/// SERVICE_ERROR_CRITICAL(0x00000003) - The startup program logs the error in the event log, if possible. 
	/// SERVICE_ERROR_IGNORE(0x00000000) - The startup program ignores the error and continues the startup operation
	/// SERVICE_ERROR_NORMAL(0x00000002) - The startup program logs the error in the event log but continues the startup operation
	/// SERVICE_ERROR_SEVERE(0x00000002) - The startup program logs the error in the event log. If the last-known-good configuration is being started, the startup operation continues.

	************************************************************************/
	std::string ServiceName;
	std::string DisplayName;
	unsigned long DesiredAccess;

	unsigned long ServiceType;


	unsigned long StartType;

	unsigned long ErrorControl;
	std::string BinaryPathName;
	std::string LoadOrderGroup;
	unsigned long TagId;
	std::string Dependencies;
	std::string ServiceStartName;
	std::string Password;

};

typedef	std::vector<unsigned long>		ULongVector;
class svc_cfg_info
{
public:
	svc_cfg_info();
	~svc_cfg_info();

	static void	qsc_to_svc_cfg(const QUERY_SERVICE_CONFIG* qsc,svc_cfg_info& svc_cfg);
	static std::string FmtServiceTypeStr(unsigned long val);
	static std::string FmtStartTypeStr(unsigned long val);
	static std::string FmtErrorControlStr(unsigned long val);
	static std::string FmtTagIdStr(unsigned long val);

	static std::string DescribServiceTypeStr(unsigned long val);
	static std::string DescribStartTypeStr(unsigned long val);
	static std::string DescribErrorControlStr(unsigned long val);
	static std::string DescribTagIdStr(unsigned long val);

	static	void		print(const svc_cfg_info& info);
	static	void		print(const svc_cfg_info& info,std::ostream& os);

	unsigned long ServiceType;  
	unsigned long StartType;  
	unsigned long ErrorControl;  
	unsigned long TagId;  
	std::string BinaryPathName;  
	std::string LoadOrderGroup;  
	std::string Dependencies;  
	std::string ServiceStartName;  
	std::string DisplayName;
};

class svc_fail_action
{
public:
	typedef	std::pair<SC_ACTION_TYPE ,unsigned long>	svc_failure_action;
	typedef std::vector<svc_failure_action>			svc_failure_action_list;

	svc_fail_action();
	~svc_fail_action();
	static void	sfa_to_svc_fail_action(const SERVICE_FAILURE_ACTIONS* sfa,svc_fail_action& fail_ac);


	static std::string FmtActionTypeStr(SC_ACTION_TYPE val);
	static std::string DescribActionTypeStr(SC_ACTION_TYPE val);

	unsigned long ResetPeriod;  
	std::string RebootMsg;  
	std::string Command;
	svc_failure_action_list	FailAactionList;
};
class svc_cfg_info_ex
{
public:
	svc_cfg_info_ex();
	~svc_cfg_info_ex();

	static std::string FmtServiceSIDTypeStr(unsigned long val);
	static std::string DescribServiceSIDTypeStr(unsigned long val);
	static	void		print(const svc_cfg_info_ex& info);
	static	void		print(const svc_cfg_info_ex& info,std::ostream& os);

	bool			EnqueueFailActionWhenExitBadly;
	bool			DelayedAutostart;
	unsigned long	PreshutdownTimeoutMS; 
	unsigned long	ServiceSIDType;
	std::string		Description;  
	svc_fail_action	FailAction; 
	std::string RequiredPrivilegesOnCfg;  
	
};

class srv_process_info
{
public:
	srv_process_info();
	~srv_process_info();

	static void	ssp_to_proc_info(const SERVICE_STATUS_PROCESS* ssp,srv_process_info& proc_info);
	static	void		print(const srv_process_info& info);
	static	void		print(const srv_process_info& info,std::ostream& os);

	static ULongVector ExtractControlsAcceptedCode(unsigned long val);
	static std::string FmtServiceTypeStr(unsigned long val);
	static std::string FmtCurrentStateStr(unsigned long val);
	static std::string FmtControlsAcceptedStr(unsigned long val);
	static std::string FmtServiceFlagsStr(unsigned long val);

	static std::string DescribServiceTypeStr(unsigned long val);
	static std::string DescribCurrentStateStr(unsigned long val);
	static std::string DescribControlsAcceptedStr(unsigned long val);
	static std::string DescribServiceFlagsStr(unsigned long val);

	static	void		print(const svc_cfg_info& info);
	static	void		print(const svc_cfg_info& info,std::ostream& os);

	//
	unsigned long ServiceType;  
	unsigned long CurrentState;  
	unsigned long ControlsAccepted;  
	unsigned long Win32ExitCode;   
	unsigned long ServiceSpecificExitCode;  
	unsigned long CheckPoint;  
	unsigned long WaitHint;  
	unsigned long ProcessId; 
	unsigned long ServiceFlags; 

};

}//WinService

#endif // svc_infomation_h__
