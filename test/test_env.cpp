//
//  Copyright (c) 2012 Artyom Beilis (Tonkikh)
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <boost/nowide/cenv.hpp>
#include <cstring>
#include <iostream>

#if defined(BOOST_NOWIDE_TEST_INCLUDE_WINDOWS) && defined(BOOST_WINDOWS)
#include <windows.h>
#endif

#include "test.hpp"

int main()
{
    try
    {
        std::string example = "\xd7\xa9-\xd0\xbc-\xce\xbd";
        char penv[256] = {0};
        strncpy(penv, ("BOOST_TEST2=" + example + "x").c_str(), sizeof(penv) - 1);

        TEST(boost::nowide::setenv("BOOST_TEST1", example.c_str(), 1) == 0);
        TEST(boost::nowide::getenv("BOOST_TEST1"));
        TEST(boost::nowide::getenv("BOOST_TEST1") == example);
        TEST(boost::nowide::setenv("BOOST_TEST1", "xx", 0) == 0);
        TEST(boost::nowide::getenv("BOOST_TEST1") == example);
        TEST(boost::nowide::putenv(penv) == 0);
        TEST(boost::nowide::getenv("BOOST_TEST2"));
        TEST(boost::nowide::getenv("BOOST_TEST_INVALID") == 0);
        TEST(boost::nowide::getenv("BOOST_TEST2") == example + "x");
#ifdef BOOST_WINDOWS
        // Passing a variable without an equals sign (before \0) is an error
        // But GLIBC has an extension that unsets the env var instead
        char penv2[256] = {0};
        const char *sPenv2 = "BOOST_TEST1SOMEGARBAGE=";
        strncpy(penv2, sPenv2, sizeof(penv2) - 1);
        // End the string before the equals sign -> Expect fail
        penv2[strlen("BOOST_TEST1")] = '\0';
        TEST(boost::nowide::putenv(penv2) == -1);
        TEST(boost::nowide::getenv("BOOST_TEST1"));
        TEST(boost::nowide::getenv("BOOST_TEST1") == example);
#endif
    } catch(std::exception const &e)
    {
        std::cerr << "Failed " << e.what() << std::endl;
        BOOST_NOWIDE_TEST_RETURN_FAILURE;
    }

    return boost::report_errors();
}

///
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
