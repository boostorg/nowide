//
//  Copyright (c) 2012 Artyom Beilis (Tonkikh)
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef NOWIDE_CONFIG_H_INCLUDED
#define NOWIDE_CONFIG_H_INCLUDED

#if(defined(__WIN32) || defined(_WIN32) || defined(WIN32)) && !defined(__CYGWIN__)
#define NOWIDE_WINDOWS
#endif

#ifdef _MSC_VER
#define NOWIDE_MSVC
#endif

#ifdef NOWIDE_WINDOWS
#if defined(DLL_EXPORT) || defined(NOWIDE_EXPORT)
#ifdef NOWIDE_SOURCE
#define NOWIDE_DECL __declspec(dllexport)
#else
#define NOWIDE_DECL __declspec(dllimport)
#endif // NOWIDE_SOURCE
#endif // DYN_LINK
#endif

#ifndef NOWIDE_DECL
#define NOWIDE_DECL
#endif

#if defined(NOWIDE_WINDOWS)
#ifdef BOOST_NOWIDE_USE_FSTREAM_REPLACEMENTS
#undef BOOST_NOWIDE_USE_FSTREAM_REPLACEMENTS
#endif
#define BOOST_NOWIDE_USE_FSTREAM_REPLACEMENTS 1
#elif !defined(BOOST_NOWIDE_USE_FSTREAM_REPLACEMENTS)
#define BOOST_NOWIDE_USE_FSTREAM_REPLACEMENTS 0
#endif

#endif
///
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
