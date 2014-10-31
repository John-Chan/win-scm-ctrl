// test_svc_ctrl.cpp : 定义控制台应用程序的入口点。
//


#include <win_service/win_service_kit.h>
#include <iostream>
#include <boost/foreach.hpp>
#include <fstream>

using namespace WinService;

void	print(const WinSvcList& svc_lst)
{

	BOOST_FOREACH(const WinSvcList::value_type& val,svc_lst){
		std::cout<<"name        :"<< val.name()<<std::endl;
		std::cout<<"===========Process Infomation=========="<<std::endl;
		val.proc_info().print(val.proc_info());
		std::cout<<"===========Config Infomation==========="<<std::endl;
		val.config_info().print(val.config_info());
		std::cout<<"===========ConfigEx Infomation==========="<<std::endl;
		val.config_ex_info().print(val.config_ex_info());
		std::cout<<"***********************************************************************"<<std::endl;
	}
}
void	print(const WinSvcList& svc_lst,const std::string& output_file)
{

	std::ofstream file;
	file.open(output_file.c_str());
	BOOST_FOREACH(const WinSvcList::value_type& val,svc_lst){
		file<<"***********************************************************************"<<std::endl;
		file<<"name        :"<< val.name()<<std::endl;
		file<<"***********************************************************************"<<std::endl;
		file<<"===========Process Infomation=========="<<std::endl;
		val.proc_info().print(val.proc_info(),file);
		file<<"===========Config Infomation==========="<<std::endl;
		val.config_info().print(val.config_info(),file);
		file<<"===========ConfigEx Infomation==========="<<std::endl;
		val.config_ex_info().print(val.config_ex_info(),file);
	}
	file.close();
}
int	list_win32(const std::string& output_file)
{
	std::string err_msg;
	int errcode=0;
	win_sc_manger scm;
	WinSvcList lst;
	errcode = scm.open_scm(err_msg);
	if(errcode != sc_success_code()){
		std::cout<<err_msg<<std::endl;
		return 1;
	}
	errcode = scm.qry_service_list(kStatAll,kSvcWin32,lst,err_msg);

	if(errcode != sc_success_code()){
		std::cout<<err_msg<<std::endl;
		return 1;
	}
	BOOST_FOREACH(WinSvcList::value_type& val,lst){
		errcode=val.reload_cfg_info_all(false,err_msg);
		if(errcode != 0){
			std::cout<< "reload_cfg_info fali on service[" << val.name()<<"],error-code "<<errcode<<",msg=" <<err_msg <<std::endl;
		}
	}

	print(lst,output_file);
	return 0;
}

int	list_driver(const std::string& output_file)
{
	std::string err_msg;
	int errcode=0;
	win_sc_manger scm;
	WinSvcList lst;
	errcode = scm.open_scm(err_msg);
	if(errcode != sc_success_code()){
		std::cout<<err_msg<<std::endl;
		return 1;
	}
	errcode = scm.qry_service_list(kStatAll,kSvcDriver,lst,err_msg);

	if(errcode != sc_success_code()){
		std::cout<<err_msg<<std::endl;
		return 1;
	}
	BOOST_FOREACH(WinSvcList::value_type& val,lst){
		errcode=val.reload_cfg_info_all(false,err_msg);
		if(errcode != 0){
			std::cout<< "reload_cfg_info fali on service[" << val.name()<<"],error-code "<<errcode<<",msg=" <<err_msg <<std::endl;
		}
	}

	print(lst,output_file);
	return 0;
}

int main(int argc, char* argv[])
{
	std::string driver_list_file=std::string(argv[0])+".Driver.txt";
	std::string win32_list_file=std::string(argv[0])+".Win32.txt";
	list_driver(driver_list_file);
	list_win32(win32_list_file);
	return 0;
}

