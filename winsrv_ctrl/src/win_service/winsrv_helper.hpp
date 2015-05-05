#ifndef winsrv_helper_h__
#define winsrv_helper_h__

#include <Windows.h>
namespace WinService
{
	namespace detail{
		class local_free_on_destruction
		{
		public:
			explicit local_free_on_destruction(void* p)
				: p_(p) {}

			~local_free_on_destruction()
			{
				::LocalFree(p_);
			}

		private:
			void* p_;
			local_free_on_destruction(const local_free_on_destruction&);  // = deleted
			local_free_on_destruction& operator=(const local_free_on_destruction&);  // = deleted
		};
	}//namespace detail

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

//////////////////////////////////////////////////////////////////////////

class sys_error_category
{
public:
	static std::string message( int ev )
	{
		LPVOID lpMsgBuf = 0;
		DWORD retval = ::FormatMessageA( 
			FORMAT_MESSAGE_ALLOCATE_BUFFER | 
			FORMAT_MESSAGE_FROM_SYSTEM | 
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			ev,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
			(LPSTR) &lpMsgBuf,
			0,
			NULL 
			);
		detail::local_free_on_destruction lfod(lpMsgBuf);
		if (retval == 0)
			return std::string("Unknown error");

		std::string str( static_cast<LPCSTR>(lpMsgBuf) );

		while ( str.size()&& (str[str.size()-1] == '\n' || str[str.size()-1] == '\r') )
		{	str.erase( str.size()-1 );}
		if ( str.size() && str[str.size()-1] == '.' ) 
		{ str.erase( str.size()-1 ); }
		return str;
	}
	static std::string category(  ){
		return "windows";
	}
};

static std::string	get_sys_err_msg(unsigned int err_code)
{
	return sys_error_category::message(err_code);
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
