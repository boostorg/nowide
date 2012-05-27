//
//  Copyright (c) 2012 Artyom Beilis (Tonkikh)
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_NOWIDE_ARGS_HPP_INCLUDED
#define BOOST_NOWIDE_ARGS_HPP_INCLUDED

#include <boost/config.hpp>
#include <boost/nowide/stackstring.hpp>
#include <vector>
#ifdef BOOST_WINDOWS
#include <windows.h>
#endif

namespace boost {
    namespace nowide {
        #ifndef BOOST_WINDOWS
        class args {
        public:
            args(int &,char **&) {}
            args(int &,char **&,char **&){}
        };

        #else

        class args {
        public:
            args(int &argc,char **&argv)
            {
                fix_args(argc,argv);
            }
            args(int &argc,char **&argv,char **&en)
            {
                fix_args(argc,argv);
                fix_env(en);
            }
        private:    
            void fix_args(int &argc,char **&argv)
            {
                    int wargc;
                    wchar_t **wargv = CommandLineToArgvW(GetCommandLineW(),&wargc);
                if(!wargv) {
                    argc = 0;
                    static char *dummy = 0;
                    argv = &dummy;
                    return;
                }
                try{ 
                    args_.resize(wargc+1,0);
                    arg_values_.resize(wargc);
                    for(int i=0;i<wargc;i++) {
                        if(!arg_values_[i].convert(wargv[i])) {
                            wargc = i;
                            break;
                        }
                        args_[i] = arg_values_[i].c_str();
                    }
                    argc = wargc;
                    argv = &args_[0];
                }
                catch(...) {
                    LocalFree(wargv);
                    throw;
                }
                LocalFree(wargv);
            }
            void fix_env(char **&en)
            {
                static char *dummy = 0;
                en = &dummy;
                wchar_t *wstrings = GetEnvironmentStringsW();
                if(!wstrings)
                    return;
                try {
                    wchar_t *wstrings_end = 0;
                    int count = 0;
                    for(wstrings_end = wstrings;*wstrings_end;wstrings_end+=wcslen(wstrings_end)+1)
                        count++;
                    if(env_.convert(wstrings,wstrings_end)) {
                        envp_.resize(count+1,0);
                        char *p=env_.c_str();
                        for(int i=0;i<count;i++) {
                            envp_[i] = p;
                            p+=strlen(p)+1;
                        }
                        en = &envp_[0];
                    }
                }
                catch(...) {
                    FreeEnvironmentStringsW(wstrings);
                    throw;
                }
                FreeEnvironmentStringsW(wstrings);

            }

            std::vector<char *> args_;
            std::vector<short_stackstring> arg_values_;
            stackstring env_;
            std::vector<char *> envp_;
        };

        #endif

    } // nowide
} // boost
#endif

///
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
