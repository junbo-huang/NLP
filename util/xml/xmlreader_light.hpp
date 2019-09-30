#ifndef __XMLREADER_LIGHT_H__
#define __XMLREADER_LIGHT_H__

#pragma once


#if defined(_WINDOWS) || defined(WINDOWS)
#   include <windows.h>
#   pragma warning(disable: 4244)
#   pragma warning(disable: 4250)
#endif

#if defined(_WINDOWS) || defined(WINDOWS)
#define USE_WCHAR       1
#else
#define USE_WCHAR       0
#endif

#if (USE_WCHAR == 1)
typedef wchar_t         char_type;
#define _S(quote)       L ## quote
#define PUGIXML_WCHAR_MODE
#else
typedef char            char_type;
#define _S(quote)       quote
#endif

#include <fstream>
#include <cctype>
#include "pugiconfig.hpp"
#include <functional>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <vector>

typedef std::basic_string<char_type>    string_type;
typedef double							value_type;

typedef std::basic_ifstream<char_type, std::char_traits<char_type>>				ifstream_type;
typedef std::basic_ofstream<char_type, std::char_traits<char_type>>				ofstream_type;
typedef std::basic_stringstream<char_type, std::char_traits<char_type>, std::allocator<char_type>>	stringstream_type;

static const char_type* EMPTY_WSTRING = _S("");
static const char_type* NEWLINE_WSTRING = _S("\n");


//#include "framework.hpp"
#include "pugiconfig.hpp"

#define GET_PRIMITIVE_VALUE_XML(xi_type)                                                                                \
		template <> inline xi_type xmlreader_light::get_value<xi_type>(const string_type& xi_query, const xi_type xi_default) const             \
        {                                                                                                               \
            xi_type result = xi_default;                                                                                \
                                                                                                                        \
            try                                                                                                         \
            {                                                                                                           \
                node_type node(my_document.select_single_node(xi_query.c_str()));                                       \
                                                                                                                        \
                if(node != nullptr)                                                                                     \
                {                                                                                                       \
                    if(node.node() != nullptr)                                                                          \
                    {                                                                                                   \
                        string_type value(node.node().first_child().value());                                           \
						result = parse_from_string_type<xi_type>(trim(value), xi_default);                                              \
                    }                                                                                                   \
                    else if(node.attribute() != nullptr)                                                                \
                    {                                                                                                   \
                        string_type value(node.attribute().value());                                                    \
						result = parse_from_string_type<xi_type>(trim(value), xi_default);                                              \
                    }                                                                                                   \
                }                                                                                                       \
            }                                                                                                           \
            catch(const std::exception&)                                                                                \
            {                                                                                                           \
            }                                                                                                           \
                                                                                                                        \
            return result;                                                                                              \
        }
#define GET_PRIMITIVE_VALUE_GIVEN_NODE_XML(xi_type)                                                                         \
		template <> inline xi_type xmlreader_light::get_value<xi_type>(const node_type& xi_node, const string_type& xi_query, const xi_type xi_default) const             \
        {                                                                                                               \
                                                                                                                        \
            xi_type result = xi_default;                                                                                \
                                                                                                                        \
            try                                                                                                         \
            {                                                                                                           \
                if (xi_node != nullptr && xi_node.node() != nullptr)													\
				{                                                                                                       \
					node_type node(xi_node.node().select_single_node(xi_query.c_str()));                                \
																														\
					if(node != nullptr)                                                                                 \
					{                                                                                                   \
						if(node.node() != nullptr)                                                                      \
						{                                                                                               \
							string_type value(node.node().first_child().value());                                       \
							result = parse_from_string_type<xi_type>(trim(value), xi_default);                         \
						}                                                                                               \
						else if(node.attribute() != nullptr)                                                            \
						{                                                                                               \
							string_type value(node.attribute().value());                                                \
							result = parse_from_string_type<xi_type>(trim(value), xi_default);                         \
						}                                                                                               \
					}																									\
				}                                                                                                       \
            }                                                                                                           \
            catch(const std::exception&)                                                                                \
            {                                                                                                           \
            }                                                                                                           \
                                                                                                                        \
            return result;                                                                                              \
        }
#define GET_PRIMITIVE_VALUES_XML(xi_type)                                                                               \
		template <> inline void xmlreader_light::get_values(const string_type& xi_query, std::vector<xi_type>& xo_values) const                 \
        {                                                                                                               \
            xo_values.clear();                                                                                          \
                                                                                                                        \
            try                                                                                                         \
            {                                                                                                           \
                nodes_type nodes(my_document.select_nodes(xi_query.c_str()));                                           \
                                                                                                                        \
                for(pugi::xpath_node_set::const_iterator it(nodes.begin()), it_end(nodes.end()); it != it_end; ++it)    \
                {                                                                                                       \
                    const node_type& node(*it);                                                                         \
                                                                                                                        \
                    if(node.node() != nullptr)                                                                          \
                    {                                                                                                   \
                        string_type value(node.node().first_child().value());                                           \
						xo_values.push_back(parse_from_string_type<xi_type>(trim(value), xi_type()));                                  \
                    }                                                                                                   \
                    else if(node.attribute() != nullptr)                                                                \
                    {                                                                                                   \
                        string_type value(node.attribute().value());                                                    \
						xo_values.push_back(parse_from_string_type<xi_type>(trim(value), xi_type()));                                  \
                    }                                                                                                   \
                }                                                                                                       \
            }                                                                                                           \
            catch(const std::exception&)                                                                                \
            {                                                                                                           \
            }                                                                                                           \
        }
#define GET_PRIMITIVE_VALUES_XMLR(xi_type)                                                                              \
		template <> inline std::vector<xi_type> xmlreader_light::get_values(const string_type& xi_query) const                                  \
        {                                                                                                               \
            std::vector<xi_type> result;                                                                                \
                                                                                                                        \
            get_values(xi_query, result);                                                                               \
                                                                                                                        \
            return result;                                                                                              \
        }

#define PARSE_PRIMITIVEW_XML(xi_type, xi_func)                                      \
	    template <> inline xi_type xmlreader_light::parse_from_string_type(const string_type& xi_value, xi_type xi_default)  \
        {                                                                           \
            xi_type result = xi_default;                                            \
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

#define PARSE_PRIMITIVEA_XML(xi_type, xi_func)                                      \
	    template <> inline xi_type xmlreader_light::parse_from_string(const std::string& xi_value, xi_type xi_default)   \
        {                                                                           \
            xi_type result = xi_default;                                            \
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

class xmlreader_light
{
    public:
        typedef pugi::xpath_node node_type;
        typedef pugi::xpath_node_set nodes_type;

        typedef pugi::xml_object_range<pugi::xml_named_node_iterator> nodes_range_type;

        typedef pugi::xml_attribute attribute_type;

        typedef pugi::xml_document document_type;

        typedef pugi::xpath_query query_type;
        typedef pugi::xml_parse_result result_type;

    private:
        document_type       my_document;        
        bool                my_loaded;

    public:
        xmlreader_light() : my_loaded(false)
        {
        }

       ~xmlreader_light()
        {
            try
            {
            }
            catch(const std::exception&)
            {
            }
        }

#if (USE_WCHAR == 1)
	   bool load(const std::string& xi_file_name)
	   {
		   bool success = true;

		   try
		   {
			   result_type result(my_document.load_file(xi_file_name.c_str()));

			   if (result.status != pugi::xml_parse_status::status_ok)
			   {
				   success = false;
			   }
			   else
			   {
				   my_loaded = true;
			   }
		   }
		   catch (const std::exception&)
		   {
			   throw;
		   }

		   return success;
	   }
#endif

	   bool load(const string_type& xi_file_name)
	   {
		   bool success = true;

		   try
		   {
			   result_type result(my_document.load_file(xi_file_name.c_str()));

			   if (result.status != pugi::xml_parse_status::status_ok)
			   {
				   success = false;
			   }
			   else
			   {
				   my_loaded = true;
			   }
		   }
		   catch (const std::exception&)
		   {
			   throw;
		   }

		   return success;
	   }


        void save(const string_type& xi_file_name)
        {
            try
            {                

                my_document.save_file(xi_file_name.c_str(), _S("    "));
            }
            catch(const std::exception&)
            {
                throw;
            }
        }

        void save_node(const string_type& xi_query, const string_type& xi_file_name)
        {
            try
            {
                node_type document(get_node(xi_query));

				ofstream_type file_stream;

                file_stream.open(xi_file_name, std::ios::out|std::ios::trunc);

                document.node().print(file_stream);

                file_stream.flush();
                file_stream.close();
            }
            catch(const std::exception&)
            {
                throw;
            }
        }

        void merge(const string_type& xi_query, const string_type& xi_file_name)
        {
            try
            {
                document_type document;

                result_type result(document.load_file(xi_file_name.c_str()));

                node_type junction_node(my_document.select_single_node(xi_query.c_str()));

                junction_node.node().append_copy(document.root().first_child());
            }
            catch(const std::exception&)
            {
                throw;
            }
        }

        void create_root_document(const string_type& xi_root_name)
        {
            try
            {
                my_loaded = false;

                string_type xml(_S("<?xml version=\"1.0\" encoding=\"UTF-8\"?><")+xi_root_name+_S("/>"));

				result_type result(my_document.load_buffer((void*)(xml.c_str()), sizeof(char_type)*xml.size()));

                my_loaded = true;
            }
            catch(const std::exception&)
            {
                throw;
            }
        }

		node_type add_node(const node_type& xi_parent_node, const string_type& xi_node_name)
		{			
			node_type new_node;
			try
			{
				if (xi_parent_node != nullptr && xi_parent_node.node() != nullptr)
				{
					new_node = xi_parent_node.node().append_child(xi_node_name.c_str());
				}
			}
			catch (const std::exception&)
			{
				throw;
			}
			return new_node;
		}

		node_type add_node(const node_type& xi_parent_node, const string_type& xi_node_name, const string_type& xi_value)
		{			
			node_type new_node;

			try
			{
				if (xi_parent_node != nullptr && xi_parent_node.node() != nullptr)
				{
					new_node = xi_parent_node.node().append_child(xi_node_name.c_str());

					if (new_node != nullptr && new_node.node() != nullptr)
					{
						new_node.node().text().set(xi_value.c_str());
					}
				}
			}
			catch (const std::exception&)
			{
				throw;
			}
			return new_node;
		}

        void add_node(const string_type& xi_parent_node, const string_type& xi_node_name, const string_type& xi_attribute_name, const string_type& xi_value)
        {
            try
            {
                node_type parent_node(get_node(xi_parent_node));

                if(parent_node != nullptr && parent_node.node() != nullptr)
                {
                    node_type new_node(parent_node.node().append_child(xi_node_name.c_str()));

                    if(new_node != nullptr && new_node.node() != nullptr && !xi_attribute_name.empty())
                    {
                        new_node.node().append_attribute(xi_attribute_name.c_str()) = xi_value.c_str();
                    }
                }
            }
            catch(const std::exception&)
            {
                throw;
            }
        }
		
		node_type add_node(const node_type& xi_parent_node, const string_type& xi_node_name, const string_type& xi_attribute_name, const string_type& xi_value)
        {            
            node_type new_node;
            try
            {
                if (xi_parent_node != nullptr && xi_parent_node.node() != nullptr)
                {
                    new_node=(xi_parent_node.node().append_child(xi_node_name.c_str()));

                    if (new_node != nullptr && new_node.node() != nullptr && !xi_attribute_name.empty())
                    {
                        new_node.node().append_attribute(xi_attribute_name.c_str()) = xi_value.c_str();
                    }
                }
            }
            catch (const std::exception&)
            {
                throw;
            }
            return new_node;
        }

		void set_node_value(node_type& xi_node, const string_type& xi_value)
		{
			try
			{
				if (xi_node != nullptr && xi_node.node() != nullptr)
				{
					xi_node.node().text().set(xi_value.c_str());
				}
			}
			catch (const std::exception&)
			{
				throw;
			}
		}

        void add_attribute(const string_type& xi_parent_node, const string_type& xi_attribute_name, const string_type& xi_value)
        {
            try
            {
                node_type parent_node(get_node(xi_parent_node));

                if(parent_node != nullptr && parent_node.node() != nullptr)
                {
                    parent_node.node().append_attribute(xi_attribute_name.c_str()) = xi_value.c_str();
                }
            }
            catch(const std::exception&)
            {
                throw;
            }
        }

        void add_attribute(const node_type& xi_parent_node, const string_type& xi_attribute_name, const string_type& xi_value)
        {
            try
            {
                if (xi_parent_node != nullptr && xi_parent_node.node() != nullptr)
                {
                    xi_parent_node.node().append_attribute(xi_attribute_name.c_str()) = xi_value.c_str();
                }
            }
            catch (const std::exception&)
            {
                throw;
            }
        }

        node_type append_child(node_type& xi_parent, const string_type& xi_child_name)
		{
			node_type new_node;
			try
			{			
				new_node = (xi_parent.node().append_child(xi_child_name.c_str()));
			}
			catch (const std::exception&)
			{
				throw;
			}
			return new_node;
		}

        void delete_node(const string_type& xi_query)
        {
            try
            {
                node_type node_to_remove(get_node(xi_query));

                if(node_to_remove != nullptr && node_to_remove.node() != nullptr && node_to_remove.parent() != nullptr)
                {
                    node_to_remove.parent().remove_child(node_to_remove.node());
                }
            }
            catch(const std::exception&)
            {
                throw;
            }
        }

        void delete_nodes(const string_type& xi_parent)
        {
            try
            {
                node_type parent_node(get_node(xi_parent));

                if(parent_node != nullptr && parent_node.node() != nullptr)
                {
					pugi::xml_object_range<pugi::xml_node_iterator> children = parent_node.node().children();
					for (pugi::xml_node_iterator it(children.begin()), it_end(children.end()); it != it_end; ++it)
                    {						
						parent_node.node().remove_child(*it);
                    }
                }
            }
            catch(const std::exception&)
            {
                throw;
            }
        }

    public:
        template <typename T> T get_value(const string_type& xi_query, T xi_default) const
        {
            return xi_default;
        }        

		string_type get_text(const string_type& xi_query) const;
		
		template <typename T> T get_value(const node_type& xi_node, const string_type& xi_query, T xi_default) const
		{
			return xi_default;
		}

        template <typename T> void get_values(const string_type& xi_query, std::vector<T>& xo_values) const
        {
        }

        template <typename T> std::vector<T> get_values(const string_type& xi_query) const
        {
            std::vector<T> result;
            return result;
        }        

		template <typename T> T get_attribute_value(const node_type& xi_node, const char_type* xi_attribute_name, T xi_default)
        {
            return xi_default;
        }		

        node_type get_node(const string_type& xi_query) const
        {
            node_type result;

            try
            {
                result = my_document.select_single_node(xi_query.c_str());
            }
            catch(const std::exception&)
            {
            }

            return result;
        }

        nodes_type get_nodes(const string_type& xi_query) const
        {
            nodes_type result;

            try
            {
                result = my_document.select_nodes(xi_query.c_str());
            }
            catch(const std::exception&)
            {
            }

            return result;
        }
		
		nodes_type get_nodes(const node_type& xi_node, const string_type& xi_query) const
		{
			nodes_type result;

			try
			{
				result = xi_node.node().select_nodes(xi_query.c_str());
			}
			catch (const std::exception&)
			{
			}

			return result;
		}

        node_type get_child_node(const node_type& xi_node, const string_type& xi_node_name) const
        {
            node_type result;

            if(xi_node != nullptr && xi_node.node() != nullptr)
            {
                try
                {
                    result = xi_node.node().find_child
                             (
                                 [&xi_node_name](const node_type& xi_node)
                                 {
                                     return (xi_node.node().name()) == xi_node_name;
                                 }
                             );
                }
                catch(const std::exception&)
                {
                }
            }

            return result;
        }

		template <typename T> T get_child_node_value(const node_type& xi_node, const char_type* xi_node_name, T xi_default) const
        {
            return xi_default;
        }		

        std::vector<node_type> get_children(const node_type& xi_parent, const string_type& xi_name) const
        {
            std::vector<node_type> result;

            try
            {
                if(xi_parent != nullptr && xi_parent.node() != nullptr)
                {
					pugi::xml_object_range<pugi::xml_named_node_iterator> names = xi_parent.node().children(xi_name.c_str());

					for (pugi::xml_named_node_iterator it(names.begin()), it_end(names.end()); it != it_end; ++it)
                    {
						result.push_back(*it);
                    }
                }
            }
            catch(const std::exception&)
            {
            }

            return result;
        }

		std::vector<node_type> get_all_children(const node_type& xi_parent) const
		{
			std::vector<node_type> result;

			try
			{
				if (xi_parent != nullptr && xi_parent.node() != nullptr)
				{
					for (auto node : xi_parent.node().children())
					{
						result.push_back(node);
					}
				}
			}
			catch (const std::exception&)
			{
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

            //xassert(result.good(), x::error::invalidarg, _S("Failed to convert to string."));

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

        template <typename T> static string_type convert_to_string_type(const T& xi_value, int xiN = -1, bool xiFixed = false)
        {
			stringstream_type result;

            if(xiFixed)
            {
                result << std::fixed;
            }

            if(xiN >= 0)
            {
                result << std::setprecision(xiN);
            }

            result << xi_value;

            //xassert(result.good(), x::error::invalidarg, _S("Failed to convert to string."));

            return result.str();
        }

		template <typename T> static bool convert_to_string_type(const T& xi_value, string_type& xo_value)
        {
            bool result = true;

            try
            {
                xo_value = std::to_wstring(xi_value);
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

		template <typename T> static T parse_from_string_type(const string_type&, T xi_default)
        {
            //xassert(false, x::notimplemented, _S("Invalid type."));
            return T();
        }        

		template <typename T> static T parse_from_string(const std::string&, T xi_default)
        {
            //xassert(false, x::notimplemented, _S("Invalid type."));
            return T();
        }      

#define STRING_CONVERSION_BUFFER_SIZE 512

		static string_type string_to_string_type(const std::string& str, const std::locale& locale = std::locale(""))
		{
			/*if (typeid(char_type) == typeid(wchar_t))
			{
			return string_to_wstring(str, locale);
			}
			else*/
			{
				return string_type(str.c_str(), str.c_str() + str.length());
			}
		}

		static std::string string_type_to_string(const string_type& str, const std::locale& locale = std::locale(""))
		{
			/*if (typeid(char_type) == typeid(wchar_t))
			{
			return wstring_to_string(str, locale);
			}
			else*/
			{
				return std::string(str.c_str(), str.c_str() + str.length());
			}
		}

        static std::wstring string_to_wstring(const std::string& xi_s)
        {
#ifdef WINDOWS
            std::wstring result;

            int count = ::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, xi_s.c_str(), -1, 0, 0);

            if(count > 0)
            {
                if(count < STRING_CONVERSION_BUFFER_SIZE/8)
                {
                    wchar_t buffer[STRING_CONVERSION_BUFFER_SIZE];

                    count = ::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, xi_s.c_str(), -1, buffer, count);

                    if(count > 0)
                    {
                        result.assign(buffer);
                    }
                }
                else
                {
                    wchar_t p[MAX_PATH];

                    count = ::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, xi_s.c_str(), -1, p, count);

                    if(count > 0)
                    {
                        result = p;
                    }
                }
            }

            return result;
#else
            std::wstring aux(xi_s.length(), L' ');

            aux.assign(xi_s.begin(), xi_s.end());

            return aux;
#endif
        }

        static std::string wstring_to_string(const std::wstring& xi_s)
        {
#ifdef WINDOWS
            std::string result;

            int count = ::WideCharToMultiByte(CP_ACP, 0, xi_s.c_str(), -1, 0, 0, 0, 0);

            if(count > 0)
            {
                if(count < STRING_CONVERSION_BUFFER_SIZE/8)
                {
                    char buffer[STRING_CONVERSION_BUFFER_SIZE];

                    count = ::WideCharToMultiByte(CP_ACP, 0, xi_s.c_str(), -1, buffer, count, 0, 0);

                    if(count > 0)
                    {
                        result.assign(buffer);
                    }
                }
                else
                {
                    char p[MAX_PATH];

                    count = ::WideCharToMultiByte(CP_ACP, 0, xi_s.c_str(), -1, p, count, 0, 0);

                    if(count > 0)
                    {
                        result = p;
                    }
                }
            }

            return result;
#else
            std::string aux(xi_s.length(), L' ');

            aux.assign(xi_s.begin(), xi_s.end());

            return aux;
#endif
        }

        static inline string_type& trim(string_type& xi_s)
        {
            return ltrim(rtrim(xi_s));
        }

        static inline string_type& ltrim(string_type& xi_s)
        {
            xi_s.erase(xi_s.begin(), std::find_if(xi_s.begin(), xi_s.end(), [](char_type& c) {return !std::isspace(c); }));
            return xi_s;
        }

        static inline string_type& rtrim(string_type& xi_s)
        {
			xi_s.erase(std::find_if(xi_s.rbegin(), xi_s.rend(), [](char_type& c) {return !std::isspace(c); }).base(), xi_s.end());
            return xi_s;
        }
};

PARSE_PRIMITIVEW_XML(int8_t, std::stoi);
PARSE_PRIMITIVEW_XML(int16_t, std::stoi);
PARSE_PRIMITIVEW_XML(int32_t, std::stol);
PARSE_PRIMITIVEW_XML(int64_t, std::stoll);
PARSE_PRIMITIVEW_XML(uint8_t, std::stoul);
PARSE_PRIMITIVEW_XML(uint16_t, std::stoul);
PARSE_PRIMITIVEW_XML(uint32_t, std::stoul);
PARSE_PRIMITIVEW_XML(uint64_t, std::stoull);
PARSE_PRIMITIVEW_XML(float, std::stof);
PARSE_PRIMITIVEW_XML(double, std::stod);

PARSE_PRIMITIVEA_XML(int8_t, std::stoi);
PARSE_PRIMITIVEA_XML(int16_t, std::stoi);
PARSE_PRIMITIVEA_XML(int32_t, std::stol);
PARSE_PRIMITIVEA_XML(int64_t, std::stoll);
PARSE_PRIMITIVEA_XML(uint8_t, std::stoul);
PARSE_PRIMITIVEA_XML(uint16_t, std::stoul);
PARSE_PRIMITIVEA_XML(uint32_t, std::stoul);
PARSE_PRIMITIVEA_XML(uint64_t, std::stoull);
PARSE_PRIMITIVEA_XML(float, std::stof);
PARSE_PRIMITIVEA_XML(double, std::stod);

GET_PRIMITIVE_VALUE_XML(int8_t);
GET_PRIMITIVE_VALUE_XML(int16_t);
GET_PRIMITIVE_VALUE_XML(int32_t);
GET_PRIMITIVE_VALUE_XML(int64_t);
GET_PRIMITIVE_VALUE_XML(uint8_t);
GET_PRIMITIVE_VALUE_XML(uint16_t);
GET_PRIMITIVE_VALUE_XML(uint32_t);
GET_PRIMITIVE_VALUE_XML(uint64_t);
GET_PRIMITIVE_VALUE_XML(float);
GET_PRIMITIVE_VALUE_XML(double);

template <> inline string_type xmlreader_light::get_value<string_type>(const string_type& xi_query, string_type xi_default) const // not ref
{
	string_type result = xi_default;

	try
	{
		node_type node(my_document.select_single_node(xi_query.c_str()));

		if (node != nullptr)
		{
			if (node.node() != nullptr)
			{
				result = node.node().first_child().value();
			}
			else if (node.attribute() != nullptr)
			{
				result = node.attribute().value();
			}
		}
	}
	catch (const std::exception&)
	{
	}

	return result;
}

#if (USE_WCHAR == 1)
template <> inline std::string xmlreader_light::get_value<std::string>(const string_type& xi_query, std::string xi_default) const // not ref
{
	std::string result = xi_default;

	try
	{
		node_type node(my_document.select_single_node(xi_query.c_str()));

		if (node != nullptr)
		{
			if (node.node() != nullptr)
			{
				result = wstring_to_string(node.node().first_child().value());
			}
			else if (node.attribute() != nullptr)
			{
				result = wstring_to_string(node.attribute().value());
			}
		}
	}
	catch (const std::exception&)
	{
	}

	return result;
}
#endif

template <> inline bool xmlreader_light::get_value<bool>(const string_type& xi_query, bool xi_default) const
{
	bool result = xi_default;

	try
	{
		node_type node(my_document.select_single_node(xi_query.c_str()));

		if (node != nullptr)
		{
			if (node.node() != nullptr)
			{
				string_type value(node.node().first_child().value());

				value = trim(value);

				result = value.compare(_S("true")) == 0 || value.compare(_S("1")) == 0 || value.compare(_S("on")) == 0 || value.compare(_S("yes")) == 0;
			}
			else if (node.attribute() != nullptr)
			{
				string_type value(node.attribute().value());

				value = trim(value);

				result = value.compare(_S("true")) == 0 || value.compare(_S("1")) == 0 || value.compare(_S("on")) == 0 || value.compare(_S("yes")) == 0;
			}
		}
	}
	catch (const std::exception&)
	{
	}

	return result;
}

inline string_type xmlreader_light::get_text(const string_type& xi_query) const
{
	return get_value<string_type>(xi_query, NEWLINE_WSTRING);
}

GET_PRIMITIVE_VALUE_GIVEN_NODE_XML(int8_t);
GET_PRIMITIVE_VALUE_GIVEN_NODE_XML(int16_t);
GET_PRIMITIVE_VALUE_GIVEN_NODE_XML(int32_t);
GET_PRIMITIVE_VALUE_GIVEN_NODE_XML(int64_t);
GET_PRIMITIVE_VALUE_GIVEN_NODE_XML(uint8_t);
GET_PRIMITIVE_VALUE_GIVEN_NODE_XML(uint16_t);
GET_PRIMITIVE_VALUE_GIVEN_NODE_XML(uint32_t);
GET_PRIMITIVE_VALUE_GIVEN_NODE_XML(uint64_t);
GET_PRIMITIVE_VALUE_GIVEN_NODE_XML(float);
GET_PRIMITIVE_VALUE_GIVEN_NODE_XML(double);

template <> inline string_type xmlreader_light::get_value<string_type>(const node_type& xi_node, const string_type& xi_query, string_type xi_default) const // not ref
{	

	string_type result = xi_default;

	try
	{		
		if (xi_node != nullptr && xi_node.node() != nullptr)
		{
			node_type node(xi_node.node().select_single_node(xi_query.c_str()));

			if (node != nullptr)
			{
				if (node.node() != nullptr)
				{
					result = node.node().first_child().value();
				}
				else if (node.attribute() != nullptr)
				{
					result = node.attribute().value();
				}
			}
		}
	}
	catch (const std::exception&)
	{
	}

	return result;
}

#if (USE_WCHAR == 1)
template <> inline std::string xmlreader_light::get_value<std::string>(const node_type& xi_node, const string_type& xi_query, std::string xi_default) const // not ref
{
	std::string result = xi_default;

	try
	{		
		if (xi_node != nullptr && xi_node.node() != nullptr)
		{
			node_type node(xi_node.node().select_single_node(xi_query.c_str()));

			if (node != nullptr)
			{
				if (node.node() != nullptr)
				{
					result = string_type_to_string(node.node().first_child().value());
				}
				else if (node.attribute() != nullptr)
				{
					result = string_type_to_string(node.attribute().value());
				}
			}
		}
	}
	catch (const std::exception&)
	{
	}

	return result;
}
#endif

template <> inline bool xmlreader_light::get_value<bool>(const node_type& xi_node, const string_type& xi_query, bool xi_default) const // not ref
{
	bool result = xi_default;

	try
	{		
		if (xi_node != nullptr && xi_node.node() != nullptr)
		{
			node_type node(xi_node.node().select_single_node(xi_query.c_str()));

			if (node != nullptr)
			{
				if (node.node() != nullptr)
				{
					string_type value(node.node().first_child().value());

					value = trim(value);

					result = value.compare(_S("true")) == 0 || value.compare(_S("1")) == 0 || value.compare(_S("on")) == 0 || value.compare(_S("yes")) == 0;
				}
				else if (node.attribute() != nullptr)
				{
					string_type value(node.attribute().value());

					value = trim(value);

					result = value.compare(_S("true")) == 0 || value.compare(_S("1")) == 0 || value.compare(_S("on")) == 0 || value.compare(_S("yes")) == 0;
				}
			}
		}
	}
	catch (const std::exception&)
	{
	}

	return result;
}


GET_PRIMITIVE_VALUES_XML(int8_t)
GET_PRIMITIVE_VALUES_XMLR(int8_t)
GET_PRIMITIVE_VALUES_XML(int16_t);
GET_PRIMITIVE_VALUES_XMLR(int16_t);
GET_PRIMITIVE_VALUES_XML(int32_t);
GET_PRIMITIVE_VALUES_XMLR(int32_t);
GET_PRIMITIVE_VALUES_XML(int64_t);
GET_PRIMITIVE_VALUES_XMLR(int64_t);
GET_PRIMITIVE_VALUES_XML(uint8_t);
GET_PRIMITIVE_VALUES_XMLR(uint8_t);
GET_PRIMITIVE_VALUES_XML(uint16_t);
GET_PRIMITIVE_VALUES_XMLR(uint16_t);
GET_PRIMITIVE_VALUES_XML(uint32_t);
GET_PRIMITIVE_VALUES_XMLR(uint32_t);
GET_PRIMITIVE_VALUES_XML(uint64_t);
GET_PRIMITIVE_VALUES_XMLR(uint64_t);
GET_PRIMITIVE_VALUES_XML(float);
GET_PRIMITIVE_VALUES_XMLR(float);
GET_PRIMITIVE_VALUES_XML(double);
GET_PRIMITIVE_VALUES_XMLR(double);

#if (USE_WCHAR == 1)
template <> inline void xmlreader_light::get_values(const string_type& xi_query, std::vector<string_type>& xo_values) const
{
	xo_values.clear();

	try
	{
		nodes_type nodes(my_document.select_nodes(xi_query.c_str()));

		for (pugi::xpath_node_set::const_iterator it(nodes.begin()), it_end(nodes.end()); it != it_end; ++it)
		{
			const node_type& node(*it);

			if (node.node() != nullptr)
			{
				string_type value(node.node().first_child().value());
				xo_values.push_back(value);
			}
			else if (node.attribute() != nullptr)
			{
				string_type value(node.attribute().value());
				xo_values.push_back(value);
			}
		}
	}
	catch (const std::exception&)
	{
	}
}
#endif

template <> inline std::vector<string_type> xmlreader_light::get_values(const string_type& xi_query) const
{
	std::vector<string_type> result;

	get_values(xi_query, result);

	return result;
}

#if (USE_WCHAR == 1)
template <> inline void xmlreader_light::get_values(const string_type& xi_query, std::vector<std::string>& xo_values) const
{
	xo_values.clear();

	try
	{
		nodes_type nodes(my_document.select_nodes(xi_query.c_str()));

		for (pugi::xpath_node_set::const_iterator it(nodes.begin()), it_end(nodes.end()); it != it_end; ++it)
		{
			const node_type& node(*it);

			if (node.node() != nullptr)
			{
				string_type value(node.node().first_child().value());
				xo_values.push_back("");//??wstring_to_string(value));
			}
			else if (node.attribute() != nullptr)
			{
				string_type value(node.attribute().value());
				xo_values.push_back("");//??wstring_to_string(value));
			}
		}
	}
	catch (const std::exception&)
	{
	}
}

template <> inline std::vector<std::string> xmlreader_light::get_values(const string_type& xi_query) const
{
	std::vector<std::string> result;

	get_values(xi_query, result);

	return result;
}
#endif

template <> inline void xmlreader_light::get_values(const string_type& xi_query, std::vector<bool>& xo_values) const
{
	xo_values.clear();

	try
	{
		nodes_type nodes(my_document.select_nodes(xi_query.c_str()));

		for (pugi::xpath_node_set::const_iterator it(nodes.begin()), it_end(nodes.end()); it != it_end; ++it)
		{
			const node_type& node(*it);

			if (node.node() != nullptr)
			{
				string_type value(node.node().first_child().value());
				value = trim(value);
				xo_values.push_back((value.compare(_S("true")) == 0) || (value.compare(_S("1")) == 0) || (value.compare(_S("on")) == 0) || (value.compare(_S("yes")) == 0));
			}
			else if (node.attribute() != nullptr)
			{
				string_type value(node.attribute().value());
				value = trim(value);
				xo_values.push_back((value.compare(_S("true")) == 0) || (value.compare(_S("1")) == 0) || (value.compare(_S("on")) == 0) || (value.compare(_S("yes")) == 0));
			}
		}
	}
	catch (const std::exception&)
	{
	}
}

template <> inline std::vector<bool> xmlreader_light::get_values(const string_type& xi_query) const
{
	std::vector<bool> result;

	get_values(xi_query, result);

	return result;
}

template <> inline string_type xmlreader_light::get_attribute_value(const node_type& xi_node, const char_type* xi_attribute_name, string_type xi_default)
{
	string_type result;

	attribute_type attribute(xi_node.node().attribute(xi_attribute_name));

	result = attribute != nullptr ? attribute.as_string() : xi_default;

	return result;
}

template <> inline int xmlreader_light::get_attribute_value(const node_type& xi_node, const char_type* xi_attribute_name, int xi_default)
{
	int result;

	attribute_type attribute(xi_node.node().attribute(xi_attribute_name));

	result = attribute != nullptr ? attribute.as_int() : xi_default;

	return result;
}

template <> inline value_type xmlreader_light::get_attribute_value(const node_type& xi_node, const char_type* xi_attribute_name, value_type xi_default)
{
	value_type result;

	attribute_type attribute(xi_node.node().attribute(xi_attribute_name));

	result = attribute != nullptr ? static_cast<value_type>(attribute.as_double()) : xi_default;

	return result;
}

template <> inline bool xmlreader_light::get_attribute_value(const node_type& xi_node, const char_type* xi_attribute_name, bool xi_default)
{
	bool result;

	attribute_type attribute(xi_node.node().attribute(xi_attribute_name));

	result = attribute != nullptr ? attribute.as_bool() : xi_default;

	return result;
}

template <> inline string_type xmlreader_light::get_child_node_value(const node_type& xi_node, const char_type* xi_node_name, string_type xi_default) const
{
	string_type result;

	try
	{
		node_type node(get_child_node(xi_node, xi_node_name));

		if (node != nullptr && node.node() != nullptr)
		{
			result = node.node().text().as_string();
		}
		else
		{
			result = xi_default;
		}
	}
	catch (const std::exception&)
	{
	}

	return result;
}

template <> inline int xmlreader_light::get_child_node_value(const node_type& xi_node, const char_type* xi_node_name, int xi_default) const
{
	int result;

	try
	{
		node_type node(get_child_node(xi_node, xi_node_name));

		if (node != nullptr && node.node() != nullptr)
		{
			result = node.node().text().as_int();
		}
		else
		{
			result = xi_default;
		}
	}
	catch (const std::exception&)
	{
	}

	return result;
}

template <> inline value_type xmlreader_light::get_child_node_value(const node_type& xi_node, const char_type* xi_node_name, value_type xi_default) const
{
	value_type result;

	try
	{
		node_type node(get_child_node(xi_node, xi_node_name));

		if (node != nullptr && node.node() != nullptr)
		{
			result = node.node().text().as_double();
		}
		else
		{
			result = xi_default;
		}
	}
	catch (const std::exception&)
	{
	}

	return result;
}

template <> inline bool xmlreader_light::get_child_node_value(const node_type& xi_node, const char_type* xi_node_name, bool xi_default) const
{
	bool result = false;

	try
	{
		node_type node(get_child_node(xi_node, xi_node_name));

		if (node != nullptr && node.node() != nullptr)
		{
			result = node.node().text().as_bool();
		}
		else
		{
			result = xi_default;
		}
	}
	catch (const std::exception&)
	{
	}

	return result;
}

#endif // __XMLREADER_LIGHT_H__
