#ifndef __FRAMEWORK_H__
#define __FRAMEWORK_H__

#pragma once

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64) || defined(_WINDOWS)
#   define WINDOWS
#endif

#ifdef WINDOWS
#   pragma warning(disable: 4996) // _CRT_SECURE_NO_WARNINGS
#   pragma warning(disable: 4130) // '!=' : logical operation on address of string constant
#   pragma warning(disable: 4127) // conditional expression is constant
#endif

#ifdef WINDOWS
#   include <stdint.h>
#   include <fcntl.h>
#   include <sys/types.h>
#   include <sys/stat.h>
#   include <io.h>
#   include <tchar.h>
#   include <time.h>
#   include <assert.h>
#   define WIN32_LEAN_AND_MEAN
#   define _WINSOCK_DEPRECATED_NO_WARNINGS
#   include <windows.h>
#   include <wincrypt.h >
#   include <ole2.h>
#   include <limits.h>
#   include <excpt.h>
#   include <stdint.h>
#else
#   include <sys/types.h>
#   include <sys/socket.h>
#   include <sys/stat.h>
#   include <netinet/in.h>
#   include <netdb.h>
#   include <errno.h>
#   include <string.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>
#include <cwctype>
#include <functional>
#include <memory>
#include <algorithm>
#include <list>
#include <vector>
#include <queue>
#include <stack>
#include <set>
#include <map>
#include <unordered_map>
#include <unordered_set>
//#include <hash_set>
#include <string>
#include <iomanip>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <type_traits>
#include <atomic>
#include <locale>

#include <features.hpp>

#undef min
#undef max

#ifdef WINDOWS
#   pragma warning(disable: 4996) // _CRT_SECURE_NO_WARNINGS
#   pragma warning(disable: 4251) // needs to have dll-interface to be used by clients of class
#endif

typedef unsigned char   byte;
typedef double          value_type;

#ifdef WINDOWS
#define USE_WCHAR       1
#else
#define USE_WCHAR       0
#endif

#if (USE_WCHAR == 1)
	typedef wchar_t      char_type;
	#define _S(quote)    L ## quote
#else
	typedef char         char_type;
	#define _S(quote)    quote
#endif
#define __S(quote)      _S(quote)

typedef std::basic_string<char_type>    string_type;

#ifdef WINDOWS
#	if (USE_WCHAR == 1)
#		define UIVSNWPRINTF _vsnwprintf_s
#		define UISNWPRINTF  _snwprintf_s
#		define FILEOPEN     _wfopen		
#		define FGETCS      fgetws
#	else
#		define UIVSNWPRINTF _vsnprintf_s
#		define UISNWPRINTF  _snprintf_s
#		define FILEOPEN     _fopen
#		define FGETCS      fgets
#	endif
#   define UIVSNPRINTF  _vsnprintf_s
#   define UISNPRINTF   _snprintf_s
#   define CUT_SHORT    _TRUNCATE	
#else
#   define UIVSNWPRINTF(buf, size, count, format, ...)  vsnprintf(buf, size, format, ##__VA_ARGS__)
#   define UIVSNPRINTF(buf, size, count, format, ...)  vsnprintf(buf, size, format, ##__VA_ARGS__)
#   define UISNWPRINTF(buf, size, count, format, ...)  snprintf(buf, size, format, ##__VA_ARGS__)
#   define UISNPRINTF(buf, size, count, format, ...)   snprintf(buf, size, format, ##__VA_ARGS__)
#   define CUT_SHORT    ((std::size_t)-1)
#   define FILEOPEN     fopen
#	define FGETCS       fgets
#	define interface struct
	typedef unsigned char   BYTE;
	typedef uint64_t     __int64;
#endif

typedef std::basic_ifstream<char_type, std::char_traits<char_type>>				ifstream_type;
typedef std::basic_ofstream<char_type, std::char_traits<char_type>>				ofstream_type;
typedef std::basic_stringstream<char_type, std::char_traits<char_type>, std::allocator<char_type>>	stringstream_type;

#define UISOFT_BEGIN_NAMESPACE(__x) namespace uisoft { namespace __x {
#define UISOFT_END_NAMESPACE }; };
#define UISOFT_USING_NAMESPACE(__x) using namespace uisoft::__x;

#define LITTLE_ENDIAN 0
#define BIG_ENDIAN    1
#define PDP_ENDIAN    2

#define ENDIAN LITTLE_ENDIAN

#if defined(_WIN64)
#   define ALIGN(x, b) (((x)+(b)-1)/(b)*(b))
#else
#   define ALIGN(x, b) (((x)+(b)-1)&~((b)-1))
#endif

#define DEFAULT_ALIGNMENT (16)

#define ABSOLUTEVAL(xiWait)     (xiWait)
#define RELATIVEVAL(xiWait)     (-(xiWait))

#define NANOSECONDS(xiNanos)    (((signed __int64)(xiNanos))/100L)
#define MICROSECONDS(xiMicros)  (((signed __int64)(xiMicros))*NANOSECONDS(1000L))
#define MILLISECONDS(xiMilli)   (((signed __int64)(xiMilli))*MICROSECONDS(1000L))
#define SECONDS(xiSeconds)      (((signed __int64)(xiSeconds))*MILLISECONDS(1000L))

#define MAX_LINE_SIZE (2048)

#ifdef WINDOWS
#   define SHARED_LIBRARY_SUFFIX (_S(".dll"))
#else
#   define SHARED_LIBRARY_SUFFIX (_S(".so"))
#endif

#ifdef WINDOWS
#   if defined(WIN32DLL) || defined(WIN64DLL) || defined(_WINDLL) || defined(WINDLL) || defined(_USRDLL)
#       define DECLSPEC __declspec(dllexport)
#   else
#       define DECLSPEC // __declspec(dllimport)
#   endif

#    define STDCALL __cdecl
#else
#    define DECLSPEC
#    define STDCALL
#endif

#ifdef DISALLOW_COPY_MOVE_ASSIGNMENT
#   undef DISALLOW_COPY_MOVE_ASSIGNMENT
#endif
#define DISALLOW_COPY_MOVE_ASSIGNMENT(__T)              \
        __T(const __T&) = delete;                       \
        __T(__T&&) = delete;                            \
        const __T& operator = (const __T&) = delete;    \
        const __T& operator = (__T&&) = delete;

#define MAX_STRING_SIZE         (32767)
#define MAX_STRING_SIZE_SAFE    (MAX_STRING_SIZE-1)
#define MAX_PATH_SIZE           (32767)
#define MAX_PATH_SIZE_SAFE      (MAX_PATH_SIZE-1)

#ifndef UNREFERENCED_PARAMETER
#   define UNREFERENCED_PARAMETER(x) (x)
#endif

#define MAX_STRING_SIZE                 (32767)

#ifndef PI
#   define PI                           (3.141592653589793)
#endif

static const char_type* EMPTY_WSTRING = _S("");
static const char_type* NEWLINE_WSTRING = _S("\n");

UISOFT_BEGIN_NAMESPACE(framework)

template <typename T, std::size_t N, typename RT = int>
inline /*?? VS 2015 constexpr*/ RT array_size(T (&)[N]) //?? VS 2015 noexcept
{
    return static_cast<RT>(N);
}

template <class T>
void shift_data(std::vector<T>& xio_data, int xi_ele_start_index, int xi_ele_size, int xi_num_elements, int xi_num_ele_shift, bool xi_backward = true)
{
    if(xi_backward)
    {
        memmove(&(xio_data[xi_ele_size*xi_ele_start_index]), &(xio_data[xi_ele_size*(xi_ele_start_index+xi_num_ele_shift)]), (xi_ele_size*(xi_num_elements-1))*sizeof(T));
    }
    else
    {
        memmove(&(xio_data[xi_ele_size*(xi_ele_start_index+xi_num_ele_shift)]), &(xio_data[xi_ele_size*xi_ele_start_index]), (xi_ele_size*(xi_num_elements))*sizeof(T));
    }
}

template <class T>
void copy_data(std::vector<T>& xi_source_data, std::vector<T>& xo_dest_data, int xi_source_ele_start_index, int xi_dest_ele_start_index, int xi_ele_size, int xi_num_elements)
{
    memcpy(&(xo_dest_data[xi_ele_size*xi_dest_ele_start_index]), &(xi_source_data[xi_ele_size*xi_source_ele_start_index]), (xi_ele_size*xi_num_elements)*sizeof(T));
}

inline double insure_range(double xi_value, double xi_minimum, double xi_maximum)
{
    return std::min(std::max(xi_value, xi_minimum), xi_maximum);
}

inline void create_directory(const std::string& xi_directory)
{
#ifdef WINDOWS
    static const std::string separators("\\/");

    DWORD attributes = ::GetFileAttributesA(xi_directory.c_str());

    if(attributes == INVALID_FILE_ATTRIBUTES)
    {
        std::size_t slash_index = xi_directory.find_last_of(separators);

        if(slash_index != std::string::npos)
        {
            create_directory(xi_directory.substr(0, slash_index));
        }

        BOOL result = ::CreateDirectoryA(xi_directory.c_str(), 0);

        if(result == FALSE && GetLastError() != ERROR_ALREADY_EXISTS)
        {
            throw std::runtime_error("Could not create directory");
        }
    }
    else
    {
        bool directory_or_junction = ((attributes & FILE_ATTRIBUTE_DIRECTORY) != 0) || ((attributes & FILE_ATTRIBUTE_REPARSE_POINT) != 0);

        if(!directory_or_junction)
        {
            throw std::runtime_error("Could not create directory because a file with the same name exists");
        }
  }
#else
	// Linux

	// This just finds the last slash
	static const std::string separators("/");
	std::size_t slash_index = xi_directory.find_last_of(separators);

	// If the last slash exists
	if (slash_index != std::wstring::npos)
	{
		// Create parent directory if it doesn't exist
		create_directory(xi_directory.substr(0, slash_index));
	}

	// Make sure something doesn't exist already at that location
	struct stat directory_stat;
	if (stat(xi_directory.c_str(), &directory_stat) == -1) 
	{
		// Check errno, if dir doesn't exist, make the dir
		if (errno == ENOENT) {
			int status = mkdir(xi_directory.c_str(), /* Are these permissions correct? Could not decuce from Windows code */ 0777);
			// Check if it was successful, if not throw with error message
			if (status != 0) {
				throw std::runtime_error(strerror(errno));
			}
		}
		// If the errno is something else, throw with error message
		else {
			throw std::runtime_error(strerror(errno));
		}
	}
	// Something already exists, if it's a file we have an error, otherwise it doesn't need to be created
	else if (!S_ISDIR(directory_stat.st_mode)) 
	{
		throw std::runtime_error("Could not create directory because a file with the same name exists");
	}
#endif
}

inline value_type pow10(int xi_i)
{
    value_type result = 1;

    for(int j = 0; j < xi_i; j++)
    {
        result *= 10;
    }

    return result;
}

inline value_type nice_number(value_type xi_x, bool xi_round) // heckbert
{
    int exp = (int)floor(std::log10(xi_x));

    value_type f = (value_type)(xi_x/pow(10.0, exp));

    value_type nf = 1;

    if(xi_round)
    {
        if(f < 1.5)
        {
            nf = 1;
        }
        else if(f < 3)
        {
            nf = 2;
        }
        else if(f < 7)
        {
            nf = 5;
        }
        else
        {
            nf = 10;
        }
    }
    else
    {
        if(f <= 1)
        {
            nf = 1;
        }
        else if(f <= 2)
        {
            nf = 2;
        }
        else if(f <= 5)
        {
            nf = 5;
        }
        else
        {
            nf = 10;
        }
    }

    return (value_type)(nf*pow(10.0, exp));
}

inline value_type calculate_interval(value_type xi_available_size, value_type xi_max_interval_size, value_type xi_range)
{
    if(xi_available_size <= 0 || !(xi_range > std::numeric_limits<value_type>::epsilon()))
    {
        return xi_range/10; // s potolka
    }

    #define exponent(x) ((value_type)(std::ceil(std::log10((x)))))
    #define mantissa(x) ((value_type)((x)/std::pow(10, exponent((x))-1)))

    // reduce intervals for horizontal axis.
    // real maximum interval count
    value_type maxIntervalCount = xi_available_size / xi_max_interval_size;

    xi_range = std::abs(xi_range);

    value_type interval = std::pow(10, exponent(xi_range));
    value_type tempInterval = interval;

    // decrease interval until interval count becomes less than maxIntervalCount
    for(;;)
    {
        int m = (int)mantissa(tempInterval);
        if (m == 5)
        {
            // reduce 5 to 2
            tempInterval = tempInterval / 2.5f;
        }
        else if (m == 2 || m == 1 || m == 10)
        {
            // reduce 2 to 1, 10 to 5, 1 to 0.5
            tempInterval = tempInterval / 2.0;
        }
        else
        {
            tempInterval = tempInterval / 2.0;
        }

        if(xi_range / tempInterval > maxIntervalCount)
        {
            break;
        }
#ifdef WINDOWS
        if(std::isnan<value_type>(tempInterval) || std::isinf<value_type>(tempInterval))
#else
        if(std::isnan(tempInterval) || std::isinf(tempInterval))
#endif
        {
            break;
        }

        interval = tempInterval;
    }

    interval = nice_number(interval, true);

    return interval;
}

//template<typename E>
//constexpr auto to_integral(E e) -> typename std::underlying_type<E>::type
//{
//    // auto value = to_integral(my_fields::field);
//    // auto redValue = to_integral(Color::Red);
//    // int a[to_integral(my_fields::field)];
//    // std::array<int, to_integral(my_fields::field)> b;
//    return static_cast<typename std::underlying_type<E>::type>(e);
//}

UISOFT_END_NAMESPACE

#endif // __FRAMEWORK_H__
