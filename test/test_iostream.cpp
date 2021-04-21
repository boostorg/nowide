//
//  Copyright (c) 2015 Artyom Beilis (Tonkikh)
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/nowide/iostream.hpp>

#include <boost/nowide/utf/utf.hpp>
#include "test.hpp"
#include <limits>
#include <string>

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

namespace nw = boost::nowide;

void test_main(int argc, char** argv, char**)
{
    const char* example = "Basic letters: \xd7\xa9-\xd0\xbc-\xce\xbd\n"
                          "East Asian Letters: \xe5\x92\x8c\xe5\xb9\xb3\n"
                          "Non-BMP letters: \xf0\x9d\x84\x9e\n"
                          "Invalid UTF-8: `\xFF' `\xd7\xFF' `\xe5\xFF\x8c' `\xf0\x9d\x84\xFF' \n"
                          "\n";

    // If we are using the standard rdbuf we can only put back 1 char
    if(nw::cin.rdbuf() == std::cin.rdbuf())
    {
        std::cout << "Using std::cin" << std::endl;
        int maxval = 15000;
        for(int i = 0; i < maxval; i++)
        {
            char c = i % 96 + ' ';
            TEST(nw::cin.putback(c));
            int ci = i % 96 + ' ';
            TEST(nw::cin.get() == ci);
        }
    } else
    {
        int maxval = 15000;
        for(int i = 0; i < maxval; i++)
        {
            char c = i % 96 + ' ';
            TEST(nw::cin.putback(c));
        }
        for(int i = maxval - 1; i >= 0; i--)
        {
            int c = i % 96 + ' ';
            TEST(nw::cin.get() == c);
        }
    }
    nw::cout << "Normal I/O:" << std::endl;
    nw::cout << example << std::endl;
    nw::cerr << example << std::endl;

    nw::cout << "Flushing each character:" << std::endl;

    for(const char* s = example; *s; s++)
    {
        nw::cout << *s << std::flush;
        TEST(nw::cout);
    }

    TEST(nw::cout);
    TEST(nw::cerr);
    if(argc == 2 && argv[1] == std::string("-i"))
    {
        nw::cout << "Input 2 strings" << std::endl;
        std::string v1, v2;
        nw::cin >> v1 >> v2;
        TEST(nw::cin);
        TEST(isValidUTF8(v1));
        TEST(isValidUTF8(v2));
        nw::cout << "First:  " << v1 << std::endl;
        nw::cout << "Second: " << v2 << std::endl;
        TEST(nw::cout);

        // Check sync
        nw::cout << "Input 2 strings\n";
        nw::cout.flush();
        TEST(nw::cin >> v1);
        nw::cin.sync();
        nw::cout << "First:  " << v1 << std::endl;
        nw::cout << "2nd string should be ignored. Input 1 more + [ENTER]" << std::endl;
        // And check getline not getting the CR
        TEST(std::getline(nw::cin, v1));
        TEST(!v1.empty());
        TEST(v1[v1.size() - 1u] != '\r');
        nw::cout << "Value:  " << v1 << std::endl;

        nw::cout << "Press ENTER to exit";
        nw::cin.clear();
        nw::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        nw::cin.get();
    }
}
