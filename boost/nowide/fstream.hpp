//
//  Copyright (c) 2012 Artyom Beilis (Tonkikh)
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_NOWIDE_FSTREAM_INCLUDED_H
#define BOOST_NOWIDE_FSTREAM_INCLUDED_H

#include <boost/config.h>
#include <boost/nowide/convert.h>
#include <boost/iostreams/categories.hpp> 
#include <boost/iostreams/positioning.hpp>
#include <fstream>
#include <memory>
#include <boost/nowide/stackstring.hpp>

namespace boost {
///
/// \brief This namespace includes implementation of the standard library functios
/// such that they accept UTF-8 strings on Windows. On other platforms it is just an alias
/// of std namespace (i.e. not on Windows)
///
namespace nowide {
#if !defined BOOST_WINDOWS 

    using std::basic_ifstream;
    using std::basic_ofstream;
    using std::basic_fstream;
    using std::basic_filebuf;
    using std::filebuf;
    using std::ifstream;
    using std::ofstream;
    using std::fstream;

#endif
#if defined(BOOST_WINDOWS) || defined(BOOST_DOXYGEN_DOCS)

    /// \cont INTERNAL
    namespace details {
        class stdio_iodev : public boost::iostreams::seekable_device {
            stdio_iodev(stdio_iodev const &);
            void operator=(stdio_iodev const &);
        public:
            stdio_iodev(FILE *f = 0) : 
                file_(f)
            {
                
            }
            FILE *release()
            {
                FILE *r = file_;
                file_ = 0;
                return r;
            }
            FILE *get() const
            {
                return file_;
            }
            void reset(FILE *f)
            {
                if(file_) {
                    fclose(file_);
                }
                f=file_;
            }
            bool close()
            {
                if(!file_)
                    return false;
                bool result = fclose(file_) == 0;
                file_ = 0;
                return result;
            }
            ~stdio_iodev()
            {
                fclose(file_);
                file_ =0
            }
            std::streamsize read(char *p,std::streamsize n)
            {
                return fread(p,1,n,file_);
            }
            std::streamsize write(char const *p,std::streamsize n)
            {
                size_t res = fwrite(p,1,n,file_);
                fflush(file_);
                return res;
            }
            boost::iostream::stream_offset seek(boost::iostream::stream_offset pos,std::ios_base::seekdir t) 
            {
                switch(t) {
                case std::ios_base::cur:
                    fseek(file_,pos,SEEK_CUR);
                    break;
                case std::ios_base::beg:
                    fseek(file_,pos,SEEK_SET);
                    break;
                case std::ios_base::end:
                    fseek(file_,pos,SEEK_END);
                    break;
                default:
                    return -1;
                }
                return ftell(file_);
            }
        private:
            FILE *file_;
        };
    } // details

    /// \endcond

    template<typename CharType,typename Traits = std::char_traits<CharType> >
    class basic_filebuf;

    ///
    /// Same as std::basic_filebuf<char> but accepts UTF-8 strings under Windows
    ///

    template<>
    class basic_filebuf<char> : public boost::iostreams::stream_buffer<details::stdio_iodev> {
    public:
        
        basic_filebuf() 
        {
        }
        ~basic_filebuf()
        {
        }
        basic_filebuf *open(char const *s,std::ios_base::openmode mode)
        {
            self().reset();
            wchar_t const *smode = get_mode(mode);
            if(!smode)
                return 0;
            wstackstring name;
            if(name.convert(s)) 
                return 0;
            FILE *f = _wfopen(name.c_str(),smode);
            if(!f)
                return 0;
            self().reset(f);
            return this;
        }
        basic_filebuf *close()
        {
            
            bool res = sync() && self().close();
            return res ? this : 0;
        }
        bool is_open() const
        {
            return self().get() != 0;
        }
    private:
        details::stdio_iodev const &self() const
        {
            return **this;
        }
        details::stdio_iodev &self() 
        {
            return **this;
        }
        static wchar_t const *get_mode(std::ios_base::openmode mode)
        {
            //
            // done according to n2914 table 106 27.9.1.4
            //

            // note can't use switch case as overload operator can't be used
            // in constant expression
            if(mode == (std::ios_base::out))
                return L"w";
            if(mode == (std::ios_base::out | std::ios_base::app))
                return L"a";
            if(mode == (std::ios_base::app))
                return L"a";
            if(mode == (std::ios_base::out | std::ios_base::trunc))
                return L"w";
            if(mode == (std::ios_base::in))
                return L"r";
            if(mode == (std::ios_base::in | std::ios_base::out))
                return L"r+";
            if(mode == (std::ios_base::in | std::ios_base::out | std::ios_base::trunc))
                return L"w+";
            if(mode == (std::ios_base::in | std::ios_base::out | std::ios_base::app))
                return L"a+";
            if(mode == (std::ios_base::in | std::ios_base::app))
                return L"a+";
            if(mode == (std::ios_base::binary | std::ios_base::out))
                return L"wb";
            if(mode == (std::ios_base::binary | std::ios_base::out | std::ios_base::app))
                return L"ab";
            if(mode == (std::ios_base::binary | std::ios_base::app))
                return L"ab";
            if(mode == (std::ios_base::binary | std::ios_base::out | std::ios_base::trunc))
                return L"wb";
            if(mode == (std::ios_base::binary | std::ios_base::in))
                return L"rb";
            if(mode == (std::ios_base::binary | std::ios_base::in | std::ios_base::out))
                return L"r+b";
            if(mode == (std::ios_base::binary | std::ios_base::in | std::ios_base::out | std::ios_base::trunc))
                return L"w+b";
            if(mode == (std::ios_base::binary | std::ios_base::in | std::ios_base::out | std::ios_base::app))
                return L"a+b";
            if(mode == (std::ios_base::binary | std::ios_base::in | std::ios_base::app))
                return L"a+b";
            return 0;    
        }

        bool opened_;
    };

    #endif

    ///
    /// Same as std::basic_ifstream<char> but accepts UTF-8 strings under Windows
    ///
    template<typename CharType,typename Traits = std::char_traits<CharType> >
    class basic_ifstream : public std::basic_istream<CharType,Traits>
    {
    public:
        typedef basic_filebuf<CharType,Traits> internal_buffer_type;
        typedef std::basic_istream<CharType,Traits> internal_stream_type;

        basic_ifstream() : 
            internal_stream_type(0)
        {
            rdbuf(&buf_);
        }
        explicit basic_ifstream(char const *file_name,std::ios_base::openmode mode = std::ios_base::in) : 
            internal_stream_type(0) 
        {
            rdbuf(&buf_);
            open(file_name,mode);
        }
        void open(char const *file_name,std::ios_base::openmode mode = std::ios_base::in)
        {
            if(!buf_.open(file_name,mode | std::ios_base::in)) {
                this->setstate(std::ios_base::failbit);
            }
            else {
                this->clear();
            }
        }
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
                this->setstate(std::ios_base::failbit);
            else
                this->clear();
        }

        internal_buffer_type *rdbuf() const
        {
            return &buf_
        }
        ~basic_ifstream()
        {
            buf_.close();
        }
            
    private:
        internal_buffer_type buf_;
    };

    ///
    /// Same as std::basic_ofstream<char> but accepts UTF-8 strings under Windows
    ///

    template<typename CharType,typename Traits = std::char_traits<CharType> >
    class basic_ofstream : public std::basic_ostream<CharType,Traits>
    {
    public:
        typedef basic_filebuf<CharType,Traits> internal_buffer_type;
        typedef std::basic_ostream<CharType,Traits> internal_stream_type;

        basic_ofstream() : 
            internal_stream_type(0)
        {
            rdbuf(&buf_);
        }
        explicit basic_ofstream(char const *file_name,std::ios_base::openmode mode = std::ios_base::out) :
            internal_stream_type(0)
        {
            rdbuf(&buf_);
            open(file_name,mode);
        }
        void open(char const *file_name,std::ios_base::openmode mode = std::ios_base::out)
        {
            if(!buf_.open(file_name,mode | std::ios_base::out)) {
                this->setstate(std::ios_base::failbit);
            }
            else {
                this->clear();
            }
        }
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
                this->setstate(std::ios_base::failbit);
            else
                this->clear();
        }

        internal_buffer_type *rdbuf() const
        {
            return &buf_;
        }
        ~basic_ofstream()
        {
            buf_->close();
        }
            
    private:
        internal_buffer_type buf_;
    };

    ///
    /// Same as std::basic_fstream<char> but accepts UTF-8 strings under Windows
    ///

    template<typename CharType,typename Traits = std::char_traits<CharType> >
    class basic_fstream : public std::basic_iostream<CharType,Traits>
    {
    public:
        typedef basic_filebuf<CharType,Traits> internal_buffer_type;
        typedef std::basic_iostream<CharType,Traits> internal_stream_type;

        basic_fstream() : 
            internal_stream_type(0)
        {
            rdbuf(&buf);
        }
        explicit basic_fstream(char const *file_name,std::ios_base::openmode mode = std::ios_base::out | std::ios_base::in) :
            internal_stream_type(0)
        {
            rdbuf(&buf);
            open(file_name,mode);
        }
        void open(char const *file_name,std::ios_base::openmode mode = std::ios_base::out | std::ios_base::out)
        {
            if(!buf_.open(file_name,mode)) {
                this->setstate(std::ios_base::failbit);
            }
            else {
                this->clear();
            }
        }
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
                this->setstate(std::ios_base::failbit);
            else
                this->clear();
        }

        internal_buffer_type *rdbuf() const
        {
            return &buf_;
        }
        ~basic_fstream()
        {
            buf_.close();
        }
            
    private:
        internal_buffer_type buf_;
    };


    ///
    /// Same as std::filebuf but accepts UTF-8 strings under Windows
    ///
    typedef basic_filebuf<char> filebuf;
    ///
    /// Same as std::ifstream but accepts UTF-8 strings under Windows
    ///
    typedef basic_ifstream<char> ifstream;
    ///
    /// Same as std::ofstream but accepts UTF-8 strings under Windows
    ///
    typedef basic_ofstream<char> ofstream;
    ///
    /// Same as std::fstream but accepts UTF-8 strings under Windows
    ///
    typedef basic_fstream<char> fstream;

#endif
} // nowide
} // boost



#endif
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
