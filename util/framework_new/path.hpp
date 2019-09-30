#ifndef __PATH_H__
#define __PATH_H__

#pragma once

#include <framework.hpp>
#include <sys/types.h>
#include <sys/stat.h>

#ifdef WINDOWS
#else
#include <dirent.h> // Requried for scandir with alphasort
#include <unistd.h> // Required for get_current_dir_name
#endif // WINDOWS

UISOFT_BEGIN_NAMESPACE(framework)

class path
{
    public:
        DISALLOW_COPY_MOVE_ASSIGNMENT(path)

    public:
        template <typename T, typename F>
		static void enumerate(const T& xi_root, F xi_filter, bool xi_recursive, std::vector<T>& xo_result)
		{
#ifdef WINDOWS
			WIN32_FIND_DATA file_information;

			T root(xi_root);

			HANDLE hfile = ::FindFirstFile(T(root + _TEXT("\\*.*")).c_str(), &file_information);

			if (hfile != INVALID_HANDLE_VALUE)
			{
				do
				{
					if (file_information.cFileName[0] != _TEXT('.'))
					{
						T path(root + _TEXT("\\") + file_information.cFileName);

						if (file_information.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
						{
							if (xi_recursive)
							{
								enumerate(path, xi_filter, xi_recursive, xo_result);
							}
						}
						else
						{
							if (xi_filter(file_information.cFileName))
							{
								xo_result.push_back(path);
							}
						}
					}

				} while (::FindNextFile(hfile, &file_information) == TRUE);
			}

			::FindClose(hfile);
#else
			// RYANS CODE:
			//not exactly sure what this function is meant to do, but this the linux equivalent from my understanding
			//parse through files until we reach the end, which is a non-directory, is my understanding

			struct dirent **namelist = NULL;
			int n;
			T root(xi_root);
			// Replacing this line to not use xi_filter:
			//n = scandir(xi_root.c_str(), &namelist, xi_filter, alphasort);
			// Because scandir is expecting a function that returns int rather than bool
			// - Dave
			n = scandir(xi_root.c_str(), &namelist, NULL, alphasort);
			if (n >= 0)
			{
				while (n--)
				{
					T path(root + __S("/") + namelist[n]->d_name);
					struct stat s;
					stat(path.c_str(), &s);

					if(S_ISDIR(s.st_mode))
					{
						// This is a directory, go into it if recursive
						if (xi_recursive)
						{
							// Make sure it's not "."" or ".."", we don't want an infinite loop
							if (strcmp(namelist[n]->d_name, ".") && strcmp(namelist[n]->d_name, ".."))
							{
								enumerate(path, xi_filter, xi_recursive, xo_result);
							}
						}
					}
					else
					{
						// This is a file, we might want to add it
						if (xi_filter(namelist[n]->d_name))
						{
							xo_result.push_back(path);
						}
					}
					free(namelist[n]);
				};
			}
			free(namelist);
#endif
        }

        template <typename T>
        static T absolute(const T& xi_relative_path)
        {
            string_type result;

#ifdef WINDOWS
			char_type buffer[MAX_PATH + 1];

            if(::GetFullPathName(xi_relative_path.c_str(), array_size(buffer), buffer, 0) > 0)
            {
                result = buffer;
            }
#else			
			// Replaced by Dave below
			// Not tested, but compiles
			char buffer[PATH_MAX + 1];
			char* resolved_path = realpath(xi_relative_path.c_str(), buffer);
			if (resolved_path != NULL) {
				result = (T)resolved_path;
			}
#endif
            return result;
        }

        template <typename T>
        static T get_current_directory()
        {
            T result;

#ifdef WINDOWS
			char_type buffer[MAX_PATH + 1];

            if(::GetCurrentDirectory(array_size(buffer), buffer) > 0)
            {
                result = buffer;
                result += _S("\\");
            }
#else
			//RYANS CODE:
			char* buffer = get_current_dir_name();
			if (buffer != NULL)
			{
				result = (T)buffer;
			}
#endif
            return result;
        }

        template <typename T>
        static void set_current_directory(const T& xi_directory)
        {
#ifdef WINDOWS
            ::SetCurrentDirectory(xi_directory.c_str());
#else
			//RYANS CODE:
			chdir(xi_directory.c_str());
#endif
        }

		template <typename T>
		static bool directory_exists(const T& xi_directory)
		{
			struct stat info;

			if (stat(xi_directory.c_str(), &info) != 0)
				return false;
			else if (info.st_mode & S_IFDIR)
				return true;
			else
				return false;
		}

		template <typename T>
		static bool file_exists(const T& xi_directory)
		{
			struct stat info;

			if (stat(xi_directory.c_str(), &info) != 0)
				return false;
			else if (info.st_mode & S_IFREG)
				return true;
			else
				return false;
		}
							
		static bool splitpath(const std::wstring& xi_path_str, std::wstring& xo_dir, std::wstring& xo_file_name, std::wstring& xo_ext)
		{			
			bool result = true;

			std::size_t found = xi_path_str.find_last_of(L"/\\");
			std::wstring name_w_ext;
			if (found > xi_path_str.length())
			{
				xo_dir = L"";
				name_w_ext = xi_path_str;
			}
			else
			{
				xo_dir = xi_path_str.substr(0, found);
				name_w_ext = xi_path_str.substr(found + 1);
			}

			std::size_t ext_found = name_w_ext.find_last_of(L".");
			if (ext_found > name_w_ext.length())
			{
				xo_file_name = name_w_ext;
				xo_ext = L"";
			}
			else
			{
				xo_file_name = name_w_ext.substr(0, ext_found);
				xo_ext = name_w_ext.substr(ext_found + 1);
			}

			return result;
		}

		static bool splitpath(const std::string& xi_path_str, std::string& xo_dir, std::string& xo_file_name, std::string& xo_ext)
		{
			bool result = true;

			std::size_t found = xi_path_str.find_last_of("/\\");
			std::string name_w_ext;
			if (found > xi_path_str.length())
			{
				xo_dir = "";
				name_w_ext = xi_path_str;
			}
			else
			{
				xo_dir = xi_path_str.substr(0, found);
				name_w_ext = xi_path_str.substr(found + 1);
			}

			std::size_t ext_found = name_w_ext.find_last_of(".");
			if (ext_found > name_w_ext.length())
			{
				xo_file_name = name_w_ext;
				xo_ext = "";
			}
			else
			{
				xo_file_name = name_w_ext.substr(0, ext_found);
				xo_ext = name_w_ext.substr(ext_found + 1);
			}			

			return result;
		}

};

UISOFT_END_NAMESPACE

#endif // __PATH_H__
