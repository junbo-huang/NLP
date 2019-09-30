#ifndef __TEXT_H__
#define __TEXT_H__

#pragma once

#include <framework.hpp>
#include <regex>

#include <boost/algorithm/string.hpp>
#include <regex>

UISOFT_BEGIN_NAMESPACE(framework)

class text
{
    public:
        static inline string_type trim(string_type& xi_s)
        {
            string_type rtrimmed_string(text::rtrim(xi_s));
            return text::ltrim(rtrimmed_string);
        }

        static inline string_type ltrim(string_type& xi_s)
        {
			xi_s.erase(xi_s.begin(), std::find_if(xi_s.begin(), xi_s.end(), std::not1(std::function<int(char_type)>(std::iswspace))));
            return xi_s;
        }

        static inline string_type rtrim(string_type& xi_s)
        {
			xi_s.erase(std::find_if(xi_s.rbegin(), xi_s.rend(), std::not1(std::function<int(char_type)>(std::iswspace))).base(), xi_s.end());
            return xi_s;
        }

        static void replace(const string_type& xi_pattern, const string_type& xi_replace, string_type& xio_src)
        {
			bool once = false;
			if (xi_replace.find(xi_pattern) != string_type::npos)
			{
				once = true;
			}

			for (size_t i = 0; (i = xio_src.find(xi_pattern, i)) != string_type::npos; i++)
			{
				xio_src.replace(i++, xi_pattern.length(), xi_replace);
				if (once)
				{
					break;
				}
			}
        }

		static void replace_whole_word(const string_type& xi_pattern, const string_type& xi_replace, string_type& xio_src, bool xi_once = false)
		{
			//check this function
			std::basic_regex<char_type> r(_S("\\b") + xi_pattern + _S("\\b"));
			string_type src = xio_src;
			int src_start = 0;
						
			std::match_results<string_type::const_iterator> match;
			while (std::regex_search(src, match, r))
			{
				bool done = false;
				for (unsigned i = 0; i<match.size(); ++i)
				{
					xio_src.replace(src_start + match.position(i), xi_pattern.length(), xi_replace);
					if (xi_once)
					{
						done = true;
						break;
					}
				}

				if (done)
				{
					break;
				}
				else
				{
					src = match.suffix();
					src_start = src_start + (int)match.position(match.size() - 1) + (int)xi_pattern.length();
				}
			}
		}

        template <typename T>
        static void split(const T& xi_string,
                          const T& xi_delimiters,
                          std::vector<T>& xo_result,
                          bool xi_include_empty_result = true,
                          int xi_atleast = -1,
                          bool xi_one_char_column = false)
        {
            typename T::size_type last = 0;
            typename T::size_type pos = 0;

            T buff;

            while((pos = xi_string.find_first_of(xi_delimiters, last)) != T::npos)
            {
                buff.assign(xi_string, last, pos-last);

                if(xi_include_empty_result || !buff.empty())
                {
                    xo_result.push_back(buff);
                }

                if(xo_result.size() == xi_atleast)
                {
                    return;
                }

                last = pos+1;

                if(!xi_one_char_column) // skip 'delims'
                {
                    while(last < xi_string.length() && xi_delimiters.find(xi_string[last]) != T::npos)
                    {
                        ++last;
                    }
                }
            }

            // we always have column after 'delims', like "a|b|c|" has 4 columns: 'a', 'b', 'c', ''
            buff.assign(xi_string, last, xi_string.length()-last);

            if(xi_include_empty_result || !buff.empty())
            {
                xo_result.push_back(buff);
            }
        }

        //static void to_lower(const string_type& xi_string, string_type& xo_result)
        //{
        //    xo_result = xi_string;
        //    boost::algorithm::to_lower(xo_result);
        //}

        //static string_type to_lower(string_type& xi_string)
        //{
        //    boost::algorithm::to_lower(xi_string);
        //    return xi_string;
        //}

        //static void to_upper(const string_type& xi_string, string_type& xo_result)
        //{
        //    xo_result = xi_string;
        //    boost::algorithm::to_upper(xo_result);
        //}

        //static string_type to_upper(string_type& xi_string)
        //{
        //    boost::algorithm::to_upper(xi_string);
        //    return xi_string;
        //}

//        static std::wstring string_to_wstring(const char* xi_s)
//        {
//            return text::string_to_wstring(std::string(xi_s));
//        }
//
//        static std::wstring string_to_wstring(const std::string& xi_s)
//        {
//            std::wstring result;
//
//#ifdef WINDOWS
//            int count = ::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, xi_s.c_str(), -1, 0, 0);
//
//            if(count > 0)
//            {
//                if(count < MAX_STRING_SIZE_SAFE/8)
//                {
//                    wchar_t buffer[MAX_STRING_SIZE];
//
//                    count = ::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, xi_s.c_str(), -1, buffer, count);
//
//                    if(count > 0)
//                    {
//                        result.assign(buffer);
//                    }
//                }
//                else
//                {
//                    std::unique_ptr<wchar_t> p(new wchar_t[count]);
//
//                    count = ::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, xi_s.c_str(), -1, p.get(), count);
//
//                    if(count > 0)
//                    {
//                        result.assign(p.get());
//                    }
//                }
//            }
//#else
//#endif
//            return result;
//        }
//
//        static std::string wstring_to_string(const wchar_t* xi_s)
//        {
//            return text::wstring_to_string(std::wstring(xi_s));
//        }
//
//        static std::string wstring_to_string(const std::wstring& xi_s)
//        {
//            std::string result;
//
//#ifdef WINDOWS
//            int count = ::WideCharToMultiByte(CP_ACP, 0, xi_s.c_str(), -1, 0, 0, 0, 0);
//
//            if(count > 0)
//            {
//                if(count < MAX_STRING_SIZE_SAFE/8)
//                {
//                    char buffer[MAX_STRING_SIZE];
//
//                    count = ::WideCharToMultiByte(CP_ACP, 0, xi_s.c_str(), -1, buffer, count, 0, 0);
//
//                    if(count > 0)
//                    {
//                        result.assign(buffer);
//                    }
//                }
//                else
//                {
//                    std::unique_ptr<char> p(new char[count]);
//
//                    count = ::WideCharToMultiByte(CP_ACP, 0, xi_s.c_str(), -1, p.get(), count, 0, 0);
//
//                    if(count > 0)
//                    {
//                        result.assign(p.get());
//                    }
//                }
//            }
//#else
//#endif
//            return result;
//        }
        
		/*
		template <typename T>
		static const T* str_last_str(const T* xi_string, const T* xi_pattern)
		{
			const T* result(0);

			if (xi_string != 0 && xi_pattern != 0)
			{
				const T* found(0);

				for (size_t pos = 0; (found = ::wcsstr(xi_string + pos, xi_pattern)) != 0;)
				{
					result = found;
					pos = (found - xi_string) + 1;
				}
			}

			return result;
		}*/

		static const wchar_t* str_last_str(const wchar_t* xi_string, const wchar_t* xi_pattern)
        {
			const wchar_t* result(0);

            if(xi_string != 0 && xi_pattern != 0)
            {
				const wchar_t* found(0);

                for(size_t pos = 0; (found = ::wcsstr(xi_string+pos, xi_pattern)) != 0;)
                {
                    result = found;
                    pos = (found-xi_string)+1;
                }
            }

            return result;
        }
		        
        static const char* str_last_str(const char* xi_string, const char* xi_pattern)
        {
            const char* result(0);

            if(xi_string != 0 && xi_pattern != 0)
            {
                const char* found(0);

                for(size_t pos = 0; (found = ::strstr(xi_string+pos, xi_pattern)) != 0;)
                {
                    result = found;
                    pos = (found-xi_string)+1;
                }
            }

            return result;
        }

        template <typename T>
        static int compare(const std::basic_string<T, std::char_traits<T>, std::allocator<T>>& s1,
                           const std::basic_string<T, std::char_traits<T>, std::allocator<T>>& s2,
                           const std::locale& locale = std::locale(""))
        {
            /*using string_type = std::basic_string<T, std::char_traits<T>, std::allocator<T>>;

			const std::collate<string_type::value_type>& collate(std::use_facet<std::collate<string_type::value_type>>(locale));*/
            const std::collate<T>& collate(std::use_facet<std::collate<T>>(locale));

            return collate.compare(s1.data(), s1.data()+s1.size(), s2.data(), s2.data()+s2.size());
        }

        template <typename T>
        static bool equal(const std::basic_string<T, std::char_traits<T>, std::allocator<T>>& s1,
                          const std::basic_string<T, std::char_traits<T>, std::allocator<T>>& s2,
                          const std::locale& locale = std::locale(""))
        {
            return text::compare(s1, s2, locale) == 0;
        }

		template <typename T>
		static bool iequal(const std::basic_string<T, std::char_traits<T>, std::allocator<T>>& s1,
			const std::basic_string<T, std::char_traits<T>, std::allocator<T>>& s2,
			const std::locale& locale = std::locale(""))
		{
			return boost::iequals(s1, s2, locale);
		}


		static string_type string_to_string_type(const std::string& str)
		{
			return string_type(str.c_str(), str.c_str() + str.length());
		}

		static std::string string_type_to_string(const string_type& str)
		{
			return std::string(str.c_str(), str.c_str() + str.length());			
		}

        static std::wstring string_to_wstring(const std::string& str, const std::locale& locale = std::locale(""))
        {
            std::wstring result;

            if(!str.empty())
            {
                std::vector<wchar_t> buffer(str.size());

                std::use_facet<std::ctype<wchar_t>>(locale).widen(str.data(), str.data()+str.size(), buffer.data());

                result.assign(buffer.data(), buffer.size());
            }

            return result;
        }

        static std::string wstring_to_string(const std::wstring& str, const std::locale& locale = std::locale(""))
        {
            std::string result;

            if(!str.empty())
            {
                std::vector<char> buffer(str.size());

                std::use_facet<std::ctype<wchar_t>>(locale).narrow(str.data(), str.data()+str.size(), '?', buffer.data());

                result.assign(buffer.data(), buffer.size());
            }

            return result;
        }

        template <typename T>
        static std::basic_string<T, std::char_traits<T>, std::allocator<T>> to_upper(const std::basic_string<T, std::char_traits<T>, std::allocator<T>>& str,
                                                                                     const std::locale& locale = std::locale(""))
        {
            using char_type = T;
            using str_type = std::basic_string<T, std::char_traits<T>, std::allocator<T>>;

            str_type aux_str(str);

            std::use_facet<std::ctype<char_type>>(locale).toupper(&aux_str[0], &aux_str[0]+aux_str.size());

            return aux_str;
        }

        template <typename T>
        static std::basic_string<T, std::char_traits<T>, std::allocator<T>> to_lower(const std::basic_string<T, std::char_traits<T>, std::allocator<T>>& str,
                                                                                     const std::locale& locale = std::locale(""))
        {
            using char_type = T;
            using str_type = std::basic_string<T, std::char_traits<T>, std::allocator<T>>;

            str_type aux_str(str);

            std::use_facet<std::ctype<char_type>>(locale).tolower(&aux_str[0], &aux_str[0]+aux_str.size());

            return aux_str;
        }
};

UISOFT_END_NAMESPACE

#endif // __TEXT_H__
