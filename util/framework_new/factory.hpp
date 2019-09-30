#ifndef __FACTORY_H__
#define __FACTORY_H__

#pragma once

#include <framework.hpp>
#include <x.hpp>

UISOFT_BEGIN_NAMESPACE(framework)

template <typename T, typename ... Ts>
class factory0
{
    using pointer_type = std::shared_ptr<T>;

    public:
        static pointer_type create(Ts&& ... xi_params)
        {
            return std::make_shared<T>(std::forward<Ts>(xi_params)...);
        }
};

template <typename T,
          typename P0  = char,
          typename P1  = char,
          typename P2  = char,
          typename P3  = char,
          typename P4  = char,
          typename P5  = char,
          typename P6  = char,
          typename P7  = char,
          typename P8  = char,
          typename P9  = char,
          typename P10 = char,
          typename P11 = char,
          typename P12 = char,
          typename P13 = char,
          typename P14 = char,
          typename P15 = char>
class factory
{
    typedef std::shared_ptr<T> pointer_type;

    struct deleter
    {
        void operator () (void * const xi_p) const
        {
            if(xi_p != 0)
            {
                ((T*)xi_p)->~T();
                ::free(xi_p);
            }
        }
    };

#define CREATE_PROLOG             \
    pointer_type o;               \
    void* p(::malloc(sizeof(T))); \
    xassert(p != 0, x::error::outofmemory, EMPTY_WSTRING);

#define CREATE_EPILOG \
    return o;

    public:
        static pointer_type create()
        {
            CREATE_PROLOG
            o = pointer_type(new (p) T(), deleter());
            CREATE_EPILOG
        }

        static pointer_type create(P0 xi_p0)
        {
            CREATE_PROLOG
            o = pointer_type(new (p) T(xi_p0), deleter());
            CREATE_EPILOG
        }

        static pointer_type create(P0 xi_p0, P1 xi_p1)
        {
            CREATE_PROLOG
            o = pointer_type(new (p) T(xi_p0, xi_p1), deleter());
            CREATE_EPILOG
        }

        static pointer_type create(P0 xi_p0, P1 xi_p1, P2 xi_p2)
        {
            CREATE_PROLOG
            o = pointer_type(new (p) T(xi_p0, xi_p1, xi_p2), deleter());
            CREATE_EPILOG
        }

        static pointer_type create(P0 xi_p0, P1 xi_p1, P2 xi_p2, P3 xi_p3)
        {
            CREATE_PROLOG
            o = pointer_type(new (p) T(xi_p0, xi_p1, xi_p2, xi_p3), deleter());
            CREATE_EPILOG
        }

        static pointer_type create(P0 xi_p0, P1 xi_p1, P2 xi_p2, P3 xi_p3, P4 xi_p4)
        {
            CREATE_PROLOG
            o = pointer_type(new (p) T(xi_p0, xi_p1, xi_p2, xi_p3, xi_p4), deleter());
            CREATE_EPILOG
        }

        static pointer_type create(P0 xi_p0, P1 xi_p1, P2 xi_p2, P3 xi_p3, P4 xi_p4, P5 xi_p5)
        {
            CREATE_PROLOG
            o = pointer_type(new (p) T(xi_p0, xi_p1, xi_p2, xi_p3, xi_p4, xi_p5), deleter());
            CREATE_EPILOG
        }

        static pointer_type create(P0 xi_p0, P1 xi_p1, P2 xi_p2, P3 xi_p3, P4 xi_p4, P5 xi_p5, P6 xi_p6)
        {
            CREATE_PROLOG
            o = pointer_type(new (p) T(xi_p0, xi_p1, xi_p2, xi_p3, xi_p4, xi_p5, xi_p6), deleter());
            CREATE_EPILOG
        }

        static pointer_type create(P0 xi_p0, P1 xi_p1, P2 xi_p2, P3 xi_p3, P4 xi_p4, P5 xi_p5, P6 xi_p6, P7 xi_p7)
        {
            CREATE_PROLOG
            o = pointer_type(new (p) T(xi_p0, xi_p1, xi_p2, xi_p3, xi_p4, xi_p5, xi_p6, xi_p7), deleter());
            CREATE_EPILOG
        }

        static pointer_type create(P0 xi_p0, P1 xi_p1, P2 xi_p2, P3 xi_p3, P4 xi_p4, P5 xi_p5, P6 xi_p6, P7 xi_p7, P8 xi_p8)
        {
            CREATE_PROLOG
            o = pointer_type(new (p) T(xi_p0, xi_p1, xi_p2, xi_p3, xi_p4, xi_p5, xi_p6, xi_p7, xi_p8), deleter());
            CREATE_EPILOG
        }

        static pointer_type create(P0 xi_p0, P1 xi_p1, P2 xi_p2, P3 xi_p3, P4 xi_p4, P5 xi_p5, P6 xi_p6, P7 xi_p7, P8 xi_p8, P9 xi_p9)
        {
            CREATE_PROLOG
            o = pointer_type(new (p) T(xi_p0, xi_p1, xi_p2, xi_p3, xi_p4, xi_p5, xi_p6, xi_p7, xi_p8, xi_p9), deleter());
            CREATE_EPILOG
        }

        static void release_memory()
        {
            //pool_t::purge_memory();
        }
};

UISOFT_END_NAMESPACE

#endif // __FACTORY_H__
