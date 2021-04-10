//  Copyright (c) 2019-2021 Alexander Grund
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_NOWIDE_FILE_TEST_HELPERS_HPP_INCLUDED
#define BOOST_NOWIDE_FILE_TEST_HELPERS_HPP_INCLUDED

#include <boost/nowide/cstdio.hpp>
#include <string>

#include "test.hpp"

namespace boost {
namespace nowide {
    namespace test {
        void create_empty_file(const std::string& filepath)
        {
            auto* f = fopen(filepath.c_str(), "w");
            TEST(f);
            std::fclose(f);
        }

        void create_file(const std::string& filepath, const std::string& contents)
        {
            auto* f = fopen(filepath.c_str(), "wb");
            TEST(f);
            TEST(std::fwrite(contents.data(), 1, contents.size(), f) == contents.size());
            std::fclose(f);
        }

        bool file_exists(const std::string& filepath)
        {
            FILE* f = fopen(filepath.c_str(), "r");
            if(f)
            {
                std::fclose(f);
                return true;
            } else
                return false;
        }

        std::string read_file(const std::string& filepath, bool binary_mode = false)
        {
            FILE* f = fopen(filepath.c_str(), binary_mode ? "rb" : "r");
            TEST(f);
            std::fseek(f, 0, SEEK_END);
            const auto file_size = ftell(f);
            std::fseek(f, 0, SEEK_SET);
            std::string content(file_size, '\0');
            const auto read_bytes = std::fread(&content[0], 1, file_size, f);
            if(read_bytes != file_size)
            {
                TEST(std::feof(f));
                content.resize(read_bytes);
            }
            std::fclose(f);
            return content;
        }

        void ensure_not_exists(const char* filepath)
        {
            if(file_exists(filepath))
            {
                remove(filepath);
                TEST(!file_exists(filepath));
            }
        }

        void ensure_not_exists(const std::string& filepath)
        {
            ensure_not_exists(filepath.c_str());
        }

        struct remove_file_at_exit
        {
            const std::string filepath;
            explicit remove_file_at_exit(std::string filepath) : filepath(std::move(filepath))
            {}
            ~remove_file_at_exit() noexcept(false)
            {
                ensure_not_exists(filepath);
            }
        };

    } // namespace test
} // namespace nowide
} // namespace boost

#endif // BOOST_NOWIDE_FILE_TEST_HELPERS_HPP_INCLUDED