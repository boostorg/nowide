//
//  Copyright (c) 2012 Artyom Beilis (Tonkikh)
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_NOWIDE_CSTDIO_H_INCLUDED
#define BOOST_NOWIDE_CSTDIO_H_INCLUDED

#include <cstdio>
#include <stdio.h>
#include <boost/config.hpp>
#include <boost/nowide/convert.hpp>
#include <boost/nowide/stackstring.hpp>

#ifdef BOOST_MSVC
#  pragma warning(push)
#  pragma warning(disable : 4996)
#endif


namespace boost {
namespace nowide {
#ifndef BOOST_WINDOWS
    using std::fopen;
    using std::freopen;
    using std::remove;
    using std::rename;
#else

inline FILE *freopen(char const *file_name,char const *mode,FILE *stream)
{
    wstackstring wname;
    wshort_stackstring wmode;
    if(!wname.convert(file_name) || !wmode.convert(mode))
        return 0;
    return _wfreopen(wname.c_str(),wmode.c_str(),stream);
}
inline FILE *fopen(char const *file_name,char const *mode)
{
    wstackstring wname;
    wshort_stackstring wmode;
    if(!wname.convert(file_name) || !wmode.convert(mode))
        return 0;
    return _wfopen(wname.c_str(),wmode.c_str());
}
inline int rename(char const *old_name,char const *new_name)
{
    wstackstring wold,wnew;
    if(!wold.convert(old_name) || !wnew.convert(new_name))
        return -1;
    return _wrename(wold.c_str(),wnew.c_str());
}
inline int remove(char const *name)
{
    wstackstring wname;
    if(!wname.convert(name))
        return -1;
    return _wremove(wname.c_str());
}
#endif
} // nowide
} // boost

#ifdef BOOST_MSVC
#pragma warning(pop)
#endif

#endif
///
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
