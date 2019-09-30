#ifndef __VARIANT_H__
#define __VARIANT_H__

#pragma once

#include <framework.hpp>

UISOFT_BEGIN_NAMESPACE(framework)

template<typename... Ts1>
struct variant_helper;

template<typename T1, typename... Ts1>
struct variant_helper<T1, Ts1...>
{
	inline static void destroy(size_t id, void* data)
	{
		if (id == typeid(T1).hash_code())
		{
			reinterpret_cast<T1*>(data)->~T1();
		}
		else
		{
			variant_helper<Ts1...>::destroy(id, data);
		}
	}

	inline static void copy(size_t old_type, void* old_value, void* new_value)
	{
		if (old_type == typeid(T1).hash_code())
		{
			new (new_value)T1(*reinterpret_cast<const T1*>(old_value));
		}
		else
		{
			variant_helper<Ts1...>::copy(old_type, old_value, new_value);
		}
	}

	inline static void move(size_t old_type, void* old_value, void* new_value)
	{
		if (old_type == typeid(T1).hash_code())
		{
			new (new_value)T1(std::move(*reinterpret_cast<T1*>(old_value)));
		}
		else
		{
			variant_helper<Ts1...>::move(old_type, old_value, new_value);
		}
	}
};

template<> struct variant_helper<>
{
	inline static void destroy(size_t id, void* data)
	{
		UNREFERENCED_PARAMETER(id);
		UNREFERENCED_PARAMETER(data);
	}

	inline static void copy(size_t old_type, void* old_value, void* new_value)
	{
		UNREFERENCED_PARAMETER(old_type);
		UNREFERENCED_PARAMETER(old_value);
		UNREFERENCED_PARAMETER(new_value);
	}

	inline static void move(size_t old_type, void* old_value, void* new_value)
	{
		UNREFERENCED_PARAMETER(old_type);
		UNREFERENCED_PARAMETER(old_value);
		UNREFERENCED_PARAMETER(new_value);
	}
};

template<typename... Ts>
struct variant
{
    // http://www.ojdip.net/2013/10/implementing-a-variant-type-in-cpp/
    using size_type = size_t;

    template <size_type T1, size_type... Ts1>
    struct static_max;

    template <size_type T1>
    struct static_max<T1>
    {
        static const size_type value = T1;
    };

    template <size_type T1, size_type T2, size_type... Ts1>
    struct static_max<T1, T2, Ts1...>
    {
        static const size_type value = T1 >= T2 ? static_max<T1, Ts1...>::value : static_max<T2, Ts1...>::value;
    };

    static const size_type k_data_size = static_max<sizeof(Ts)...>::value;
    static const size_type k_data_align = static_max<__alignof(Ts)...>::value;

    using data_type = typename std::aligned_storage<k_data_size, k_data_align>::type;
	
    using helper_type = variant_helper<Ts...>;

    private:
        size_type my_type_id;
        data_type my_data;

    private:
        static inline size_type invalid_type()
        {
            return typeid(void).hash_code();
        }

    public:
        variant() : my_type_id(invalid_type())
        {
        }

        variant(const variant<Ts...>& other) : my_type_id(other.my_type_id)
        {
            helper_type::copy(other.my_type_id, (void*)&other.my_data, &my_data);
        }

        variant(variant<Ts...>&& other) : my_type_id(other.my_type_id)
        {
            helper_type::move(other.my_type_id, &other.my_data, &my_data);
        }

       ~variant()
        {
            helper_type::destroy(my_type_id, &my_data);
        }

        const variant<Ts...>& operator = (variant<Ts...> other) // move and the copy asignment operator
        {
            std::swap(my_type_id, other.my_type_id);
            std::swap(my_data, other.my_data);

            return *this;
        }

        template<typename T>
        bool is() const
        {
            return (my_type_id == typeid(T).hash_code());
        }

        bool valid() const
        {
            return (my_type_id != invalid_type());
        }

        template<typename T1, typename... Ts1>
        void set(Ts1&&... args)
        {
            helper_type::destroy(my_type_id, &my_data);

            new (&my_data) T1(std::forward<Ts1>(args)...);

            my_type_id = typeid(T1).hash_code();
        }

        template<typename T>
        const T& get() const
        {
            if(my_type_id != typeid(T).hash_code())
            {
                throw std::bad_cast();
            }

            return *reinterpret_cast<const T*>(&my_data);
        }

        template<typename T>
        T& get()
        {
            if(my_type_id != typeid(T).hash_code())
            {
                throw std::bad_cast();
            }

            return *reinterpret_cast<T*>(&my_data);
        }
};

UISOFT_END_NAMESPACE

#endif // __VARIANT_H__
