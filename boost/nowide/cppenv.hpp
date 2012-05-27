//
//  Copyright (c) 2012 Artyom Beilis (Tonkikh)
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_NOWIDE_CPPENV_H_INCLUDED
#define BOOST_NOWIDE_CPPENV_H_INCLUDED

#include <string>
#include <stdlib.h>
#include <map>
#include <boost/config.hpp>
#include <boost/nowide/stackstring.hpp>
#include <boost/locale/encoding_errors.hpp>
#include <vector>
#ifdef BOOST_WINDOWS
#include <windows.h>
#include <wchar.h>
#endif

namespace boost {
    namespace nowide {
        #ifndef BOOST_WINDOWS
        inline bool has_enironment(char const *key)
        {
            return ::getenv(key)!=0;
        }
        inline bool has_enironment(std::string const &key)
        {
            return ::getenv(key.c_str())!=0;
        }
        inline std::string get_environment(char const *key,char const *def="")
        {
            char const *v=::getenv(key);
            if(!v) 
                return def;
            return v;
        }
        inline std::string get_environment(std::string const &key,std::string const &def=std::string())
        {
            char const *v=::getenv(key.c_str());
            if(!v) 
                return def;
            return v;
        }
        inline int set_environment(char const *key,char const *value,bool override=true)
        {
            return ::setenv(key,value,int(override));
        }
        inline int set_environment(std::string const &key,std::string const &value,bool override=true)
        {
            return ::setenv(key.c_str(),value.c_str(),int(override));
        }
        inline int unset_environment(char const *key)
        {
            return ::unsetenv(key);
        }
        inline int unset_environment(std::string const &key)
        {
            return ::unsetenv(key.c_str());
        }
        inline std::map<std::string,std::string> get_environment_strings()
        {
            std::map<std::string,std::string> result;
            char **e = environ;
            if(!e)
                return result;
            for(;*e;e++) {
                char *key = *e;
                char *key_end = strchr(key,'=');
                if(key_end == 0)
                    continue;
                std::string skey = std::string(key,key_end);
                std::string svalue(key_end + 1);
                result[skey]=svalue;
            }
            return result;
        }
        #else
        inline bool has_enironment(char const *key)
        {
            wshort_stackstring name;
            if(!name.convert(key)) {
                throw boost::locale::conv::conversion_error();
            }
            wchar_t unused;
            if(GetEnvironmentVariableW(name.c_str(),&unused,1)==0 && GetLastError() == ERROR_ENVVAR_NOT_FOUND)
                return false;
            return true;
        }
        inline bool has_enironment(std::string const &key)
        {
            return has_enironment(key.c_str());
        }
        
        /// \cond INTERNAL 
        namespace details {
            inline bool get_environment_value(char const *key,std::string &result)
            {
                wshort_stackstring name;
                if(!name.convert(key)) {
                    throw boost::locale::conv::conversion_error();
                }
                for(;;) {
                    static const size_t buf_size = 64;
                    wchar_t buf[buf_size];
                    size_t n = GetEnvironmentVariableW(name.c_str(),buf,buf_size);
                    if(n == 0 && GetLastError() == ERROR_ENVVAR_NOT_FOUND)
                        return false;
                    if(n >= buf_size) {
                        std::vector<wchar_t> tmp(n);
                        n = GetEnvironmentVariableW(name.c_str(),&tmp[0],tmp.size());
                        // The size may have changed
                        if(n == 0 && GetLastError() == ERROR_ENVVAR_NOT_FOUND)
                            return false;
                        if(n >= tmp.size())
                            continue;
                        std::string v = convert(&tmp[0]);
                        v.swap(result);
                        return true;
                    }
                    std::string v = convert(buf);
                    v.swap(result);
                    return true;
                }
            }
        }
        
        /// \endcond
        
        inline std::string get_environment(std::string const &key,std::string const &def=std::string())
        {
            std::string result;
            if(details::get_environment_value(key.c_str(),result))
                return result;
            return def;
        }
        inline std::string get_environment(char const *key,char const *def="")
        {
            std::string result;
            if(details::get_environment_value(key,result))
                return result;
            return def;
        }
        inline int set_environment(char const *key,char const *value,bool override=true)
        {
            wshort_stackstring name;
            if(!name.convert(key)) {
                throw boost::locale::conv::conversion_error();
            }
            if(!override) {
                wchar_t unused;
                if(!(GetEnvironmentVariableW(name.c_str(),&unused,1)==0 && GetLastError() == ERROR_ENVVAR_NOT_FOUND))
                    return 0;
            }
            wstackstring wval;
            if(!wval.convert(value))
                throw boost::locale::conv::conversion_error();
            if(SetEnvironmentVariableW(name.c_str(),wval.c_str()))
                return 0;
            return -1;
        }
        inline int set_environment(std::string const &key,std::string const &value,bool override=true)
        {
            return set_environment(key.c_str(),value.c_str(),override);
        }
        inline int unset_environment(char const *key)
        {
            wshort_stackstring name;
            if(!name.convert(key)) {
                throw boost::locale::conv::conversion_error();
            }
            if(SetEnvironmentVariableW(name.c_str(),0))
                return 0;
            return -1;
        }
        inline int unset_environment(std::string const &key)
        {
            return unset_environment(key.c_str());
        }
        inline std::map<std::string,std::string> get_environment_strings()
        {
            std::map<std::string,std::string> result;
            wchar_t *wstrings = 0;
            try {
                wstrings = GetEnvironmentStringsW();
                if(!wstrings)
                    return result;
                for(;*wstrings!=0;wstrings += wcslen(wstrings) + 1) {
                    wchar_t *key_end = wcschr(wstrings,L'=');
                    if(key_end == 0)
                        continue;
                    short_stackstring key;
                    if(!key.convert(wstrings,key_end))
                        continue;
                    stackstring val;
                    if(!val.convert(key_end+1))
                        continue;
                    result[key.c_str()] = val.c_str();
                }

            }
            catch(...) {
                if(wstrings) {
                    FreeEnvironmentStringsW(wstrings);
                    wstrings = 0;
                }
                throw;
            }
            if(wstrings) {
                FreeEnvironmentStringsW(wstrings);
                wstrings = 0;
            }
            return result;
        }

        #endif
    } // nowide
} // boost

#endif
///
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
