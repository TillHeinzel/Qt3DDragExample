/*=============================================================================
    Copyright (c) 2014 Paul Fultz II
    compose.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef HOF_GUARD_FUNCTION_COMPOSE_H
#define HOF_GUARD_FUNCTION_COMPOSE_H

/// compose
/// =======
/// 
/// Description
/// -----------
/// 
/// The `compose` function adaptor provides function composition. It produces
/// a function object that composes a set of functions, ie the output of one
/// function becomes the input of the second function. So, `compose(f, g)(0)`
/// is equivalent to `f(g(0))`.
/// 
/// 
/// Synopsis
/// --------
/// 
///     template<class... Fs>
///     constexpr compose_adaptor<Fs...> compose(Fs... fs);
/// 
/// Semantics
/// ---------
/// 
///     assert(compose(f, g)(xs...) == f(g(xs...)));
/// 
/// Requirements
/// ------------
/// 
/// Fs must be:
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
///     struct increment
///     {
///         template<class T>
///         T operator()(T x) const
///         {
///             return x + 1;
///         }
///     };
/// 
///     struct decrement
///     {
///         template<class T>
///         T operator()(T x) const
///         {
///             return x - 1;
///         }
///     };
/// 
///     int main() {
///         int r = compose(increment(), decrement(), increment())(3);
///         assert(r == 4);
///     }
/// 
/// References
/// ----------
/// 
/// * [Function composition](https://en.wikipedia.org/wiki/Function_composition)
/// 
/// 

#include <hof/detail/callable_base.hpp>
#include <hof/always.hpp>
#include <hof/detail/delegate.hpp>
#include <hof/detail/compressed_pair.hpp>
#include <hof/detail/join.hpp>
#include <tuple>
#include <hof/detail/move.hpp>
#include <hof/detail/make.hpp>
#include <hof/detail/result_type.hpp>
#include <hof/detail/static_const_var.hpp>

namespace boost { namespace hof { namespace detail {

template<class F1, class F2>
struct compose_kernel : detail::compressed_pair<F1, F2>, compose_function_result_type<F1, F2>
{
    typedef detail::compressed_pair<F1, F2> base_type;

    HOF_INHERIT_CONSTRUCTOR(compose_kernel, base_type)

    HOF_RETURNS_CLASS(compose_kernel);

    template<class... Ts>
    constexpr HOF_SFINAE_RESULT(const F1&, result_of<const F2&, id_<Ts>...>) 
    operator()(Ts&&... xs) const HOF_SFINAE_RETURNS
    (
        HOF_MANGLE_CAST(const F1&)(HOF_CONST_THIS->first(xs...))(
            HOF_MANGLE_CAST(const F2&)(HOF_CONST_THIS->second(xs...))(HOF_FORWARD(Ts)(xs)...)
        )
    );
};
}

template<class F, class... Fs>
struct compose_adaptor 
: detail::compose_kernel<detail::callable_base<F>, HOF_JOIN(compose_adaptor, detail::callable_base<Fs>...)>
{
    typedef compose_adaptor fit_rewritable_tag;
    typedef HOF_JOIN(compose_adaptor, detail::callable_base<Fs>...) tail;
    typedef detail::compose_kernel<detail::callable_base<F>, tail> base_type;

    HOF_INHERIT_DEFAULT(compose_adaptor, base_type)

    template<class X, class... Xs, 
        HOF_ENABLE_IF_CONSTRUCTIBLE(detail::callable_base<F>, X), 
        HOF_ENABLE_IF_CONSTRUCTIBLE(tail, Xs...)
    >
    constexpr compose_adaptor(X&& f1, Xs&& ... fs)
    HOF_NOEXCEPT(HOF_IS_NOTHROW_CONSTRUCTIBLE(base_type, X&&, tail) && HOF_IS_NOTHROW_CONSTRUCTIBLE(tail, Xs&&...))
    : base_type(HOF_FORWARD(X)(f1), tail(HOF_FORWARD(Xs)(fs)...))
    {}

    template<class X,
        HOF_ENABLE_IF_CONSTRUCTIBLE(detail::callable_base<F>, X)
    >
    constexpr compose_adaptor(X&& f1) 
    HOF_NOEXCEPT_CONSTRUCTIBLE(base_type, X&&)
    : base_type(HOF_FORWARD(X)(f1))
    {}
};

template<class F>
struct compose_adaptor<F> : detail::callable_base<F>
{
    typedef compose_adaptor fit_rewritable_tag;

    HOF_INHERIT_DEFAULT(compose_adaptor, detail::callable_base<F>)

    template<class X, HOF_ENABLE_IF_CONVERTIBLE(X, detail::callable_base<F>)>
    constexpr compose_adaptor(X&& f1) 
    HOF_NOEXCEPT_CONSTRUCTIBLE(detail::callable_base<F>, X&&)
    : detail::callable_base<F>(HOF_FORWARD(X)(f1))
    {}

};

template<class F1, class F2>
struct compose_adaptor<F1, F2>
: detail::compose_kernel<detail::callable_base<F1>, detail::callable_base<F2>>
{
    typedef compose_adaptor fit_rewritable_tag;
    typedef detail::compose_kernel<detail::callable_base<F1>, detail::callable_base<F2>> base_type;

    HOF_INHERIT_CONSTRUCTOR(compose_adaptor, base_type)
};

HOF_DECLARE_STATIC_VAR(compose, detail::make<compose_adaptor>);

}} // namespace boost::hof

#endif
