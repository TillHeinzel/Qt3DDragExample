/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    identity.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef HOF_GUARD_FUNCTION_IDENTITY_H
#define HOF_GUARD_FUNCTION_IDENTITY_H

/// identity
/// ========
/// 
/// Description
/// -----------
/// 
/// The `identity` function is an unary function object that returns whats given to it. 
/// 
/// Semantics
/// ---------
/// 
///     assert(identity(x) == x);
/// 
/// Synopsis
/// --------
/// 
///     template<class T>
///     constexpr T identity(T&& x);
/// 

#include <utility>
#include <initializer_list>
#include <hof/detail/forward.hpp>
#include <hof/detail/static_const_var.hpp>

namespace boost { namespace hof { namespace identity_detail {

struct identity_base
{
    template<class T>
    constexpr T operator()(T&& x) const 
    noexcept(std::is_reference<T>::value || HOF_IS_NOTHROW_MOVE_CONSTRUCTIBLE(T))
    {
        return HOF_FORWARD(T)(x);
    }

    template<class T>
    constexpr std::initializer_list<T>& operator()(std::initializer_list<T>& x) const noexcept
    {
        return x;
    }

    template<class T>
    constexpr const std::initializer_list<T>& operator()(const std::initializer_list<T>& x) const noexcept
    {
        return x;
    }

    template<class T>
    constexpr std::initializer_list<T> operator()(std::initializer_list<T>&& x) const noexcept(noexcept(std::initializer_list<T>(std::move(x))))
    {
        return HOF_FORWARD(std::initializer_list<T>)(x);
    }
};

}

HOF_DECLARE_STATIC_VAR(identity, identity_detail::identity_base);

}} // namespace boost::hof

#endif
