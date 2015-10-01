//
//  Copyright (c) 2012 Artyom Beilis (Tonkikh)
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/nowide/cenv.hpp>
#include <iostream>
#include "test.hpp"

#if defined(BOOST_NOWIDE_TEST_INCLUDE_WINDOWS) && defined(BOOST_WINDOWS)
#include <windows.h>
#endif

#ifdef BOOST_MSVC
#  pragma warning(disable : 4996)
#endif


int main()
{
    try {
        std::string example = "\xd7\xa9-\xd0\xbc-\xce\xbd";
        char penv[256] = {0};
        strncpy(penv,("BOOST_TEST2=" + example + "x").c_str(),sizeof(penv)-1);
        
        TEST(boost::nowide::setenv("BOOST_TEST1",example.c_str(),1)==0);
        TEST(boost::nowide::getenv("BOOST_TEST1"));
        TEST(boost::nowide::getenv("BOOST_TEST1")==example);
        TEST(boost::nowide::setenv("BOOST_TEST1","xx",0)==0);
        TEST(boost::nowide::getenv("BOOST_TEST1")==example);
        TEST(boost::nowide::putenv(penv)==0);
        TEST(boost::nowide::getenv("BOOST_TEST2"));
        TEST(boost::nowide::getenv("BOOST_TEST_INVALID")==0);
        TEST(boost::nowide::getenv("BOOST_TEST2")==example + "x");
        
        std::cout << "Ok" << std::endl;
        return 0;
    }
    catch(std::exception const &e) {
        std::cerr << "Failed " << e.what() << std::endl;
        return 1;
    }
}

///
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
