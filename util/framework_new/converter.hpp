#ifndef __CONVERTER_H__
#define __CONVERTER_H__

#pragma once

#include <framework.hpp>
#include <x.hpp>

UISOFT_BEGIN_NAMESPACE(framework)

#define PARSE_PRIMITIVEW(xi_type, xi_func)                                          \
		template <> inline xi_type converter::parse_from_string_type(const string_type& xi_value)  \
        {                                                                           \
            typedef xi_type value_type;                                             \
                                                                                    \
            value_type result = value_type();                                       \
                                                                                    \
            try                                                                     \
            {                                                                       \
                result = (xi_type)xi_func(xi_value);                                \
            }                                                                       \
            catch(const std::invalid_argument&)                                     \
            {                                                                       \
            }                                                                       \
            catch(const std::out_of_range&)                                         \
            {                                                                       \
            }                                                                       \
                                                                                    \
            return result;                                                          \
        }

#define PARSE_PRIMITIVEA(xi_type, xi_func)                                          \
		template <> inline xi_type converter::parse_from_string(const std::string& xi_value)   \
        {                                                                           \
            typedef xi_type value_type;                                             \
                                                                                    \
            value_type result = value_type();                                       \
                                                                                    \
            try                                                                     \
            {                                                                       \
                result = (xi_type)xi_func(xi_value);                                \
            }                                                                       \
            catch(const std::invalid_argument&)                                     \
            {                                                                       \
            }                                                                       \
            catch(const std::out_of_range&)                                         \
            {                                                                       \
            }                                                                       \
                                                                                    \
            return result;                                                          \
        }

class converter
{
    public:
		template <typename T> static string_type convert_to_string_type(const T& xi_value, int xiN = -1, bool xiFixed = false)
		{
			stringstream_type result;

			if (xiFixed)
			{
				result << std::fixed;
			}

			if (xiN >= 0)
			{
				result << std::setprecision(xiN);
			}

			result << xi_value;

			xassert(result.good(), x::error::invalidarg, _S("Failed to convert to string."));

			return result.str();
		}

		template <typename T> static bool convert_to_string_type(const T& xi_value, string_type& xo_value, int xiN = -1, bool xiFixed = false)
		{
			bool result = true;

			try
			{
#if (USE_WCHAR == 1)
				xo_value = std::to_wstring(xi_value);
#else
				xo_value = std::to_string(xi_value);
#endif
			}
			catch (const std::invalid_argument&)
			{
				result = false;
			}
			catch (const std::out_of_range&)
			{
				result = false;
			}

			return result;
		}

        template <typename T> static std::string convert_to_string(const T& xi_value, int xiN = -1, bool xiFixed = false)
        {
            std::ostringstream result;

            if(xiFixed)
            {
                result << std::fixed;
            }

            if(xiN >= 0)
            {
                result << std::setprecision(xiN);
            }

            result << xi_value;

            xassert(result.good(), x::error::invalidarg, _S("Failed to convert to string."));

            return result.str();
        }

        template <typename T> static bool convert_to_string(const T& xi_value, std::string& xo_value, int xiN = -1, bool xiFixed = false)
        {
            bool result = true;

            try
            {
                xo_value = std::to_string(xi_value);
            }
            catch(const std::invalid_argument&)
            {
                result = false;
            }
            catch(const std::out_of_range&)
            {
                result = false;
            }

            return result;
        }        

        template <typename T> static T parse_from_string_type(const string_type&)
        {
			xassert(false, x::error::notimplemented, _S("Invalid type."));
            return T();
        }

        

        template <typename T> static T parse_from_string(const std::string&)
        {
			xassert(false, x::error::notimplemented, _S("Invalid type."));
            return T();
        }       
};

PARSE_PRIMITIVEW(int8_t, std::stoi);
PARSE_PRIMITIVEW(int16_t, std::stoi);
PARSE_PRIMITIVEW(int32_t, std::stol);
PARSE_PRIMITIVEW(int64_t, std::stoll);
PARSE_PRIMITIVEW(uint8_t, std::stoul);
PARSE_PRIMITIVEW(uint16_t, std::stoul);
PARSE_PRIMITIVEW(uint32_t, std::stoul);
PARSE_PRIMITIVEW(uint64_t, std::stoull);
PARSE_PRIMITIVEW(float, std::stof);
PARSE_PRIMITIVEW(double, std::stod);

PARSE_PRIMITIVEA(int8_t, std::stoi);
PARSE_PRIMITIVEA(int16_t, std::stoi);
PARSE_PRIMITIVEA(int32_t, std::stol);
PARSE_PRIMITIVEA(int64_t, std::stoll);
PARSE_PRIMITIVEA(uint8_t, std::stoul);
PARSE_PRIMITIVEA(uint16_t, std::stoul);
PARSE_PRIMITIVEA(uint32_t, std::stoul);
PARSE_PRIMITIVEA(uint64_t, std::stoull);
PARSE_PRIMITIVEA(float, std::stof);
PARSE_PRIMITIVEA(double, std::stod);

UISOFT_END_NAMESPACE

#endif // __CONVERTER_H__
