//  Copyright (c) 2015 Artyom Beilis (Tonkikh)
//  Copyright (c) 2019-2021 Alexander Grund
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <boost/nowide/fstream.hpp>

#include <string>

#include "file_test_helpers.hpp"
#include "test.hpp"

namespace nw = boost::nowide;
using namespace boost::nowide::test;

void test_ifstream_open_read(const char* filename)
{
    // Create test file
    {
        nw::ofstream fo(filename);
        TEST(fo << "test" << std::endl);
    }

    // char* Ctor
    {
        nw::ifstream fi(filename);
        TEST(fi);
        std::string tmp;
        TEST(fi >> tmp);
        TEST(tmp == "test");
    }
    // char* open
    {
        nw::ifstream fi;
        fi.open(filename);
        TEST(fi);
        std::string tmp;
        TEST(fi >> tmp);
        TEST(tmp == "test");
    }
    // string ctor
    {
        std::string name = filename;
        nw::ifstream fi(name);
        TEST(fi);
        std::string tmp;
        TEST(fi >> tmp);
        TEST(tmp == "test");
    }
    // string open
    {
        nw::ifstream fi;
        fi.open(std::string(filename));
        TEST(fi);
        std::string tmp;
        TEST(fi >> tmp);
        TEST(tmp == "test");
    }
    // Binary mode
    {
        nw::ifstream fi(filename, std::ios::binary);
        TEST(fi);
        std::string tmp;
        TEST(fi >> tmp);
        TEST(tmp == "test");
    }
    // At end
    {
        // Need binary file or position check might be throw off by newline conversion
        {
            nw::ofstream fo(filename, nw::fstream::binary);
            TEST(fo << "test");
        }
        nw::ifstream fi(filename, nw::fstream::ate | nw::fstream::binary);
        TEST(fi);
        TEST(fi.tellg() == std::streampos(4));
        fi.seekg(-2, std::ios_base::cur);
        std::string tmp;
        TEST(fi >> tmp);
        TEST(tmp == "st");
    }
    // Fail on non-existing file
    TEST(nw::remove(filename) == 0);
    {
        nw::ifstream fi(filename);
        TEST(!fi);
    }
}

void test_main(int, char** argv, char**)
{
    const std::string exampleFilename = std::string(argv[0]) + "-\xd7\xa9-\xd0\xbc-\xce\xbd.txt";

    test_ifstream_open_read(exampleFilename.c_str());
}
