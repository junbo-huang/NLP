#ifndef __UTIL_HPP
#define __UTIL_HPP

#include <functional>
#include <cwctype>
#include <string.h>
#include <algorithm>

static void get_display_time(time_t& xi_time, std::string& xo_time, bool xi_time_in_utc)
{
	if (xi_time_in_utc)
	{
		char buff[20];
		strftime(buff, 20, "%Y-%m-%d %H:%M:%S", gmtime(&xi_time));
		xo_time = buff;
	}
	else
	{
		char buff[20];
		strftime(buff, 20, "%Y-%m-%d %H:%M:%S", localtime(&xi_time));
		xo_time = buff;
	}
}

static std::string ltrim(std::string& xi_s)
{
	xi_s.erase(xi_s.begin(), std::find_if(xi_s.begin(), xi_s.end(), std::not1(std::function<int(char)>(std::iswspace))));
	return xi_s;
}

static std::string rtrim(std::string& xi_s)
{
	xi_s.erase(std::find_if(xi_s.rbegin(), xi_s.rend(), std::not1(std::function<int(char)>(std::iswspace))).base(), xi_s.end());
	return xi_s;
}

static std::string trim(std::string& xi_s)
{
	std::string rtrimmed_string(rtrim(xi_s));
	return ltrim(rtrimmed_string);
}


// fix_slashes
// remove any whitespace, multiple slashes, and trailing slashes from a path
// also converts all of the slashes to whatever char FILESEP is defined as
// path CAN NOT be a literal string, as it overwritten by this function
// n is the size of the path input
static std::string fix_slashes(std::string& path)
{
	std::string dest;
	int c, x, lastslash = 0;

	if (path.empty()) 	return path;

	dest.resize(path.size());
	x = 0;
	//copy one string to the other but skip any whitespace or multiple slashes
	for (c = 0; c < (int)path.size(); c++)
	{
		if ((path[c] == '/') || (path[c] == '\\'))
		{
			//only copy the first slash in a sequence
			if (!lastslash)
			{
				//dont actually copy it, use FILESEP
				dest[x] = '/';
				x++;
			}
			lastslash = 1;
		}
		else
		{
			//copy all the other chars normally
			lastslash = 0;
			if (1)//if ((path[c] != ' ') && (path[c] != '\t')) //ignore whitespace // need to check -thamas
			{
				dest[x] = path[c];
				x++;
			}
		}
	}
	if ((x>0) && (dest[x - 1] == '/')){ x--; }  //dont leave any trailing slashes, ever
	dest[x] = '\0';					//close off the string

	path = dest;

	path = trim(path);

	return path;
}

// resolve_path
// - given a relative_path and the "current_path", this function will resolve
//		the relative_path to an absolute one using the current_path to fill in the beginning.
// - current_path and rel_path can be literal strings. The destination (dest) string must be 
//		writeable, and it is returned by the function.
// - cp_is_a_file will subtract one additional field from the current_path during processing,
//		to account for there being a filename at the end of current_path
// - n is the size of the dest buffer.
// - all slashes will get changed to whatever FILESEP is defined as
// - if rel_path is actually an absolute path (ie, if it contains the char ':'), 
//		then it is returned directly and current_path is ignored
// - rel_path can start with nothing, '/', './', '../', or a drive letter.
//		* '/' means either type of slash for this discussion
//		* the first 3 options mean the same thing (ie that it is a relative path,
//			but dont move up a folder)
//		* '../' moves up one folder within current_path. It may occur repeatedly 
//			at the beginning of rel_path, or after '/' or './'. It may not occur
//			anywhere except the BEGINNING of rel_path.


static std::string resolve_path(std::string& dest, std::string current_path, bool cp_is_a_file, std::string rel_path)
{	
	int ls, c, slashes;
	std::string cp, rp, dp;
	const char* tmp;
	char a[10] = "../";  //this contains the string we are looking for with strstr
	a[2] = '/';		//replace the slash with the actual filesep
		
	if (current_path.empty())
	{
		dest = rel_path;
		return dest;
	}
	if (rel_path.empty())
	{
		dest = rel_path;
		return dest;
	}

	//catch absolute paths, stop if found and simply return it
	for (c = 0; c<(int)rel_path.size(); c++)
	{
		if (rel_path[c] == ':')
		{
			dest = rel_path;
			fix_slashes(dest); //required.
			return dest;
		}
	}

	if (cp_is_a_file) cp_is_a_file = 1;  //make sure this is only 1 or 0, not some larger number
	slashes = cp_is_a_file; //if the current path is a file, take one more item off of the current_path
	
	cp = current_path;
	rp = rel_path;
	dp = dest;

	//hack to remove redundant "./" part of path if it exists
	if ((cp[0] == '.') && ((cp[1] == '/') || (cp[1] == '\\')))
	{
		cp[0] = ' ';
		cp[1] = ' ';
	}
	if ((rp[0] == '.') && ((rp[1] == '/') || (rp[1] == '\\')))
	{
		rp[0] = ' ';
		rp[1] = ' ';
	}

	//make the paths into well-formed ones (single filesep, no whitespace)
	fix_slashes(cp);
	fix_slashes(rp);

	//count the number of occurances of "../" that occur in the relative path.
	rel_path = rp;
	tmp = strstr(rp.c_str(), a);
	while (tmp != NULL)
	{
		rel_path = tmp + 2;       //hijacking this pointer to point to the remainder of the path in the rp buffer! dont get confused.
		tmp = strstr(tmp + 3, a);
		slashes++;
		if (rel_path[1] != '.') //stop at the first character that is not a dot
		{
			break;
		}
	}

	ls = 0;
	//and then chop that many folders off of the current path
	for (c = (int)cp.size(); c >= 0; c--)
	{
		//count down slashes
		if (cp[c] == '/' )
		{
			slashes--;
			ls = c; //keep track of where last slash was
		}

		//if we hit the right amount of slashes, cut off the string and stop
		if (slashes == 0)
		{
			cp.resize(c);			
			break;
		}

		//if we hit "../", cut it off there and stop
		if (strstr(&cp[c], a)) //(checks for "../" at current position)
		{
			cp.resize(c + 3);			
			break;
		}

		//if there are more ../ in rp than fields in cp, then just cut away all of them except drive letter
		if (c == 1)
		{
			cp.resize(ls); //lop it off where we saw the last slash
			
			if (cp[1] == ':')  //save the drive letter, 
			{
				slashes = 0;   //make sure we dont put "../" before a drive letter, ever
			}
			else if (cp[1] != '.')  //destroy anyting else except leading ../
			{
				//if we didnt see a ., then assume this is another folder and count down one more 
				// (due to implied leading slash at cp[-1])
				slashes--;
				cp.clear();
			}
			break;
		}
	}

	if (slashes < 0) { slashes = 0; }
	//now add any residual ../ back into the path
	dp.clear();
	for (slashes = slashes; slashes>0; slashes--)
	{
		dp += a;
	}
	dp += cp;	

	// if rel_path does not have a leading slash, sneak it in
	if (rel_path[0] != '/')
	{
		//a[0] = '/';
		//a[1] = '\0';
		dp += '/';
	}
	dp = dp + rel_path; //rel_path usually gets hijacked to be a pointer to the rest of the relative path in rp 
	
	dest = dp;		

	fix_slashes(dest); //required.
	return dest;
}

static bool get_path(std::string& dest, std::string rel_path, bool rel_to_given_path, std::string given_path, bool gp_is_a_file)
{
	//C2M output: <out_val, dest>
	if (rel_to_given_path)
	{
		resolve_path(dest, given_path, gp_is_a_file, rel_path);
	}
	else
	{
		dest = rel_path;
		fix_slashes(dest);
	}

	return true;
}

#endif