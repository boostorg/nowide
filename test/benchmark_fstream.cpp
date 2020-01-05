//
//  Copyright (c) 2012 Artyom Beilis (Tonkikh)
//  Copyright (c) 2019 Alexander Grund
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "test.hpp"
#include <boost/nowide/convert.hpp>
#include <boost/nowide/cstdio.hpp>
#include <boost/nowide/fstream.hpp>
#define BOOST_CHRONO_HEADER_ONLY
#include <boost/chrono.hpp>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#ifdef BOOST_MSVC
#pragma warning(disable : 4996) // function unsafe/deprecated
#endif

namespace nw = boost::nowide;
template<typename FStream>
class io_fstream
{
public:
    void open(char const *file)
    {
        f_.open(file, std::fstream::out | std::fstream::in | std::fstream::trunc);
        TEST(f_);
    }
    void write(char *buf, int size)
    {
        f_.write(buf, size);
    }
    void read(char *buf, int size)
    {
        f_.read(buf, size);
    }
    void rewind()
    {
        f_.seekg(0);
        f_.seekp(0);
    }
    void flush()
    {
        f_ << std::flush;
    }
    void close()
    {
        f_.close();
    }

private:
    FStream f_;
};

class io_stdio
{
public:
    void open(char const *file)
    {
        f_ = fopen(file, "w+");
        TEST(f_);
    }
    void write(char *buf, int size)
    {
        fwrite(buf, 1, size, f_);
    }
    void read(char *buf, int size)
    {
        size_t res = fread(buf, 1, size, f_);
        (void)res;
    }
    void rewind()
    {
        ::rewind(f_);
    }
    void flush()
    {
        fflush(f_);
    }
    void close()
    {
        fclose(f_);
        f_ = 0;
    }

private:
    FILE *f_;
};

template<typename FStream>
void test_io(const char *file, char const *type)
{
    std::cout << "Testing I/O performance " << type << std::endl;
    FStream tmp;
    tmp.open(file);
    int data_size = 64 * 1024 * 1024;
    for(int block_size = 16; block_size <= 8192; block_size *= 2)
    {
        std::vector<char> buf(block_size, ' ');
        int size = 0;
        tmp.rewind();
        boost::chrono::high_resolution_clock::time_point t1 = boost::chrono::high_resolution_clock::now();
        while(size < data_size)
        {
            tmp.write(&buf[0], block_size);
            size += block_size;
        }
        tmp.flush();
        boost::chrono::high_resolution_clock::time_point t2 = boost::chrono::high_resolution_clock::now();
        double tm = boost::chrono::duration_cast<boost::chrono::milliseconds>(t2 - t1).count() * 1e-3;
        // heatup
        if(block_size >= 32)
            std::cout << "  write block size " << std::setw(8) << block_size << " " << std::fixed << std::setprecision(3)
                      << (data_size / 1024.0 / 1024 / tm) << " MB/s" << std::endl;
    }
    for(int block_size = 32; block_size <= 8192; block_size *= 2)
    {
        std::vector<char> buf(block_size, ' ');
        int size = 0;
        tmp.rewind();
        boost::chrono::high_resolution_clock::time_point t1 = boost::chrono::high_resolution_clock::now();
        while(size < data_size)
        {
            tmp.read(&buf[0], block_size);
            size += block_size;
        }
        boost::chrono::high_resolution_clock::time_point t2 = boost::chrono::high_resolution_clock::now();
        double tm = boost::chrono::duration_cast<boost::chrono::milliseconds>(t2 - t1).count() * 1e-3;
        std::cout << "   read block size " << std::setw(8) << block_size << " " << std::fixed << std::setprecision(3)
                  << (data_size / 1024.0 / 1024 / tm) << " MB/s" << std::endl;
    }
    tmp.close();
    std::remove(file);
}

void test_perf(const char *file)
{
    test_io<io_stdio>(file, "stdio");
    test_io<io_fstream<std::fstream> >(file, "std::fstream");
    test_io<io_fstream<nw::fstream> >(file, "nowide::fstream");
}

int main(int argc, char **argv)
{
    std::string filename = "perf_test_file.dat";
    if(argc == 2)
    {
        filename = argv[1];
    } else if(argc != 1)
    {
        std::cerr << "Usage: " << argv[0] << " [test_filepath]" << std::endl;
        return 1;
    }
    test_perf(filename.c_str());
    return 0;
}
