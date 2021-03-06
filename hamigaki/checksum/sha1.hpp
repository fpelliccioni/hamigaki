// sha1.hpp: SHA-1 checksum

// Copyright Takeshi Mouri 2006-2008.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// See http://hamigaki.sourceforge.jp/libs/checksum for library home page.

#ifndef HAMIGAKI_CHECKSUM_SHA1_HPP
#define HAMIGAKI_CHECKSUM_SHA1_HPP

#include <hamigaki/integer/byte_swap.hpp>
#include <hamigaki/integer/rotate.hpp>
#include <boost/array.hpp>
#include <boost/assert.hpp>
#include <boost/cstdint.hpp>
#include <cstddef>

namespace hamigaki { namespace checksum {

namespace sha1_detail
{

typedef boost::uint32_t word;
typedef boost::array<word,16> block;

inline void copy_word(boost::uint8_t* dst, word n)
{
#if defined(BOOST_BIG_ENDIAN)
    std::memcpy(dst, &n, sizeof(word));
#elif defined(_M_IX86) || defined(__i386__)
    *reinterpret_cast<word*>(dst) = byte_swap32(n);
#else
    dst[0] = static_cast<boost::uint8_t>(n >> 24);
    dst[1] = static_cast<boost::uint8_t>(n >> 16);
    dst[2] = static_cast<boost::uint8_t>(n >>  8);
    dst[3] = static_cast<boost::uint8_t>(n      );
#endif
}

class sha1_impl
{
public:
    typedef boost::array<boost::uint8_t,20> value_type;

    sha1_impl()
    {
        reset();
    }

    void reset()
    {
        h_[0] = 0x67452301;
        h_[1] = 0xEFCDAB89;
        h_[2] = 0x98BADCFE;
        h_[3] = 0x10325476;
        h_[4] = 0xC3D2E1F0;
    }

    void process_block(const block& x)
    {
        word w[80];
        std::copy(x.begin(), x.end(), &w[0]);

        for (word t = 16; t < 80; ++t)
            w[t] = rotate_left(w[t-3] ^ w[t-8] ^ w[t-14] ^ w[t-16], 1);

        word a = h_[0];
        word b = h_[1];
        word c = h_[2];
        word d = h_[3];
        word e = h_[4];

        for (word t = 0; t < 80; ++t)
        {
            word temp = rotate_left(a, 5) + f(t, b, c, d) + e + w[t] + k(t);
            e = d;
            d = c;
            c = rotate_left(b, 30);
            b = a;
            a = temp;
        }

        h_[0] += a;
        h_[1] += b;
        h_[2] += c;
        h_[3] += d;
        h_[4] += e;
    }

    value_type output()
    {
        value_type tmp;
        copy_word(&tmp.elems[ 0], h_[0]);
        copy_word(&tmp.elems[ 4], h_[1]);
        copy_word(&tmp.elems[ 8], h_[2]);
        copy_word(&tmp.elems[12], h_[3]);
        copy_word(&tmp.elems[16], h_[4]);
        return tmp;
    }

private:
    word h_[5];

    static word f(word t, word b, word c, word d)
    {
        if (t < 20)
            return (b & c) | (~b & d);
        else if (t < 40)
            return b ^ c ^ d;
        else if (t < 60)
            return (b & c) | (b & d) | (c & d);
        else
            return b ^ c ^ d;
    }

    static word k(word t)
    {
        if (t < 20)
            return 0x5A827999;
        else if (t < 40)
            return 0x6ED9EBA1;
        else if (t < 60)
            return 0x8F1BBCDC;
        else
            return 0xCA62C1D6;
    }
};

} // namespace sha1_detail

class sha1
{
    typedef sha1_detail::word word;
    typedef sha1_detail::sha1_impl impl_type;

public:
    typedef impl_type::value_type value_type;

    sha1()
    {
        reset();
    }

    void reset()
    {
        buffer_.assign(0);
        bits_ = 0;
        impl_.reset();
    }

    void process_bit(bool bit)
    {
        std::size_t index = static_cast<std::size_t>((bits_ % 512) / 32);
        std::size_t offset = static_cast<std::size_t>(bits_ % 32);
        buffer_[index] |= static_cast<word>(bit) << (31 - offset);
        if ((++bits_ % 512) == 0)
        {
            impl_.process_block(buffer_);
            buffer_.assign(0);
        }
    }

    void process_bits(unsigned char bits, std::size_t bit_count)
    {
        while (bit_count--)
            process_bit(((bits >> bit_count) & 1) != 0);
    }

    void process_byte(unsigned char byte)
    {
        process_bits(byte, 8);
    }

    void process_block(const void* bytes_begin, const void* bytes_end)
    {
        typedef unsigned char uchar;
        const uchar* beg = static_cast<const uchar*>(bytes_begin);
        const uchar* end = static_cast<const uchar*>(bytes_end);
        while (beg != end)
            process_byte(*(beg++));
    }

    void process_bytes(const void* buffer, std::size_t byte_count)
    {
        typedef unsigned char uchar;
        const uchar* beg = static_cast<const uchar*>(buffer);
        process_block(beg, beg+byte_count);
    }

    value_type checksum() const
    {
        sha1 tmp(*this);

        boost::uint64_t total = tmp.bits_;

        std::size_t pad_size =
            static_cast<std::size_t>(512 - (tmp.bits_ + 64)%512);

        tmp.process_bit(true);
        while (--pad_size)
            tmp.process_bit(false);

        for (int i = 56; i >= 0; i -= 8)
            tmp.process_byte(static_cast<unsigned char>(total >> i));

        return tmp.impl_.output();
    }

    void operator()(unsigned char byte)
    {
        process_byte(byte);
    }

    value_type operator()() const
    {
        return checksum();
    }

private:
    sha1_detail::block buffer_;
    boost::uint64_t bits_;
    impl_type impl_;
};

class sha1_optimal
{
    typedef sha1_detail::word word;
    typedef sha1_detail::sha1_impl impl_type;

public:
    typedef impl_type::value_type value_type;

    sha1_optimal()
    {
        reset();
    }

    void reset()
    {
        buffer_.assign(0);
        bytes_ = 0;
        impl_.reset();
    }

    void process_byte(unsigned char byte)
    {
        std::size_t index = static_cast<std::size_t>((bytes_ % 64) / 4);
        std::size_t offset = static_cast<std::size_t>(bytes_ % 4) * 8;
        buffer_[index] |= static_cast<boost::uint32_t>(byte) << (24 - offset);
        if ((++bytes_ % 64) == 0)
        {
            impl_.process_block(buffer_);
            buffer_.assign(0);
        }
    }

    void process_block(const void* bytes_begin, const void* bytes_end)
    {
        typedef unsigned char uchar;
        const uchar* beg = static_cast<const uchar*>(bytes_begin);
        const uchar* end = static_cast<const uchar*>(bytes_end);
        std::size_t index = static_cast<std::size_t>((bytes_ % 64) / 4);
        std::size_t offset = 24 - static_cast<std::size_t>(bytes_ % 4) * 8;
        bytes_ += (end - beg);
        while (beg != end)
        {
            boost::uint32_t b = *(beg++);
            buffer_[index] |= b << offset;
            if (offset == 0)
            {
                offset = 24;
                if (++index == 16)
                {
                    index = 0;
                    impl_.process_block(buffer_);
                    buffer_.assign(0);
                }
            }
            else
                offset -= 8;
        }
    }

    void process_bytes(const void* buffer, std::size_t byte_count)
    {
        typedef unsigned char uchar;
        const uchar* beg = static_cast<const uchar*>(buffer);
        process_block(beg, beg+byte_count);
    }

    value_type checksum()
    {
        boost::uint64_t total = bytes_ * 8;

        process_byte(0x80);
        if (bytes_ % 64 >= 64-8)
        {
            impl_.process_block(buffer_);
            buffer_.assign(0);
        }

        buffer_[14] = static_cast<boost::uint32_t>(total >> 32);
        buffer_[15] = static_cast<boost::uint32_t>(total      );
        impl_.process_block(buffer_);

        return impl_.output();
    }

    void operator()(unsigned char byte)
    {
        process_byte(byte);
    }

    value_type operator()()
    {
        return checksum();
    }

private:
    sha1_detail::block buffer_;
    boost::uint64_t bytes_;
    impl_type impl_;
};

} } // End namespaces checksum, hamigaki.

#endif // HAMIGAKI_CHECKSUM_SHA1_HPP
