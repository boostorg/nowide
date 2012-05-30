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

__stdcall wchar_t *GetEnvironmentStringsW(void);
__stdcall int FreeEnvironmentStringsW(wchar_t *);
__stdcall wchar_t *GetCommandLineW(void);
__stdcall wchar_t **CommandLineToArgvW(wchar_t const *,int *);
__stdcall unsigned long GetLastError();
__stdcall void *LocalFree(void *);
__stdcall int SetEnvironmentVariableW(wchar_t const *,wchar_t const *);
__stdcall unsigned long GetEnvironmentVariableW(wchar_t const *,wchar_t *,unsigned long);

}

#endif 



#endif
///
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
