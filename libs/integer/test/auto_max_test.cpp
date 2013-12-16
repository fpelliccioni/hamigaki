// auto_max_test.cpp: test case for auto_max

// Copyright Takeshi Mouri 2006-2008.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// See http://hamigaki.sourceforge.jp/libs/integer for library home page.

#include <hamigaki/integer/auto_max.hpp>
#include <boost/integer/static_min_max.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/cstdint.hpp>
#include <boost/integer_traits.hpp>

namespace ut = boost::unit_test;
using hamigaki::detail::auto_max_result;

template<typename T, typename U>
struct auto_max_range_check
{
    typedef typename auto_max_result<T,U>::type result_type;

    static const boost::int64_t t_min =
        static_cast<boost::int64_t>(
            boost::integer_traits<T>::const_min);

    static const boost::int64_t t_max =
        static_cast<boost::int64_t>(
            boost::integer_traits<T>::const_max);

    static const boost::int64_t u_min =
        static_cast<boost::int64_t>(
            boost::integer_traits<U>::const_min);

    static const boost::int64_t u_max =
        static_cast<boost::int64_t>(
            boost::integer_traits<U>::const_max);

    static const boost::int64_t r_min =
        static_cast<boost::int64_t>(
            boost::integer_traits<result_type>::const_min);

    static const boost::int64_t r_max =
        static_cast<boost::int64_t>(
            boost::integer_traits<result_type>::const_max);

    static const bool value =
        (t_max <= r_max) &&
        (u_max <= r_max) &&
        (t_min < u_min ? r_min <= u_min : r_min <= t_min);

    typedef boost::mpl::bool_<value> type;
};

void auto_max_result_test()
{
    BOOST_MPL_ASSERT((auto_max_range_check<boost::int8_t  ,boost::int8_t  >));
    BOOST_MPL_ASSERT((auto_max_range_check<boost::int8_t  ,boost::uint8_t >));
    BOOST_MPL_ASSERT((auto_max_range_check<boost::int8_t  ,boost::int16_t >));
    BOOST_MPL_ASSERT((auto_max_range_check<boost::int8_t  ,boost::uint16_t>));
    BOOST_MPL_ASSERT((auto_max_range_check<boost::int8_t  ,boost::int32_t >));
    BOOST_MPL_ASSERT((auto_max_range_check<boost::int8_t  ,boost::uint32_t>));
    BOOST_MPL_ASSERT((auto_max_range_check<boost::int8_t  ,boost::int64_t >));

    BOOST_MPL_ASSERT((auto_max_range_check<boost::uint8_t ,boost::int8_t  >));
    BOOST_MPL_ASSERT((auto_max_range_check<boost::uint8_t ,boost::uint8_t >));
    BOOST_MPL_ASSERT((auto_max_range_check<boost::uint8_t ,boost::int16_t >));
    BOOST_MPL_ASSERT((auto_max_range_check<boost::uint8_t ,boost::uint16_t>));
    BOOST_MPL_ASSERT((auto_max_range_check<boost::uint8_t ,boost::int32_t >));
    BOOST_MPL_ASSERT((auto_max_range_check<boost::uint8_t ,boost::uint32_t>));
    BOOST_MPL_ASSERT((auto_max_range_check<boost::uint8_t ,boost::int64_t >));

    BOOST_MPL_ASSERT((auto_max_range_check<boost::int16_t ,boost::int8_t  >));
    BOOST_MPL_ASSERT((auto_max_range_check<boost::int16_t ,boost::uint8_t >));
    BOOST_MPL_ASSERT((auto_max_range_check<boost::int16_t ,boost::int16_t >));
    BOOST_MPL_ASSERT((auto_max_range_check<boost::int16_t ,boost::uint16_t>));
    BOOST_MPL_ASSERT((auto_max_range_check<boost::int16_t ,boost::int32_t >));
    BOOST_MPL_ASSERT((auto_max_range_check<boost::int16_t ,boost::uint32_t>));
    BOOST_MPL_ASSERT((auto_max_range_check<boost::int16_t ,boost::int64_t >));

    BOOST_MPL_ASSERT((auto_max_range_check<boost::uint16_t,boost::int8_t  >));
    BOOST_MPL_ASSERT((auto_max_range_check<boost::uint16_t,boost::uint8_t >));
    BOOST_MPL_ASSERT((auto_max_range_check<boost::uint16_t,boost::int16_t >));
    BOOST_MPL_ASSERT((auto_max_range_check<boost::uint16_t,boost::uint16_t>));
    BOOST_MPL_ASSERT((auto_max_range_check<boost::uint16_t,boost::int32_t >));
    BOOST_MPL_ASSERT((auto_max_range_check<boost::uint16_t,boost::uint32_t>));
    BOOST_MPL_ASSERT((auto_max_range_check<boost::uint16_t,boost::int64_t >));

    BOOST_MPL_ASSERT((auto_max_range_check<boost::int32_t ,boost::int8_t  >));
    BOOST_MPL_ASSERT((auto_max_range_check<boost::int32_t ,boost::uint8_t >));
    BOOST_MPL_ASSERT((auto_max_range_check<boost::int32_t ,boost::int16_t >));
    BOOST_MPL_ASSERT((auto_max_range_check<boost::int32_t ,boost::uint16_t>));
    BOOST_MPL_ASSERT((auto_max_range_check<boost::int32_t ,boost::int32_t >));
    BOOST_MPL_ASSERT((auto_max_range_check<boost::int32_t ,boost::uint32_t>));
    BOOST_MPL_ASSERT((auto_max_range_check<boost::int32_t ,boost::int64_t >));

    BOOST_MPL_ASSERT((auto_max_range_check<boost::uint32_t,boost::int8_t  >));
    BOOST_MPL_ASSERT((auto_max_range_check<boost::uint32_t,boost::uint8_t >));
    BOOST_MPL_ASSERT((auto_max_range_check<boost::uint32_t,boost::int16_t >));
    BOOST_MPL_ASSERT((auto_max_range_check<boost::uint32_t,boost::uint16_t>));
    BOOST_MPL_ASSERT((auto_max_range_check<boost::uint32_t,boost::int32_t >));
    BOOST_MPL_ASSERT((auto_max_range_check<boost::uint32_t,boost::uint32_t>));
    BOOST_MPL_ASSERT((auto_max_range_check<boost::uint32_t,boost::int64_t >));

    BOOST_MPL_ASSERT((auto_max_range_check<boost::int64_t ,boost::int8_t  >));
    BOOST_MPL_ASSERT((auto_max_range_check<boost::int64_t ,boost::uint8_t >));
    BOOST_MPL_ASSERT((auto_max_range_check<boost::int64_t ,boost::int16_t >));
    BOOST_MPL_ASSERT((auto_max_range_check<boost::int64_t ,boost::uint16_t>));
    BOOST_MPL_ASSERT((auto_max_range_check<boost::int64_t ,boost::int32_t >));
    BOOST_MPL_ASSERT((auto_max_range_check<boost::int64_t ,boost::uint32_t>));
    BOOST_MPL_ASSERT((auto_max_range_check<boost::int64_t ,boost::int64_t >));
}

void auto_max_test()
{
    using hamigaki::auto_max;

    BOOST_CHECK_EQUAL(auto_max(0,0), 0);
    BOOST_CHECK_EQUAL(auto_max(1,2), 2);
    BOOST_CHECK_EQUAL(auto_max(1,0u), 1u);
    BOOST_CHECK_EQUAL(auto_max(-1,1), 1);
    BOOST_CHECK_EQUAL(auto_max(1,-1), 1);
    BOOST_CHECK_EQUAL(auto_max(-2147483647,0xFFFFFFFFu), 0xFFFFFFFF);
    BOOST_CHECK_EQUAL(auto_max(-2147483647-1,0xFFFFFFFFu), 0xFFFFFFFF);
    BOOST_CHECK_EQUAL(auto_max(-1,0xFFFFFFFFu), 0xFFFFFFFF);
    BOOST_CHECK_EQUAL(auto_max(-2,0xFFFFFFFFu), 0xFFFFFFFF);
    BOOST_CHECK_EQUAL(auto_max(1,0u), 1u);

    BOOST_CHECK_EQUAL(
        auto_max(
            boost::integer_traits<long long>::const_max,
            boost::integer_traits<unsigned long long>::const_max
        ),
        boost::integer_traits<unsigned long long>::const_max
    );

    BOOST_CHECK_EQUAL(
        auto_max(
            boost::integer_traits<long long>::const_min,
            boost::integer_traits<unsigned long long>::const_max
        ),
        boost::integer_traits<unsigned long long>::const_max
    );

    BOOST_CHECK_EQUAL(
        auto_max(
            boost::integer_traits<long long>::const_max,
            boost::integer_traits<unsigned long long>::const_min
        ),
        static_cast<unsigned long long>(
            boost::integer_traits<long long>::const_max
        )
    );

    BOOST_CHECK_EQUAL(
        auto_max(static_cast<long long>(-1), 0xFFFFFFFFFFFFFFFEull),
        0xFFFFFFFFFFFFFFFEull
    );
}

ut::test_suite* init_unit_test_suite(int, char* [])
{
    ut::test_suite* test = BOOST_TEST_SUITE("auto_max test");
    test->add(BOOST_TEST_CASE(&auto_max_result_test));
    test->add(BOOST_TEST_CASE(&auto_max_test));
    return test;
}
