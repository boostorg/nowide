//  Copyright (c) 2015 Artyom Beilis (Tonkikh)
//  Copyright (c) 2019-2021 Alexander Grund
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <boost/nowide/fstream.hpp>

#include <boost/nowide/convert.hpp>
#include <boost/nowide/cstdio.hpp>
#include <fstream>
#include <iostream>
#include <string>

#include "file_test_helpers.hpp"
#include "test.hpp"

namespace nw = boost::nowide;
using namespace boost::nowide::test;

template<class T>
void test_ctor(const T& filename)
{
    remove_file_at_exit _(filename);

    // Fail on non-existing file
    {
        ensure_not_exists(filename);
        nw::fstream f(filename);
        TEST(!f);
    }
    TEST(!file_exists(filename));

    // Create empty file
    {
        ensure_not_exists(filename);
        nw::fstream f(filename, std::ios::out);
        TEST(f);
    }
    TEST(read_file(filename).empty());

    // Read+write existing file
    create_txt_file(filename, "Hello");
    {
        nw::fstream f(filename);
        std::string tmp;
        TEST(f >> tmp);
        TEST(f.eof());
        TEST(tmp == "Hello");
        f.clear();
        TEST(f << "World");
    }
    TEST(read_file(filename) == "HelloWorld");

    // Readonly existing file
    create_txt_file(filename, "Hello");
    {
        nw::fstream f(filename, std::ios::in);
        std::string tmp;
        TEST(f >> tmp);
        TEST(tmp == "Hello");
        TEST(!(f << "World"));
    }
    TEST(read_file(filename) == "Hello");

    // Write existing file
    create_txt_file(filename, "Hello");
    {
        nw::fstream f(filename, std::ios::out);
        std::string tmp;
        TEST(!(f >> tmp));
        f.clear();
        TEST(f << "World");
    }
    TEST(read_file(filename) == "World");
    // Write existing file with explicit trunc
    create_txt_file(filename, "Hello");
    {
        nw::fstream f(filename, std::ios::out | std::ios::trunc);
        std::string tmp;
        TEST(!(f >> tmp));
        f.clear();
        TEST(f << "World");
    }
    TEST(read_file(filename) == "World");

    // append existing file
    /*create_txt_file(filename, "Hello");
    {
        std::fstream f(filename, std::ios::app);
        TEST(f << "World");
    }
    TEST(read_file(filename) == "HelloWorld");
    create_txt_file(filename, "Hello");
    {
        nw::fstream f(filename, std::ios::out | std::ios::app);
        TEST(f << "World");
    }
    TEST(read_file(filename) == "HelloWorld");
    */

    // read+write+truncate existing file
    create_txt_file(filename, "Hello");
    {
        nw::fstream f(filename, std::ios::out | std::ios::in | std::ios::trunc);
        std::string tmp;
        TEST(!(f >> tmp));
        f.clear();
        TEST(f << "World");
        f.seekg(0);
        TEST(f >> tmp);
        TEST(tmp == "World");
    }
    TEST(read_file(filename) == "World");

    // read+write+append existing file
    /*create_txt_file(filename, "Hello");
    {
        nw::fstream f(filename, std::ios::out | std::ios::in | std::ios::app);
        std::string tmp;
        TEST(f >> tmp);
        TEST(tmp == "Hello");
        f.seekg(0);
        TEST(f << "World");
    }
    TEST(read_file(filename) == "HelloWorld");
    create_txt_file(filename, "Hello");
    {
        nw::fstream f(filename, std::ios::in | std::ios::app);
        std::string tmp;
        TEST(f >> tmp);
        TEST(tmp == "Hello");
        f.seekg(0);
        TEST(f << "World");
    }
    TEST(read_file(filename) == "HelloWorld");
    */

    // Write at end
    create_txt_file(filename, "Hello");
    {
        nw::fstream f(filename, std::ios::out | std::ios::in | std::ios::ate);
        std::string tmp;
        TEST(!(f >> tmp));
        f.clear();
        TEST(f << "World");
        f.seekg(0);
        TEST(f >> tmp);
        TEST(tmp == "HelloWorld");
    }
    TEST(read_file(filename) == "HelloWorld");

    // Trunc & binary
    create_txt_file(filename, "Hello");
    {
        nw::fstream f(filename, std::ios::in | std::ios::out | std::ios::trunc | std::ios::binary);
        TEST(f);
        TEST(f << "test\r\n");
        std::string tmp(6, '\0');
        TEST(f.seekg(0));
        TEST(f.read(&tmp[0], 6));
        TEST(tmp == "test\r\n");
    }
    TEST(read_file(filename, true) == "test\r\n");
}

template<typename T>
void test_open(const T& filename)
{
    remove_file_at_exit _(filename);

    // Fail on non-existing file
    {
        ensure_not_exists(filename);
        nw::fstream f;
        f.open(filename);
        TEST(!f);
    }
    TEST(!file_exists(filename));

    // Create empty file
    {
        ensure_not_exists(filename);
        nw::fstream f;
        f.open(filename, std::ios::out);
        TEST(f);
    }
    TEST(read_file(filename).empty());

    // Read+write existing file
    create_txt_file(filename, "Hello");
    {
        nw::fstream f;
        f.open(filename);
        std::string tmp;
        TEST(f >> tmp);
        TEST(f.eof());
        TEST(tmp == "Hello");
        f.clear();
        TEST(f << "World");
    }
    TEST(read_file(filename) == "HelloWorld");

    // Readonly existing file
    create_txt_file(filename, "Hello");
    {
        nw::fstream f;
        f.open(filename, std::ios::in);
        std::string tmp;
        TEST(f >> tmp);
        TEST(tmp == "Hello");
        TEST(!(f << "World"));
    }
    TEST(read_file(filename) == "Hello");

    // remaining mode cases skipped as they are already tested by the ctor tests
}

template<typename T>
bool is_open(T& stream)
{
    // There are const and non const versions of is_open, so test both
    TEST(stream.is_open() == const_cast<const T&>(stream).is_open());
    return stream.is_open();
}

template<typename T>
void do_test_is_open(const char* filename)
{
    T f;
    TEST(!is_open(f));
    f.open(filename);
    TEST(f);
    TEST(is_open(f));
    f.close();
    TEST(f);
    TEST(!is_open(f));
}

/// Test is_open for all 3 fstream classes
void test_is_open(const char* filename)
{
    // Note the order: Output before input so file exists
    do_test_is_open<nw::ofstream>(filename);
    remove_file_at_exit _(filename);
    do_test_is_open<nw::ifstream>(filename);
    do_test_is_open<nw::fstream>(filename);
}

template<typename FStream>
void test_flush(const char* filepath)
{
    FStream fo(filepath, std::ios_base::out | std::ios::trunc);
    TEST(fo);
    std::string curValue;
    for(int repeat = 0; repeat < 2; repeat++)
    {
        for(size_t len = 1; len <= 1024; len *= 2)
        {
            char c = static_cast<char>(len % 13 + repeat + 'a'); // semi-random char
            std::string input(len, c);
            fo << input;
            curValue += input;
            TEST(fo.flush());
            std::string s;
            // Note: Flush on read area is implementation defined, so check whole file instead
            FStream fi(filepath, std::ios_base::in);
            TEST(fi >> s);
            // coverity[tainted_data]
            TEST(s == curValue);
        }
    }
}

void test_main(int, char** argv, char**)
{
    const std::string exampleFilename = std::string(argv[0]) + "-\xd7\xa9-\xd0\xbc-\xce\xbd.txt";
    const std::string exampleFilename2 = std::string(argv[0]) + "-\xd7\xa9-\xd0\xbc-\xce\xbd 2.txt";

    std::cout << "Testing fstream" << std::endl;
    test_ctor<const char*>(exampleFilename.c_str());
    test_ctor<std::string>(exampleFilename);
    test_open<const char*>(exampleFilename.c_str());
    test_open<std::string>(exampleFilename);
    test_is_open(exampleFilename.c_str());

    std::cout << "Flush - Sanity Check" << std::endl;
    test_flush<std::fstream>(exampleFilename.c_str());
    std::cout << "Flush - Test" << std::endl;
    test_flush<nw::fstream>(exampleFilename.c_str());
}
