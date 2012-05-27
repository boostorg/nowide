//
//  Copyright (c) 2012 Artyom Beilis (Tonkikh)
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_NOWIDE_CENV_H_INCLUDED
#define BOOST_NOWIDE_CENV_H_INCLUDED

#include <string>
#include <stdexcept>
#include <stdlib.h>
#include <boost/config.hpp>
#include <boost/nowide/stackstring.hpp>

#ifdef BOOST_WINDOWS
#include <windows.h>
#endif

namespace boost {
    namespace nowide {
        #ifndef BOOST_WINDOWS
        using ::getenv;
        using ::setenv;
        using ::unsetenv;
        using ::putenv;
        #else
        inline char *getenv(char const *key)
        {
            static stackstring value;
            
            wshort_stackstring name;
            if(!name.convert(key))
                return 0;

            static const size_t buf_size = 64;
            wchar_t buf[buf_size];
            std::vector<wchar_t> tmp;
            wchar_t *ptr = buf;
            size_t n = GetEnvironmentVariableW(name.c_str(),buf,buf_size);
            if(n == 0 && GetLastError() == ERROR_ENVVAR_NOT_FOUND)
                return 0;
            if(n >= buf_size) {
                tmp.resize(n+1,L'\0');
                n = GetEnvironmentVariableW(name.c_str(),&tmp[0],tmp.size() - 1);
                // The size may have changed
                if(n >= tmp.size() - 1)
                    return 0;
                ptr = &tmp[0];
            }
            if(!value.convert(ptr))
                return 0;
            return value.c_str();
        }
        inline int setenv(char const *key,char const *value,int override)
        {
            wshort_stackstring name;
            if(!name.convert(key))
                return -1;
            if(!override) {
                wchar_t unused[2];
                if(!(GetEnvironmentVariableW(name.c_str(),unused,2)==0 && GetLastError() == ERROR_ENVVAR_NOT_FOUND))
                    return 0;
            }
            wstackstring wval;
            if(!wval.convert(value))
                return -1;
            if(SetEnvironmentVariableW(name.c_str(),wval.c_str()))
                return 0;
            return -1;
        }
        inline int unsetenv(char const *key)
        {
            wshort_stackstring name;
            if(!name.convert(key))
                return -1;
            if(SetEnvironmentVariableW(name.c_str(),0))
                return 0;
            return -1;
        }
        inline int putenv(char *string)
        {
            char const *key = string;
            char const *key_end = string;
            while(*key_end!='=' && key_end!='\0')
                key_end++;
            if(*key_end == '\0')
                return -1;
            wshort_stackstring wkey;
            if(!wkey.convert(key,key_end))
                return -1;
            
            wstackstring wvalue;
            if(!wvalue.convert(key_end+1))
                return -1;

            if(SetEnvironmentVariableW(wkey.c_str(),wvalue.c_str()))
                return 0;
            return -1;
        }
        #endif
    } // nowide
} // boost

#endif
///
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
