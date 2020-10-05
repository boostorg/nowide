//
//  Copyright (c) 2020 Alexander Grund
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_NOWIDE_DETAIL_IS_STRING_CONTAINER_HPP_INCLUDED
#define BOOST_NOWIDE_DETAIL_IS_STRING_CONTAINER_HPP_INCLUDED

#include <cstddef>
#include <type_traits>

namespace boost {
namespace nowide {
    namespace detail {
        template<class...>
        struct make_void
        {
            typedef void type;
        };

        template<class... Ts>
        using void_t = typename make_void<Ts...>::type;

        template<typename T>
        struct is_char_type : std::false_type
        {};
        template<>
        struct is_char_type<char> : std::true_type
        {};
        template<>
        struct is_char_type<wchar_t> : std::true_type
        {};
        template<>
        struct is_char_type<char16_t> : std::true_type
        {};
        template<>
        struct is_char_type<char32_t> : std::true_type
        {};
#ifdef __cpp_char8_t
        template<>
        struct is_char_type<char8_t> : std::true_type
        {};
#endif
        template<typename T>
        using requires_char_type = typename std::enable_if<is_char_type<T>::value>::type;

        template<typename T>
        struct is_c_string : std::false_type
        {};
        template<typename T>
        struct is_c_string<const T*> : is_char_type<T>
        {};

        template<typename T>
        using const_data_result = decltype(std::declval<const T>().data());
        template<typename T>
        using size_result = decltype(std::declval<T>().size());

        /// Return true if T is a string container, e.g. std::basic_string, std::basic_string_view
        /// Requires a static value `npos` a member function `size()` returning an integral,
        /// and a member function `data()` returning a C string
        template<typename T, typename = void>
        struct is_string_container : std::false_type
        {};
        template<typename T>
        struct is_string_container<T, void_t<decltype(T::npos), size_result<T>, const_data_result<T>>>
            : std::integral_constant<bool,
                                     std::is_integral<decltype(T::npos)>::value
                                       && std::is_integral<size_result<T>>::value
                                       && is_c_string<const_data_result<T>>::value>
        {};
        template<typename T>
        using requires_string_container = typename std::enable_if<is_string_container<T>::value>::type;

        template<typename T, typename = void>
        struct get_data_width : std::integral_constant<std::size_t, 0>
        {};
        template<typename T>
        struct get_data_width<T, void_t<const_data_result<T>>>
            : std::integral_constant<std::size_t, sizeof(typename std::remove_pointer<const_data_result<T>>::type)>
        {};
        template<typename T>
        using requires_narrow_data = typename std::enable_if<get_data_width<T>::value == 1>::type;
        template<typename T>
        using requires_wide_data = typename std::enable_if<(get_data_width<T>::value > 1)>::type;

    } // namespace detail
} // namespace nowide
} // namespace boost

#endif
