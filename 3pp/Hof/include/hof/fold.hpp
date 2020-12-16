/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    fold.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef HOF_GUARD_FOLD_H
#define HOF_GUARD_FOLD_H

/// fold
/// ========
/// 
/// Description
/// -----------
/// 
/// The `fold` function adaptor uses a binary function to apply a
/// [fold](https://en.wikipedia.org/wiki/Fold_%28higher-order_function%29)
/// operation to the arguments passed to the function. Additionally, an
/// optional initial state can be provided, otherwise the first argument is
/// used as the initial state.
/// 
/// The arguments to the binary function, take first the state and then the
/// argument.
/// 
/// Synopsis
/// --------
/// 
///     template<class F, class State>
///     constexpr fold_adaptor<F, State> fold(F f, State s);
/// 
///     template<class F>
///     constexpr fold_adaptor<F> fold(F f);
/// 
/// Semantics
/// ---------
/// 
///     assert(fold(f, z)() == z);
///     assert(fold(f, z)(x, xs...) == fold(f, f(z, x))(xs...));
///     assert(fold(f)(x) == x);
///     assert(fold(f)(x, y, xs...) == fold(f)(f(x, y), xs...));
/// 
/// Requirements
/// ------------
/// 
/// State must be:
/// 
/// * CopyConstructible
/// 
/// F must be:
/// 
/// * [BinaryInvocable](BinaryInvocable)
/// * MoveConstructible
/// 
/// Example
/// -------
/// 
///     #include <hof.hpp>
///     #include <cassert>
/// 
///     struct max_f
///     {
///         template<class T, class U>
///         constexpr T operator()(T x, U y) const
///         {
///             return x > y ? x : y;
///         }
///     };
///     int main() {
///         assert(boost::hof::fold(max_f())(2, 3, 4, 5) == 5);
///     }
/// 
/// References
/// ----------
/// 
/// * [Fold](https://en.wikipedia.org/wiki/Fold_(higher-order_function))
/// * [Variadic sum](<Variadic sum>)
/// 

#include <hof/detail/callable_base.hpp>
#include <hof/detail/delegate.hpp>
#include <hof/detail/compressed_pair.hpp>
#include <hof/detail/move.hpp>
#include <hof/detail/make.hpp>
#include <hof/detail/static_const_var.hpp>

namespace boost { namespace hof { namespace detail {

struct v_fold
{
    HOF_RETURNS_CLASS(v_fold);
    template<class F, class State, class T, class... Ts>
    constexpr HOF_SFINAE_MANUAL_RESULT(const v_fold&, id_<const F&>, result_of<const F&, id_<State>, id_<T>>, id_<Ts>...)
    operator()(const F& f, State&& state, T&& x, Ts&&... xs) const HOF_SFINAE_MANUAL_RETURNS
    (
        (*HOF_CONST_THIS)(f, f(HOF_FORWARD(State)(state), HOF_FORWARD(T)(x)), HOF_FORWARD(Ts)(xs)...)
    );

    template<class F, class State>
    constexpr State operator()(const F&, State&& state) const noexcept
    {
        return HOF_FORWARD(State)(state);
    }
};

}

template<class F, class State=void>
struct fold_adaptor
: detail::compressed_pair<detail::callable_base<F>, State>
{
    typedef detail::compressed_pair<detail::callable_base<F>, State> base_type;
    HOF_INHERIT_CONSTRUCTOR(fold_adaptor, base_type)

    template<class... Ts>
    constexpr const detail::callable_base<F>& base_function(Ts&&... xs) const noexcept
    {
        return this->first(xs...);
    }

    template<class... Ts>
    constexpr State get_state(Ts&&... xs) const noexcept
    {
        return this->second(xs...);
    }

    HOF_RETURNS_CLASS(fold_adaptor);

    template<class... Ts>
    constexpr HOF_SFINAE_RESULT(detail::v_fold, id_<const detail::callable_base<F>&>, id_<State>, id_<Ts>...)
    operator()(Ts&&... xs) const HOF_SFINAE_RETURNS
    (
        detail::v_fold()(
            HOF_MANGLE_CAST(const detail::callable_base<F>&)(HOF_CONST_THIS->base_function(xs...)), 
            HOF_MANGLE_CAST(State)(HOF_CONST_THIS->get_state(xs...)), 
            HOF_FORWARD(Ts)(xs)...
        )
    )
};


template<class F>
struct fold_adaptor<F, void>
: detail::callable_base<F>
{
    HOF_INHERIT_CONSTRUCTOR(fold_adaptor, detail::callable_base<F>)

    template<class... Ts>
    constexpr const detail::callable_base<F>& base_function(Ts&&... xs) const noexcept
    {
        return boost::hof::always_ref(*this)(xs...);
    }

    HOF_RETURNS_CLASS(fold_adaptor);

    template<class... Ts>
    constexpr HOF_SFINAE_RESULT(detail::v_fold, id_<const detail::callable_base<F>&>, id_<Ts>...)
    operator()(Ts&&... xs) const HOF_SFINAE_RETURNS
    (
        detail::v_fold()(
            HOF_MANGLE_CAST(const detail::callable_base<F>&)(HOF_CONST_THIS->base_function(xs...)), 
            HOF_FORWARD(Ts)(xs)...
        )
    )
};

HOF_DECLARE_STATIC_VAR(fold, detail::make<fold_adaptor>);

}} // namespace boost::hof

#endif
