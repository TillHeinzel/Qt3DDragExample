/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    delgate.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef HOF_GUARD_FUNCTION_DELGATE_H
#define HOF_GUARD_FUNCTION_DELGATE_H

#include <type_traits>
#include <utility>
#include <hof/config.hpp>
#include <hof/detail/and.hpp>
#include <hof/detail/holder.hpp>
#include <hof/detail/forward.hpp>
#include <hof/detail/using.hpp>
#include <hof/detail/intrinsics.hpp>
#include <hof/detail/noexcept.hpp>


#define HOF_ENABLE_IF_CONVERTIBLE(...) \
    class=typename std::enable_if<HOF_IS_CONVERTIBLE(__VA_ARGS__)>::type

#define HOF_ENABLE_IF_CONVERTIBLE_UNPACK(...) \
    class=typename std::enable_if<HOF_AND_UNPACK(HOF_IS_CONVERTIBLE(__VA_ARGS__))>::type

#define HOF_ENABLE_IF_BASE_OF(...) \
    class=typename std::enable_if<HOF_IS_BASE_OF(__VA_ARGS__)>::type

#define HOF_ENABLE_IF_CONSTRUCTIBLE(...) \
    class=typename std::enable_if<HOF_IS_CONSTRUCTIBLE(__VA_ARGS__)>::type

#define HOF_NOEXCEPT_CONSTRUCTIBLE(...) \
    HOF_NOEXCEPT(HOF_IS_NOTHROW_CONSTRUCTIBLE(__VA_ARGS__))

#define HOF_INHERIT_DEFAULT(C, ...) \
    template<bool FitPrivateEnableBool_##__LINE__=true, \
    class=typename std::enable_if<FitPrivateEnableBool_##__LINE__ && boost::hof::detail::is_default_constructible_c<__VA_ARGS__>()>::type> \
    constexpr C() HOF_NOEXCEPT(boost::hof::detail::is_nothrow_default_constructible_c<__VA_ARGS__>()) {}

#define HOF_INHERIT_DEFAULT_EMPTY(C, ...) \
    template<bool FitPrivateEnableBool_##__LINE__=true, \
    class=typename std::enable_if<FitPrivateEnableBool_##__LINE__ && \
        boost::hof::detail::is_default_constructible_c<__VA_ARGS__>() && HOF_IS_EMPTY(__VA_ARGS__) \
    >::type> \
    constexpr C() HOF_NOEXCEPT(boost::hof::detail::is_nothrow_default_constructible_c<__VA_ARGS__>()) {}

#if HOF_NO_TYPE_PACK_EXPANSION_IN_TEMPLATE

#define HOF_DELGATE_PRIMITIVE_CONSTRUCTOR(constexpr_, C, T, var) \
    template<class... FitXs, typename boost::hof::detail::enable_if_constructible<C, T, FitXs...>::type = 0> \
    constexpr_ C(FitXs&&... fit_xs) \
    HOF_NOEXCEPT_CONSTRUCTIBLE(T, FitXs&&...) \
    : var((FitXs&&)boost::hof::forward<FitXs>(fit_xs)...) {}
    
#else
#define HOF_DELGATE_PRIMITIVE_CONSTRUCTOR(constexpr_, C, T, var) \
    template<class... FitXs, HOF_ENABLE_IF_CONSTRUCTIBLE(T, FitXs&&...)> \
    constexpr_ C(FitXs&&... fit_xs) \
    HOF_NOEXCEPT_CONSTRUCTIBLE(T, FitXs&&...) \
    : var(HOF_FORWARD(FitXs)(fit_xs)...) {}

#endif

#define HOF_DELEGATE_CONSTRUCTOR(C, T, var) HOF_DELGATE_PRIMITIVE_CONSTRUCTOR(constexpr, C, T, var)

// Currently its faster to use `HOF_DELEGATE_CONSTRUCTOR` than `using
// Base::Base;`
#if 1
#define HOF_INHERIT_CONSTRUCTOR(Derived, Base) HOF_DELEGATE_CONSTRUCTOR(Derived, Base, Base)
#else
#define HOF_INHERIT_CONSTRUCTOR(Derived, Base) \
    using fit_inherit_base = Base; \
    using fit_inherit_base::fit_inherit_base; \
    Derived()=default; \
    template<class FitX, HOF_ENABLE_IF_CONVERTIBLE(FitX, Base)> \
    constexpr Derived(FitX&& fit_x) : Base(HOF_FORWARD(FitX)(fit_x)) {}
#endif

namespace boost { namespace hof {
namespace detail {

template<class... Xs>
constexpr bool is_nothrow_default_constructible_c()
{
    return HOF_AND_UNPACK(HOF_IS_NOTHROW_CONSTRUCTIBLE(Xs));
}

template<class... Xs>
constexpr bool is_default_constructible_c()
{
    return HOF_AND_UNPACK(HOF_IS_DEFAULT_CONSTRUCTIBLE(Xs));
}

template<class... Xs>
HOF_USING(is_default_constructible, std::integral_constant<bool, is_default_constructible_c<Xs...>()>);

template<class C, class X, class... Xs>
struct enable_if_constructible
: std::enable_if<is_constructible<X, Xs&&...>::value, int>
{};

}
}} // namespace boost::hof

#endif
