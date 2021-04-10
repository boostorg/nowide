//  Copyright (c) 2015 Artyom Beilis (Tonkikh)
//  Copyright (c) 2019-2021 Alexander Grund
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <boost/nowide/filebuf.hpp>

#include <cstdint>
#include <random>
#include <string>
#include <type_traits>
#include "file_test_helpers.hpp"
#include "test.hpp"

namespace nw = boost::nowide;
using namespace boost::nowide::test;

// Check member types
static_assert(std::is_same<nw::filebuf::char_type, char>::value, "!");
static_assert(std::is_same<nw::filebuf::traits_type::char_type, char>::value, "!");
static_assert(std::is_same<nw::filebuf::int_type, nw::filebuf::traits_type::int_type>::value, "!");
static_assert(std::is_same<nw::filebuf::pos_type, nw::filebuf::traits_type::pos_type>::value, "!");
static_assert(std::is_same<nw::filebuf::off_type, nw::filebuf::traits_type::off_type>::value, "!");

void test_open_close(const std::string& filepath)
{
    const std::string filepath2 = filepath + ".2";
    ensure_not_exists(filepath2);
    remove_file_at_exit _(filepath);
    remove_file_at_exit _2(filepath2);

    nw::filebuf buf;
    TEST(buf.open(filepath, std::ios_base::out) == &buf);
    TEST(buf.is_open());

    // Opening when already open fails
    TEST(buf.open(filepath2, std::ios_base::out) == nullptr);
    // Still open
    TEST(buf.is_open());
    TEST(buf.close() == &buf);
    // Failed opening did not create file
    TEST(!file_exists(filepath2));

    // But it should work now:
    TEST(buf.open(filepath2, std::ios_base::out) == &buf);
    TEST(buf.close() == &buf);
    TEST(file_exists(filepath2));
}

void test_pubseekpos(const std::string& filepath)
{
    const std::string data = random_data(BUFSIZ * 4, data_type::binary);
    create_file(filepath, data, data_type::binary);
    nw::filebuf buf;
    TEST(buf.open(filepath, std::ios_base::in | std::ios_base::binary) == &buf);

    // Fuzzy test: Seek to a couple random positions
    std::minstd_rand rng(std::random_device{}());
    const auto eofPos = nw::filebuf::pos_type(data.size());
    std::uniform_int_distribution<std::size_t> distr(0, eofPos);
    using traits = nw::filebuf::traits_type;
    for(int i = 0; i < 100; i++)
    {
        const nw::filebuf::pos_type pos = distr(rng);
        TEST(buf.pubseekpos(pos) == pos);
        if(pos == eofPos)
            TEST(buf.sgetc() == traits::eof());
        else
            TEST(buf.sgetc() == traits::to_int_type(data[pos]));
    }
    // Seek to first and last as corner case tests
    TEST(buf.pubseekpos(0) == 0);
    TEST(buf.sgetc() == traits::to_int_type(data[0]));
    TEST(buf.pubseekpos(eofPos) == eofPos);
    TEST(buf.sgetc() == traits::eof());
}

void test_pubseekoff(const std::string& filepath)
{
    const std::string data = random_data(BUFSIZ * 4, data_type::binary);
    create_file(filepath, data, data_type::binary);
    nw::filebuf buf;
    TEST(buf.open(filepath, std::ios_base::in | std::ios_base::binary) == &buf);

    // Fuzzy test: Seek to a couple random positions
    std::minstd_rand rng(std::random_device{}());
    using pos_type = nw::filebuf::pos_type;
    using off_type = nw::filebuf::off_type;
    const auto eofPos = pos_type(data.size());
    std::uniform_int_distribution<std::size_t> distr(0, eofPos);
    using traits = nw::filebuf::traits_type;

    // tellg/tellp function as called by basic_[io]fstream
    const auto tellg = [&]() { return buf.pubseekoff(0, std::ios_base::cur); };

    for(int i = 0; i < 100; i++)
    {
        // beg
        pos_type pos = distr(rng);
        TEST(buf.pubseekoff(pos, std::ios_base::beg) == pos);
        TEST(tellg() == pos);
        if(pos == eofPos)
            TEST(buf.sgetc() == traits::eof());
        else
            TEST(buf.sgetc() == traits::to_int_type(data[pos]));
        // cur
        off_type diff = static_cast<pos_type>(distr(rng)) - pos;
        pos += diff;
        TEST(buf.pubseekoff(diff, std::ios_base::cur) == pos);
        TEST(tellg() == pos);
        if(pos == eofPos)
            TEST(buf.sgetc() == traits::eof());
        else
            TEST(buf.sgetc() == traits::to_int_type(data[pos]));
        // end
        diff = static_cast<pos_type>(distr(rng)) - eofPos;
        pos = eofPos + diff;
        TEST(buf.pubseekoff(diff, std::ios_base::end) == pos);
        TEST(tellg() == pos);
        if(pos == eofPos)
            TEST(buf.sgetc() == traits::eof());
        else
            TEST(buf.sgetc() == traits::to_int_type(data[pos]));
    }
    // Seek to first and last as corner case tests
    TEST(buf.pubseekoff(0, std::ios_base::beg) == 0);
    TEST(tellg() == pos_type(0));
    TEST(buf.sgetc() == traits::to_int_type(data[0]));
    TEST(buf.pubseekoff(0, std::ios_base::end) == eofPos);
    TEST(tellg() == eofPos);
    TEST(buf.sgetc() == traits::eof());
}

void test_64_bit_seek(const std::string& filepath)
{
    // Create a value which does not fit into a 32 bit value.
    // Use an unsigned intermediate to have the truncation defined to wrap to 0
    using unsigned_off_type = std::make_unsigned<nw::filebuf::off_type>::type;
    const nw::filebuf::off_type offset = static_cast<unsigned_off_type>(std::uint64_t(1) << 33u);

#ifdef BOOST_MSVC
#pragma warning(push)
#pragma warning(disable : 4127)
#endif
    // coverity[result_independent_of_operands]
    if((offset == nw::filebuf::off_type(0)))
        return; // Can't use 64 bit offsets throught the API, so don't test anything
#ifdef BOOST_MSVC
#pragma warning(pop)
#endif

    create_file(filepath, "test");
    remove_file_at_exit _(filepath);

    nw::filebuf buf;
    TEST(buf.open(filepath, std::ios_base::in | std::ios_base::binary) == &buf);
    const std::streampos knownPos = 2;
    TEST(buf.pubseekpos(knownPos) == knownPos); // Just to make sure we know where we are
    const std::streampos newPos = buf.pubseekoff(offset, std::ios_base::cur);
    // On 32 bit mode or when seek beyond EOF is not allowed, the current position should be unchanged
    if(newPos == std::streampos(-1))
        TEST(buf.pubseekoff(0, std::ios_base::cur) == knownPos);
    else
    {
        TEST(newPos == offset + knownPos);
        TEST(buf.pubseekoff(0, std::ios_base::cur) == newPos);
    }
}

void test_main(int, char** argv, char**)
{
    const std::string exampleFilename = std::string(argv[0]) + "-\xd7\xa9-\xd0\xbc-\xce\xbd.txt";

    test_open_close(exampleFilename);
    test_pubseekpos(exampleFilename);
    test_pubseekoff(exampleFilename);
    test_64_bit_seek(exampleFilename);
}
