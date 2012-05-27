//
//  Copyright (c) 2012 Artyom Beilis (Tonkikh)
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_NOWIDE_IOSTREAM_HPP_INCLUDED
#define BOOST_NOWIDE_IOSTREAM_HPP_INCLUDED

#include <boost/nowide/config.hpp>
#include <boost/scoped_ptr.hpp>
#include <iostream>
#include <ostream>
#include <istream>

namespace boost {
    namespace nowide {
        #ifndef BOOST_WINDOWS
        using std::cout;
        using std::cerr;
        using std::cin;
        using std::clog;
        #else
        
        /// \cond INTERNAL 
        namespace details {
            class console_output_buffer;
            class console_input_buffer;
            
            class BOOST_NOWIDE_DECL winconsole_ostream : public std::ostream {
                winconsole_ostream(winconsole_ostream const &);
                void operator=(winconsole_ostream const &);
            public:
                winconsole_ostream(int fd);
                ~winconsole_ostream();
            private:
                boost::scoped_ptr<console_output_buffer> d;
            };

            class BOOST_NOWIDE_DECL winconsole_istream : public std::istream {
                winconsole_istream(winconsole_istream const &);
                void operator=(winconsole_istream const &);
            public:
                
                winconsole_istream();
                ~winconsole_istream();
            private:
                struct data;
                boost::scoped_ptr<console_input_buffer> d;
            };
        } // details 
        
        /// \endcond


        extern BOOST_NOWIDE_DECL details::winconsole_istream cin;
        extern BOOST_NOWIDE_DECL details::winconsole_ostream cout;
        extern BOOST_NOWIDE_DECL details::winconsole_ostream cerr;
        extern BOOST_NOWIDE_DECL details::winconsole_ostream clog;

        #endif

    } // nowide
} // boost

#endif
///
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
