//  Copyright (c) 2015 Artyom Beilis (Tonkikh)
//  Copyright (c) 2019-2021 Alexander Grund
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <boost/nowide/fstream.hpp>

#include <boost/nowide/cstdio.hpp>
#include "file_test_helpers.hpp"
#include "test.hpp"
#include <string>

namespace nw = boost::nowide;
using namespace boost::nowide::test;

template<typename T>
void test_ctor(const T& filename)
{
    // Create file if not exist
    ensure_not_exists(filename);
    {
        nw::ofstream f(filename);
        TEST(f);
    }
    TEST(file_exists(filename));
    TEST(read_file(filename) == "");

    // Clear file if exists
    create_file(filename, "test");
    // Default
    {
        nw::ofstream f(filename);
        TEST(f);
    }
    TEST(read_file(filename) == "");

    // At end
    {
        nw::ofstream f(filename, std::ios::ate);
        TEST(f);
    }
    TEST(read_file(filename) == "");

    // Binary mode
    {
        nw::ofstream f(filename, std::ios::binary);
        TEST(f);
        TEST(f << "test\r\n");
    }
    TEST(read_file(filename, data_type::binary) == "test\r\n");
}

template<typename T>
void test_open(const T& filename)
{
        // Create file if not exist
    ensure_not_exists(filename);
    {
        nw::ofstream f;
        f.open(filename);
        TEST(f);
    }
    TEST(file_exists(filename));
    TEST(read_file(filename) == "");

    // Clear file if exists
    create_file(filename, "test");
    // Default
    {
        nw::ofstream f;
        f.open(filename);
        TEST(f);
    }
    TEST(read_file(filename) == "");

    // At end
    {
        nw::ofstream f;
        f.open(filename, std::ios::ate);
        TEST(f);
    }
    TEST(read_file(filename) == "");

    // Binary mode
    {
        nw::ofstream f;
        f.open(filename, std::ios::binary);
        TEST(f);
        TEST(f << "test\r\n");
    }
    TEST(read_file(filename, data_type::binary) == "test\r\n");
}

void test_main(int, char** argv, char**)
{
    const std::string exampleFilename = std::string(argv[0]) + "-\xd7\xa9-\xd0\xbc-\xce\xbd.txt";

    test_ctor(exampleFilename.c_str());
    test_ctor(exampleFilename);
    test_open(exampleFilename.c_str());
    test_open(exampleFilename);
}
