//
//  Copyright (c) 2012 Artyom Beilis (Tonkikh)
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_NOWIDE_CSTDIO_HPP_INCLUDED
#define BOOST_NOWIDE_CSTDIO_HPP_INCLUDED

#include <boost/config.hpp>

#ifdef BOOST_WINDOWS
#include <boost/nowide/stackstring.hpp>
#endif
#include <cstdio>

#ifdef BOOST_MSVC
#pragma warning(push)
#pragma warning(disable : 4996)
#endif

namespace boost {
namespace nowide {
#if !defined(BOOST_WINDOWS) && !defined(BOOST_NOWIDE_DOXYGEN)
    using std::fopen;
    using std::freopen;
    using std::remove;
    using std::rename;
#else

    ///
    /// \brief Same as freopen but file_name and mode are UTF-8 strings
    ///
    inline FILE *freopen(const char *file_name, const char *mode, FILE *stream)
    {
        const wstackstring wname(file_name);
        const wshort_stackstring wmode(mode);
        return _wfreopen(wname.get(), wmode.get(), stream);
    }
    ///
    /// \brief Same as fopen but file_name and mode are UTF-8 strings
    ///
    inline FILE *fopen(const char *file_name, const char *mode)
    {
        const wstackstring wname(file_name);
        const wshort_stackstring wmode(mode);
        return _wfopen(wname.get(), wmode.get());
    }
    ///
    /// \brief Same as rename but old_name and new_name are UTF-8 strings
    ///
    inline int rename(const char *old_name, const char *new_name)
    {
        const wstackstring wold(old_name), wnew(new_name);
        return _wrename(wold.get(), wnew.get());
    }
    ///
    /// \brief Same as rename but name is UTF-8 string
    ///
    inline int remove(const char *name)
    {
        const wstackstring wname(name);
        return _wremove(wname.get());
    }
#endif
} // namespace nowide
} // namespace boost

#ifdef BOOST_MSVC
#pragma warning(pop)
#endif

#endif
