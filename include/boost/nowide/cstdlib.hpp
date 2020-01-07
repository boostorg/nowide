//
//  Copyright (c) 2012 Artyom Beilis (Tonkikh)
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_NOWIDE_CSTDLIB_HPP_INCLUDED
#define BOOST_NOWIDE_CSTDLIB_HPP_INCLUDED

#include <boost/config.hpp>
#ifdef BOOST_WINDOWS
#include <boost/nowide/stackstring.hpp>
#include <boost/nowide/windows.hpp>
#include <vector>
#else
#include <cstdlib>
#endif

namespace boost {
namespace nowide {
#if !defined(BOOST_WINDOWS) && !defined(BOOST_NOWIDE_DOXYGEN)
    using std::getenv;
    using ::setenv;
    using ::unsetenv;
    using ::putenv;
    using std::system;
#else
    ///
    /// \brief UTF-8 aware getenv. Returns 0 if the variable is not set.
    ///
    /// This function is not thread safe or reenterable as defined by the standard library
    ///
    inline char *getenv(const char *key)
    {
        static stackstring value;

        const wshort_stackstring name(key);

        static const size_t buf_size = 64;
        wchar_t buf[buf_size];
        std::vector<wchar_t> tmp;
        wchar_t *ptr = buf;
        size_t n = GetEnvironmentVariableW(name.get(), buf, buf_size);
        if(n == 0 && GetLastError() == 203) // ERROR_ENVVAR_NOT_FOUND
            return 0;
        if(n >= buf_size)
        {
            tmp.resize(n + 1, L'\0');
            n = GetEnvironmentVariableW(name.get(), &tmp[0], static_cast<unsigned>(tmp.size() - 1));
            // The size may have changed
            if(n >= tmp.size() - 1)
                return 0;
            ptr = &tmp[0];
        }
        value.convert(ptr);
        return value.get();
    }
    ///
    /// \brief  UTF-8 aware setenv, \a key - the variable name, \a value is a new UTF-8 value,
    ///
    /// if overwrite is not 0, that the old value is always overwritten, otherwise,
    /// if the variable exists it remains unchanged
    ///
    inline int setenv(const char *key, const char *value, int overwrite)
    {
        const wshort_stackstring name(key);
        if(!overwrite)
        {
            wchar_t unused[2];
            if(!(GetEnvironmentVariableW(name.get(), unused, 2) == 0 && GetLastError() == 203)) // ERROR_ENVVAR_NOT_FOUND
                return 0;
        }
        const wstackstring wval(value);
        if(SetEnvironmentVariableW(name.get(), wval.get()))
            return 0;
        return -1;
    }
    ///
    /// \brief Remove environment variable \a key
    ///
    inline int unsetenv(const char *key)
    {
        const wshort_stackstring name(key);
        if(SetEnvironmentVariableW(name.get(), 0))
            return 0;
        return -1;
    }
    ///
    /// \brief UTF-8 aware putenv implementation, expects string in format KEY=VALUE
    ///
    inline int putenv(char *string)
    {
        const char *key = string;
        const char *key_end = string;
        while(*key_end != '=' && *key_end != '\0')
            key_end++;
        if(*key_end == '\0')
            return -1;
        const wshort_stackstring wkey(key, key_end);
        const wstackstring wvalue(key_end + 1);

        if(SetEnvironmentVariableW(wkey.get(), wvalue.get()))
            return 0;
        return -1;
    }

    ///
    /// Same as std::system but cmd is UTF-8.
    ///
    inline int system(const char *cmd)
    {
        if(!cmd)
            return _wsystem(0);
        const wstackstring wcmd(cmd);
        return _wsystem(wcmd.get());
    }
#endif
} // namespace nowide
} // namespace boost

#endif
