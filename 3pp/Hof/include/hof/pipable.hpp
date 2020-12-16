/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    pipable.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef HOF_GUARD_FUNCTION_PIPABLE_H
#define HOF_GUARD_FUNCTION_PIPABLE_H

/// pipable
/// =======
/// 
/// Description
/// -----------
/// 
/// The `pipable` function adaptor provides an extension method. The first
/// parameter of the function can be piped into the function using the pipe
/// `|` operator. This can be especially convenient when there are a lot of
/// nested function calls. Functions that are made pipable can still be called
/// the traditional way without piping in the first parameter.
/// 
/// Synopsis
/// --------
/// 
///     template<class F>
///     constexpr pipable_adaptor<F> pipable(F f);
/// 
/// Semantics
/// ---------
/// 
///     assert(x | pipable(f)(ys...) == f(x, ys...));
/// 
/// Requirements
/// ------------
/// 
/// F must be:
/// 
/// * [ConstInvocable](ConstInvocable)
/// * MoveConstructible
/// 
/// Example
/// -------
/// 
///     #include <hof.hpp>
///     #include <cassert>
///     using namespace boost::hof;
/// 
///     struct sum
///     {
///         template<class T, class U>
///         T operator()(T x, U y) const
///         {
///             return x+y;
///         }
///     };
/// 
///     int main() {
///         assert(3 == (1 | pipable(sum())(2)));
///         assert(3 == pipable(sum())(1, 2));
///     }
/// 
/// References
/// ----------
/// 
/// * [Extension methods](<Extension methods>)
/// 

#include <hof/first_of.hpp>
#include <hof/pack.hpp>
#include <hof/detail/delegate.hpp>
#include <hof/detail/move.hpp>
#include <hof/detail/make.hpp>
#include <hof/detail/static_const_var.hpp>
#include <hof/limit.hpp>

namespace boost { namespace hof { 
 
template<class F>
struct pipable_adaptor;

namespace detail {

template<class F, class Pack>
struct pipe_closure : F, Pack
{
    
    template<class X, class P>
    constexpr pipe_closure(X&& fp, P&& packp) 
    HOF_NOEXCEPT(HOF_IS_NOTHROW_CONSTRUCTIBLE(F, X&&) && HOF_IS_NOTHROW_CONSTRUCTIBLE(Pack, P&&))
    : F(HOF_FORWARD(X)(fp)), Pack(HOF_FORWARD(P)(packp))
    {}

    template<class... Ts>
    constexpr const F& base_function(Ts&&...) const noexcept
    {
        return *this;
    }

    template<class... Ts>
    constexpr const Pack& get_pack(Ts&&...) const noexcept
    {
        return *this;
    }

    template<class A>
    struct invoke
    {
        A a;
        const pipe_closure * self;
        template<class X>
        constexpr invoke(X&& xp, const pipe_closure * selfp) 
        HOF_NOEXCEPT(HOF_IS_NOTHROW_CONSTRUCTIBLE(A, X&&))
        : a(HOF_FORWARD(X)(xp)), self(selfp)
        {}

        HOF_RETURNS_CLASS(invoke);

        template<class... Ts>
        constexpr HOF_SFINAE_RESULT(const F&, id_<A>, id_<Ts>...) 
        operator()(Ts&&... xs) const HOF_SFINAE_RETURNS
        (HOF_RETURNS_STATIC_CAST(const F&)(*HOF_CONST_THIS->self)(HOF_FORWARD(A)(a), HOF_FORWARD(Ts)(xs)...));
    };

    HOF_RETURNS_CLASS(pipe_closure);

    template<class A>
    constexpr HOF_SFINAE_RESULT(const Pack&, id_<invoke<A&&>>) 
    operator()(A&& a) const HOF_SFINAE_RETURNS
    (HOF_MANGLE_CAST(const Pack&)(HOF_CONST_THIS->get_pack(a))(invoke<A&&>(HOF_FORWARD(A)(a), HOF_CONST_THIS)));
};

template<class F, class Pack>
constexpr auto make_pipe_closure(F f, Pack&& p) HOF_RETURNS
(
    pipe_closure<F, typename std::remove_reference<Pack>::type>(HOF_RETURNS_STATIC_CAST(F&&)(f), HOF_FORWARD(Pack)(p))
);


template<class Derived, class F>
struct pipe_pack
{
    template<class... Ts>
    constexpr const F& get_function(Ts&&...) const noexcept
    {
        return static_cast<const F&>(static_cast<const Derived&>(*this));
    }

    HOF_RETURNS_CLASS(pipe_pack);

    template<class... Ts, class=typename std::enable_if<
        (sizeof...(Ts) < function_param_limit<F>::value)
    >::type>
    constexpr auto operator()(Ts&&... xs) const HOF_RETURNS
    (make_pipe_closure(HOF_RETURNS_C_CAST(F&&)(get_function(xs...)), boost::hof::pack_forward(HOF_FORWARD(Ts)(xs)...)));
};
    
template<class A, class F, class Pack>
constexpr auto operator|(A&& a, const pipe_closure<F, Pack>& p) HOF_RETURNS
(p(HOF_FORWARD(A)(a)));

}

template<class F>
struct pipable_adaptor 
: detail::basic_first_of_adaptor<detail::callable_base<F>, detail::pipe_pack<pipable_adaptor<F>, detail::callable_base<F>> >
{
    typedef detail::basic_first_of_adaptor<detail::callable_base<F>, detail::pipe_pack<pipable_adaptor<F>, detail::callable_base<F>> > base;
    typedef pipable_adaptor fit_rewritable_tag;

    HOF_INHERIT_CONSTRUCTOR(pipable_adaptor, base);

    constexpr const detail::callable_base<F>& base_function() const noexcept
    {
        return *this;
    }
};

template<class A, class F>
constexpr auto operator|(A&& a, const pipable_adaptor<F>& p) HOF_RETURNS
(p(HOF_FORWARD(A)(a)));

HOF_DECLARE_STATIC_VAR(pipable, detail::make<pipable_adaptor>);

namespace detail {

template<class F>
struct static_function_wrapper;

// Operators for static_function_wrapper adaptor
template<class A, class F>
auto operator|(A&& a, const boost::hof::detail::static_function_wrapper<F>& f) HOF_RETURNS
(f(HOF_FORWARD(A)(a)));

template<class F>
struct static_default_function;

// Operators for static_default_function adaptor
template<class A, class F>
auto operator|(A&& a, const boost::hof::detail::static_default_function<F>& f) HOF_RETURNS
(f(HOF_FORWARD(A)(a)));

}

template<class F>
struct static_;

// Operators for static_ adaptor
template<class A, class F>
auto operator|(A&& a, static_<F> f) HOF_RETURNS
(f.base_function().base_function()(HOF_FORWARD(A)(a)));

}} // namespace boost::hof

#endif
