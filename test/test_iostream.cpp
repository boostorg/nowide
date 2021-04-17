//  Copyright (c) 2015 Artyom Beilis (Tonkikh)
//  Copyright (c) 2020 - 2021 Alexander Grund
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <boost/nowide/iostream.hpp>

#include <boost/nowide/utf/utf.hpp>
#include "test.hpp"
#include <fstream>
#include <limits>
#include <string>

namespace nw = boost::nowide;

const std::string outputString =
  // German umlauts (aou with 2 dots), cyrillic small m, greek small nu
  "Basic letters: \xc3\xa4-\xc3\xb6-\xc3\xbc-\xd0\xbc-\xce\xbd\n"
  "East Asian Letters: \xe5\x92\x8c-\xe5\xb9\xb3\n"
  "Non-BMP letter: \xf0\x9d\x84\x9e\n" // musical symbol g clef
  "Invalid UTF-8: `\xFF' `\xd7\xFF' `\xe5\xFF\x8c' `\xf0\x9d\x84\xFF' \n"
  "\n";

const bool usesNowideRdBufIn = nw::cin.rdbuf() != std::cin.rdbuf();
const bool usesNowideRdBufOut = nw::cout.rdbuf() != std::cout.rdbuf();

bool isValidUTF8(const std::string& s)
{
    using namespace boost::nowide::utf;
    for(std::string::const_iterator it = s.begin(); it != s.end();)
    {
        code_point c = utf_traits<char>::decode(it, s.end());
        if(!is_valid_codepoint(c))
            return false;
    }
    return true;
}

void test_tie()
{
    TEST(nw::cin.tie() == &nw::cout);
    TEST(nw::cerr.tie() == &nw::cout);
    TEST((nw::cerr.flags() & std::ios_base::unitbuf) != 0);
    TEST(nw::clog.tie() == nullptr);
}

void test_putback_and_get()
{
    if(usesNowideRdBufIn)
    {
        // Test with a few small values and around power-of-2 values as buffer size doubles.
        // Finally test a large value
        for(const int num_putback_chars : {1, 2, 3, 4, 5, 7, 8, 9, 15, 16, 17, 1000})
        {
            for(int i = 0; i < num_putback_chars; i++)
            {
                char c = i % 96 + ' ';
                TEST(nw::cin.putback(c));
            }
            for(int i = num_putback_chars - 1; i >= 0; i--)
            {
                int c = i % 96 + ' ';
                TEST(nw::cin.get() == c);
            }
        }
    } else
    {
        // If we are using the standard rdbuf we can only put back 1 char
        int maxval = 15000;
        for(int i = 0; i < maxval; i++)
        {
            char c = i % 96 + ' ';
            TEST(nw::cin.putback(c));
            int ci = i % 96 + ' ';
            TEST(nw::cin.get() == ci);
        }
    }
}

void test_cout()
{
    TEST(nw::cout << outputString << std::endl);
}

void test_cout_single_char()
{
    for(const char s : outputString)
        TEST(nw::cout << s << std::flush);
    TEST(nw::cout);
}

void test_cerr()
{
    TEST(nw::cerr << outputString << std::endl);
}

void test_cerr_single_char()
{
    for(const char s : outputString)
        TEST(nw::cout << s << std::flush);
    TEST(nw::cout);
}

void test_cin()
{
    nw::cout << "Input 2 strings, e.g. 'Hello World<ENTER>'" << std::endl;
    std::string v1, v2;
    nw::cin >> v1 >> v2;
    TEST(nw::cin);
    TEST(isValidUTF8(v1));
    TEST(isValidUTF8(v2));
    TEST(nw::cout << "First:  " << v1 << std::endl);
    TEST(nw::cout << "Second: " << v2 << std::endl);

    // Check sync
    nw::cout << "Input 2 strings, e.g. 'Two more<ENTER>'" << std::endl;
    TEST(nw::cin >> v1);
    nw::cin.sync();
    nw::cout << "The 2nd string should have been ignored. Input 1 more + [ENTER]" << std::endl;
    TEST(nw::cin >> v2);
    TEST(!v2.empty());
    nw::cout << "First:  " << v1 << std::endl;
    nw::cout << "Second:  " << v2 << std::endl;
}

void test_cin_getline()
{
    std::string value;
    for(int i = 0; i < 10; i++)
    {
        nw::cout << "Input a line of text or simply press ENTER to exit, e.g. 'Hello World to you!<ENTER>'"
                 << std::endl;
        TEST(std::getline(nw::cin, value));
        if(value.empty())
        {
            nw::cout << "END\n";
            break;
        }
        // It should not include the CR
        TEST(value.back() != '\r');
        nw::cout << i << ": " << value << std::endl;
    }
}

void test_ctrl_z_is_eof()
{
    std::string value;
    nw::cout << "Input a line of text and then press CTRL+Z, e.g. 'Hello World!<ENTER><CTRL+Z><ENTER>'" << std::endl;
    TEST(std::getline(nw::cin, value));
    // It should not include the CR
    TEST(!value.empty());
    TEST(value.back() != '\r');
    nw::cout << "Value: " << value << std::endl;
    TEST(!std::getline(nw::cin, value));
    TEST(nw::cin.eof());
    nw::cin.clear();
    nw::cout << "clear() called, input another line, e.g. 'Hi there!<ENTER>'" << std::endl;
    TEST(std::getline(nw::cin, value));
    // It should not include the CR
    TEST(!value.empty());
    TEST(value.back() != '\r');
    nw::cout << "Value: " << value << std::endl;
}

// coverity [root_function]
void test_main(int argc, char** argv, char**)
{
    // Note: Intentionally using explicit if-else to verify executed code using coverage
    // Possible program parameter:
    // - None/"ci": check that output function generally work, i.e. non-interactive
    // - "interactive": interactively check the functions and their output
    // - "passthrough": Read string from cin and write to cout

    const std::string arg = (argc == 1) ? "" : argv[1];

    if(usesNowideRdBufIn)
        nw::cerr << "Using Nowide input buffer\n";
    else
        nw::cerr << "NOT using Nowide input buffer\n";
    if(usesNowideRdBufOut)
        nw::cerr << "Using Nowide output buffer\n";
    else
        nw::cerr << "NOT using Nowide output buffer\n";

    if(arg.empty() || arg == "ci")
    {
        test_tie();
        test_putback_and_get();
        test_cout();
        test_cout_single_char();
        test_cerr();
        test_cerr_single_char();
    } else if(arg == "interactive") // Interactive tests
    {
        // LCOV_EXCL_START
        nw::cout << "Output different chars:" << std::endl;
        test_cout();
        nw::cout << "Same again:" << std::endl;
        test_cout_single_char();

        nw::cout << "Same 2 outputs but to stderr:" << std::endl;
        test_cerr();
        test_cerr_single_char();

        nw::cout << "Basic cin tests:" << std::endl;
        test_cin();

        nw::cout << "getline test:" << std::endl;
        nw::cin.ignore(std::numeric_limits<int>::max(), '\n'); // Clear newline from last test
        test_cin_getline();

        nw::cout << "CTRL+Z test:" << std::endl;
        test_ctrl_z_is_eof();
        // LCOV_EXCL_STOP
    } else if(arg == "passthrough")
    {
        std::string s;
        TEST(std::getline(nw::cin, s));
        TEST(nw::cout << s);
    }
}
