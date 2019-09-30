#ifndef __OBSERVER_H__
#define __OBSERVER_H__

#pragma once

#include <framework.hpp>

UISOFT_BEGIN_NAMESPACE(framework)

template <typename T>
interface observer
{
    virtual void notify_observer(T& xi_observable, void* xi_data = nullptr) = 0;
};

UISOFT_END_NAMESPACE

#endif // __OBSERVER_H__
