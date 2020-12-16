/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    callable_base.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef HOF_GUARD_CALLABLE_BASE_H
#define HOF_GUARD_CALLABLE_BASE_H

#include <hof/detail/delegate.hpp>
#include <hof/detail/result_of.hpp>
#include <hof/apply.hpp>

#ifndef HOF_CALLABLE_BASE_USE_TEMPLATE_ALIAS
#if (defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ < 7)
#define HOF_CALLABLE_BASE_USE_TEMPLATE_ALIAS 0
#else
#define HOF_CALLABLE_BASE_USE_TEMPLATE_ALIAS 1
#endif
#endif

namespace boost { namespace hof { namespace detail {

template<class F>
struct non_class_function
{
    F f;
    HOF_DELEGATE_CONSTRUCTOR(non_class_function, F, f)

    template<class... Ts>
    constexpr HOF_SFINAE_RESULT(apply_f, id_<F>, id_<Ts>...) 
    operator()(Ts&&... xs) const HOF_SFINAE_RETURNS
    (
        boost::hof::apply(f, HOF_FORWARD(Ts)(xs)...)
    );
};

template<class F>
struct callable_base_type
: std::conditional<(HOF_IS_CLASS(F) && !HOF_IS_FINAL(F) && !HOF_IS_POLYMORPHIC(F)), F, non_class_function<F>>
{};

#if HOF_CALLABLE_BASE_USE_TEMPLATE_ALIAS
template<class F>
using callable_base = typename callable_base_type<F>::type;
#else
template<class F>
struct callable_base
: callable_base_type<F>::type
{
    typedef typename callable_base_type<F>::type base;
    HOF_INHERIT_CONSTRUCTOR(callable_base, base)
};

template<class F>
struct callable_base_type<callable_base<F>>
: callable_base_type<F>
{};

#endif

}}} // namespace boost::hof

#endif
