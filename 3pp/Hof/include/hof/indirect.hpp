/*=============================================================================
    Copyright (c) 2014 Paul Fultz II
    indirect.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef HOF_GUARD_FUNCTION_INDIRECT_H
#define HOF_GUARD_FUNCTION_INDIRECT_H

/// indirect
/// ========
/// 
/// Description
/// -----------
/// 
/// The `indirect` function adaptor dereferences the object before calling it.
/// 
/// Synopsis
/// --------
/// 
///     template<class F>
///     constexpr indirect_adaptor<F> indirect(F f);
/// 
/// Semantics
/// ---------
/// 
///     assert(indirect(f)(xs...) == (*f)(xs...));
/// 
/// Requirements
/// ------------
/// 
/// F must be:
/// 
/// * MoveConstructible
/// * Dereferenceable
/// 
/// Example
/// -------
/// 
///     #include <hof.hpp>
///     #include <cassert>
///     #include <memory>
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
///         int r = indirect(std::make_unique<sum>())(3,2);
///         assert(r == 5);
///     }
/// 

#include <hof/detail/delegate.hpp>
#include <hof/detail/result_of.hpp>
#include <hof/reveal.hpp>
#include <hof/always.hpp>
#include <hof/detail/move.hpp>
#include <hof/detail/make.hpp>
#include <hof/detail/static_const_var.hpp>

namespace boost { namespace hof {
// TODO: Support non-classes as well
template<class F>
struct indirect_adaptor : F
{
    typedef indirect_adaptor fit_rewritable1_tag;
    HOF_INHERIT_CONSTRUCTOR(indirect_adaptor, F);

    template<class... Ts>
    constexpr const F& base_function(Ts&&... xs) const noexcept
    {
        return boost::hof::always_ref(*this)(xs...);
    }

    struct failure
    : failure_for<decltype(*std::declval<F>())>
    {};

    HOF_RETURNS_CLASS(indirect_adaptor);

    template<class... Ts>
    constexpr HOF_SFINAE_RESULT(decltype(*std::declval<F>()), id_<Ts>...) 
    operator()(Ts&&... xs) const HOF_SFINAE_RETURNS
    (
        (*HOF_MANGLE_CAST(const F&)(HOF_CONST_THIS->base_function(xs...)))(HOF_FORWARD(Ts)(xs)...)
    );
};

template<class F>
struct indirect_adaptor<F*>
{
    typedef indirect_adaptor fit_rewritable1_tag;
    F* f;
    constexpr indirect_adaptor() noexcept
    {}

    constexpr indirect_adaptor(F* x) noexcept
    : f(x)
    {}

    template<class... Ts>
    constexpr F& base_function(Ts&&...) const noexcept
    {
        return *f;
    }

    struct failure
    : failure_for<F>
    {};

    HOF_RETURNS_CLASS(indirect_adaptor);

    template<class... Ts>
    constexpr HOF_SFINAE_RESULT(F, id_<Ts>...) 
    operator()(Ts&&... xs) const HOF_SFINAE_RETURNS
    (
        (HOF_MANGLE_CAST(F&)(HOF_CONST_THIS->base_function(xs...)))(HOF_FORWARD(Ts)(xs)...)
    );
};

HOF_DECLARE_STATIC_VAR(indirect, detail::make<indirect_adaptor>);

}} // namespace boost::hof

#endif
