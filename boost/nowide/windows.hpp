//
//  Copyright (c) 2012 Artyom Beilis (Tonkikh)
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_NOWIDE_WINDOWS_HPP_INCLUDED
#define BOOST_NOWIDE_WINDOWS_HPP_INCLUDED

#include <stddef.h>

#ifdef BOOST_NOWIDE_USE_WINDOWS_H
#include <windows.h>
#else

//
// These are function prototypes... Allow to to include windows.h
//
extern "C" {

wchar_t*        __stdcall GetEnvironmentStringsW(void);
int             __stdcall FreeEnvironmentStringsW(wchar_t *);
wchar_t*        __stdcall GetCommandLineW(void);
wchar_t**       __stdcall CommandLineToArgvW(wchar_t const *,int *);
unsigned long   __stdcall GetLastError();
void*           __stdcall LocalFree(void *);
int             __stdcall SetEnvironmentVariableW(wchar_t const *,wchar_t const *);
unsigned long   __stdcall GetEnvironmentVariableW(wchar_t const *,wchar_t *,unsigned long);

}

#endif 



#endif
///
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
