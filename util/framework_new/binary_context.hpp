#ifndef __BINARY_CONTEXT_H__
#define __BINARY_CONTEXT_H__

#pragma once

#include <framework.hpp>
#include <x.hpp>
#include <serialization_context.hpp>

#ifdef WINDOWS
#   pragma warning(disable: 4127) // conditional expression is constant
#endif

UISOFT_BEGIN_NAMESPACE(framework)

#ifdef WINDOWS
#   define htons0(x) (static_cast<unsigned short>(_byteswap_ushort(x)))
#else
#   define htons0(x) ((((uint16_t)(x) & 0xFF00) >> 8) | (((uint16_t)(x) & 0x00FF) << 8))
#endif

#ifdef WINDOWS
#   define htonl0(x) (static_cast<unsigned long>(_byteswap_ulong(x)))
#else
#   define htonl0(x) ((((uint32_t)(x) & 0xFF000000) >> 24) | (((uint32_t)(x) & 0x00FF0000) >> 8) | (((uint32_t)(x) & 0x0000FF00) << 8) | (((uint32_t)(x) & 0x000000FF) << 24))
#endif

#ifdef WINDOWS
#   define hton64(x) (static_cast<__int64>(_byteswap_uint64(x)))
#else
#   define hton64(x) ((((uint64_t)(x) & 0xFF00000000000000ULL) >> 56) | (((uint64_t)(x) & 0x00FF000000000000ULL) >> 40) | (((uint64_t)(x) & 0x0000FF0000000000ULL) >> 24) | (((uint64_t)(x) & 0x000000FF00000000ULL) >> 8) | (((uint64_t)(x) & 0x00000000FF000000ULL) << 8) | (((uint64_t)(x) & 0x0000000000FF0000ULL) << 24) | (((uint64_t)(x) & 0x000000000000FF00ULL) << 40) | (((uint64_t)(x) & 0x00000000000000FFULL) << 56))
#endif

#define htonf0(x) (binary_context::arrange_type<float>(x))
#define htond0(x) (binary_context::arrange_type<double>(x))

#define hton16(x) (binary_context::is_little_endian() ? htons0((x)) : (x));
#define hton32(x) (binary_context::is_little_endian() ? htonl0((x)) : (x));
#define htonfl(x) (binary_context::is_little_endian() ? htonf0((x)) : (x));
#define htondb(x) (binary_context::is_little_endian() ? htond0((x)) : (x));
#define ntoh16(x) (binary_context::is_little_endian() ? htons0((x)) : (x));
#define ntoh32(x) (binary_context::is_little_endian() ? htonl0((x)) : (x));
#define ntoh64(x) (binary_context::is_little_endian() ? hton64((x)) : (x));
#define ntohfl(x) (binary_context::is_little_endian() ? htonf0((x)) : (x));
#define ntohdb(x) (binary_context::is_little_endian() ? htond0((x)) : (x));

#define READ8(xi_n)                         \
    byte* p = (byte*)(&(xi_n));             \
    *(p+0) = my_data[0+my_offset];          \
    my_offset += sizeof(int8_t);

#define WRITE8(xi_n)                        \
    byte* p = (byte*)(&(xi_n));             \
    my_data.push_back(*(p+0));              \
    my_offset += sizeof(int8_t);

#define READ16(xi_n)                        \
    byte* p = (byte*)(&(xi_n));             \
    *(p+0) = my_data[0+my_offset];          \
    *(p+1) = my_data[1+my_offset];          \
    xi_n = ntoh16(xi_n);                    \
    my_offset += sizeof(int16_t);

#define WRITE16(xi_n)                       \
    xi_n = hton16((xi_n));                  \
    byte* p = (byte*)(&(xi_n));             \
    my_data.push_back(*(p+0));              \
    my_data.push_back(*(p+1));              \
    my_offset += sizeof(int16_t);

#define READ32(xi_n)                        \
    byte* p = (byte*)(&(xi_n));             \
    *(p+0) = my_data[0+my_offset];          \
    *(p+1) = my_data[1+my_offset];          \
    *(p+2) = my_data[2+my_offset];          \
    *(p+3) = my_data[3+my_offset];          \
    xi_n = ntoh32((xi_n));                  \
    my_offset += sizeof(int32_t);

#define WRITE32(xi_n)                       \
    xi_n = hton32((xi_n));                  \
    byte* p = (byte*)(&(xi_n));             \
    my_data.push_back(*(p+0));              \
    my_data.push_back(*(p+1));              \
    my_data.push_back(*(p+2));              \
    my_data.push_back(*(p+3));              \
    my_offset += sizeof(int32_t);

#define READ64(xi_n)                        \
    byte* p = (byte*)(&(xi_n));             \
    *(p+0) = my_data[0+my_offset];          \
    *(p+1) = my_data[1+my_offset];          \
    *(p+2) = my_data[2+my_offset];          \
    *(p+3) = my_data[3+my_offset];          \
    *(p+4) = my_data[4+my_offset];          \
    *(p+5) = my_data[5+my_offset];          \
    *(p+6) = my_data[6+my_offset];          \
    *(p+7) = my_data[7+my_offset];          \
    xi_n = ntoh64((xi_n));                  \
    my_offset += sizeof(int64_t);

#define WRITE64(xi_n)                       \
    xi_n = hton64((xi_n));                  \
    byte* p = (byte*)(&(xi_n));             \
    my_data.push_back(*(p+0));              \
    my_data.push_back(*(p+1));              \
    my_data.push_back(*(p+2));              \
    my_data.push_back(*(p+3));              \
    my_data.push_back(*(p+4));              \
    my_data.push_back(*(p+5));              \
    my_data.push_back(*(p+6));              \
    my_data.push_back(*(p+7));              \
    my_offset += sizeof(int64_t);

#define READFLT(xi_n)                       \
    byte* p = (byte*)(&(xi_n));             \
    *(p+0) = my_data[0+my_offset];          \
    *(p+1) = my_data[1+my_offset];          \
    *(p+2) = my_data[2+my_offset];          \
    *(p+3) = my_data[3+my_offset];          \
    xi_n = ntohfl((xi_n));                  \
    my_offset += sizeof(float);

#define WRITEFLT(xi_n)                      \
    xi_n = htonfl((xi_n));                  \
    byte* p = (byte*)(&(xi_n));             \
    my_data.push_back(*(p+0));              \
    my_data.push_back(*(p+1));              \
    my_data.push_back(*(p+2));              \
    my_data.push_back(*(p+3));              \
    my_offset += sizeof(float);

#define READDBL(xi_n)                       \
    byte* p = (byte*)(&(xi_n));             \
    *(p+0) = my_data[0+my_offset];          \
    *(p+1) = my_data[1+my_offset];          \
    *(p+2) = my_data[2+my_offset];          \
    *(p+3) = my_data[3+my_offset];          \
    *(p+4) = my_data[4+my_offset];          \
    *(p+5) = my_data[5+my_offset];          \
    *(p+6) = my_data[6+my_offset];          \
    *(p+7) = my_data[7+my_offset];          \
    xi_n = ntohdb((xi_n));                  \
    my_offset += sizeof(double);

#define WRITEDBL(xi_n)                      \
    xi_n = htondb((xi_n));                  \
    byte* p = (byte*)(&(xi_n));             \
    my_data.push_back(*(p+0));              \
    my_data.push_back(*(p+1));              \
    my_data.push_back(*(p+2));              \
    my_data.push_back(*(p+3));              \
    my_data.push_back(*(p+4));              \
    my_data.push_back(*(p+5));              \
    my_data.push_back(*(p+6));              \
    my_data.push_back(*(p+7));              \
    my_offset += sizeof(double);

#define WRITE8_TO_BUFFER(xi_n,             \
						xi_buffer,			\
						xi_offset)			\
	xi_n = ((xi_n));						\
	byte* p = (byte*)(&(xi_n));             \
	*(xi_buffer + xi_offset + 0) = (*(p + 0));    \
	xi_offset += sizeof(int8_t);

#define WRITE16_TO_BUFFER(xi_n,             \
                          xi_buffer,        \
                          xi_offset)        \
    xi_n = hton16((xi_n));                  \
    byte* p = (byte*)(&(xi_n));             \
    *(xi_buffer+xi_offset+0) = (*(p+0));    \
    *(xi_buffer+xi_offset+1) = (*(p+1));    \
    xi_offset += sizeof(int16_t);

#define WRITE32_TO_BUFFER(xi_n,             \
                          xi_buffer,        \
                          xi_offset)        \
    xi_n = hton32((xi_n));                  \
    byte* p = (byte*)(&(xi_n));             \
    *(xi_buffer+xi_offset+0) = (*(p+0));    \
    *(xi_buffer+xi_offset+1) = (*(p+1));    \
    *(xi_buffer+xi_offset+2) = (*(p+2));    \
    *(xi_buffer+xi_offset+3) = (*(p+3));    \
    xi_offset += sizeof(int32_t);

#define WRITEDBL_TO_BUFFER(xi_n,            \
                           xi_buffer,       \
                           xi_offset)       \
    {                                        \
        xi_n = htondb((xi_n));               \
        byte* p = (byte*)(&(xi_n));          \
        *(xi_buffer+xi_offset+0) = (*(p+0)); \
        *(xi_buffer+xi_offset+1) = (*(p+1)); \
        *(xi_buffer+xi_offset+2) = (*(p+2)); \
        *(xi_buffer+xi_offset+3) = (*(p+3)); \
        *(xi_buffer+xi_offset+4) = (*(p+4)); \
        *(xi_buffer+xi_offset+5) = (*(p+5)); \
        *(xi_buffer+xi_offset+6) = (*(p+6)); \
        *(xi_buffer+xi_offset+7) = (*(p+7)); \
        xi_offset += sizeof(double);         \
    }


class binary_context : public serialization_context
{
    public:
        using data_type = std::vector<byte>;

    private:
        data_type   my_data;
        size_t      my_offset;

    public:
        DISALLOW_COPY_MOVE_ASSIGNMENT(binary_context)

    public:
        explicit binary_context() : my_offset(0)
        {
        }

        explicit binary_context(const data_type& xi_data) : my_offset(0)
        {
            my_data.assign(xi_data.begin(), xi_data.end());
        }

        const data_type& data() const
        {
            return my_data;
        }

        data_type& data()
        {
            return my_data;
        }

        static bool is_little_endian()
        {
            static bool result;

            static bool set = false;

            if(!set)
            {
                int32_t x = 1;
                result = (*(char*)&x == 1);
                set = true;
            }

            return result;
        }

        template <typename T> static T arrange_type(T xi_val)
        {
            T result;

            byte* pval((byte*)&xi_val);
            byte* pres((byte*)&result);

            for(int32_t i = 0, j = sizeof(T)-1; j >= 0;)
            {
                pres[i++] = pval[j--];
            }

            return result;
        }

        void get_bool(const string_type& xi_field_name, bool& xo_val)
        {
            xassert(my_offset < my_data.size(), x::error::invalidcontext, xi_field_name.c_str());

            xo_val = my_data[my_offset++] == 1;
        }

        void set_bool(const string_type& /*xi_field_name*/, bool xi_val)
        {
            my_data.push_back(xi_val ? 1 : 0);
            my_offset++;
        }

        void get_bools(const string_type& xi_field_name, std::vector<bool>& xo_val)
        {
            xo_val.clear();

            xassert(my_offset+sizeof(int32_t) <= my_data.size(), x::error::invalidcontext, xi_field_name.c_str());

            int32_t n;

            READ32(n);

            if(n > 0)
            {
                for(int32_t i = 0; i < n; i++, my_offset++)
                {
                    xassert(my_offset < my_data.size(), x::error::invalidcontext, xi_field_name.c_str());

                    xo_val.push_back(my_data[my_offset] == 1);
                }
            }
        }

        void set_bools(const string_type& /*xi_field_name*/, const std::vector<bool>& xi_val)
        {
            int32_t n = (int32_t)xi_val.size();

            WRITE32(n);

            if(!xi_val.empty())
            {
                for(int32_t i = 0; i < (int32_t)xi_val.size(); i++, my_offset++)
                {
                    my_data.push_back(xi_val[i]);
                }
            }
        }

        void get_byte(const string_type& xi_field_name, byte& xo_val)
        {
            xassert(my_offset < my_data.size(), x::error::invalidcontext, xi_field_name.c_str());

            int8_t n;

            READ8(n);

            xo_val = n;
        }

        void set_byte(const string_type& /*xi_field_name*/, byte xi_val)
        {
            WRITE8(xi_val);
        }

        void get_bytes(const string_type& xi_field_name, std::vector<byte>& xo_val)
        {
            xo_val.clear();

            xassert(my_offset+sizeof(int32_t) <= my_data.size(), x::error::invalidcontext, xi_field_name.c_str());

            int32_t n;

            READ32(n);

            for(int32_t i = 0; i < n; i++)
            {
                xassert(my_offset < my_data.size(), x::error::invalidcontext, xi_field_name.c_str());

                int8_t v;

                READ8(v);

                xo_val.push_back(v);
            }
        }

        void set_bytes(const string_type& /*xi_field_name*/, const std::vector<byte>& xi_val)
        {
            int32_t n = (int32_t)xi_val.size();

            WRITE32(n);

            if(!xi_val.empty())
            {
                for(int32_t i = 0; i < (int32_t)xi_val.size(); i++)
                {
                    int8_t v = xi_val[i]; 

                    WRITE8(v);
                }
            }
        }

        void get_int8(const string_type& xi_field_name, int8_t& xo_val)
        {
            xassert(my_offset < my_data.size(), x::error::invalidcontext, xi_field_name.c_str());

            int8_t n;

            READ8(n);

            xo_val = n;
        }

        void set_int8(const string_type& /*xi_field_name*/, int8_t xi_val)
        {
            WRITE8(xi_val);
        }

        void get_int8s(const string_type& xi_field_name, std::vector<int8_t>& xo_val)
        {
            xo_val.clear();

            xassert(my_offset+sizeof(int32_t) <= my_data.size(), x::error::invalidcontext, xi_field_name.c_str());

            int32_t n;

            READ32(n);

            for(int32_t i = 0; i < n; i++)
            {
                xassert(my_offset < my_data.size(), x::error::invalidcontext, xi_field_name.c_str());

                int8_t v;

                READ8(v);

                xo_val.push_back(v);
            }
        }

        void set_int8s(const string_type& /*xi_field_name*/, const std::vector<int8_t>& xi_val)
        {
            int32_t n = (int32_t)xi_val.size();

            WRITE32(n);

            if(!xi_val.empty())
            {
                for(int32_t i = 0; i < (int32_t)xi_val.size(); i++)
                {
                    int8_t v = xi_val[i]; 

                    WRITE8(v);
                }
            }
        }

        void get_uint8(const string_type& xi_field_name, uint8_t& xo_val)
        {
            xassert(my_offset < my_data.size(), x::error::invalidcontext, xi_field_name.c_str());

            uint8_t n;

            READ8(n);

            xo_val = n;
        }

        void set_uint8(const string_type& /*xi_field_name*/, uint8_t xi_val)
        {
            WRITE8(xi_val);
        }

        void get_uint8s(const string_type& xi_field_name, std::vector<uint8_t>& xo_val)
        {
            xo_val.clear();

            xassert(my_offset+sizeof(int32_t) <= my_data.size(), x::error::invalidcontext, xi_field_name.c_str());

            int32_t n;

            READ32(n);

            for(int32_t i = 0; i < n; i++)
            {
                xassert(my_offset < my_data.size(), x::error::invalidcontext, xi_field_name.c_str());

                uint8_t v;

                READ8(v);

                xo_val.push_back(v);
            }
        }

        void set_uint8s(const string_type& /*xi_field_name*/, const std::vector<uint8_t>& xi_val)
        {
            int32_t n = (int32_t)xi_val.size();

            WRITE32(n);

            if(!xi_val.empty())
            {
                for(int32_t i = 0; i < (int32_t)xi_val.size(); i++)
                {
                    uint8_t v = xi_val[i]; 

                    WRITE8(v);
                }
            }
        }

        void get_int16(const string_type& xi_field_name, int16_t& xo_val)
        {
            xassert(my_offset+sizeof(int16_t) <= my_data.size(), x::error::invalidcontext, xi_field_name.c_str());

            int16_t n;

            READ16(n);

            xo_val = n;
        }

        void set_int16(const string_type& /*xi_field_name*/, int16_t xi_val)
        {
            WRITE16(xi_val);
        }

        void get_int16s(const string_type& xi_field_name, std::vector<int16_t>& xo_val)
        {
            xo_val.clear();

            xassert(my_offset+sizeof(int32_t) <= my_data.size(), x::error::invalidcontext, xi_field_name.c_str());

            int32_t n;

            READ32(n);

            for(int32_t i = 0; i < n; i++)
            {
                xassert(my_offset+sizeof(int16_t) <= my_data.size(), x::error::invalidcontext, xi_field_name.c_str());

                int16_t v;

                READ16(v);

                xo_val.push_back(v);
            }
        }

        void set_int16s(const string_type& /*xi_field_name*/, const std::vector<int16_t>& xi_val)
        {
            int32_t n = (int32_t)xi_val.size();

            WRITE32(n);

            if(!xi_val.empty())
            {
                for(int32_t i = 0; i < (int32_t)xi_val.size(); i++)
                {
                    int16_t v = xi_val[i];

                    WRITE16(v);
                }
            }
        }

        void get_uint16(const string_type& xi_field_name, uint16_t& xo_val)
        {
            xassert(my_offset+sizeof(int16_t) <= my_data.size(), x::error::invalidcontext, xi_field_name.c_str());

            int16_t n;

            READ16(n);

            xo_val = n;
        }

        void set_uint16(const string_type& /*xi_field_name*/, uint16_t xi_val)
        {
            WRITE16(xi_val);
        }

        void get_uint16s(const string_type& xi_field_name, std::vector<uint16_t>& xo_val)
        {
            xo_val.clear();

            xassert(my_offset+sizeof(int32_t) <= my_data.size(), x::error::invalidcontext, xi_field_name.c_str());

            int32_t n;

            READ32(n);

            for(int32_t i = 0; i < n; i++)
            {
                xassert(my_offset+sizeof(int16_t) <= my_data.size(), x::error::invalidcontext, xi_field_name.c_str());

                int16_t v;

                READ16(v);

                xo_val.push_back(v);
            }
        }

        void set_uint16s(const string_type& /*xi_field_name*/, const std::vector<uint16_t>& xi_val)
        {
            int32_t n = (int32_t)xi_val.size();

            WRITE32(n);

            if(!xi_val.empty())
            {
                for(int32_t i = 0; i < (int32_t)xi_val.size(); i++)
                {
                    int16_t v = xi_val[i];

                    WRITE16(v);
                }
            }
        }

        void get_int32(const string_type& xi_field_name, int32_t& xo_val)
        {
            xassert(my_offset+sizeof(int32_t) <= my_data.size(), x::error::invalidcontext, xi_field_name.c_str());

            int32_t n;

            READ32(n);

            xo_val = n;
        }

        void set_int32(const string_type& /*xi_field_name*/, int32_t xi_val)
        {
            WRITE32(xi_val);
        }

        void get_int32s(const string_type& xi_field_name, std::vector<int32_t>& xo_val)
        {
            xo_val.clear();

            xassert(my_offset+sizeof(int32_t) <= my_data.size(), x::error::invalidcontext, xi_field_name.c_str());

            int32_t n;

            READ32(n);

            for(int32_t i = 0; i < n; i++)
            {
                xassert(my_offset+sizeof(int32_t) <= my_data.size(), x::error::invalidcontext, xi_field_name.c_str());

                int32_t v;

                READ32(v);

                xo_val.push_back(v);
            }
        }

        void set_int32s(const string_type& /*xi_field_name*/, const std::vector<int32_t>& xi_val)
        {
            int32_t n = (int32_t)xi_val.size();

            WRITE32(n);

            if(!xi_val.empty())
            {
                for(int32_t i = 0; i < (int32_t)xi_val.size(); i++)
                {
                    int32_t v = xi_val[i];

                    WRITE32(v);
                }
            }
        }

        void get_uint32(const string_type& xi_field_name, uint32_t& xo_val)
        {
            xassert(my_offset+sizeof(int32_t) <= my_data.size(), x::error::invalidcontext, xi_field_name.c_str());

            int32_t n;

            READ32(n);

            xo_val = n;
        }

        void set_uint32(const string_type& /*xi_field_name*/, uint32_t xi_val)
        {
            WRITE32(xi_val);
        }

        void get_uint32s(const string_type& xi_field_name, std::vector<uint32_t>& xo_val)
        {
            xo_val.clear();

            xassert(my_offset+sizeof(int32_t) <= my_data.size(), x::error::invalidcontext, xi_field_name.c_str());

            int32_t n;

            READ32(n);

            for(int32_t i = 0; i < n; i++)
            {
                xassert(my_offset+sizeof(int32_t) <= my_data.size(), x::error::invalidcontext, xi_field_name.c_str());

                int32_t v;

                READ32(v);

                xo_val.push_back(v);
            }
        }

        void set_uint32s(const string_type& /*xi_field_name*/, const std::vector<uint32_t>& xi_val)
        {
            int32_t n = (uint32_t)xi_val.size();

            WRITE32(n);

            if(!xi_val.empty())
            {
                for(int32_t i = 0; i < (int32_t)xi_val.size(); i++)
                {
                    int32_t v = xi_val[i];

                    WRITE32(v);
                }
            }
        }

        void get_int64(const string_type& xi_field_name, int64_t& xo_val)
        {
            xassert(my_offset+sizeof(int64_t) <= my_data.size(), x::error::invalidcontext, xi_field_name.c_str());

            int64_t n;

            READ64(n);

            xo_val = n;
        }

        void set_int64(const string_type& /*xi_field_name*/, int64_t& xi_val)
        {
            int64_t n = xi_val;

            WRITE64(n);
        }

        void get_int64s(const string_type& xi_field_name, std::vector<int64_t>& xo_val)
        {
            xo_val.clear();

            xassert(my_offset+sizeof(int32_t) <= my_data.size(), x::error::invalidcontext, xi_field_name.c_str());

            int32_t n;

            READ32(n);

            for(int32_t i = 0; i < n; i++)
            {
                xassert(my_offset+sizeof(int64_t) <= my_data.size(), x::error::invalidcontext, xi_field_name.c_str());

                int64_t v;

                READ64(v);

                xo_val.push_back(v);
            }
        }

        void set_int64s(const string_type& /*xi_field_name*/, const std::vector<int64_t>& xi_val)
        {
            int32_t n = (int32_t)xi_val.size();

            WRITE32(n);

            if(!xi_val.empty())
            {
                for(int32_t i = 0; i < (int32_t)xi_val.size(); i++)
                {
                    int64_t v = xi_val[i];

                    WRITE64(v);
                }
            }
        }

        void get_uint64(const string_type& xi_field_name, uint64_t& xo_val)
        {
            xassert(my_offset+sizeof(uint64_t) <= my_data.size(), x::error::invalidcontext, xi_field_name.c_str());

            uint64_t n;

            READ64(n);

            xo_val = n;
        }

        void set_uint64(const string_type& /*xi_field_name*/, uint64_t& xi_val)
        {
            uint64_t n = xi_val;

            WRITE64(n);
        }

        void get_uint64s(const string_type& xi_field_name, std::vector<uint64_t>& xo_val)
        {
            xo_val.clear();

            xassert(my_offset+sizeof(int32_t) <= my_data.size(), x::error::invalidcontext, xi_field_name.c_str());

            int32_t n;

            READ32(n);

            for(int32_t i = 0; i < n; i++)
            {
                xassert(my_offset+sizeof(uint64_t) <= my_data.size(), x::error::invalidcontext, xi_field_name.c_str());

                uint64_t v;

                READ64(v);

                xo_val.push_back(v);
            }
        }

        void set_uint64s(const string_type& /*xi_field_name*/, const std::vector<uint64_t>& xi_val)
        {
            int32_t n = (int32_t)xi_val.size();

            WRITE32(n);

            if(!xi_val.empty())
            {
                for(int32_t i = 0; i < (int32_t)xi_val.size(); i++)
                {
                    uint64_t v = xi_val[i];

                    WRITE64(v);
                }
            }
        }

        void get_float(const string_type& xi_field_name, float& xo_val)
        {
            xassert(my_offset+sizeof(float) <= my_data.size(), x::error::invalidcontext, xi_field_name.c_str());

            float n;

            READFLT(n);

            xo_val = n;
        }

        void set_float(const string_type& /*xi_field_name*/, float xi_val)
        {
            WRITEFLT(xi_val);
        }

        void get_floats(const string_type& xi_field_name, std::vector<float>& xo_val)
        {
            xo_val.clear();

            xassert(my_offset+sizeof(int32_t) <= my_data.size(), x::error::invalidcontext, xi_field_name.c_str());

            int32_t n;

            READ32(n);

            for(int32_t i = 0; i < n; i++)
            {
                xassert(my_offset+sizeof(float) <= my_data.size(), x::error::invalidcontext, xi_field_name.c_str());

                float v;

                READFLT(v);

                xo_val.push_back(v);
            }
        }

        void set_floats(const string_type& /*xi_field_name*/, const std::vector<float>& xi_val)
        {
            int32_t n = (int32_t)xi_val.size();

            WRITE32(n);

            if(!xi_val.empty())
            {
                for(int32_t i = 0; i < (int32_t)xi_val.size(); i++)
                {
                    float v = xi_val[i];

                    WRITEFLT(v);
                }
            }
        }

        void get_double(const string_type& xi_field_name, double& xo_val)
        {
            xassert(my_offset+sizeof(double) <= my_data.size(), x::error::invalidcontext, xi_field_name.c_str());

            double n;

            READDBL(n);

            xo_val = n;
        }

        void set_double(const string_type& /*xi_field_name*/, double xi_val)
        {
            WRITEDBL(xi_val);
        }

        void get_doubles(const string_type& xi_field_name, std::vector<double>& xo_val)
        {
            xo_val.clear();

            xassert(my_offset+sizeof(int32_t) <= my_data.size(), x::error::invalidcontext, xi_field_name.c_str());

            int32_t n;

            READ32(n);

            for(int32_t i = 0; i < n; i++)
            {
                xassert(my_offset+sizeof(double) <= my_data.size(), x::error::invalidcontext, xi_field_name.c_str());

                double v;

                READDBL(v);

                xo_val.push_back(v);
            }
        }

        void set_doubles(const string_type& /*xi_field_name*/, const std::vector<double>& xi_val)
        {
            int32_t n = (int32_t)xi_val.size();

            WRITE32(n);

            if(!xi_val.empty())
            {
                for(int32_t i = 0; i < (int32_t)xi_val.size(); i++)
                {
                    double v = xi_val[i];

                    WRITEDBL(v);
                }
            }
        }

        void get_char(const string_type& xi_field_name, char& xo_val)
        {
            xassert(my_offset < my_data.size(), x::error::invalidcontext, xi_field_name.c_str());

            int8_t n;

            READ8(n);

            xo_val = n;
        }

        void set_char(const string_type& /*xi_field_name*/, char xi_val)
        {
            WRITE8(xi_val);
        }

        void get_chars(const string_type& xi_field_name, std::vector<char>& xo_val)
        {
            xo_val.clear();

            xassert(my_offset+sizeof(int32_t) <= my_data.size(), x::error::invalidcontext, xi_field_name.c_str());

            int32_t n;

            READ32(n);

            for(int32_t i = 0; i < n; i++)
            {
                xassert(my_offset < my_data.size(), x::error::invalidcontext, xi_field_name.c_str());

                int8_t v;

                READ8(v);

                xo_val.push_back(v);
            }
        }

        void set_chars(const string_type& /*xi_field_name*/, const std::vector<char>& xi_val)
        {
            int32_t n = (int32_t)xi_val.size();

            WRITE32(n);

            if(!xi_val.empty())
            {
                for(int32_t i = 0; i < (int32_t)xi_val.size(); i++)
                {
                    int8_t v = xi_val[i]; 

                    WRITE8(v);
                }
            }
        }

        void get_wchar(const string_type& xi_field_name, wchar_t& xo_val)
        {
            xassert(my_offset+sizeof(int16_t) <= my_data.size(), x::error::invalidcontext, xi_field_name.c_str());

            wchar_t v;

            READ16(v);

            xo_val = v;
        }

        void set_wchar(const string_type& xi_field_name, wchar_t xi_val)
        {
            WRITE16(xi_val);
        }

        void get_wchars(const string_type& xi_field_name, std::vector<wchar_t>& xo_val)
        {
            xo_val.clear();

            xassert(my_offset+sizeof(int32_t) <= my_data.size(), x::error::invalidcontext, xi_field_name.c_str());

            int32_t n;

            READ32(n);

            for(int32_t i = 0; i < n; i++)
            {
                xassert(my_offset+sizeof(wchar_t) <= my_data.size(), x::error::invalidcontext, xi_field_name.c_str());

                wchar_t v;

                READ16(v);

                xo_val.push_back(v);
            }
        }

        void set_wchars(const string_type& xi_field_name, const std::vector<wchar_t>& xi_val)
        {
            int32_t n = (int32_t)xi_val.size();

            WRITE32(n);

            if(!xi_val.empty())
            {
                for(int32_t i = 0; i < (int32_t)xi_val.size(); i++)
                {
                    wchar_t v = xi_val[i];

                    WRITE16(v);
                }
            }
        }

#if (USE_WCHAR == 1)
        void get_string(const string_type& xi_field_name, std::string& xo_val)
        {
            xo_val.clear();

            xassert(my_offset+sizeof(int32_t) <= my_data.size(), x::error::invalidcontext, xi_field_name.c_str());

            int32_t n;

            READ32(n);

            for(int32_t i = 0; i < n; i++)
            {
                xassert(my_offset+sizeof(int8_t) <= my_data.size(), x::error::invalidcontext, xi_field_name.c_str());

                char v;

                READ8(v);

                xo_val += v;
            }
        }

        void set_string(const string_type& xi_field_name, const std::string& xi_val)
        {
            int32_t n = (int32_t)xi_val.size();

            WRITE32(n);

            if(!xi_val.empty())
            {
                for(int32_t i = 0; i < (int32_t)xi_val.size(); i++)
                {
                    WRITE8(xi_val[i]);
                }
            }
        }

        void get_strings(const string_type& xi_field_name, std::vector<std::string>& xo_val)
        {
            xo_val.clear();

            xassert(my_offset+sizeof(int32_t) <= my_data.size(), x::error::invalidcontext, xi_field_name.c_str());

            int32_t n;

            READ32(n);

            for(int32_t i = 0; i < n; i++)
            {
                std::string s;

                get_string(xi_field_name, s);

                xo_val.push_back(s);
            }
        }

        void set_strings(const string_type& xi_field_name, const std::vector<std::string>& xi_val)
        {
            int32_t n = (int32_t)xi_val.size();

            WRITE32(n);

            if(!xi_val.empty())
            {
                std::vector<std::string>::const_iterator it(xi_val.begin());
                std::vector<std::string>::const_iterator it_end(xi_val.end());

                for(; it != it_end; ++it)
                {
                    set_string(xi_field_name, (std::string&)(*it));
                }
            }
        }
#endif

        void get_string(const string_type& xi_field_name, string_type& xo_val)
        {
            xo_val.clear();

            xassert(my_offset+sizeof(int32_t) <= my_data.size(), x::error::invalidcontext, xi_field_name.c_str());

            int32_t n;

            READ32(n);

            for(int32_t i = 0; i < n; i++)
            {
                xassert(my_offset+sizeof(int16_t) <= my_data.size(), x::error::invalidcontext, xi_field_name.c_str());

                wchar_t v;

                READ16(v);

                xo_val += v;
            }
        }

        void set_string(const string_type& xi_field_name, const string_type& xi_val)
        {
            int32_t n = (int32_t)xi_val.size();

            WRITE32(n);

            if(!xi_val.empty())
            {
                for(int32_t i = 0; i < (int32_t)xi_val.size(); i++)
                {
                    wchar_t v = xi_val[i];

                    WRITE16(v);
                }
            }
        }

        void get_strings(const string_type& xi_field_name, std::vector<string_type>& xo_val)
        {
            xo_val.clear();

            xassert(my_offset+sizeof(int32_t) <= my_data.size(), x::error::invalidcontext, xi_field_name.c_str());

            int32_t n;

            READ32(n);

            for(int32_t i = 0; i < n; i++)
            {
                string_type s;

                get_string(xi_field_name, s);

                xo_val.push_back(s);
            }
        }

        void set_strings(const string_type& xi_field_name, const std::vector<string_type>& xi_val)
        {
            int32_t n = (int32_t)xi_val.size();

            WRITE32(n);

            if(!xi_val.empty())
            {
                std::vector<string_type>::const_iterator it(xi_val.begin());
                std::vector<string_type>::const_iterator it_end(xi_val.end());

                for(; it != it_end; ++it)
                {
                    set_string(xi_field_name, (string_type&)(*it));
                }
            }
        }

        void get_tm(const string_type& xi_field_name, tm& xo_val)
        {
            {
                xassert(my_offset+sizeof(int32_t) <= my_data.size(), x::error::invalidcontext, xi_field_name.c_str());
                READ32(xo_val.tm_sec);
            }
            {
                xassert(my_offset+sizeof(int32_t) <= my_data.size(), x::error::invalidcontext, xi_field_name.c_str());
                READ32(xo_val.tm_min);
            }
            {
                xassert(my_offset+sizeof(int32_t) <= my_data.size(), x::error::invalidcontext, xi_field_name.c_str());
                READ32(xo_val.tm_hour);
            }
            {
                xassert(my_offset+sizeof(int32_t) <= my_data.size(), x::error::invalidcontext, xi_field_name.c_str());
                READ32(xo_val.tm_mday);
            }
            {
                xassert(my_offset+sizeof(int32_t) <= my_data.size(), x::error::invalidcontext, xi_field_name.c_str());
                READ32(xo_val.tm_mon);
            }
            {
                xassert(my_offset+sizeof(int32_t) <= my_data.size(), x::error::invalidcontext, xi_field_name.c_str());
                READ32(xo_val.tm_year);
            }
            {
                xassert(my_offset+sizeof(int32_t) <= my_data.size(), x::error::invalidcontext, xi_field_name.c_str());
                READ32(xo_val.tm_wday);
            }
            {
                xassert(my_offset+sizeof(int32_t) <= my_data.size(), x::error::invalidcontext, xi_field_name.c_str());
                READ32(xo_val.tm_yday);
            }
            {
                xassert(my_offset+sizeof(int32_t) <= my_data.size(), x::error::invalidcontext, xi_field_name.c_str());
                READ32(xo_val.tm_isdst);
            }
        }

        void set_tm(const string_type& /*xi_field_name*/, tm& xi_val)
        {
            {
                int32_t v = xi_val.tm_sec;
                WRITE32(v);
            }
            {
                int32_t v = xi_val.tm_min;
                WRITE32(v);
            }
            {
                int32_t v = xi_val.tm_hour;
                WRITE32(v);
            }
            {
                int32_t v = xi_val.tm_mday;
                WRITE32(v);
            }
            {
                int32_t v = xi_val.tm_mon;
                WRITE32(v);
            }
            {
                int32_t v = xi_val.tm_year;
                WRITE32(v);
            }
            {
                int32_t v = xi_val.tm_wday;
                WRITE32(v);
            }
            {
                int32_t v = xi_val.tm_yday;
                WRITE32(v);
            }
            {
                int32_t v = xi_val.tm_isdst;
                WRITE32(v);
            }
        }

        void get_tms(const string_type& xi_field_name, std::vector<tm>& xo_val)
        {
            xo_val.clear();

            xassert(my_offset+sizeof(int32_t) <= my_data.size(), x::error::invalidcontext, xi_field_name.c_str());

            int32_t n;

            READ32(n);

            for(int32_t i = 0; i < n; i++)
            {
                tm time;

                get_tm(xi_field_name, time);

                xo_val.push_back(time);
            }
        }

        void set_tms(const string_type& xi_field_name, std::vector<tm>& xi_val)
        {
            int32_t n = (int32_t)xi_val.size();

            WRITE32(n);

            if(!xi_val.empty())
            {
                for(int32_t i = 0; i < (int32_t)xi_val.size(); i++)
                {
                    set_tm(xi_field_name, xi_val[i]);
                }
            }
        }

        void clear()
        {
            my_data.clear();
            my_offset = 0;
        }

        // http://base64.sourceforge.net/b64.c
        // encode 3 '8-bit' binary bytes as 4 '6-bit' characters
        static void encode_block(uint8_t* xi_in, uint8_t* xo_out, int32_t xi_len)
        {
            // RFC1113
            static const uint8_t encode_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

            xo_out[0] = (uint8_t)encode_table[(int32_t)(xi_in[0] >> 2)];
            xo_out[1] = (uint8_t)encode_table[(int32_t)(((xi_in[0] & 0x03) << 4) | ((xi_in[1] & 0xF0) >> 4))];
            xo_out[2] = (uint8_t)(xi_len > 1 ? encode_table[(int32_t)(((xi_in[1] & 0x0F) << 2) | ((xi_in[2] & 0xC0) >> 6))] : '=');
            xo_out[3] = (uint8_t)(xi_len > 2 ? encode_table[(int32_t)(xi_in[2] & 0x3F)] : '=');
        }

        // decode 4 '6-bit' characters into 3 8-bit binary bytes
        static void decode_block(uint8_t* xi_in, uint8_t* xo_out)
        {
            xo_out[0] = (uint8_t)((xi_in[0] << 2 | xi_in[1] >> 4) & 0xFF);
            xo_out[1] = (uint8_t)((xi_in[1] << 4 | xi_in[2] >> 2) & 0xFF);
            xo_out[2] = (uint8_t)((((xi_in[2] << 6) & 0xC0) | xi_in[3]) & 0xFF);
        }

        static void to_base64(const byte* xi_data, int xi_data_size, std::string& xo_data)
        {
            xo_data.clear();

            if(xi_data_size > 0)
            {
                xo_data.reserve((xi_data_size+2)/3*4+1);

                uint8_t in[3];
                uint8_t out[4];

                *in = 0;
                *out = 0;

                const uint8_t* data(&xi_data[0]);

                for(size_t k = 0, n = xi_data_size; k < n;)
                {
                    int32_t len = 0;

                    for(int32_t i = 0; i < 3; i++, k++)
                    {
                        if(k < n)
                        {
                            in[i] = data[k];
                            len++;
                        }
                        else
                        {
                            in[i] = 0;
                        }
                    }

                    if(len > 0)
                    {
                        encode_block(in, out, len);

                        for(int32_t i = 0; i < 4; i++)
                        {
                            xo_data += out[i];
                        }
                    }
                }
            }
        }

        static void from_base64(const byte* xi_data, int xi_data_size, data_type& xo_data)
        {
            xassert(xi_data_size % 4 == 0, x::error::invalidarg, _S("Unable to decode base64 data. Invalid input."))

            static const char decode_table[]="|$$$}rstuvwxyz{$$$$$$$>?@ABCDEFGHIJKLMNOPQRSTUVW$$$$$$XYZ[\\]^_`abcdefghijklmnopq";

            static const uint8_t validation_table[] =
            {
                /*(nul)*/ 0x00, /*(soh)*/ 0x00, /*(stx)*/ 0x00, /*(etx)*/ 0x00, /*(eot)*/ 0x00, /*(enq)*/ 0x00, /*(ack)*/ 0x00, /*(bel)*/ 0x00,
                /*(bs) */ 0x00, /*(ht) */ 0x00, /*(nl) */ 0x00, /*(vt) */ 0x00, /*(np) */ 0x00, /*(cr) */ 0x00, /*(so) */ 0x00, /*(si) */ 0x00,
                /*(dle)*/ 0x00, /*(dc1)*/ 0x00, /*(dc2)*/ 0x00, /*(dc3)*/ 0x00, /*(dc4)*/ 0x00, /*(nak)*/ 0x00, /*(syn)*/ 0x00, /*(etb)*/ 0x00,
                /*(can)*/ 0x00, /*(em) */ 0x00, /*(sub)*/ 0x00, /*(esc)*/ 0x00, /*(fs) */ 0x00, /*(gs) */ 0x00, /*(rs) */ 0x00, /*(us) */ 0x00,

                /*(sp)*/ 0x00, /*!   */ 0x00, /*"   */ 0x00, /*#   */ 0x00, /*$   */ 0x00, /*%   */ 0x00, /*&   */ 0x00, /*'   */ 0x00,
                /*(   */ 0x00, /*)   */ 0x00, /**   */ 0x00, /*+   */ 0x01, /*,   */ 0x00, /*-   */ 0x00, /*.   */ 0x00, /*/   */ 0x01,
                /*0   */ 0x01, /*1   */ 0x01, /*2   */ 0x01, /*3   */ 0x01, /*4   */ 0x01, /*5   */ 0x01, /*6   */ 0x01, /*7   */ 0x01,
                /*8   */ 0x01, /*9   */ 0x01, /*:   */ 0x00, /*;   */ 0x00, /*<   */ 0x00, /*=   */ 0x01, /*>   */ 0x00, /*?   */ 0x00, 

                /*@*/ 0x00, /*A*/ 0x01, /*B*/ 0x01, /*C*/ 0x01, /*D*/ 0x01, /*E*/ 0x01, /*F*/ 0x01, /*G*/ 0x01,
                /*H*/ 0x01, /*I*/ 0x01, /*J*/ 0x01, /*K*/ 0x01, /*L*/ 0x01, /*M*/ 0x01, /*N*/ 0x01, /*O*/ 0x01, 
                /*P*/ 0x01, /*Q*/ 0x01, /*R*/ 0x01, /*S*/ 0x01, /*T*/ 0x01, /*U*/ 0x01, /*V*/ 0x01, /*W*/ 0x01, 
                /*X*/ 0x01, /*Y*/ 0x01, /*Z*/ 0x01, /*[*/ 0x00, /*\*/ 0x00, /*]*/ 0x00, /*^*/ 0x00, /*_*/ 0x00, 

                /*`*/ 0x00, /*a*/ 0x01, /*b*/ 0x01, /*c*/ 0x01, /*d*/ 0x01, /*e*/ 0x01, /*f*/ 0x01, /*g    */ 0x01,
                /*h*/ 0x01, /*i*/ 0x01, /*j*/ 0x01, /*k*/ 0x01, /*l*/ 0x01, /*m*/ 0x01, /*n*/ 0x01, /*o    */ 0x01,
                /*p*/ 0x01, /*q*/ 0x01, /*r*/ 0x01, /*s*/ 0x01, /*t*/ 0x01, /*u*/ 0x01, /*v*/ 0x01, /*w    */ 0x01,
                /*x*/ 0x01, /*y*/ 0x01, /*z*/ 0x01, /*{*/ 0x00, /*|*/ 0x00, /*}*/ 0x00, /*~*/ 0x00, /*(del)*/ 0x00
            };

            xo_data.clear();

            if(xi_data_size > 0)
            {
                xo_data.reserve(xi_data_size/4*3+2);

                uint8_t in[4];
                uint8_t out[3];

                *in = 0;
                *out = 0;

                const byte* data(xi_data); // must be contiguous

                for(size_t k = 0, n = xi_data_size; k < n;)
                {
                    int32_t len = 0;

                    for(int32_t i = 0; i < 4; i++, k++)
                    {
                        int ch = data[k];

                        bool valid = (ch >= 0x2B && ch <= 0x7A) &&   // whithin table
                                     (validation_table[ch] == 0x01); // and not in gaps

                        xassert(valid, x::error::invalidarg, _S("Unable to decode base64 data. Invalid input: cahracter [%x], position [%d]."), ch, (int)k)

                        ch = decode_table[ch-0x2B];

                        if(ch != 0)
                        {
                            ch = ((ch == (int32_t)'$') ? 0 : ch-0x3D);
                        }

                        if(ch != 0)
                        {
                            in[i] = (uint8_t)(ch-1);
                            len++;
                        }
                        else
                        {
                            in[i] = 0;
                        }
                    }

                    if(len > 0)
                    {
                        decode_block(in, out);

                        for(int32_t i = 0; i < len-1; i++)
                        {
                            xo_data.push_back(out[i]);
                        }
                    }
                }
            }
        }
};

UISOFT_END_NAMESPACE

#endif // __BINARY_CONTEXT_H__
