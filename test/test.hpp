//
//  Copyright (c) 2012 Artyom Beilis (Tonkikh)
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_NOWIDE_LIB_TEST_H_INCLUDED
#define BOOST_NOWIDE_LIB_TEST_H_INCLUDED

#if 0

#include <iostream>
#include <sstream>
#include <stdexcept>

#define TEST(x)                                                                         \
    do                                                                                  \
    {                                                                                   \
        if(x)                                                                           \
            break;                                                                      \
        std::ostringstream ss;                                                          \
        ss << "Error " #x " in " << __FILE__ << ':' << __LINE__ << " " << __FUNCTION__; \
        throw std::runtime_error(ss.str());                                             \
    } while(0)

namespace boost
{

int report_errors()
{
    std::cout << "Ok" << std::endl;
    return 0;
}
#define BOOST_NOWIDE_TEST_RETURN_FAILURE return 1

} // namespace boost

#else

#include <boost/core/lightweight_test.hpp>
#define TEST BOOST_TEST
#define BOOST_NOWIDE_TEST_RETURN_FAILURE \
    BOOST_TEST(false);                   \
    return boost::report_errors()

#endif

#endif // #ifndef BOOST_NOWIDE_LIB_TEST_H_INCLUDED
///
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
