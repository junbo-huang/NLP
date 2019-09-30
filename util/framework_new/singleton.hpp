#ifndef __SINGLETON_H__
#define __SINGLETON_H__

#pragma once

#define BOOST_DATE_TIME_NO_LIB
#include <boost/interprocess/managed_shared_memory.hpp>

#include <framework.hpp>
#include <x.hpp>

UISOFT_BEGIN_NAMESPACE(framework)

template<typename T>
class singletonmanager : public T
{
    public:
        static bool& destroyed()
        {
            static bool destroyed = false;
            return destroyed;
        }

       ~singletonmanager()
        {
            try
            {
                singletonmanager::destroyed() = true;
            }
            catch(const std::exception&)
            {
            }
        }
};

class singletonguard
{
    private:
        static bool& get_lock()
        {
            static bool lock = false;
            return lock;
        }

    public:
        static void lock()
        {
            get_lock() = true;
        }

        static void unlock()
        {
            get_lock() = false;
        }

        static bool is_locked()
        {
            return get_lock();
        }
};

template <typename T>
class singleton : public singletonguard
{
    private:
        static T& our_instance;

    private:
        static void touch(const T&)
        {
            // triggers instantiation at pre-execution time (before main)
        }

    public:
        static T& instance()
        {
            xassert(!singletonmanager<T>::destroyed(), x::error::invalidcontext, _S("Singleton has been destroyed."));
            xassert(!singletonguard::is_locked(), x::error::invalidoperation, _S("Unable to get instance of locked singleton(s)."));

            static singletonmanager<T> instance;

            touch(singleton<T>::our_instance);

            return static_cast<T&>(instance);
        }

        static bool is_destroyed()
        {
            return singletonmanager<T>::destroyed();
        }
};

template<typename T> T& singleton<T>::our_instance = singleton<T>::instance();

//
#define SHARED_MEMORY_NAME      ("multivis.sm:%x")
#define SHARED_MEMORY_COUNT     (32)
#define SHARED_MEMORY_SIZE      (SHARED_MEMORY_COUNT*128)
#define SHARED_MEMORY_TYPE_NAME ("multivis.sm.type")

template <typename T, unsigned long Id>
class singleton_shared_memory
{
    typedef std::recursive_mutex mutex_type;
    typedef std::lock_guard<mutex_type> lock_type;

    typedef long tag_type;

    typedef std::shared_ptr<T> instance_type;

    struct singleton_entry
    {
        tag_type tag;

        instance_type instance;

        singleton_entry() : tag(-1)
        {
        }
    };

    typedef singleton_entry singleton_entry_type;

    private:
        static T* our_instance;
        static std::once_flag our_once_flag;

    public:
        static T& instance()
        {
            std::call_once
            (
                singleton_shared_memory::our_once_flag,
                []
                {
                    char buffer[SHARED_MEMORY_SIZE];


#ifdef WINDOWS
					DWORD process = GetCurrentProcessId();
#else
					pid_t process = getpid();
#endif    

                    UISNPRINTF(buffer, array_size(buffer), CUT_SHORT, SHARED_MEMORY_NAME, process);

                    boost::interprocess::managed_shared_memory shared_memory(boost::interprocess::open_or_create, buffer, SHARED_MEMORY_SIZE);

                    singleton_entry* entries(shared_memory.find_or_construct<singleton_entry>(SHARED_MEMORY_TYPE_NAME, std::nothrow)[SHARED_MEMORY_COUNT]());

                    if(entries != 0)
                    {
                        int i;

                        for(i = 0; i < SHARED_MEMORY_COUNT; i++)
                        {
                            const singleton_entry& entry(entries[i]);

                            if(entry.tag == -1)
                            {
                                break;
                            }

                            if(entry.tag == Id)
                            {
                                our_instance = entry.instance.get();
                                break;
                            }
                        }

                        if(our_instance == 0)
                        {
                            singleton_entry& entry(entries[i]);

                            entry.tag = Id;

                            entry.instance.reset(new T());

                            our_instance = entry.instance.get();
                        }
                    }
                }
            );

            return *our_instance;
        }

        static void cleanup()
        {
            char buffer[SHARED_MEMORY_SIZE];

#ifdef WINDOWS
			DWORD process = GetCurrentProcessId();
#else
			pid_t process = getpid();
#endif            

            UISNPRINTF(buffer, array_size(buffer), CUT_SHORT, SHARED_MEMORY_NAME, process);

            boost::interprocess::managed_shared_memory shared_memory(boost::interprocess::open_or_create, buffer, SHARED_MEMORY_SIZE);

            std::pair<singleton_entry_type*, size_t> entries(shared_memory.find<singleton_entry_type>(SHARED_MEMORY_TYPE_NAME));

            if(entries.first != 0)
            {
                shared_memory.destroy_ptr(entries.first);
            }

            boost::interprocess::shared_memory_object::remove(buffer);
        }
};

template<typename T, unsigned long Id> T* singleton_shared_memory<T, Id>::our_instance(nullptr);
template<typename T, unsigned long Id> std::once_flag singleton_shared_memory<T, Id>::our_once_flag;

class singleton_shared_memory_manager : public singleton<singleton_shared_memory_manager>
{
    class cleaner : public singleton_shared_memory<cleaner, 'clnr'>
    {
    };

    public:
        singleton_shared_memory_manager()
        {
        }

       ~singleton_shared_memory_manager()
        {
            cleaner::instance().cleanup();
        }
};

UISOFT_END_NAMESPACE

#endif // __SINGLETON_H__
