/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    flow.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef HOF_GUARD_FUNCTION_FLOW_H
#define HOF_GUARD_FUNCTION_FLOW_H

/// flow
/// ====
/// 
/// Description
/// -----------
/// 
/// The `flow` function adaptor provides function composition. It is useful as
/// an alternative to using the pipe operator `|` when chaining functions. It is
/// similiar to [`compose`](compose.md) except the evaluation order is
/// reversed. So, `flow(f, g)(0)` is equivalent to `g(f(0))`.
/// 
/// 
/// Synopsis
/// --------
/// 
///     template<class... Fs>
///     constexpr flow_adaptor<Fs...> flow(Fs... fs);
/// 
/// Semantics
/// ---------
/// 
///     assert(flow(f, g)(xs...) == g(f(xs...)));
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
///         int r = flow(increment(), decrement(), increment())(3);
///         assert(r == 4);
///     }
/// 
/// References
/// ----------
/// 
/// * [Function composition](https://en.wikipedia.org/wiki/Function_composition)
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
struct flow_kernel : detail::compressed_pair<detail::callable_base<F1>, detail::callable_base<F2>>, compose_function_result_type<F2, F1>
{
    typedef detail::compressed_pair<detail::callable_base<F1>, detail::callable_base<F2>> base_type;

    HOF_INHERIT_CONSTRUCTOR(flow_kernel, base_type)

    HOF_RETURNS_CLASS(flow_kernel);

    template<class... Ts>
    constexpr HOF_SFINAE_RESULT(const detail::callable_base<F2>&, result_of<const detail::callable_base<F1>&, id_<Ts>...>) 
    operator()(Ts&&... xs) const HOF_SFINAE_RETURNS
    (
        HOF_MANGLE_CAST(const detail::callable_base<F2>&)(HOF_CONST_THIS->second(xs...))(
            HOF_MANGLE_CAST(const detail::callable_base<F1>&)(HOF_CONST_THIS->first(xs...))(HOF_FORWARD(Ts)(xs)...)
        )
    );
};
}

template<class F, class... Fs>
struct flow_adaptor : detail::flow_kernel<F, HOF_JOIN(flow_adaptor, Fs...)>
{
    typedef flow_adaptor fit_rewritable_tag;
    typedef HOF_JOIN(flow_adaptor, Fs...) tail;
    typedef detail::flow_kernel<F, tail> base_type;

    HOF_INHERIT_DEFAULT(flow_adaptor, base_type)

    template<class X, class... Xs, 
        HOF_ENABLE_IF_CONSTRUCTIBLE(detail::callable_base<F>, X), 
        HOF_ENABLE_IF_CONSTRUCTIBLE(tail, Xs...)
    >
    constexpr flow_adaptor(X&& f1, Xs&& ... fs) 
    HOF_NOEXCEPT(HOF_IS_NOTHROW_CONSTRUCTIBLE(base_type, X&&, tail) && HOF_IS_NOTHROW_CONSTRUCTIBLE(tail, Xs&&...))
    : base_type(HOF_FORWARD(X)(f1), tail(HOF_FORWARD(Xs)(fs)...))
    {}

    template<class X,
        HOF_ENABLE_IF_CONSTRUCTIBLE(detail::callable_base<F>, X)
    >
    constexpr flow_adaptor(X&& f1) 
    HOF_NOEXCEPT_CONSTRUCTIBLE(base_type, X&&)
    : base_type(HOF_FORWARD(X)(f1))
    {}
};

template<class F>
struct flow_adaptor<F> : detail::callable_base<F>
{
    typedef flow_adaptor fit_rewritable_tag;
    HOF_INHERIT_DEFAULT(flow_adaptor, detail::callable_base<F>)

    template<class X, HOF_ENABLE_IF_CONVERTIBLE(X, detail::callable_base<F>)>
    constexpr flow_adaptor(X&& f1) 
    HOF_NOEXCEPT_CONSTRUCTIBLE(detail::callable_base<F>, X&&)
    : detail::callable_base<F>(HOF_FORWARD(X)(f1))
    {}

};

template<class F1, class F2>
struct flow_adaptor<F1, F2>
: detail::flow_kernel<detail::callable_base<F1>, detail::callable_base<F2>>
{
    typedef flow_adaptor fit_rewritable_tag;
    typedef detail::flow_kernel<detail::callable_base<F1>, detail::callable_base<F2>> base_type;

    HOF_INHERIT_CONSTRUCTOR(flow_adaptor, base_type)
};

HOF_DECLARE_STATIC_VAR(flow, detail::make<flow_adaptor>);

}} // namespace boost::hof

#endif
