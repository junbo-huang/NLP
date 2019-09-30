#ifndef __OBSERVABLE_BASE_H__
#define __OBSERVABLE_BASE_H__

#pragma once

#include <framework.hpp>
#include <observer.hpp>
#include <observable.hpp>

UISOFT_BEGIN_NAMESPACE(framework)

template <typename T>
class observable_base : observable<T>
{
    using mutex_type = std::recursive_mutex;
    using lock_type = std::lock_guard<mutex_type>;

    using observer_type = observable<T>::observer_type;
    using observers_type = std::list<std::weak_ptr<observer<T>>>;

    private:
        observers_type      my_observers;
        mutable mutex_type  my_mutex;

    public:
        observable_base()
        {
        }

        virtual ~observable_base()
        {
            try
            {
                lock_type lock(my_mutex);
                my_observers.clear();
            }
            catch(...)
            {
            }
        }

        void register_observer(const observer_type& observer) override
        {
            lock_type lock(my_mutex);
            my_observers.push_back(observer);
        }

        void unregister_observer(const observer_type& observer) override
        {
            lock_type lock(my_mutex);

            auto it(my_observers.begin());

            while(it != my_observers.end())
            {
                observer_type observer((*it).lock());

                if(observer == nullptr || observer == observer)
                {
                    it = my_observers.erase(it);
                }
                else
                {
                    ++it;
                }
            }
        }

        void notify_observers(void* data = nullptr) override
        {
            observers_type observers;

            {
                lock_type lock(my_mutex);

                auto it(my_observers.begin());

                while(it != my_observers.end())
                {
                    observer_type observer((*it).lock());

                    if(observer != nullptr)
                    {
                        observers.emplace_back(observer);
                        ++it;
                    }
                    else
                    {
                        it = my_observers.erase(it);
                    }
                }
            }

            for(const auto& observer : observers)
            {
                (*observer).notify_observer(data);
            }
        }
};

UISOFT_END_NAMESPACE

#endif // __OBSERVABLE_BASE_H__
