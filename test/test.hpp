//
//  Copyright (c) 2012 Artyom Beilis (Tonkikh)
//  Copyright (c) 2019 Alexander Grund
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_NOWIDE_LIB_TEST_H_INCLUDED
#define BOOST_NOWIDE_LIB_TEST_H_INCLUDED

#include <sstream>
#include <stdexcept>

/// Function called when a test failed to be able set a breakpoint for debugging
inline void test_failed(const std::string& msg)
{
    throw std::runtime_error(msg);
}

#define TEST(x)                                                                         \
    do                                                                                  \
    {                                                                                   \
        if(x)                                                                           \
            break;                                                                      \
        std::ostringstream ss;                                                          \
        ss << "Error " #x " in " << __FILE__ << ':' << __LINE__ << " " << __FUNCTION__; \
        test_failed(ss.str());                                                          \
    } while(0)

#endif // #ifndef BOOST_NOWIDE_LIB_TEST_H_INCLUDED
