//
//  Copyright (c) 2012 Artyom Beilis (Tonkikh)
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/nowide/system.hpp>
#include <boost/nowide/args.hpp>
#include <boost/nowide/cenv.hpp>
#include <boost/nowide/cppenv.hpp>
#include <iostream>
#include "test.hpp"

int main(int argc,char **argv,char **env)
{
    try {
        std::string example = "\xd7\xa9-\xd0\xbc-\xce\xbd";
        std::wstring wexample = L"\u05e9-\u043c-\u03bd";
        boost::nowide::args a(argc,argv,env);
        if(argc==2 && argv[1][0]!='-') {
            TEST(argv[1]==example);
            TEST(argv[2] == 0);
            TEST(boost::nowide::getenv("BOOST_NOWIDE_TEST"));
            TEST(boost::nowide::has_enironment("BOOST_NOWIDE_TEST"));
            TEST(boost::nowide::getenv("BOOST_NOWIDE_TEST_NONE") == 0);
            TEST(!boost::nowide::has_enironment("BOOST_NOWIDE_TEST_NONE"));
            TEST(boost::nowide::getenv("BOOST_NOWIDE_TEST")==example);
            TEST(boost::nowide::get_environment("BOOST_NOWIDE_TEST")==example);
            TEST(boost::nowide::get_environment("BOOST_NOWIDE_TEST","default")==example);
            TEST(boost::nowide::get_environment("BOOST_NOWIDE_TEST_NONE","default")=="default");
            std::string sample = "BOOST_NOWIDE_TEST=" + example;
            bool found = false;
            std::map<std::string,std::string> strs = boost::nowide::get_environment_strings();
            size_t total = 0;
            for(char **e=env;*e!=0;e++) {
                char *key_end = strchr(*e,'=');
                TEST(key_end);
                std::string key = std::string(*e,key_end);
                std::string value = key_end + 1;
                TEST(strs.count(key) == 1);
                TEST(strs[key]==value);
                if(*e == sample)
                    found = true;
                total ++;
            }
            TEST(found);
            TEST(strs.size() == total);
            std::cout << "Subprocess ok" << std::endl;
        }
        else if(argc==2 && argv[1][0]=='-') {
            switch(argv[1][1]) {
            case 'w': 
                {
                    #ifdef BOOST_WINDOWS
                    std::wstring env = L"BOOST_NOWIDE_TEST=" + wexample;
                    _wputenv(env.c_str());
                    std::wstring wcommand = boost::nowide::convert(argv[0]);
                    wcommand += L" ";
                    wcommand += wexample;
                    TEST(_wsystem(wcommand.c_str()) == 0);
                    std::cout << "Wide Parent ok" << std::endl;
                    #else
                    std::cout << "Wide API is irrelevant" << std::endl;
                    #endif
                }
                return 0;
            case '1':
                TEST(boost::nowide::set_environment(std::string("BOOST_NOWIDE_TEST"),example) == 0);
                TEST(boost::nowide::set_environment(std::string("BOOST_NOWIDE_TEST_NONE"),example) == 0);
                TEST(boost::nowide::unset_environment(std::string("BOOST_NOWIDE_TEST_NONE")) == 0);
                break;
            case '2':
                TEST(boost::nowide::set_environment("BOOST_NOWIDE_TEST",example.c_str()) == 0);
                TEST(boost::nowide::set_environment("BOOST_NOWIDE_TEST_NONE",example.c_str()) == 0);
                TEST(boost::nowide::unset_environment("BOOST_NOWIDE_TEST_NONE") == 0);
                break;
            case '3':
                TEST(boost::nowide::setenv("BOOST_NOWIDE_TEST",example.c_str(),1) == 0);
                TEST(boost::nowide::setenv("BOOST_NOWIDE_TEST_NONE",example.c_str(),1) == 0);
                TEST(boost::nowide::unsetenv("BOOST_NOWIDE_TEST_NONE") == 0);
                break;
            default:
                std::cout << "Invalid parameters expected '-1/-2/-3/-w'" << std::endl;
                return 1;
            }
            std::string command  = argv[0];
            command += " ";
            command += example;
            TEST(boost::nowide::system(command.c_str()) == 0);
            std::cout << "Parent ok" << std::endl;
        }
        else {
            std::cerr << "Invalid parameters" << std::endl;
            return 1;
        }
        return 0;
    }
    catch(std::exception const &e) {
        std::cerr << "Failed " << e.what() << std::endl;
        return 1;
    }
}

///
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
