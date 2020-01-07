//
//  Copyright (c) 2012 Artyom Beilis (Tonkikh)
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_NOWIDE_TEST_SETS_HPP_INCLUDED
#define BOOST_NOWIDE_TEST_SETS_HPP_INCLUDED

#include <boost/config.hpp>
#include <iostream>
#include <string>

struct utf8_to_wide
{
    char const *utf8;
    wchar_t const *wide;
};

struct wide_to_utf8
{
    wchar_t const *wide;
    char const *utf8;
};

#if defined(BOOST_MSVC) && BOOST_MSVC < 1700
#pragma warning(disable : 4428) // universal-character-name encountered in source
#endif

utf8_to_wide n2w_tests[] = {{"\xf0\x9d\x92\x9e-\xD0\xBF\xD1\x80\xD0\xB8\xD0\xB2\xD0\xB5\xD1\x82-\xE3\x82\x84\xE3\x81\x82.txt",
                             L"\U0001D49E-\u043F\u0440\u0438\u0432\u0435\u0442-\u3084\u3042.txt"},
                            {"\xFF\xD0\xBF\xD1\x80\xD0\xB8\xD0\xB2\xD0\xB5\xD1\x82", L"\uFFFD\u043F\u0440\u0438\u0432\u0435\u0442"},
                            {"\xD0\xBF\xD1\x80\xD0\xB8\xD0\xB2\xD0\xB5\xD1\x82\xFF", L"\u043F\u0440\u0438\u0432\u0435\u0442\uFFFD"},
                            {"\xE3\x82\xFF\xE3\x81\x82", L"\ufffd\u3042"},
                            {"\xE3\xFF\x84\xE3\x81\x82", L"\ufffd\ufffd\u3042"}};

utf8_to_wide u2w_tests[] = {{"\xf0\x9d\x92\x9e-\xD0\xBF\xD1\x80\xD0\xB8\xD0\xB2\xD0\xB5\xD1\x82-\xE3\x82\x84\xE3\x81\x82.txt",
                             L"\U0001D49E-\u043F\u0440\u0438\u0432\u0435\u0442-\u3084\u3042.txt"},
                            {"\xFF\xD0\xBF\xD1\x80\xD0\xB8\xD0\xB2\xD0\xB5\xD1\x82", L"\uFFFD\u043F\u0440\u0438\u0432\u0435\u0442"},
                            {"\xD0\xBF\xD1\x80\xD0\xB8\xD0\xB2\xD0\xB5\xD1\x82\xFF", L"\u043F\u0440\u0438\u0432\u0435\u0442\uFFFD"},
                            {"\xE3\x82\xFF\xE3\x81\x82", L"\ufffd\u3042"},
                            {"\xE3\xFF\x84\xE3\x81\x82", L"\ufffd\ufffd\u3042"}};

wide_to_utf8 w2n_tests_utf16[] = {
  {
    L"\U0001D49E-\u043F\u0440\u0438\u0432\u0435\u0442-\u3084\u3042.txt",
    "\xf0\x9d\x92\x9e-\xD0\xBF\xD1\x80\xD0\xB8\xD0\xB2\xD0\xB5\xD1\x82-\xE3\x82\x84\xE3\x81\x82.txt",
  },
  {L"\xD800\x20\u043F\u0440\u0438\u0432\u0435\u0442-\u3084\u3042",
   "\xEF\xBF\xBD\xD0\xBF\xD1\x80\xD0\xB8\xD0\xB2\xD0\xB5\xD1\x82-\xE3\x82\x84\xE3\x81\x82"},
  {L"\xDC00\x20\u043F\u0440\u0438\u0432\u0435\u0442-\u3084\u3042",
   "\xEF\xBF\xBD \xD0\xBF\xD1\x80\xD0\xB8\xD0\xB2\xD0\xB5\xD1\x82-\xE3\x82\x84\xE3\x81\x82"},
  {L"\u3084\u3042\xD800\x20\u043F\u0440\u0438\u0432\u0435\u0442-\u3084\u3042",
   "\xE3\x82\x84\xE3\x81\x82\xEF\xBF\xBD\xD0\xBF\xD1\x80\xD0\xB8\xD0\xB2\xD0\xB5\xD1\x82-\xE3\x82\x84\xE3\x81\x82"},
  {L"\u3084\u3042\xDC00\x20\u043F\u0440\u0438\u0432\u0435\u0442-\u3084\u3042",
   "\xE3\x82\x84\xE3\x81\x82\xEF\xBF\xBD \xD0\xBF\xD1\x80\xD0\xB8\xD0\xB2\xD0\xB5\xD1\x82-\xE3\x82\x84\xE3\x81\x82"}};

wide_to_utf8 w2n_tests_utf32[] = {
  {
    L"\U0001D49E-\u043F\u0440\u0438\u0432\u0435\u0442-\u3084\u3042.txt",
    "\xf0\x9d\x92\x9e-\xD0\xBF\xD1\x80\xD0\xB8\xD0\xB2\xD0\xB5\xD1\x82-\xE3\x82\x84\xE3\x81\x82.txt",
  },
  {L"\xD800\x20\u043F\u0440\u0438\u0432\u0435\u0442-\u3084\u3042",
   "\xEF\xBF\xBD \xD0\xBF\xD1\x80\xD0\xB8\xD0\xB2\xD0\xB5\xD1\x82-\xE3\x82\x84\xE3\x81\x82"},
  {L"\xDC00\x20\u043F\u0440\u0438\u0432\u0435\u0442-\u3084\u3042",
   "\xEF\xBF\xBD \xD0\xBF\xD1\x80\xD0\xB8\xD0\xB2\xD0\xB5\xD1\x82-\xE3\x82\x84\xE3\x81\x82"},
  {L"\u3084\u3042\xD800\x20\u043F\u0440\u0438\u0432\u0435\u0442-\u3084\u3042",
   "\xE3\x82\x84\xE3\x81\x82\xEF\xBF\xBD \xD0\xBF\xD1\x80\xD0\xB8\xD0\xB2\xD0\xB5\xD1\x82-\xE3\x82\x84\xE3\x81\x82"},
  {L"\u3084\u3042\xDC00\x20\u043F\u0440\u0438\u0432\u0435\u0442-\u3084\u3042",
   "\xE3\x82\x84\xE3\x81\x82\xEF\xBF\xBD \xD0\xBF\xD1\x80\xD0\xB8\xD0\xB2\xD0\xB5\xD1\x82-\xE3\x82\x84\xE3\x81\x82"}};

#ifdef BOOST_MSVC
#pragma warning(push)
#pragma warning(disable : 4127) // Constant expression detected
#endif

void run_all(std::wstring (*to_wide)(std::string const &), std::string (*to_narrow)(std::wstring const &))
{
    for(size_t i = 0; i < sizeof(n2w_tests) / sizeof(n2w_tests[0]); i++)
    {
        std::cout << "  N2W  " << i << std::endl;
        TEST(to_wide(n2w_tests[i].utf8) == n2w_tests[i].wide);
    }
    int total = 0;
    wide_to_utf8 const *ptr = 0;
    if(sizeof(wchar_t) == 2)
    {
        ptr = w2n_tests_utf16;
        total = sizeof(w2n_tests_utf16) / sizeof(w2n_tests_utf16[0]);
    } else
    {
        ptr = w2n_tests_utf32;
        total = sizeof(w2n_tests_utf32) / sizeof(w2n_tests_utf32[0]);
    }
    for(int i = 0; i < total; i++)
    {
        std::cout << "  W2N  " << i << std::endl;
        TEST(to_narrow(ptr[i].wide) == ptr[i].utf8);
    }
}

#endif

#ifdef BOOST_MSVC
#pragma warning(pop)
#endif
