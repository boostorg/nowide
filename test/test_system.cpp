//
//  Copyright (c) 2012 Artyom Beilis (Tonkikh)
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "test.hpp"
#include <boost/nowide/args.hpp>
#include <boost/nowide/cstdlib.hpp>
#include <cstring>
#include <iostream>

int main(int argc, char **argv, char **env)
{
    try
    {
        std::string example = "\xd7\xa9-\xd0\xbc-\xce\xbd";
        boost::nowide::args a(argc, argv, env);
        if(argc == 2 && argv[1][0] != '-')
        {
            TEST(argv[1] == example);
            TEST(argv[2] == 0);
            TEST(boost::nowide::getenv("BOOST_NOWIDE_TEST"));
            TEST(boost::nowide::getenv("BOOST_NOWIDE_TEST_NONE") == 0);
            TEST(boost::nowide::getenv("BOOST_NOWIDE_TEST") == example);
            std::string sample = "BOOST_NOWIDE_TEST=" + example;
            bool found = false;
            for(char **e = env; *e != 0; e++)
            {
                char *eptr = *e;
                // printf("%s\n",eptr);
                char *key_end = strchr(eptr, '=');
                TEST(key_end);
                std::string key = std::string(eptr, key_end);
                std::string value = key_end + 1;
                TEST(boost::nowide::getenv(key.c_str()));
                TEST(boost::nowide::getenv(key.c_str()) == value);
                if(*e == sample)
                    found = true;
            }
            TEST(found);
            std::cout << "Subprocess ok" << std::endl;
        } else if(argc == 1)
        {
#if BOOST_NOWIDE_TEST_USE_NARROW
            TEST(boost::nowide::setenv("BOOST_NOWIDE_TEST", example.c_str(), 1) == 0);
            TEST(boost::nowide::setenv("BOOST_NOWIDE_TEST_NONE", example.c_str(), 1) == 0);
            TEST(boost::nowide::unsetenv("BOOST_NOWIDE_TEST_NONE") == 0);
            std::string command = "\"";
            command += argv[0];
            command += "\" ";
            command += example;
            TEST(boost::nowide::system(command.c_str()) == 0);
            std::cout << "Parent ok" << std::endl;
#else
            std::wstring envVar = L"BOOST_NOWIDE_TEST=" + boost::nowide::widen(example);
            TEST(_wputenv(envVar.c_str()) == 0);
            std::wstring wcommand = boost::nowide::widen(argv[0]) + L" " + boost::nowide::widen(example);
            TEST(_wsystem(wcommand.c_str()) == 0);
            std::cout << "Wide Parent ok" << std::endl;
#endif
        }
    } catch(std::exception const &e)
    {
        std::cerr << "Failed " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

///
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
