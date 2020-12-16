/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    unpack.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef HOF_GUARD_UNPACK_H
#define HOF_GUARD_UNPACK_H

/// unpack
/// ======
/// 
/// Description
/// -----------
/// 
/// The `unpack` function adaptor takes a sequence and uses the elements of
/// the sequence for the arguments to the function. Multiple sequences can be
/// passed to the function. All elements from each sequence will be passed
/// into the function. 
/// 
/// 
/// Synopsis
/// --------
/// 
///     template<class F>
///     unpack_adaptor<F> unpack(F f);
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
///         int r = unpack(sum())(std::make_tuple(3,2));
///         assert(r == 5);
///     }
/// 
/// References
/// ----------
/// 
/// * [std::apply](http://en.cppreference.com/w/cpp/utility/apply) - C++17 function to unpack a tuple
/// * [`unpack_sequence`](unpack_sequence)
/// 

#include <hof/unpack_sequence.hpp>
#include <hof/is_unpackable.hpp>
#include <hof/detail/seq.hpp>
#include <hof/capture.hpp>
#include <hof/always.hpp>
#include <hof/reveal.hpp>
#include <hof/detail/and.hpp>
#include <hof/detail/delegate.hpp>
#include <hof/detail/holder.hpp>
#include <hof/detail/move.hpp>
#include <hof/detail/make.hpp>
#include <hof/detail/static_const_var.hpp>

namespace boost { namespace hof {

namespace detail {

template<class F, class Sequence>
constexpr auto unpack_simple(F&& f, Sequence&& s) HOF_RETURNS
(
    detail::unpack_impl(HOF_FORWARD(F)(f), HOF_FORWARD(Sequence)(s))
)

template<class F, class... Sequences>
constexpr auto unpack_join(F&& f, Sequences&&... s) HOF_RETURNS
(
    boost::hof::pack_join(unpack_simple(boost::hof::pack_forward, HOF_FORWARD(Sequences)(s))...)(HOF_FORWARD(F)(f))
);

}

template<class F>
struct unpack_adaptor : detail::callable_base<F>
{
    typedef unpack_adaptor fit_rewritable1_tag;
    HOF_INHERIT_CONSTRUCTOR(unpack_adaptor, detail::callable_base<F>);

    template<class... Ts>
    constexpr const detail::callable_base<F>& base_function(Ts&&... xs) const noexcept
    {
        return boost::hof::always_ref(*this)(xs...);
    }

    struct unpack_failure
    {
        template<class Failure>
        struct apply
        {
            struct deducer
            {
                template<class... Ts>
                typename Failure::template of<Ts...> operator()(Ts&&...) const;
            };

            template<class T, class=typename std::enable_if<(
                is_unpackable<T>::value
            )>::type>
            static auto deduce(T&& x)
            HOF_RETURNS
            (
                boost::hof::detail::unpack_simple(deducer(), HOF_FORWARD(T)(x))
            );

            template<class T, class... Ts, class=typename std::enable_if<(
                is_unpackable<T>::value && HOF_AND_UNPACK(is_unpackable<Ts>::value)
            )>::type>
            static auto deduce(T&& x, Ts&&... xs) HOF_RETURNS
            (
                boost::hof::detail::unpack_join(deducer(), HOF_FORWARD(T)(x), HOF_FORWARD(Ts)(xs)...)
            );
#ifdef _MSC_VER
            template<class... Ts>
            struct nop_failure;
            template<class... Ts, class=typename std::enable_if<(
                !HOF_AND_UNPACK(is_unpackable<Ts>::value)
            )>::type>
            static as_failure<nop_failure> deduce(Ts&&... xs);
#endif
            template<class... Ts>
            struct of
#if (defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ < 7) || defined (_MSC_VER)
            : std::enable_if<true, decltype(apply::deduce(std::declval<Ts>()...))>::type
#else
            : decltype(apply::deduce(std::declval<Ts>()...))
#endif
            {};
        };
    };

    struct failure
    : failure_map<unpack_failure, detail::callable_base<F>>
    {};

    HOF_RETURNS_CLASS(unpack_adaptor);
    template<class T, class=typename std::enable_if<(
        is_unpackable<T>::value
    )>::type>
    constexpr auto operator()(T&& x) const
    HOF_RETURNS
    (
        boost::hof::detail::unpack_simple(HOF_MANGLE_CAST(const detail::callable_base<F>&)(HOF_CONST_THIS->base_function(x)), HOF_FORWARD(T)(x))
    );

    template<class T, class... Ts, class=typename std::enable_if<(
        is_unpackable<T>::value && HOF_AND_UNPACK(is_unpackable<Ts>::value)
    )>::type>
    constexpr auto operator()(T&& x, Ts&&... xs) const HOF_RETURNS
    (
        boost::hof::detail::unpack_join(HOF_MANGLE_CAST(const detail::callable_base<F>&)(HOF_CONST_THIS->base_function(x)), HOF_FORWARD(T)(x), HOF_FORWARD(Ts)(xs)...)
    );
};

HOF_DECLARE_STATIC_VAR(unpack, detail::make<unpack_adaptor>);

}} // namespace boost::hof

#endif
