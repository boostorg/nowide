//
//  Copyright (c) 2012 Artyom Beilis (Tonkikh)
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifdef _MSC_VER
# define _CRT_SECURE_NO_WARNINGS
#endif

#include <boost/nowide/cstdio.hpp>
#include <boost/filesystem/operations.hpp>
#include <iostream>
#include <string.h>

#include "test.hpp"

int main()
{
    try {
        std::string prefix = boost::filesystem::unique_path( "nowide-%%%%-%%%%-" ).string();

        std::string example = prefix + "\xd7\xa9-\xd0\xbc-\xce\xbd.txt";
        std::wstring wexample = boost::nowide::widen( prefix ) + L"\u05e9-\u043c-\u03bd.txt";

        #ifdef BOOST_WINDOWS
        FILE *f=_wfopen(wexample.c_str(),L"w");
        #else
        FILE *f=std::fopen(example.c_str(),"w");
        #endif
        TEST(f);
        std::fprintf(f,"test\n");
        std::fclose(f);
        f=0;

        TEST((f=boost::nowide::fopen(example.c_str(),"r"))!=0);
        char buf[16];
        TEST(std::fgets(buf,16,f)!=0);
        TEST(strcmp(buf,"test\n")==0);
        TEST((f=boost::nowide::freopen(example.c_str(),"r+",f))!=0);
        std::fclose(f);
        f=0;

        TEST(boost::nowide::rename(example.c_str(),(example+".1").c_str())==0);
        TEST(boost::nowide::remove(example.c_str())<0);
        TEST((f=boost::nowide::fopen((example+".1").c_str(),"r"))!=0);
        std::fclose(f);
        f=0;
        TEST(boost::nowide::remove(example.c_str())<0);
        TEST(boost::nowide::remove((example+".1").c_str())==0);
    }
    catch(std::exception const &e) {
        std::cerr << "Failed " << e.what() << std::endl;
        BOOST_NOWIDE_TEST_RETURN_FAILURE;
    }

    return boost::report_errors();
}

///
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
