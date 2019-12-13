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
#ifdef BOOST_WINDOWS
#include <boost/nowide/stackstring.hpp>
#include <vector>
#include <boost/nowide/windows.hpp>
#include <stdexcept>
#endif

namespace boost {
namespace nowide {
    #if !defined(BOOST_WINDOWS) && !defined(BOOST_NOWIDE_DOXYGEN)
    class args {
    public:
        args(int &,char **&) {}
        args(int &,char **&,char **&){}
        ~args() {}
    };

    #else

    ///
    /// \brief args is a class that fixes standard main() function arguments and changes them to UTF-8 under
    /// Microsoft Windows.
    ///
    /// The class uses \c GetCommandLineW(), \c CommandLineToArgvW() and \c GetEnvironmentStringsW()
    /// in order to obtain the information. It does not relate to actual values of argc,argv and env
    /// under Windows.
    ///
    /// It restores the original values in its destructor
    ///
    /// If any of the system calls fails, an exception of type std::runtime_error will be thrown
    /// and argc, argv, env remain unchanged.
    ///
    /// \note the class owns the memory of the newly allocated strings
    ///
    class args {
    public:

        ///
        /// Fix command line arguments 
        ///
        args(int &argc,char **&argv) :
            old_argc_(argc),
            old_argv_(argv),
            old_env_(0),
            old_argc_ptr_(&argc),
            old_argv_ptr_(&argv),
            old_env_ptr_(0)
        {
            fix_args(argc,argv);
        }
        ///
        /// Fix command line arguments and environment
        ///
        args(int &argc,char **&argv,char **&env) :
            old_argc_(argc),
            old_argv_(argv),
            old_env_(env),
            old_argc_ptr_(&argc),
            old_argv_ptr_(&argv),
            old_env_ptr_(&env)
        {
            fix_args(argc,argv);
            fix_env(env);
        }
        ///
        /// Restore original argc,argv,env values, if changed
        ///
        ~args()
        {
            if(old_argc_ptr_)
                *old_argc_ptr_ = old_argc_;
            if(old_argv_ptr_)
                *old_argv_ptr_ = old_argv_;
            if(old_env_ptr_)
                *old_env_ptr_ = old_env_;
        }
    private:
        void fix_args(int &argc,char **&argv)
        {
            int wargc;
            wchar_t **wargv = CommandLineToArgvW(GetCommandLineW(),&wargc);
            if(!wargv)
                throw std::runtime_error("Could not get command line!");
            args_.resize(wargc+1,0);
            arg_values_.resize(wargc);
            for(int i=0;i<wargc;i++) 
                args_[i] = arg_values_[i].convert(wargv[i]);
            argc = wargc;
            argv = &args_[0];
            LocalFree(wargv);
        }
        void fix_env(char **&env)
        {
            wchar_t *wstrings = GetEnvironmentStringsW();
            if(!wstrings)
                throw std::runtime_error("Could not get environment strings!");
            wchar_t *wstrings_end = 0;
            int count = 0;
            for(wstrings_end = wstrings;*wstrings_end;wstrings_end+=wcslen(wstrings_end)+1)
                    count++;
            env_.convert(wstrings,wstrings_end);
            envp_.resize(count+1,0);
            char *p=env_.c_str();
            int pos = 0;
            for(int i=0;i<count;i++) {
                if(*p!='=')
                    envp_[pos++] = p;
                p+=strlen(p)+1;
            }
            env = &envp_[0];
            FreeEnvironmentStringsW(wstrings);
        }

        std::vector<char *> args_;
        std::vector<short_stackstring> arg_values_;
        stackstring env_;
        std::vector<char *> envp_;

        int old_argc_;
        char **old_argv_;
        char **old_env_;

        int  *old_argc_ptr_;
        char ***old_argv_ptr_;
        char ***old_env_ptr_;
    };

    #endif

} // nowide
} // namespace boost
#endif

///
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
