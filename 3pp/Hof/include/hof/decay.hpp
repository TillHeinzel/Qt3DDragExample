/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    decay.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef HOF_GUARD_DECAY_H
#define HOF_GUARD_DECAY_H

/// decay
/// =====
/// 
/// Description
/// -----------
/// 
/// The `decay` function is a unary function object that returns whats given to it after decaying its type.
/// 
/// Synopsis
/// --------
/// 
///     struct
///     {
///         template<class T>
///         constexpr typename decay<T>::type operator()(T&& x) const
///         {
///             return boost::hof::forward<T>(x);
///         }
///     } decay;
/// 
/// References
/// ----------
/// 
/// * [n3255](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2011/n3255.html) - Proposal for `decay_copy`
/// 

#include <hof/detail/delegate.hpp>
#include <hof/detail/unwrap.hpp>
#include <hof/detail/static_const_var.hpp>
#include <hof/detail/forward.hpp>

namespace boost { namespace hof { namespace detail {

template<class T>
struct decay_mf
: unwrap_reference<typename std::decay<T>::type>
{};

struct decay_f
{
    template<
        class T, 
        class Result=typename unwrap_reference<typename std::decay<T>::type>::type, 
        class=typename std::enable_if<(HOF_IS_CONSTRUCTIBLE(Result, T))>::type
    >
    constexpr Result operator()(T&& x) const HOF_NOEXCEPT_CONSTRUCTIBLE(Result, T&&)
    {
        return HOF_FORWARD(T)(x);
    }
};

}

HOF_DECLARE_STATIC_VAR(decay, detail::decay_f);

}} // namespace boost::hof

#endif
