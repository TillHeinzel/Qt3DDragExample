/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    rotate.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef HOF_GUARD_ROTATE_H
#define HOF_GUARD_ROTATE_H

/// rotate
/// ====
/// 
/// Description
/// -----------
/// 
/// The `rotate` function adaptor moves the first parameter to the last
/// parameter.
/// 
/// Synopsis
/// --------
/// 
///     template<class F>
///     rotate_adaptor<F> rotate(F f);
/// 
/// Semantics
/// ---------
/// 
///     assert(rotate(f)(x, xs...) == f(xs..., x));
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
/// 
///     int main() {
///         int r = boost::hof::rotate(boost::hof::_ - boost::hof::_)(2, 5);
///         assert(r == 3);
///     }
/// 

#include <hof/detail/result_of.hpp>
#include <hof/reveal.hpp>
#include <hof/detail/make.hpp>
#include <hof/detail/static_const_var.hpp>

namespace boost { namespace hof {

template<class F>
struct rotate_adaptor : detail::callable_base<F>
{
    typedef rotate_adaptor fit_rewritable1_tag;
    HOF_INHERIT_CONSTRUCTOR(rotate_adaptor, detail::callable_base<F>);

    template<class... Ts>
    constexpr const detail::callable_base<F>& base_function(Ts&&... xs) const noexcept
    {
        return boost::hof::always_ref(*this)(xs...);
    }

    struct rotate_failure
    {
        template<class Failure>
        struct apply
        {
            template<class T, class... Ts>
            struct of
            : Failure::template of<Ts..., T>
            {};
        };
    };

    struct failure
    : failure_map<rotate_failure, detail::callable_base<F>>
    {};

    HOF_RETURNS_CLASS(rotate_adaptor);

    template<class T, class... Ts>
    constexpr HOF_SFINAE_RESULT(const detail::callable_base<F>&, id_<Ts>..., id_<T>) 
    operator()(T&& x, Ts&&... xs) const HOF_SFINAE_RETURNS
    (
        (HOF_MANGLE_CAST(const detail::callable_base<F>&)(HOF_CONST_THIS->base_function(xs...)))
            (HOF_FORWARD(Ts)(xs)..., HOF_FORWARD(T)(x))
    );
};

HOF_DECLARE_STATIC_VAR(rotate, detail::make<rotate_adaptor>);

}} // namespace boost::hof

#endif
