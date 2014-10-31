#ifndef winsrv_helper_h__
#define winsrv_helper_h__

#include <Windows.h>
namespace WinService
{


template <class T>
class buffer
{
public:
	buffer(unsigned int count)
		:count_(count)
	{
		ptr_=new T[count_];
	}
	~buffer()
	{
		delete []ptr_;
		ptr_=NULL;
		count_=0;
	}

	T*	ptr(){return ptr_;}
	const T*	ptr()const {return ptr_;}
	unsigned int count()const {return count_;}
private:
	T*	ptr_;
	unsigned int count_;
};


/************************************************************************
//  free function
//  --------------------------------------------------------------------
//                                                                  
************************************************************************/

static	bool sc_opt_success(int err_code){return err_code==0x00000000;};
static	int	 sc_success_code(){return 0x00000000;};

static const char*	null_as_empty_str(const char* src)
{
	return (NULL == src)?"":src;
}
static bool	invalid_svc_handle(SC_HANDLE handle)
{
	return handle==NULL;
}

static std::string	get_sys_err_msg(unsigned int err_code)
{
	static const unsigned int char_count=512;
	char msg_buff[char_count]={0};
	::FormatMessageA(
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		err_code,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		msg_buff,
		(unsigned long)char_count, NULL );
	return std::string(msg_buff);
}
static int	close_svc_handle(SC_HANDLE handle,std::string& what_wrong)
{
	int err=0;
	if(!invalid_svc_handle(handle))
	{
		if(!::CloseServiceHandle(handle)){
			err=::GetLastError();
			what_wrong=get_sys_err_msg(err);
		}
	}
	return err;
}

}//WinService


#endif // winsrv_helper_h__
