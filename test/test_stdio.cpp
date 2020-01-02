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

#include <boost/nowide/cstdio.hpp>
#include <cstring>
#include <iostream>

#include "test.hpp"

int main(int, char **argv)
{
    const std::string prefix = argv[0];
    const std::string example = prefix + "\xd7\xa9-\xd0\xbc-\xce\xbd.txt";

    try
    {
#ifdef BOOST_WINDOWS
        FILE *f = _wfopen(boost::nowide::widen(example).c_str(), L"w");
#else
        FILE *f = std::fopen(example.c_str(), "w");
#endif
        TEST(f);
        std::fprintf(f, "test\n");
        std::fclose(f);
        f = 0;

        TEST((f = boost::nowide::fopen(example.c_str(), "r")) != 0);
        char buf[16];
        TEST(std::fgets(buf, 16, f) != 0);
        TEST(std::strcmp(buf, "test\n") == 0);
        TEST((f = boost::nowide::freopen(example.c_str(), "r+", f)) != 0);
        std::fclose(f);
        f = 0;

        TEST(boost::nowide::rename(example.c_str(), (example + ".1").c_str()) == 0);
        TEST(boost::nowide::remove(example.c_str()) < 0);
        TEST((f = boost::nowide::fopen((example + ".1").c_str(), "r")) != 0);
        std::fclose(f);
        f = 0;
        TEST(boost::nowide::remove(example.c_str()) < 0);
        TEST(boost::nowide::remove((example + ".1").c_str()) == 0);
    } catch(std::exception const &e)
    {
        std::cerr << "Failed " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

///
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
