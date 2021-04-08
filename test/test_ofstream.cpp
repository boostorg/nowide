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

void test_ofstream_creates_file(const char* filename)
{
    TEST(!file_exists(filename) || nw::remove(filename) == 0);
    TEST(!file_exists(filename));
    // Ctor
    {
        nw::ofstream fo(filename);
        TEST(fo);
    }
    {
        TEST(file_exists(filename));
        remove_file_at_exit _(filename);
        TEST(read_file(filename).empty());
    }
    // Open
    {
        nw::ofstream fo;
        fo.open(filename);
        TEST(fo);
    }
    {
        TEST(file_exists(filename));
        remove_file_at_exit _(filename);
        TEST(read_file(filename).empty());
    }
}

void test_ofstream_write(const char* filename)
{
    remove_file_at_exit _(filename);

    // char* ctor
    {
        nw::ofstream fo(filename);
        TEST(fo << "test" << 2 << std::endl);
    }
    // char* open
    TEST(read_file(filename) == "test2\n");
    TEST(nw::remove(filename) == 0);
    {
        nw::ofstream fo;
        fo.open(filename);
        TEST(fo << "test" << 2 << std::endl);
    }
    TEST(read_file(filename) == "test2\n");
    TEST(nw::remove(filename) == 0);
    // string ctor
    {
        std::string name = filename;
        nw::ofstream fo(name);
        TEST(fo << "test" << 2 << std::endl);
    }
    TEST(read_file(filename) == "test2\n");
    TEST(nw::remove(filename) == 0);
    // string open
    {
        nw::ofstream fo;
        fo.open(std::string(filename));
        TEST(fo << "test" << 2 << std::endl);
    }
    TEST(read_file(filename) == "test2\n");
    TEST(nw::remove(filename) == 0);
    // Binary mode
    {
        nw::ofstream fo(filename, std::ios::binary);
        TEST(fo << "test" << 2 << std::endl);
    }
    TEST(read_file(filename, true) == "test2\n");
    TEST(nw::remove(filename) == 0);
    // At end
    {
        {
            nw::ofstream fo(filename);
            TEST(fo << "test" << 2 << std::endl);
        }
        nw::ofstream fo(filename, std::ios::ate | std::ios::in);
        fo << "second" << 2 << std::endl;
    }
    TEST(read_file(filename) == "test2\nsecond2\n");
}

void test_main(int, char** argv, char**)
{
    const std::string exampleFilename = std::string(argv[0]) + "-\xd7\xa9-\xd0\xbc-\xce\xbd.txt";

    test_ofstream_creates_file(exampleFilename.c_str());
    test_ofstream_write(exampleFilename.c_str());
}
