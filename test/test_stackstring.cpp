//
//  Copyright (c) 2012 Artyom Beilis (Tonkikh)
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "test.hpp"
#include "test_sets.hpp"
#include <boost/nowide/stackstring.hpp>
#include <iostream>

#if defined(BOOST_MSVC) && BOOST_MSVC < 1700
#pragma warning(disable : 4428) // universal-character-name encountered in source
#endif

std::wstring stackstring_to_wide(std::string const &s)
{
    const boost::nowide::wstackstring ss(s.c_str());
    return ss.c_str();
}

std::string stackstring_to_narrow(std::wstring const &s)
{
    const boost::nowide::stackstring ss(s.c_str());
    return ss.c_str();
}

int main()
{
    try
    {
        std::string hello = "\xd7\xa9\xd7\x9c\xd7\x95\xd7\x9d";
        std::wstring whello = L"\u05e9\u05dc\u05d5\u05dd";

        {
            boost::nowide::short_stackstring const s;
            TEST(s.c_str());
            TEST(*s.c_str() == '\0');
        }
        {
            TEST(whello.size() >= 3);
            boost::nowide::basic_stackstring<wchar_t, char, 3> sw;
            TEST(sw.convert(hello.c_str()));
            TEST(sw.c_str() == whello);
            TEST(sw.convert(hello.c_str(), hello.c_str() + hello.size()));
            TEST(sw.c_str() == whello);
        }
        {
            TEST(whello.size() < 5);
            boost::nowide::basic_stackstring<wchar_t, char, 5> sw;
            TEST(sw.convert(hello.c_str()));
            TEST(sw.c_str() == whello);
            TEST(sw.convert(hello.c_str(), hello.c_str() + hello.size()));
            TEST(sw.c_str() == whello);
        }
        {
            TEST(hello.size() >= 5);
            boost::nowide::basic_stackstring<char, wchar_t, 5> sw;
            TEST(sw.convert(whello.c_str()));
            TEST(sw.c_str() == hello);
            TEST(sw.convert(whello.c_str(), whello.c_str() + whello.size()));
            TEST(sw.c_str() == hello);
        }
        {
            TEST(hello.size() < 10);
            boost::nowide::basic_stackstring<char, wchar_t, 10> sw;
            TEST(sw.convert(whello.c_str()));
            TEST(sw.c_str() == hello);
            TEST(sw.convert(whello.c_str(), whello.c_str() + whello.size()));
            TEST(sw.c_str() == hello);
        }
        {
            typedef boost::nowide::basic_stackstring<char, wchar_t, 5> stackstring;
            const std::string heapVal = hello;
            TEST(heapVal.size() >= 5); // Will be put on heap
            const std::wstring wtest = L"test";
            const std::string stackVal = "test";
            TEST(stackVal.size() < 5); // Will be put on stack
            stackstring const heap(whello.c_str());
            stackstring const stack(wtest.c_str());

            {
                stackstring sw2(heap), sw3;
                sw3 = heap;
                TEST(sw2.c_str() == heapVal);
                TEST(sw3.c_str() == heapVal);
            }
            {
                stackstring sw2(stack), sw3;
                sw3 = stack;
                TEST(sw2.c_str() == stackVal);
                TEST(sw3.c_str() == stackVal);
            }
            {
                stackstring sw2(stack);
                sw2 = heap;
                TEST(sw2.c_str() == heapVal);
            }
            {
                stackstring sw2(heap);
                sw2 = stack;
                TEST(sw2.c_str() == stackVal);
            }
            {
                stackstring sw2(heap), sw3(stack);
                swap(sw2, sw3);
                TEST(sw2.c_str() == stackVal);
                TEST(sw3.c_str() == heapVal);
                swap(sw2, sw3);
                TEST(sw2.c_str() == heapVal);
                TEST(sw3.c_str() == stackVal);
            }
            {
                stackstring sw2(heap), sw3(heap);
                sw3.c_str()[0] = 'z';
                const std::string val2 = sw3.c_str();
                swap(sw2, sw3);
                TEST(sw2.c_str() == val2);
                TEST(sw3.c_str() == heapVal);
            }
            {
                stackstring sw2(stack), sw3(stack);
                sw3.c_str()[0] = 'z';
                const std::string val2 = sw3.c_str();
                swap(sw2, sw3);
                TEST(sw2.c_str() == val2);
                TEST(sw3.c_str() == stackVal);
            }
            // Sanity check
            TEST(stack.c_str() == stackVal);
            TEST(heap.c_str() == heapVal);
        }
        std::cout << "- Substitutions" << std::endl;
        run_all(stackstring_to_wide, stackstring_to_narrow);
    } catch(std::exception const &e)
    {
        std::cerr << "Failed :" << e.what() << std::endl;
        return 1;
    }

    return 0;
}

///
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
