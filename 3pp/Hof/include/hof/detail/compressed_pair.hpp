/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    compressed_pair.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef HOF_GUARD_COMPRESSED_PAIR_H
#define HOF_GUARD_COMPRESSED_PAIR_H

#include <hof/detail/delegate.hpp>
#include <hof/detail/move.hpp>
#include <hof/detail/forward.hpp>
#include <hof/config.hpp>
#include <hof/always.hpp>
#include <hof/alias.hpp>

#ifndef HOF_COMPRESSED_PAIR_USE_EBO_WORKAROUND
#define HOF_COMPRESSED_PAIR_USE_EBO_WORKAROUND !HOF_HAS_EBO
#endif

namespace boost { namespace hof { namespace detail {

template<class First, class Second, class=void>
struct compressed_pair;

template<int I, class T, class U>
struct pair_tag
{};

#if HOF_COMPRESSED_PAIR_USE_EBO_WORKAROUND

template<class T, class U>
struct is_same_template
: std::false_type
{};

template<template<class...> class X, class... Ts, class... Us>
struct is_same_template<X<Ts...>, X<Us...>>
: std::true_type
{};

#if (defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ < 7)

template<class T, class U>
struct is_related_template
: std::false_type
{};

#else

template<class T, class U>
struct is_related_template
: is_same_template<T, U>
{};

#endif

template<class T, class U>
struct is_related
: std::integral_constant<bool, std::is_base_of<T, U>::value || std::is_base_of<U, T>::value || is_related_template<T, U>::value>
{};

template<int I, class T, class U>
struct pair_holder
: std::conditional<(
    is_related<T, U>::value), 
    detail::alias_empty<T, pair_tag<I, T, U>>,
    detail::alias_try_inherit<T, pair_tag<I, T, U>>
>::type
{};
#else
template<int I, class T, class U>
struct pair_holder
: detail::alias_try_inherit<T, pair_tag<I, T, U>>
{};
#endif

// TODO: Empty optimizations for MSVC
template<
    class First, 
    class Second
>
struct compressed_pair<First, Second>
: pair_holder<0, First, Second>::type, pair_holder<1, Second, First>::type
{
    typedef typename pair_holder<0, First, Second>::type first_base;
    typedef typename pair_holder<1, Second, First>::type second_base;
    template<class X, class Y, 
        HOF_ENABLE_IF_CONSTRUCTIBLE(First, X&&), 
        HOF_ENABLE_IF_CONSTRUCTIBLE(Second, Y&&)
    >
    constexpr compressed_pair(X&& x, Y&& y) 
    noexcept(HOF_IS_NOTHROW_CONSTRUCTIBLE(first_base, X&&) && HOF_IS_NOTHROW_CONSTRUCTIBLE(second_base, Y&&))
    : first_base(HOF_FORWARD(X)(x)), second_base(HOF_FORWARD(Y)(y))
    {}

    HOF_INHERIT_DEFAULT(compressed_pair, first_base, second_base)

    template<class Base, class... Xs>
    constexpr const Base& get_alias_base(Xs&&... xs) const noexcept
    {
        return boost::hof::always_ref(*this)(xs...);
    }

    template<class... Xs>
    constexpr const First& first(Xs&&... xs) const noexcept
    {
        return boost::hof::alias_value(this->get_alias_base<first_base>(xs...), xs...);
    }

    template<class... Xs>
    constexpr const Second& second(Xs&&... xs) const noexcept
    {
        return boost::hof::alias_value(this->get_alias_base<second_base>(xs...), xs...);
    }

};

template<class T, class U>
constexpr compressed_pair<T, U> make_compressed_pair(T x, U y)
noexcept(HOF_IS_NOTHROW_MOVE_CONSTRUCTIBLE(T) && HOF_IS_NOTHROW_MOVE_CONSTRUCTIBLE(U))
{
    return {static_cast<T&&>(x), static_cast<U&&>(y)};
}


}}} // namespace boost::hof

#endif
