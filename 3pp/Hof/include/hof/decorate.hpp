/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    decorate.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef HOF_GUARD_DECORATE_H
#define HOF_GUARD_DECORATE_H

/// decorate
/// ========
/// 
/// Description
/// -----------
/// 
/// The `decorate` function adaptor helps create simple function decorators. 
/// 
/// A function adaptor takes a function and returns a new functions whereas a
/// decorator takes some parameters and returns a function adaptor. The
/// `decorate` function adaptor will return a decorator that returns a
/// function adaptor. Eventually, it will invoke the function with the user-
/// provided parameter and function.
/// 
/// Synopsis
/// --------
/// 
///     template<class F>
///     constexpr decorate_adaptor<F> decorate(F f);
/// 
/// Semantics
/// ---------
/// 
///     assert(decorate(f)(x)(g)(xs...) == f(x, g, xs...));
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
///     #include <iostream>
///     #include <string>
///     using namespace boost::hof;
/// 
///     struct logger_f
///     {
///         template<class F, class... Ts>
///         auto operator()(const std::string& message, F&& f, Ts&&... xs) const 
///             -> decltype(f(std::forward<Ts>(xs)...))
///         {
///             // Message to print out when the function is called
///             std::cout << message << std::endl;
///             // Call the function
///             return f(std::forward<Ts>(xs)...);
///         }
///     };
///     // The logger decorator
///     HOF_STATIC_FUNCTION(logger) = boost::hof::decorate(logger_f());
///     
///     struct sum_f
///     {
///         template<class T, class U>
///         T operator()(T x, U y) const
///         {
///             return x+y;
///         }
///     };
///     
///     HOF_STATIC_FUNCTION(sum) = sum_f();
///     int main() {
///         // Use the logger decorator to print "Calling sum" when the function is called
///         assert(3 == logger("Calling sum")(sum)(1, 2));
///     }
/// 

#include <hof/reveal.hpp>
#include <hof/detail/delegate.hpp>
#include <hof/detail/move.hpp>
#include <hof/detail/make.hpp>
#include <hof/detail/callable_base.hpp>
#include <hof/detail/static_const_var.hpp>
#include <hof/detail/compressed_pair.hpp>

namespace boost { namespace hof { namespace detail {

template<class D, class T, class F>
struct decorator_invoke
// : compressed_pair<compressed_pair<F, T>, D>
: compressed_pair<compressed_pair<D, T>, F>
{
    // typedef compressed_pair<F, T> base;
    typedef compressed_pair<compressed_pair<D, T>, F> base;

    HOF_INHERIT_CONSTRUCTOR(decorator_invoke, base)

    template<class... Ts>
    constexpr const compressed_pair<D, T>& get_pair(Ts&&... xs) const noexcept
    {
        return this->first(xs...);
    }

    template<class... Ts>
    constexpr const F& base_function(Ts&&... xs) const noexcept
    {
        return this->second(xs...);
    }

    template<class... Ts>
    constexpr const D& get_decorator(Ts&&... xs) const noexcept
    {
        return this->get_pair(xs...).first(xs...);
    }

    template<class... Ts>
    constexpr const T& get_data(Ts&&... xs) const noexcept
    {
        return this->get_pair(xs...).second(xs...);
    }

    HOF_RETURNS_CLASS(decorator_invoke);

    struct decorator_invoke_failure
    {
        template<class Failure>
        struct apply
        {
            template<class... Ts>
            struct of
            : Failure::template of<const T&, const F&, Ts...>
            {};
        };
    };

    struct failure
    : failure_map<decorator_invoke_failure, D>
    {};

    template<class... Ts>
    constexpr HOF_SFINAE_RESULT(const D&, id_<const T&>, id_<const F&>, id_<Ts>...) 
    operator()(Ts&&... xs) const HOF_SFINAE_RETURNS
    (
        HOF_MANGLE_CAST(const D&)(HOF_CONST_THIS->get_decorator(xs...))(
            HOF_MANGLE_CAST(const T&)(HOF_CONST_THIS->get_data(xs...)),
            HOF_MANGLE_CAST(const F&)(HOF_CONST_THIS->base_function(xs...)),
            HOF_FORWARD(Ts)(xs)...
        )
    );
};

template<class D, class T>
struct decoration
: compressed_pair<D, T>
{
    typedef compressed_pair<D, T> base;
    HOF_INHERIT_CONSTRUCTOR(decoration, base)

    template<class... Ts>
    constexpr const D& get_decorator(Ts&&... xs) const noexcept
    {
        return this->first(xs...);
    }

    template<class... Ts>
    constexpr const T& get_data(Ts&&... xs) const noexcept
    {
        return this->second(xs...);
    }

    template<class F>
    constexpr decorator_invoke<D, T, detail::callable_base<F>> operator()(F f) const
    HOF_NOEXCEPT(HOF_IS_NOTHROW_CONSTRUCTIBLE(decorator_invoke<D, T, detail::callable_base<F>>, compressed_pair<D, T>, detail::callable_base<F>&&))
    {
        return decorator_invoke<D, T, detail::callable_base<F>>(
            *this, static_cast<detail::callable_base<F>&&>(f)
        );
    }
};

}

template<class F>
struct decorate_adaptor : detail::callable_base<F>
{
    typedef decorate_adaptor fit_rewritable1_tag;
    typedef detail::callable_base<F> base;
    HOF_INHERIT_CONSTRUCTOR(decorate_adaptor, detail::callable_base<F>)

    template<class... Ts>
    constexpr const base& base_function(Ts&&... xs) const noexcept
    {
        return boost::hof::always_ref(*this)(xs...);
    }

    // TODO: Add predicate for constraints

    template<class T>
    constexpr detail::decoration<base, T> operator()(T x) const 
    HOF_NOEXCEPT(HOF_IS_NOTHROW_CONSTRUCTIBLE(base, const base&) && HOF_IS_NOTHROW_CONSTRUCTIBLE(T, T&&))
    {
        return detail::decoration<base, T>(this->base_function(x), static_cast<T&&>(x));
    }

};

HOF_DECLARE_STATIC_VAR(decorate, detail::make<decorate_adaptor>);

}} // namespace boost::hof

#endif
