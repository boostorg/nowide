#define _UNICODE
#include <windows.h>
#include <wchar.h>
#include <string.h>
#include <boost/locale/encoding_utf.hpp>
#include <string>


void write(wchar_t const *s)
{
	HANDLE h=GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD written = 0;
	DWORD dummy;
	if(GetConsoleMode(h,&dummy)) {
		WriteConsoleW(h,s,wcslen(s),&written,0);
	}
	else {
		std::string tmp = boost::locale::conv::utf_to_utf<char>(s);
		WriteFile(h,tmp.c_str(),tmp.size(),&written,0);
	}
}

int read(wchar_t *buf,size_t n)
{
	HANDLE h=GetStdHandle(STD_INPUT_HANDLE);
	DWORD read = 0;
	DWORD dummy;
	if(GetConsoleMode(h,&dummy)) {
		ReadConsoleW(h,buf,n-1,&read,0);
		buf[read] = 0;
	}
	else {
		char tmp[256];
		ReadFile(h,tmp,sizeof(tmp),&read,0);
		tmp[read] = 0;
		std::wstring wtmp = boost::locale::conv::utf_to_utf<wchar_t>(tmp);
		buf[n-1]=0;
		wcsncpy(buf,wtmp.c_str(),n-1);
		read = wcslen(buf);
	}
	return read;
}

int main()
{
	write(L"שלום ασφγσφγ мир\n");
	wchar_t buf[256];
	read(buf,256);
	write(buf);
	return 0;
}
