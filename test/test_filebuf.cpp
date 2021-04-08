//  Copyright (c) 2015 Artyom Beilis (Tonkikh)
//  Copyright (c) 2019-2021 Alexander Grund
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <boost/nowide/filebuf.hpp>

#include <string>

#include "file_test_helpers.hpp"
#include "test.hpp"

namespace nw = boost::nowide;
using namespace boost::nowide::test;

void test_close(const char* filepath)
{
    const std::string filepath2 = std::string(filepath) + ".2";
    // Make sure file does not exist yet
    TEST(!file_exists(filepath2.c_str()) || nw::remove(filepath2.c_str()) == 0);
    TEST(!file_exists(filepath2.c_str()));
    nw::filebuf buf;
    TEST(buf.open(filepath, std::ios_base::out) == &buf);
    TEST(buf.is_open());
    // Opening when already open fails
    TEST(buf.open(filepath2.c_str(), std::ios_base::out) == NULL);
    // Still open
    TEST(buf.is_open());
    TEST(buf.close() == &buf);
    // Failed opening did not create file
    TEST(!file_exists(filepath2.c_str()));
    // But it should work now:
    TEST(buf.open(filepath2.c_str(), std::ios_base::out) == &buf);
    TEST(buf.close() == &buf);
    TEST(file_exists(filepath2.c_str()));
    TEST(nw::remove(filepath) == 0);
    TEST(nw::remove(filepath2.c_str()) == 0);
}

void test_main(int, char** argv, char**)
{
    const std::string exampleFilename = std::string(argv[0]) + "-\xd7\xa9-\xd0\xbc-\xce\xbd.txt";

    test_close(exampleFilename.c_str());
}
