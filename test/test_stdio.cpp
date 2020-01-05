//
//  Copyright (c) 2012 Artyom Beilis (Tonkikh)
//  Copyright (c) 2019 Alexander Grund
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/nowide/cstdio.hpp>
#include <cstring>
#include <iostream>

#include "test.hpp"

#ifdef BOOST_MSVC
#pragma warning(disable : 4996) // function unsafe/deprecated
#endif

bool file_exists(const std::string &filename)
{
#ifdef BOOST_WINDOWS
    FILE *f = _wfopen(boost::nowide::widen(filename).c_str(), L"r");
#else
    FILE *f = std::fopen(filename.c_str(), "r");
#endif
    bool result = false;
    if(f)
    {
        std::fclose(f);
        result = true;
    }
    return result;
}

void create_test_file(const std::string &filename)
{
#ifdef BOOST_WINDOWS
    FILE *f = _wfopen(boost::nowide::widen(filename).c_str(), L"w");
#else
    FILE *f = std::fopen(filename.c_str(), "w");
#endif
    TEST(f);
    TEST(std::fputs("test\n", f) >= 0);
    std::fclose(f);
}

int main(int, char **argv)
{
    const std::string prefix = argv[0];
    const std::string filename = prefix + "\xd7\xa9-\xd0\xbc-\xce\xbd.txt";

    try
    {
        // fopen - existing file
        {
            create_test_file(filename);
            FILE *f = boost::nowide::fopen(filename.c_str(), "r");
            TEST(f);
            char buf[16];
            TEST(std::fgets(buf, 16, f) != 0);
            TEST(strcmp(buf, "test\n") == 0);
            std::fclose(f);
        }
        // remove
        {
            create_test_file(filename);
            TEST(file_exists(filename));
            TEST(boost::nowide::remove(filename.c_str()) == 0);
            TEST(!file_exists(filename));
        }
        // fopen non-existing file
        {
            boost::nowide::remove(filename.c_str());
            TEST(!file_exists(filename));
            TEST(boost::nowide::fopen(filename.c_str(), "r") == NULL);
            TEST(!file_exists(filename));
        }
        // freopen
        {
            create_test_file(filename);
            FILE *f = boost::nowide::fopen(filename.c_str(), "r+");
            TEST(f);
            // Can read & write
            {
                char buf[32];
                TEST(std::fgets(buf, 32, f) != 0);
                TEST(strcmp(buf, "test\n") == 0);
                TEST(std::fseek(f, 0, SEEK_END) == 0);
                TEST(std::fputs("foobar\n", f) >= 0);
            }
            // Reopen in read mode
            // Note that changing the mode is not possibly on all implementations
            // E.g. MSVC disallows NULL completely as the file parameter
            FILE *f2 = boost::nowide::freopen(NULL, "r", f);
            if(!f2)
                f2 = boost::nowide::freopen(filename.c_str(), "r", f);
            // no write possible
            {
                TEST(f2 == f);
                TEST(std::fputs("not-written\n", f) < 0);
                TEST(std::fseek(f, 0, SEEK_SET) == 0);
                char buf[32];
                TEST(std::fgets(buf, 32, f) != 0);
                TEST(strcmp(buf, "test\n") == 0);
                TEST(std::fgets(buf, 32, f) != 0);
                TEST(strcmp(buf, "foobar\n") == 0);
            }
            // Reopen different file
            const std::string filename2 = filename + ".1.txt";
            TEST(boost::nowide::freopen(filename2.c_str(), "w", f) == f);
            {
                char buf[32];
                TEST(std::fputs("baz\n", f) >= 0);
                std::fclose(f);
                f = boost::nowide::fopen(filename2.c_str(), "r");
                TEST(f);
                TEST(std::fgets(buf, 32, f) != 0);
                TEST(strcmp(buf, "baz\n") == 0);
            }
            std::fclose(f);
            boost::nowide::remove(filename2.c_str());
        }
        // rename
        {
            create_test_file(filename);
            const std::string filename2 = filename + ".1.txt";
            boost::nowide::remove(filename2.c_str());
            TEST(file_exists(filename));
            TEST(!file_exists(filename2));
            TEST(boost::nowide::rename(filename.c_str(), filename2.c_str()) == 0);
            TEST(!file_exists(filename));
            TEST(file_exists(filename2));
            TEST(boost::nowide::remove(filename.c_str()) < 0);
            TEST(boost::nowide::remove(filename2.c_str()) == 0);
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
