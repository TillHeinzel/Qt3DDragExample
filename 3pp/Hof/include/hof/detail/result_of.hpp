/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    result_of.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef HOF_GUARD_DETAIL_RESULT_OF_H
#define HOF_GUARD_DETAIL_RESULT_OF_H

#include <hof/returns.hpp>
#include <hof/config.hpp>

#if HOF_HAS_MANUAL_DEDUCTION || HOF_NO_EXPRESSION_SFINAE

#include <hof/detail/and.hpp>
#include <hof/detail/holder.hpp>
#include <hof/detail/can_be_called.hpp>

namespace boost { namespace hof { namespace detail {

template<class F, class Args, class=void>
struct result_of_impl {};

template<class F, class... Ts>
struct result_of_impl<
    F, 
    holder<Ts...>, 
    typename std::enable_if<can_be_called<F, typename Ts::type...>::value>::type
>
{
    typedef decltype(std::declval<F>()(std::declval<typename Ts::type>()...)) type;
};
}

template<class T>
struct id_
{
    typedef T type;
};

template<class F, class... Ts>
struct result_of
: detail::result_of_impl<F, detail::holder<Ts...>>
{};

// template<class F, class... Ts>
// using result_of = detail::result_of_impl<F, detail::holder<Ts...>>;
// using result_of = id_<decltype(std::declval<F>()(std::declval<typename Ts::type>()...))>;

}} // namespace boost::hof
#endif

#if HOF_NO_EXPRESSION_SFINAE

#define HOF_SFINAE_RESULT(...) typename boost::hof::result_of<__VA_ARGS__>::type
#define HOF_SFINAE_RETURNS(...) HOF_RETURNS_DEDUCE_NOEXCEPT(__VA_ARGS__) { return __VA_ARGS__; }

#else

#define HOF_SFINAE_RESULT(...) auto
#define HOF_SFINAE_RETURNS HOF_RETURNS

#endif

#if HOF_HAS_MANUAL_DEDUCTION

#define HOF_SFINAE_MANUAL_RESULT(...) typename boost::hof::result_of<__VA_ARGS__>::type
#if HOF_HAS_COMPLETE_DECLTYPE && HOF_HAS_MANGLE_OVERLOAD
#define HOF_SFINAE_MANUAL_RETURNS(...) HOF_RETURNS_DEDUCE_NOEXCEPT(__VA_ARGS__) { return (__VA_ARGS__); }
#else
#define HOF_SFINAE_MANUAL_RETURNS(...) HOF_RETURNS_DEDUCE_NOEXCEPT(__VA_ARGS__) { HOF_RETURNS_RETURN(__VA_ARGS__); }
#endif

#else

#define HOF_SFINAE_MANUAL_RESULT HOF_SFINAE_RESULT
#define HOF_SFINAE_MANUAL_RETURNS HOF_SFINAE_RETURNS

#endif

#endif
