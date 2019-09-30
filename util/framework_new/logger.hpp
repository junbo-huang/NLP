#ifndef __LOGGER_H__
#define __LOGGER_H__

#pragma once

#include <framework.hpp>
#include <singleton.hpp>
#include <observable.hpp>
#include <text.hpp>

UISOFT_BEGIN_NAMESPACE(framework)

class logger : public
#if defined(WIN32DLL) || defined(WIN64DLL) || defined(_WINDLL) || defined(WINDLL) || defined(_USRDLL)
singleton_shared_memory<logger, 'lggr'>,
#else
singleton<logger>,
#endif
public observable<logger>
{
    typedef observer<logger> observer_type;
    typedef std::vector<observer_type*> observers_type;

    typedef std::recursive_mutex mutex_type;
    typedef std::lock_guard<mutex_type> lock_type;

    public:
        enum class logseverity
        {
            info = 0,
            warning,
            error,
            critical,
            debug
        };

        struct logstate
        {
			char_type* message;
            logseverity severity;
        };

    private:
		ofstream_type       my_stream;

        logseverity         my_severity_level;
        bool                my_show_location;

        observers_type      my_observers;

        mutable mutex_type  my_mutex;

    public:
        DISALLOW_COPY_MOVE_ASSIGNMENT(logger)

    private:
		void format_time(int xi_buffer_size, char_type* xo_buffer)
        {
            time_t current_time;

            time(&current_time);

            tm new_time;
#ifdef WINDOWS
            localtime_s(&new_time, &current_time);
#else
			new_time = *localtime(&current_time);
#endif
            UISNWPRINTF(xo_buffer, xi_buffer_size, CUT_SHORT, _S("%02d%02d%04d %02d:%02d:%02d"), new_time.tm_mday, new_time.tm_mon+1, new_time.tm_year+1900, new_time.tm_hour, new_time.tm_min, new_time.tm_sec);
        }

    public:
        logger() : my_show_location(false)
        {
        }

       ~logger()
        {
            try
            {
                uninitialize();
            }
            catch(const std::exception&)
            {
            }
        }

    public:
        void initialize(const string_type& xi_file_name)
        {
            if(my_stream.is_open())
            {
                my_stream.flush();
                my_stream.close();
            }

            my_stream.open(xi_file_name, std::ios::out|std::ios::app);

            xassert(my_stream.is_open() && !(my_stream.bad() || my_stream.fail()), x::error::invalidarg, _S("Unable initialize logger: %s."), xi_file_name.c_str());
        }

        void uninitialize()
        {
            try
            {
                if(my_stream.is_open())
                {
                    my_stream.flush();
                    my_stream.close();
                }
            }
            catch(const std::exception&)
            {
            }
        }

        void set_severity_level(logseverity xi_severity_level)
        {
            my_severity_level = xi_severity_level;
        }

        void set_severity_level(const string_type& xi_severity_level)
        {
            if(xi_severity_level == _S("DEBUG") || xi_severity_level == _S("debug"))
            {
                set_severity_level(logger::logseverity::debug);
            }
            else if(xi_severity_level == _S("INFO") || xi_severity_level == _S("info"))
            {
                set_severity_level(logger::logseverity::info);
            }
            else if(xi_severity_level == _S("WARNING") || xi_severity_level == _S("warning"))
            {
                set_severity_level(logger::logseverity::warning);
            }
            else if(xi_severity_level == _S("ERROR") || xi_severity_level == _S("error"))
            {
                set_severity_level(logger::logseverity::error);
            }
            else if(xi_severity_level == _S("CRITICAL") || xi_severity_level == _S("critical"))
            {
                set_severity_level(logger::logseverity::critical);
            }
            else // default
            {
#if defined(DEBUG) || defined(_DEBUG) || defined(DBG) || defined(_DBG)
                set_severity_level(logseverity::debug);
#else
                set_severity_level(logseverity::critical);
#endif
            }
        }

        void set_show_location(bool xi_show)
        {
            my_show_location = xi_show;
        }

		void log(const char_type* xi_file_name, int xi_line, const char_type* xi_function_name, logseverity xi_severity_level, const char_type* xi_template, ...)
        {
            // [filename(line):function]:[datetime]:[level] message
            // datetime - ISO(day month year) hh:mm:ss.ms
            if(my_stream.is_open() && !(my_stream.bad() || my_stream.fail()))
            {
                if(xi_severity_level <= my_severity_level)
                {
                    lock_type lock(my_mutex);

                    char_type time_buffer[32];

                    format_time(array_size(time_buffer), time_buffer);

					char_type* severity_level_name;

                    switch(xi_severity_level)
                    {
                        case logger::logseverity::debug:
                            severity_level_name = _S("DEBUG");
                            break;
                        case logger::logseverity::info:
                            severity_level_name = _S("INFO");
                            break;
                        case logger::logseverity::warning:
                            severity_level_name = _S("WARNING");
                            break;
                        case logger::logseverity::error:
                            severity_level_name = _S("ERROR");
                            break;
                        case logger::logseverity::critical:
                            severity_level_name = _S("CRITICAL");
                            break;
                        default:
                            severity_level_name = _S("UNKNOWN");
                            break;
                    }

					char_type buffer[4096];

                    if(my_show_location)
                    {
                        UISNWPRINTF(buffer, array_size(buffer), CUT_SHORT, _S("[%s]:[%s(%d):%s][%s]  "), time_buffer, xi_file_name, xi_line, xi_function_name, severity_level_name);
                    }
                    else
                    {
                        UISNWPRINTF(buffer, array_size(buffer), CUT_SHORT, _S("[%s]:[%s]  "), time_buffer, severity_level_name);
                    }

                    my_stream << buffer;

                    if(xi_template != nullptr)
                    {
                        va_list arguments;

                        va_start(arguments, xi_template);

                        UIVSNWPRINTF(buffer, array_size(buffer), CUT_SHORT, xi_template, arguments);

                        va_end(arguments);

                        my_stream << buffer << std::endl;
                    }

                    my_stream.flush();

                    logstate state;

                    state.message = buffer;
                    state.severity = xi_severity_level;

                    notify_observers(&state);
                }

                my_stream.clear();
            }
        }

        virtual void register_observer(observer_type* xi_observer)
        {
            lock_type lock(my_mutex);

            my_observers.push_back(xi_observer);
        }

        virtual void unregister_observer(observer_type* xi_observer)
        {
            lock_type lock(my_mutex);

            observers_type::iterator it(std::find(my_observers.begin(), my_observers.end(), xi_observer));

            if(it != my_observers.end())
            {
                my_observers.erase(it);
            }
        }

        virtual void notify_observers(void* xi_data = nullptr)
        {
            lock_type lock(my_mutex);

            for(const auto& observer : my_observers)
            {
                (*observer).notify_observer(*this, xi_data);
            }
        }
};

#if defined(DEBUG) || defined(_DEBUG)
#   define logdebug(__template, ...)    { uisoft::framework::logger::instance().log(__S(__FILE__), __LINE__, __S(__FUNCTION__), uisoft::framework::logger::logseverity::debug, __template, ##__VA_ARGS__); }
#else
#   define logdebug(__template, ...)
#endif
#define loginfo(__template, ...)     { uisoft::framework::logger::instance().log(__S(__FILE__), __LINE__, __S(__FUNCTION__), uisoft::framework::logger::logseverity::info, __template, ##__VA_ARGS__); }
#define logwarning(__template, ...)  { uisoft::framework::logger::instance().log(__S(__FILE__), __LINE__, __S(__FUNCTION__), uisoft::framework::logger::logseverity::warning, __template, ##__VA_ARGS__); }
#define logerror(__template, ...)    { uisoft::framework::logger::instance().log(__S(__FILE__), __LINE__, __S(__FUNCTION__), uisoft::framework::logger::logseverity::error, __template, ##__VA_ARGS__); }
#define logcritical(__template, ...) { uisoft::framework::logger::instance().log(__S(__FILE__), __LINE__, __S(__FUNCTION__), uisoft::framework::logger::logseverity::critical, __template, ##__VA_ARGS__); }

#define logstdexception(xi_ex, xi_text, ...)                                    \
{                                                                               \
    logcritical(xi_text, ##__VA_ARGS__);                                        \
    if(xi_ex.what() != nullptr)                                                 \
    {                                                                           \
        logerror(text::string_to_string_type(ex.what()).c_str(), ##__VA_ARGS__);    \
    }                                                                           \
}
#define logstdcriticalexception(xi_ex, xi_text, ...)                            \
{                                                                               \
    logcritical(xi_text, ##__VA_ARGS__);                                        \
    if(xi_ex.what() != nullptr)                                                 \
    {                                                                           \
		logcritical(text::string_to_string_type(ex.what()).c_str());                \
    }                                                                           \
}

#define logexception(xi_ex, xi_text, ...)                                       \
{                                                                               \
    logcritical(xi_text, ##__VA_ARGS__);                                        \
    logcritical(const_cast<x&>(xi_ex).why());                                   \
}
#define logcriticalexception(xi_ex, xi_text, ...)                               \
{                                                                               \
    logcritical(xi_text, ##__VA_ARGS__);                                        \
    logcritical(const_cast<x&>(xi_ex).why());                                   \
}

UISOFT_END_NAMESPACE

#endif // __LOGGER_H__
