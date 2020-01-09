//
//  Copyright (c) 2012 Artyom Beilis (Tonkikh)
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_NOWIDE_CONVERT_HPP_INCLUDED
#define BOOST_NOWIDE_CONVERT_HPP_INCLUDED

#include <boost/nowide/detail/convert.hpp>
#include <string>

namespace boost {
namespace nowide {

    ///
    /// Convert NULL terminated UTF source string to NULL terminated \a output string of size at
    /// most output_size (including NULL)
    ///
    /// In case of success output is returned, if there is not enough room NULL is returned.
    /// Any illegal sequences are replaced with the replacement character, see #BOOST_NOWIDE_REPLACEMENT_CHARACTER
    ///
    inline char* narrow(char* output, size_t output_size, const wchar_t* source)
    {
        return detail::convert_buffer(output, output_size, source, detail::strend(source));
    }
    ///
    /// Convert UTF text in range [begin,end) to NULL terminated \a output string of size at
    /// most output_size (including NULL)
    ///
    /// In case of success output is returned, if there is not enough room NULL is returned.
    /// Any illegal sequences are replaced with the replacement character, see #BOOST_NOWIDE_REPLACEMENT_CHARACTER
    ///
    inline char* narrow(char* output, size_t output_size, const wchar_t* begin, const wchar_t* end)
    {
        return detail::convert_buffer(output, output_size, begin, end);
    }
    ///
    /// Convert NULL terminated UTF source string to NULL terminated \a output string of size at
    /// most output_size (including NULL)
    ///
    /// In case of success output is returned, if there is not enough room NULL is returned.
    /// Any illegal sequences are replaced with the replacement character, see #BOOST_NOWIDE_REPLACEMENT_CHARACTER
    ///
    inline wchar_t* widen(wchar_t* output, size_t output_size, const char* source)
    {
        return detail::convert_buffer(output, output_size, source, detail::strend(source));
    }
    ///
    /// Convert UTF text in range [begin,end) to NULL terminated \a output string of size at
    /// most output_size (including NULL)
    ///
    /// In case of success output is returned, if there is not enough room NULL is returned.
    /// Any illegal sequences are replaced with the replacement character, see #BOOST_NOWIDE_REPLACEMENT_CHARACTER
    ///
    inline wchar_t* widen(wchar_t* output, size_t output_size, const char* begin, const char* end)
    {
        return detail::convert_buffer(output, output_size, begin, end);
    }

    ///
    /// Convert between Wide - UTF-16/32 string and UTF-8 string.
    ///
    /// Any illegal sequences are replaced with the replacement character, see #BOOST_NOWIDE_REPLACEMENT_CHARACTER
    ///
    inline std::string narrow(const wchar_t* s)
    {
        return detail::convert_string<char>(s, detail::strend(s));
    }
    ///
    /// Convert between UTF-8 and UTF-16 string
    ///
    /// Any illegal sequences are replaced with the replacement character, see #BOOST_NOWIDE_REPLACEMENT_CHARACTER
    ///
    inline std::wstring widen(const char* s)
    {
        return detail::convert_string<wchar_t>(s, detail::strend(s));
    }
    ///
    /// Convert between Wide - UTF-16/32 string and UTF-8 string
    ///
    /// Any illegal sequences are replaced with the replacement character, see #BOOST_NOWIDE_REPLACEMENT_CHARACTER
    ///
    inline std::string narrow(const std::wstring& s)
    {
        return detail::convert_string<char>(s.c_str(), s.c_str() + s.size());
    }
    ///
    /// Convert between UTF-8 and UTF-16 string
    ///
    /// Any illegal sequences are replaced with the replacement character, see #BOOST_NOWIDE_REPLACEMENT_CHARACTER
    ///
    inline std::wstring widen(const std::string& s)
    {
        return detail::convert_string<wchar_t>(s.c_str(), s.c_str() + s.size());
    }

} // namespace nowide
} // namespace boost

#endif
