//
//  Copyright (c) 2012 Artyom Beilis (Tonkikh)
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_NOWIDE_FILEBUF_HPP
#define BOOST_NOWIDE_FILEBUF_HPP

#include <iosfwd>
#include <boost/config.hpp>
#include <boost/nowide/stackstring.hpp>
#include <fstream>
#include <streambuf>
#include <stdio.h>

#ifdef BOOST_MSVC
#  pragma warning(push)
#  pragma warning(disable : 4996 4244 4800)
#endif


namespace boost {
namespace nowide {
#if !defined(BOOST_WINDOWS) && !defined(BOOST_NOWIDE_FSTREAM_TESTS) && !defined(BOOST_NOWIDE_DOXYGEN)
    using std::basic_filebuf;
    using std::filebuf;
#else // Windows
    
    ///
    /// \brief This forward declaration defined the basic_filebuf type.
    ///
    /// it is implemented and specialized for CharType = char, it behaves
    /// implements std::filebuf over standard C I/O
    ///
    template<typename CharType,typename Traits = std::char_traits<CharType> >
    class basic_filebuf;
    
    ///
    /// \brief This is implementation of std::filebuf
    ///
    /// it is implemented and specialized for CharType = char, it behaves
    /// implements std::filebuf over standard C I/O
    ///
    template<>
    class basic_filebuf<char> : public std::basic_filebuf<char>
    {
    public:
        basic_filebuf() {}

        virtual ~basic_filebuf() {}

#if __cplusplus > 199711L
        // Move operations
        basic_filebuf(basic_filebuf &&move) :
            std::basic_filebuf<char>(std::move(move))
        {
        }
        basic_filebuf &operator=(basic_filebuf &&move)
        {
            std::basic_filebuf<char>::operator=(std::move(move));
            return *this;
        }
#endif

        ///
        /// Same as std::filebuf::open but s is UTF-8 string
        ///
        basic_filebuf *open(std::string const &s,std::ios_base::openmode mode)
        {
            return open(s.c_str(),mode);
        }
        ///
        /// Same as std::filebuf::open but s is UTF-8 string
        ///
        basic_filebuf *open(char const *s,std::ios_base::openmode mode)
        {
            // 
            wstackstring name;
            if(!name.convert(s)) 
                return 0;

            bool success = (std::basic_filebuf<char>::open(name.c_str(), mode) != NULL);

            return (success ? this : NULL);
        }
    };
    
    ///
    /// \brief Convinience typedef
    ///
    typedef basic_filebuf<char> filebuf;
    
    #endif // windows
    
} // nowide
} // namespace boost

#ifdef BOOST_MSVC
#  pragma warning(pop)
#endif


#endif

// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
