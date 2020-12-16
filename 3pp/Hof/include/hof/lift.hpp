/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    lift.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef HOF_GUARD_FUNCTION_LIFT_H
#define HOF_GUARD_FUNCTION_LIFT_H

/// HOF_LIFT
/// ========
/// 
/// Description
/// -----------
/// 
/// The macros `HOF_LIFT` and `HOF_LIFT_CLASS` provide a lift operator that
/// will wrap a template function in a function object so it can be passed to
/// higher-order functions. The `HOF_LIFT` macro will wrap the function using
/// a generic lambda. As such, it will not preserve `constexpr`. The
/// `HOF_LIFT_CLASS` can be used to declare a class that will wrap function.
/// This will preserve `constexpr` and it can be used on older compilers that
/// don't support generic lambdas yet.
/// 
/// Limitation
/// ----------
/// 
/// In C++14, `HOF_LIFT` doesn't support `constexpr` due to using a generic
/// lambda. Instead, `HOF_LIFT_CLASS` can be used. In C++17, there is no such
/// limitation.
/// 
/// Synopsis
/// --------
/// 
///     // Wrap the function in a generic lambda
///     #define HOF_LIFT(...)
/// 
///     // Declare a class named `name` that will forward to the function
///     #define HOF_LIFT_CLASS(name, ...)
/// 
/// Example
/// -------
/// 
///     #include <hof.hpp>
///     #include <cassert>
///     #include <algorithm>
/// 
///     // Declare the class `max_f`
///     HOF_LIFT_CLASS(max_f, std::max);
/// 
///     int main() {
///         auto my_max = HOF_LIFT(std::max);
///         assert(my_max(3, 4) == std::max(3, 4));
///         assert(max_f()(3, 4) == std::max(3, 4));
///     }
/// 

#include <hof/detail/delegate.hpp>
#include <hof/returns.hpp>
#include <hof/lambda.hpp>
#include <hof/detail/forward.hpp>

namespace boost { namespace hof { namespace detail {

template<class F, class NoExcept>
struct lift_noexcept : F
{
    HOF_INHERIT_CONSTRUCTOR(lift_noexcept, F);

    template<class... Ts>
    constexpr auto operator()(Ts&&... xs) const
    noexcept(decltype(std::declval<NoExcept>()(HOF_FORWARD(Ts)(xs)...)){})
    -> decltype(std::declval<F>()(HOF_FORWARD(Ts)(xs)...))
    { return F(*this)(HOF_FORWARD(Ts)(xs)...);}
};

template<class F, class NoExcept>
constexpr lift_noexcept<F, NoExcept> make_lift_noexcept(F f, NoExcept)
{
    return {f};
}

}

}} // namespace boost::hof

#define HOF_LIFT_IS_NOEXCEPT(...) std::integral_constant<bool, noexcept(decltype(__VA_ARGS__)(__VA_ARGS__))>{}

#if defined (_MSC_VER)
#define HOF_LIFT(...) (HOF_STATIC_LAMBDA { HOF_LIFT_CLASS(fit_local_lift_t, __VA_ARGS__); return fit_local_lift_t(); }())
#elif defined (__clang__)
#define HOF_LIFT(...) (boost::hof::detail::make_lift_noexcept( \
    HOF_STATIC_LAMBDA(auto&&... xs) \
    -> decltype((__VA_ARGS__)(HOF_FORWARD(decltype(xs))(xs)...)) \
    { return (__VA_ARGS__)(HOF_FORWARD(decltype(xs))(xs)...); }, \
    HOF_STATIC_LAMBDA(auto&&... xs) { return HOF_LIFT_IS_NOEXCEPT((__VA_ARGS__)(HOF_FORWARD(decltype(xs))(xs)...)); } \
))
#else
#define HOF_LIFT(...) (HOF_STATIC_LAMBDA(auto&&... xs) HOF_RETURNS((__VA_ARGS__)(HOF_FORWARD(decltype(xs))(xs)...)))
#endif

#define HOF_LIFT_CLASS(name, ...) \
struct name \
{ \
    template<class... Ts> \
    constexpr auto operator()(Ts&&... xs) const \
    HOF_RETURNS((__VA_ARGS__)(HOF_FORWARD(Ts)(xs)...)) \
}

#endif
