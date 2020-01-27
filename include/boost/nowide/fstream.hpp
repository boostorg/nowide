//
//  Copyright (c) 2012 Artyom Beilis (Tonkikh)
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_NOWIDE_FSTREAM_HPP_INCLUDED
#define BOOST_NOWIDE_FSTREAM_HPP_INCLUDED

#include <boost/nowide/config.hpp>
#include <boost/nowide/filebuf.hpp>
#include <istream>
#include <ostream>

namespace boost {
namespace nowide {
    /// \cond INTERNAL
    namespace detail {
        // clang-format off
        struct StreamTypeIn
        {
            static std::ios_base::openmode mode() { return std::ios_base::in; }
            static std::ios_base::openmode mode_modifier() { return mode(); }
        };
        struct StreamTypeOut
        {
            static std::ios_base::openmode mode() { return std::ios_base::out; }
            static std::ios_base::openmode mode_modifier() { return mode(); }
        };
        struct StreamTypeInOut
        {
            static std::ios_base::openmode mode() { return std::ios_base::in | std::ios_base::out; }
            static std::ios_base::openmode mode_modifier() { return std::ios_base::openmode(); }
        };
        // clang-format on

        /// CRTP Base class for all basic_*fstream classes
        /// Contains basic_filebuf instance so its pointer can be used to construct basic_*stream
        /// Provides common functions to reduce boilerplate code
        /// \tparam T_StreamType One of StreamType* above.
        ///         Class used instead of value, because openmode::operator| may not be constexpr
        template<typename T_Basic_FStream, typename T_StreamType>
        class fstream_impl;

        template<typename Path, typename Result>
        struct enable_if_path;
    } // namespace detail
    /// \endcond

    ///
    /// \brief Same as std::basic_ifstream<char> but accepts UTF-8 strings under Windows
    ///
    template<typename CharType, typename Traits = std::char_traits<CharType> >
    class basic_ifstream : public detail::fstream_impl<basic_ifstream<CharType, Traits>, detail::StreamTypeIn>,
                           public std::basic_istream<CharType, Traits>
    {
        typedef detail::fstream_impl<basic_ifstream, detail::StreamTypeIn> fstream_impl;
        typedef std::basic_istream<CharType, Traits> stream_base;

    public:
        basic_ifstream() : stream_base(&this->buf_)
        {}

        explicit basic_ifstream(const char* file_name, std::ios_base::openmode mode = std::ios_base::in) :
            stream_base(&this->buf_)
        {
            open(file_name, mode);
        }
#ifdef BOOST_WINDOWS
        explicit basic_ifstream(const wchar_t* file_name, std::ios_base::openmode mode = std::ios_base::in) :
            stream_base(&this->buf_)
        {
            open(file_name, mode);
        }
#endif

        explicit basic_ifstream(const std::string& file_name, std::ios_base::openmode mode = std::ios_base::in) :
            stream_base(&this->buf_)
        {
            open(file_name, mode);
        }

        template<typename Path>
        explicit basic_ifstream(
          const Path& file_name,
          typename detail::enable_if_path<Path, std::ios_base::openmode>::type mode = std::ios_base::in) :
            stream_base(&this->buf_)
        {
            open(file_name, mode);
        }
        using fstream_impl::open;
        using fstream_impl::is_open;
        using fstream_impl::close;
        using fstream_impl::rdbuf;
    };

    ///
    /// \brief Same as std::basic_ofstream<char> but accepts UTF-8 strings under Windows
    ///

    template<typename CharType, typename Traits = std::char_traits<CharType> >
    class basic_ofstream : public detail::fstream_impl<basic_ofstream<CharType, Traits>, detail::StreamTypeOut>,
                           public std::basic_ostream<CharType, Traits>
    {
        typedef detail::fstream_impl<basic_ofstream, detail::StreamTypeOut> fstream_impl;
        typedef std::basic_ostream<CharType, Traits> stream_base;

    public:
        basic_ofstream() : stream_base(&this->buf_)
        {}
        explicit basic_ofstream(const char* file_name, std::ios_base::openmode mode = std::ios_base::out) :
            stream_base(&this->buf_)
        {
            open(file_name, mode);
        }
#ifdef BOOST_WINDOWS
        explicit basic_ofstream(const wchar_t* file_name, std::ios_base::openmode mode = std::ios_base::out) :
            stream_base(&this->buf_)
        {
            open(file_name, mode);
        }
#endif
        explicit basic_ofstream(const std::string& file_name, std::ios_base::openmode mode = std::ios_base::out) :
            stream_base(&this->buf_)
        {
            open(file_name, mode);
        }
        template<typename Path>
        explicit basic_ofstream(
          const Path& file_name,
          typename detail::enable_if_path<Path, std::ios_base::openmode>::type mode = std::ios_base::out) :
            stream_base(&this->buf_)
        {
            open(file_name, mode);
        }

        using fstream_impl::open;
        using fstream_impl::is_open;
        using fstream_impl::close;
        using fstream_impl::rdbuf;
    };

#ifdef BOOST_MSVC
#pragma warning(push)
#pragma warning(disable : 4250) // <class> : inherits <method> via dominance
#endif
    ///
    /// \brief Same as std::basic_fstream<char> but accepts UTF-8 strings under Windows
    ///
    template<typename CharType, typename Traits = std::char_traits<CharType> >
    class basic_fstream : public detail::fstream_impl<basic_fstream<CharType, Traits>, detail::StreamTypeInOut>,
                          public std::basic_iostream<CharType, Traits>
    {
        typedef detail::fstream_impl<basic_fstream, detail::StreamTypeInOut> fstream_impl;
        typedef std::basic_iostream<CharType, Traits> stream_base;

    public:
        basic_fstream() : stream_base(&this->buf_)
        {}
        explicit basic_fstream(const char* file_name,
                               std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out) :
            stream_base(&this->buf_)
        {
            open(file_name, mode);
        }
#ifdef BOOST_WINDOWS
        explicit basic_fstream(const wchar_t* file_name,
                               std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out) :
            stream_base(&this->buf_)
        {
            open(file_name, mode);
        }
#endif
        explicit basic_fstream(const std::string& file_name,
                               std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out) :
            stream_base(&this->buf_)
        {
            open(file_name, mode);
        }
        template<typename Path>
        explicit basic_fstream(const Path& file_name,
                               typename detail::enable_if_path<Path, std::ios_base::openmode>::type mode =
                                 std::ios_base::in | std::ios_base::out) :
            stream_base(&this->buf_)
        {
            open(file_name, mode);
        }

        using fstream_impl::open;
        using fstream_impl::is_open;
        using fstream_impl::close;
        using fstream_impl::rdbuf;
    };
#ifdef BOOST_MSVC
#pragma warning(pop)
#endif

    ///
    /// Same as std::filebuf but accepts UTF-8 strings under Windows
    ///
    typedef basic_filebuf<char> filebuf;
    ///
    /// Same as std::ifstream but accepts UTF-8 strings under Windows
    /// and *::filesystem::path on all systems
    ///
    typedef basic_ifstream<char> ifstream;
    ///
    /// Same as std::ofstream but accepts UTF-8 strings under Windows
    /// and *::filesystem::path on all systems
    ///
    typedef basic_ofstream<char> ofstream;
    ///
    /// Same as std::fstream but accepts UTF-8 strings under Windows
    /// and *::filesystem::path on all systems
    ///
    typedef basic_fstream<char> fstream;

    // Implementation
    namespace detail {
        template<template<typename, typename> class T_Basic_FStream,
                 typename CharType,
                 typename Traits,
                 typename T_StreamType>
        class fstream_impl<T_Basic_FStream<CharType, Traits>, T_StreamType>
        {
            typedef T_Basic_FStream<CharType, Traits> Underlying;
            typedef basic_filebuf<CharType, Traits> internal_buffer_type;

            Underlying* underlying()
            {
                return static_cast<Underlying*>(this);
            }

        protected:
            void open(const std::string& file_name, std::ios_base::openmode mode = T_StreamType::mode())
            {
                open(file_name.c_str(), mode);
            }
            template<typename Path>
            typename detail::enable_if_path<Path, void>::type open(const Path& file_name,
                                                                   std::ios_base::openmode mode = T_StreamType::mode())
            {
                return open(file_name.c_str(), mode);
            }
            void open(const char* file_name, std::ios_base::openmode mode = T_StreamType::mode())
            {
                if(!buf_.open(file_name, mode | T_StreamType::mode_modifier()))
                    underlying()->setstate(std::ios_base::failbit);
                else
                    underlying()->clear();
            }
#ifdef BOOST_WINDOWS
            void open(const wchar_t* file_name, std::ios_base::openmode mode = T_StreamType::mode())
            {
                if(!buf_.open(file_name, mode | T_StreamType::mode_modifier()))
                    underlying()->setstate(std::ios_base::failbit);
                else
                    underlying()->clear();
            }
#endif
            bool is_open()
            {
                return buf_.is_open();
            }
            bool is_open() const
            {
                return buf_.is_open();
            }
            void close()
            {
                if(!buf_.close())
                    underlying()->setstate(std::ios_base::failbit);
            }

            internal_buffer_type* rdbuf() const
            {
                return const_cast<internal_buffer_type*>(&this->buf_);
            }

            internal_buffer_type buf_;
        };

        /// Trait to heuristically check for a *::filesystem::path
        /// Done by checking for make_preferred and filename member functions with correct signature
        template<typename T>
        class is_path
        {
            typedef char one;
            struct two
            {
                char dummy[2];
            };

            template<typename U, U& (U::*)(), U (U::*)() const>
            struct Check;
            template<typename U>
            static one test(Check<U, &U::make_preferred, &U::filename>*);
            template<typename U>
            static two test(...);

        public:
            enum
            {
                value = sizeof(test<T>(0)) == sizeof(one)
            };
        };
        template<bool B, typename T>
        struct enable_if
        {};
        template<typename T>
        struct enable_if<true, T>
        {
            typedef T type;
        };
        /// SFINAE trait which has a member "type = Result" if the Path is a *::filesystem::path
        template<typename Path, typename Result>
        struct enable_if_path : enable_if<is_path<Path>::value, Result>
        {};
    } // namespace detail
} // namespace nowide
} // namespace boost

#endif
