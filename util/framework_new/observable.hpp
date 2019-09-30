#ifndef __OBSERVABLE_H__
#define __OBSERVABLE_H__

#pragma once

#include <framework.hpp>
#include <observer.hpp>

UISOFT_BEGIN_NAMESPACE(framework)

template <typename T>
interface observable
{
    virtual void register_observer(observer<T>* xi_observer) = 0;
    virtual void unregister_observer(observer<T>* xi_observer) = 0;

    virtual void notify_observers(void* xi_data = nullptr) = 0;
};

UISOFT_END_NAMESPACE

#endif // __OBSERVABLE_H__
