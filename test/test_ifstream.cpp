//  Copyright (c) 2015 Artyom Beilis (Tonkikh)
//  Copyright (c) 2019-2021 Alexander Grund
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <boost/nowide/fstream.hpp>

#include "file_test_helpers.hpp"
#include "test.hpp"
#include <string>

namespace nw = boost::nowide;
using namespace boost::nowide::test;

template<typename T>
void test_ctor(const T& filename)
{
    // Fail on non-existing file
    ensure_not_exists(filename);
    {
        nw::ifstream f(filename);
        TEST(!f);
    }
    TEST(!file_exists(filename));

    create_file(filename, "test");

    // Default
    {
        nw::ifstream f(filename);
        TEST(f);
        std::string tmp;
        TEST(f >> tmp);
        TEST(tmp == "test");
    }
    TEST(read_file(filename) == "test");

    // At end
    {
        nw::ifstream f(filename, std::ios::ate);
        TEST(f);
        std::string tmp;
        TEST(!(f >> tmp));
        TEST(f.eof());
        f.clear();
        f.seekg(0, std::ios::beg);
        TEST(f >> tmp);
        TEST(tmp == "test");
    }
    TEST(read_file(filename) == "test");

    create_file(filename, "test\r\n", data_type::binary);
    // Binary mode
    {
        nw::ifstream f(filename, std::ios::binary);
        TEST(f);
        std::string tmp(6, '\0');
        TEST(f.read(&tmp[0], 6));
        TEST(tmp == "test\r\n");
    }
}

template<typename T>
void test_open(const T& filename)
{
    // Fail on non-existing file
    ensure_not_exists(filename);
    {
        nw::ifstream f;
        f.open(filename);
        TEST(!f);
    }
    TEST(!file_exists(filename));

    create_file(filename, "test");

    // Default
    {
        nw::ifstream f;
        f.open(filename);
        TEST(f);
        std::string tmp;
        TEST(f >> tmp);
        TEST(tmp == "test");
    }
    TEST(read_file(filename) == "test");

    // At end
    {
        nw::ifstream f;
        f.open(filename, std::ios::ate);
        TEST(f);
        std::string tmp;
        TEST(!(f >> tmp));
        TEST(f.eof());
        f.clear();
        f.seekg(0, std::ios::beg);
        TEST(f >> tmp);
        TEST(tmp == "test");
    }
    TEST(read_file(filename) == "test");

    create_file(filename, "test\r\n", data_type::binary);
    // Binary mode
    {
        nw::ifstream f;
        f.open(filename, std::ios::binary);
        TEST(f);
        std::string tmp(6, '\0');
        TEST(f.read(&tmp[0], 6));
        TEST(tmp == "test\r\n");
    }
}

void test_main(int, char** argv, char**)
{
    const std::string exampleFilename = std::string(argv[0]) + "-\xd7\xa9-\xd0\xbc-\xce\xbd.txt";

    test_ctor(exampleFilename.c_str());
    test_ctor(exampleFilename);
    test_open(exampleFilename.c_str());
    test_open(exampleFilename);
}
