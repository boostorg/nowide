//
//  Copyright (c) 2012 Artyom Beilis (Tonkikh)
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_NOWIDE_CSTDLIB_HPP
#define BOOST_NOWIDE_CSTDLIB_HPP

#include <stdlib.h>
#include <errno.h>
#include <boost/nowide/stackstring.hpp>
namespace boost {
namespace nowide {
#ifndef BOOST_WINDOWS

using ::system;

#else // Windows

inline int system(char const *cmd)
{
    if(!cmd)
        return _wsystem(0);
    wstackstring wcmd;
    if(!wcmd.convert(cmd)) {
        errno = EINVAL;
        return -1;
    }
    return _wsystem(wcmd.c_str());
}

#endif
} // boost
} // nowide

#endif
///
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
