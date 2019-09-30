#ifndef __SERIALIZATION_CONTEXT_H__
#define __SERIALIZATION_CONTEXT_H__

#pragma once

#include <framework.hpp>
#include <x.hpp>

UISOFT_BEGIN_NAMESPACE(framework)

class serialization_context
{
    public:
        virtual void    get_bool(const string_type& xi_field_name, bool& xo_val)                                = 0;
        virtual void    set_bool(const string_type& xi_field_name, bool xi_val)                                 = 0;

        virtual void    get_bools(const string_type& xi_field_name, std::vector<bool>& xo_val)                  = 0;
        virtual void    set_bools(const string_type& xi_field_name, const std::vector<bool>& xi_val)            = 0;

        virtual void    get_byte(const string_type& xi_field_name, byte& xo_val)                                = 0;
        virtual void    set_byte(const string_type& xi_field_name, byte xi_val)                                 = 0;

        virtual void    get_bytes(const string_type& xi_field_name, std::vector<byte>& xo_val)                  = 0;
        virtual void    set_bytes(const string_type& xi_field_name, const std::vector<byte>& xi_val)            = 0;

        virtual void    get_int8(const string_type& xi_field_name, int8_t& xo_val)                              = 0;
        virtual void    set_int8(const string_type& xi_field_name, int8_t xi_val)                               = 0;

        virtual void    get_int8s(const string_type& xi_field_name, std::vector<int8_t>& xo_val)                = 0;
        virtual void    set_int8s(const string_type& xi_field_name, const std::vector<int8_t>& xi_val)          = 0;

        virtual void    get_uint8(const string_type& xi_field_name, uint8_t& xo_val)                            = 0;
        virtual void    set_uint8(const string_type& xi_field_name, uint8_t xi_val)                             = 0;

        virtual void    get_uint8s(const string_type& xi_field_name, std::vector<uint8_t>& xo_val)              = 0;
        virtual void    set_uint8s(const string_type& xi_field_name, const std::vector<uint8_t>& xi_val)        = 0;

        virtual void    get_int16(const string_type& xi_field_name, int16_t& xo_val)                            = 0;
        virtual void    set_int16(const string_type& xi_field_name, int16_t xi_val)                             = 0;

        virtual void    get_int16s(const string_type& xi_field_name, std::vector<int16_t>& xo_val)              = 0;
        virtual void    set_int16s(const string_type& xi_field_name, const std::vector<int16_t>& xi_val)        = 0;

        virtual void    get_uint16(const string_type& xi_field_name, uint16_t& xo_val)                          = 0;
        virtual void    set_uint16(const string_type& xi_field_name, uint16_t xi_val)                           = 0;

        virtual void    get_uint16s(const string_type& xi_field_name, std::vector<uint16_t>& xo_val)            = 0;
        virtual void    set_uint16s(const string_type& xi_field_name, const std::vector<uint16_t>& xi_val)      = 0;

        virtual void    get_int32(const string_type& xi_field_name, int32_t& xo_val)                            = 0;
        virtual void    set_int32(const string_type& xi_field_name, int32_t xi_val)                             = 0;

        virtual void    get_int32s(const string_type& xi_field_name, std::vector<int32_t>& xo_val)              = 0;
        virtual void    set_int32s(const string_type& xi_field_name, const std::vector<int32_t>& xi_val)        = 0;

        virtual void    get_uint32(const string_type& xi_field_name, uint32_t& xo_val)                          = 0;
        virtual void    set_uint32(const string_type& xi_field_name, uint32_t xi_val)                           = 0;

        virtual void    get_uint32s(const string_type& xi_field_name, std::vector<uint32_t>& xo_val)            = 0;
        virtual void    set_uint32s(const string_type& xi_field_name, const std::vector<uint32_t>& xi_val)      = 0;

        virtual void    get_int64(const string_type& xi_field_name, int64_t& xo_val)                            = 0;
        virtual void    set_int64(const string_type& xi_field_name, int64_t& xi_val)                            = 0;

        virtual void    get_int64s(const string_type& xi_field_name, std::vector<int64_t>& xo_val)              = 0;
        virtual void    set_int64s(const string_type& xi_field_name, const std::vector<int64_t>& xi_val)        = 0;

        virtual void    get_uint64(const string_type& xi_field_name, uint64_t& xo_val)                          = 0;
        virtual void    set_uint64(const string_type& xi_field_name, uint64_t& xi_val)                          = 0;

        virtual void    get_uint64s(const string_type& xi_field_name, std::vector<uint64_t>& xo_val)            = 0;
        virtual void    set_uint64s(const string_type& xi_field_name, const std::vector<uint64_t>& xi_val)      = 0;

        virtual void    get_float(const string_type& xi_field_name, float& xo_val)                              = 0;
        virtual void    set_float(const string_type& xi_field_name, float xi_val)                               = 0;

        virtual void    get_floats(const string_type& xi_field_name, std::vector<float>& xo_val)                = 0;
        virtual void    set_floats(const string_type& xi_field_name, const std::vector<float>& xi_val)          = 0;

        virtual void    get_double(const string_type& xi_field_name, double& xo_val)                            = 0;
        virtual void    set_double(const string_type& xi_field_name, double xi_val)                             = 0;

        virtual void    get_doubles(const string_type& xi_field_name, std::vector<double>& xo_val)              = 0;
        virtual void    set_doubles(const string_type& xi_field_name, const std::vector<double>& xi_val)        = 0;

        virtual void    get_char(const string_type& xi_field_name, char& xo_val)                                = 0;
        virtual void    set_char(const string_type& xi_field_name, char xi_val)                                 = 0;

        virtual void    get_chars(const string_type& xi_field_name, std::vector<char>& xo_val)                  = 0;
        virtual void    set_chars(const string_type& xi_field_name, const std::vector<char>& xi_val)            = 0;

        virtual void    get_wchar(const string_type& xi_field_name, wchar_t& xo_val)                            = 0;
        virtual void    set_wchar(const string_type& xi_field_name, wchar_t xi_val)                             = 0;

        virtual void    get_wchars(const string_type& xi_field_name, std::vector<wchar_t>& xo_val)              = 0;
        virtual void    set_wchars(const string_type& xi_field_name, const std::vector<wchar_t>& xi_val)        = 0;

        virtual void    get_string(const string_type& xi_field_name, string_type& xo_val)                       = 0;
        virtual void    set_string(const string_type& xi_field_name, const string_type& xi_val)                 = 0;

        virtual void    get_strings(const string_type& xi_field_name, std::vector<string_type>& xo_val)         = 0;
        virtual void    set_strings(const string_type& xi_field_name, const std::vector<string_type>& xi_val)   = 0;

        virtual void    get_tm(const string_type& xi_field_name, tm& xo_val)                                    = 0;
        virtual void    set_tm(const string_type& xi_field_name, tm& xi_val)                                    = 0;
        virtual void    get_tms(const string_type& xi_field_name, std::vector<tm>& xo_val)                      = 0;
        virtual void    set_tms(const string_type& xi_field_name, std::vector<tm>& xi_val)                      = 0;

        virtual void    clear()                                                                                 = 0;
};

UISOFT_END_NAMESPACE

#endif // __SERIALIZATION_CONTEXT_H__
