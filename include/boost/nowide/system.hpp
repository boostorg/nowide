//
//  Copyright (c) 2012 Artyom Beilis (Tonkikh)
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_NOWIDE_CSTDLIB_HPP
#define BOOST_NOWIDE_CSTDLIB_HPP

#include <boost/config.hpp>
#ifdef BOOST_WINDOWS
#include <boost/nowide/stackstring.hpp>
#endif
#include <cstdlib>

namespace boost {
namespace nowide {

#if !defined(BOOST_WINDOWS) && !defined(BOOST_NOWIDE_DOXYGEN)

    using std::system;

#else // Windows

    ///
    /// Same as std::system but cmd is UTF-8.
    ///
    inline int system(char const *cmd)
    {
        if(!cmd)
            return _wsystem(0);
        wstackstring const wcmd(cmd);
        return _wsystem(wcmd.c_str());
    }

#endif
} // namespace nowide
} // namespace boost

#endif
///
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
