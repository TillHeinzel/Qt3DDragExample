/*=============================================================================
    Copyright (c) 2014 Paul Fultz II
    infix.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef HOF_GUARD_FUNCTION_INFIX_H
#define HOF_GUARD_FUNCTION_INFIX_H

/// infix
/// =====
/// 
/// Description
/// -----------
/// 
/// The `infix` function adaptor allows the function to be used as an infix
/// operator. The operator must be placed inside the angle brackets(ie `<`
/// and `>`).
/// 
/// Synopsis
/// --------
/// 
///     template<class F>
///     constexpr infix_adaptor<F> infix(F f);
/// 
/// Semantics
/// ---------
/// 
///     assert(x <infix(f)> y == f(x, y));
/// 
/// Requirements
/// ------------
/// 
/// F must be:
/// 
/// * [BinaryInvocable](BinaryInvocable)
/// * MoveConstructible
/// 
/// Operator precedence
/// -------------------
/// 
/// Infix operators have the precedence of relational operators. This means
/// operators such as `+` or `*` have higher precedence:
/// 
///     assert((x + y <infix(f)> z) == ((x + y) <infix(f)> z));
///     assert((x * y <infix(f)> z) == ((x * y) <infix(f)> z));
/// 
/// However, operators such as `|` or `==` have lower precedence::
/// 
///     assert((x | y <infix(f)> z) == (x | (y <infix(f)> z)));
///     assert((x == y <infix(f)> z) == (x == (y <infix(f)> z)));
/// 
/// Also, infix operators have left-to-right associativity:
/// 
///     assert(x <infix(f)> y <infix(g)> z == ((x <infix(f)> y) <infix(g)> z));
/// 
/// Example
/// -------
/// 
///     #include <hof.hpp>
///     #include <cassert>
///     using namespace boost::hof;
/// 
///     struct plus_f
///     {
///         template<class T, class U>
///         T operator()(T x, U y) const
///         {
///             return x+y;
///         }
///     };
///     
///     int main() {
///         constexpr infix_adaptor<plus_f> plus = {};
///         int r = 3 <plus> 2;
///         assert(r == 5);
///     }
/// 

#include <hof/detail/delegate.hpp>
#include <hof/detail/callable_base.hpp>
#include <hof/always.hpp>
#include <hof/reveal.hpp>
#include <hof/detail/move.hpp>
#include <hof/detail/make.hpp>
#include <hof/detail/static_const_var.hpp>

namespace boost { namespace hof {
 
namespace detail{
template<class T, class F>
struct postfix_adaptor : F
{
    T x;

    template<class X, class XF>
    constexpr postfix_adaptor(X&& xp, XF&& fp)
    HOF_NOEXCEPT(HOF_IS_NOTHROW_CONSTRUCTIBLE(F, XF&&) && HOF_IS_NOTHROW_CONSTRUCTIBLE(T, X&&)) 
    : F(HOF_FORWARD(XF)(fp)), x(HOF_FORWARD(X)(xp))
    {}

    template<class... Ts>
    constexpr const F& base_function(Ts&&... xs) const noexcept
    {
        return boost::hof::always_ref(*this)(xs...);
    }

    HOF_RETURNS_CLASS(postfix_adaptor);

    template<class... Ts>
    constexpr HOF_SFINAE_RESULT(const F&, id_<T&&>, id_<Ts>...)
    operator()(Ts&&... xs) const HOF_SFINAE_RETURNS
    (
        (HOF_MANGLE_CAST(const F&)(HOF_CONST_THIS->base_function(xs...)))(HOF_RETURNS_C_CAST(T&&)(HOF_CONST_THIS->x), HOF_FORWARD(Ts)(xs)...)
    );

    template<class A>
    constexpr HOF_SFINAE_RESULT(const F&, id_<T&&>, id_<A>)
    operator>(A&& a) const HOF_SFINAE_RETURNS
    (
        (HOF_MANGLE_CAST(const F&)(HOF_CONST_THIS->base_function(a)))(HOF_RETURNS_C_CAST(T&&)(HOF_CONST_THIS->x), HOF_FORWARD(A)(a))
    );
};

template<class T, class F>
constexpr postfix_adaptor<T, F> make_postfix_adaptor(T&& x, F f)
HOF_NOEXCEPT_CONSTRUCTIBLE(postfix_adaptor<T, F>, T&&, F&&)
{
    return postfix_adaptor<T, F>(HOF_FORWARD(T)(x), static_cast<F&&>(f));
}
}

template<class F>
struct infix_adaptor : detail::callable_base<F>
{
    typedef infix_adaptor fit_rewritable1_tag;
    HOF_INHERIT_CONSTRUCTOR(infix_adaptor, detail::callable_base<F>);

    template<class... Ts>
    constexpr const detail::callable_base<F>& base_function(Ts&&... xs) const noexcept
    {
        return boost::hof::always_ref(*this)(xs...);
    }

    template<class... Ts>
    constexpr const detail::callable_base<F>& infix_base_function(Ts&&... xs) const noexcept
    {
        return boost::hof::always_ref(*this)(xs...);
    }

    HOF_RETURNS_CLASS(infix_adaptor);

    template<class... Ts>
    constexpr auto operator()(Ts&&... xs) const HOF_RETURNS
    (
        (HOF_MANGLE_CAST(const detail::callable_base<F>&)(HOF_CONST_THIS->base_function(xs...)))(HOF_FORWARD(Ts)(xs)...)
    );
};

template<class T, class F>
constexpr auto operator<(T&& x, const infix_adaptor<F>& i) HOF_RETURNS
(detail::make_postfix_adaptor(HOF_FORWARD(T)(x), boost::hof::move(i.base_function(x))));

// TODO: Operators for static_

namespace detail {

template<class F>
struct static_function_wrapper;

// Operators for static_function_wrapper adaptor
template<class T, class F>
auto operator<(T&& x, const boost::hof::detail::static_function_wrapper<F>& f) HOF_RETURNS
(
    detail::make_postfix_adaptor(HOF_FORWARD(T)(x), boost::hof::move(f.base_function().infix_base_function()))
);

template<class F>
struct static_default_function;

// Operators for static_default_function adaptor
template<class T, class F>
auto operator<(T&& x, const boost::hof::detail::static_default_function<F>&) HOF_RETURNS
(
    detail::make_postfix_adaptor(HOF_FORWARD(T)(x), boost::hof::move(F().infix_base_function()))
);
}
// This overload is needed for gcc
template<class T, class F>
constexpr auto operator<(T&& x, const boost::hof::reveal_adaptor<F>& f) HOF_RETURNS
(
    detail::make_postfix_adaptor(HOF_FORWARD(T)(x), f.infix_base_function())
);

HOF_DECLARE_STATIC_VAR(infix, detail::make<infix_adaptor>);

}} // namespace boost::hof

#endif
