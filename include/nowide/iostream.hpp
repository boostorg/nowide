//
//  Copyright (c) 2012 Artyom Beilis (Tonkikh)
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef NOWIDE_IOSTREAM_HPP_INCLUDED
#define NOWIDE_IOSTREAM_HPP_INCLUDED

#include <nowide/config.hpp>
#ifdef NOWIDE_WINDOWS
#include <nowide/scoped_ptr.hpp>
#include <istream>
#include <ostream>

#else
#include <iostream>
#endif

#ifdef NOWIDE_MSVC
#pragma warning(push)
#pragma warning(disable : 4251)
#endif

namespace nowide {
#if !defined(NOWIDE_WINDOWS) && !defined(NOWIDE_DOXYGEN)
    using std::cout;
    using std::cerr;
    using std::cin;
    using std::clog;
#else

    /// \cond INTERNAL
    namespace detail {
        class console_output_buffer;
        class console_input_buffer;

        class NOWIDE_DECL winconsole_ostream : public std::ostream
        {
            winconsole_ostream(const winconsole_ostream&);
            void operator=(const winconsole_ostream&);

        public:
            winconsole_ostream(int fd, winconsole_ostream* tieStream);
            ~winconsole_ostream();

        private:
            nowide::scoped_ptr<console_output_buffer> d;
        };

        class NOWIDE_DECL winconsole_istream : public std::istream
        {
            winconsole_istream(const winconsole_istream&);
            void operator=(const winconsole_istream&);

        public:
            explicit winconsole_istream(winconsole_ostream* tieStream);
            ~winconsole_istream();

        private:
            nowide::scoped_ptr<console_input_buffer> d;
        };
    } // namespace detail

    /// \endcond

    ///
    /// \brief Same as std::cin, but uses UTF-8
    ///
    /// Note, the stream is not synchronized with stdio and not affected by std::ios::sync_with_stdio
    ///
    extern NOWIDE_DECL detail::winconsole_istream cin;
    ///
    /// \brief Same as std::cout, but uses UTF-8
    ///
    /// Note, the stream is not synchronized with stdio and not affected by std::ios::sync_with_stdio
    ///
    extern NOWIDE_DECL detail::winconsole_ostream cout;
    ///
    /// \brief Same as std::cerr, but uses UTF-8
    ///
    /// Note, the stream is not synchronized with stdio and not affected by std::ios::sync_with_stdio
    ///
    extern NOWIDE_DECL detail::winconsole_ostream cerr;
    ///
    /// \brief Same as std::clog, but uses UTF-8
    ///
    /// Note, the stream is not synchronized with stdio and not affected by std::ios::sync_with_stdio
    ///
    extern NOWIDE_DECL detail::winconsole_ostream clog;

#endif

} // namespace nowide

#ifdef NOWIDE_MSVC
#pragma warning(pop)
#endif

#ifdef NOWIDE_WINDOWS
#endif

#endif
