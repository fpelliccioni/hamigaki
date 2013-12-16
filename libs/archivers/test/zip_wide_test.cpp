// zip_wide_test.cpp: test case for ZIP (Unicode)

// Copyright Takeshi Mouri 2008.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// See http://hamigaki.sourceforge.jp/libs/archivers for library home page.

#include <hamigaki/archivers/zip_file.hpp>
#include <hamigaki/iostreams/device/tmp_file.hpp>
#include <hamigaki/iostreams/dont_close.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/test/unit_test.hpp>
#include <string>

namespace ar = hamigaki::archivers;
namespace io_ex = hamigaki::iostreams;
namespace fs = boost::filesystem;
namespace io = boost::iostreams;
namespace ut = boost::unit_test;

void check_header(const ar::zip::wheader& old, const ar::zip::wheader& now)
{
    BOOST_CHECK(old.path == now.path);
    BOOST_CHECK(old.link_path == now.link_path);
    BOOST_CHECK_EQUAL(old.encrypted, now.encrypted);
    BOOST_CHECK(old.method == now.method);
    BOOST_CHECK((now.update_time - old.update_time) >= 0);
    BOOST_CHECK((now.update_time - old.update_time) <= 1);
    BOOST_CHECK_EQUAL(old.attributes, now.attributes);
    BOOST_CHECK_EQUAL(old.permissions, now.permissions);
    BOOST_CHECK(old.comment == now.comment);
}

void unicode_test()
{
    ar::zip::wheader head;
    head.path = L"\x3053\x3093\x306B\x3061\x306F/\x4F60\x597D.txt";
    head.encrypted = false;
    head.method = ar::zip::method::store;
    head.update_time = std::time(0);
    head.file_size = 0;
    head.attributes = ar::msdos::attributes::read_only;
    head.permissions = 0123;
    head.comment = L"\uC548\uB155\uD558\uC2ED\uB2C8\uAE4C";

    io_ex::tmp_file archive;
    ar::basic_zip_file_sink<
        io_ex::dont_close_device<io_ex::tmp_file>,
        boost::filesystem::wpath
    > sink(io_ex::dont_close(archive));

    sink.create_entry(head);
    sink.close();
    sink.close_archive();

    io::seek(archive, 0, BOOST_IOS::beg);

    ar::basic_zip_file_source<
        io_ex::tmp_file,
        boost::filesystem::wpath
    > src(archive);

    BOOST_CHECK(src.next_entry());

    check_header(head, src.header());

    BOOST_CHECK(!src.next_entry());
}

void narrow_to_wide_test()
{
    ar::zip::header head;
    head.path = "Hello.txt";
    head.method = ar::zip::method::store;

    io_ex::tmp_file archive;
    ar::basic_zip_file_sink<
        io_ex::dont_close_device<io_ex::tmp_file>,
        fs::path
    > sink(io_ex::dont_close(archive));

    sink.create_entry(head);
    sink.close();
    sink.close_archive();

    io::seek(archive, 0, BOOST_IOS::beg);

    ar::basic_zip_file_source<io_ex::tmp_file,fs::wpath> src(archive);

    BOOST_CHECK(src.next_entry());

    BOOST_CHECK(src.header().path == L"Hello.txt");

    BOOST_CHECK(!src.next_entry());
}

void wide_to_narrow_test()
{
    ar::zip::wheader head;
    head.path = L"Hello.txt";
    head.method = ar::zip::method::store;

    io_ex::tmp_file archive;
    ar::basic_zip_file_sink<
        io_ex::dont_close_device<io_ex::tmp_file>,
        fs::wpath
    > sink(io_ex::dont_close(archive));

    sink.create_entry(head);
    sink.close();
    sink.close_archive();

    io::seek(archive, 0, BOOST_IOS::beg);

    ar::basic_zip_file_source<io_ex::tmp_file,fs::path> src(archive);

    BOOST_CHECK(src.next_entry());

    BOOST_CHECK(src.header().path == "Hello.txt");

    BOOST_CHECK(!src.next_entry());
}

void symlink_test()
{
    ar::zip::wheader head;
    head.path = L"\u3053\u3093\u306B\u3061\u306F";
    head.link_path = L"\u4F60\u597D";
    head.encrypted = false;
    head.method = ar::zip::method::store;
    head.update_time = std::time(0);
    head.file_size = 0;
    head.attributes = ar::msdos::attributes::read_only;
    head.permissions = 0120123;
    head.comment = L"\uC548\uB155\uD558\uC2ED\uB2C8\uAE4C";

    io_ex::tmp_file archive;
    ar::basic_zip_file_sink<
        io_ex::dont_close_device<io_ex::tmp_file>,
        boost::filesystem::wpath
    > sink(io_ex::dont_close(archive));

    sink.create_entry(head);
    sink.close();
    sink.close_archive();

    io::seek(archive, 0, BOOST_IOS::beg);

    ar::basic_zip_file_source<
        io_ex::tmp_file,
        boost::filesystem::wpath
    > src(archive);

    BOOST_CHECK(src.next_entry());

    check_header(head, src.header());

    BOOST_CHECK(!src.next_entry());
}

ut::test_suite* init_unit_test_suite(int, char* [])
{
    ut::test_suite* test = BOOST_TEST_SUITE("ZIP wide test");
    test->add(BOOST_TEST_CASE(&unicode_test));
    test->add(BOOST_TEST_CASE(&narrow_to_wide_test));
    test->add(BOOST_TEST_CASE(&wide_to_narrow_test));
    test->add(BOOST_TEST_CASE(&symlink_test));
    return test;
}