/*=============================================================================
    Copyright (c) 2014 Paul Fultz II
    proj.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef HOF_GUARD_FUNCTION_ON_H
#define HOF_GUARD_FUNCTION_ON_H

/// proj
/// ====
/// 
/// Description
/// -----------
/// 
/// The `proj` function adaptor applies a projection onto the parameters of
/// another function. This is useful, for example, to define a function for
/// sorting such that the ordering is based off of the value of one of its
/// member fields. 
/// 
/// Also, if just a projection is given, then the projection will be called
/// for each of its arguments.
/// 
/// Note: All projections are always evaluated in order from left-to-right.
/// 
/// Synopsis
/// --------
/// 
///     template<class Projection, class F>
///     constexpr proj_adaptor<Projection, F> proj(Projection p, F f);
/// 
///     template<class Projection>
///     constexpr proj_adaptor<Projection> proj(Projection p);
/// 
/// Semantics
/// ---------
/// 
///     assert(proj(p, f)(xs...) == f(p(xs)...));
///     assert(proj(p)(xs...) == p(xs)...);
/// 
/// Requirements
/// ------------
/// 
/// Projection must be:
/// 
/// * [UnaryInvocable](UnaryInvocable)
/// * MoveConstructible
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
///     struct foo
///     {
///         foo(int x_) : x(x_)
///         {}
///         int x;
///     };
/// 
///     int main() {
///         assert(boost::hof::proj(&foo::x, _ + _)(foo(1), foo(2)) == 3);
///     }
/// 
/// References
/// ----------
/// 
/// * [Projections](Projections)
/// * [Variadic print](<Variadic print>)
/// 



#include <utility>
#include <hof/always.hpp>
#include <hof/detail/callable_base.hpp>
#include <hof/detail/result_of.hpp>
#include <hof/detail/move.hpp>
#include <hof/detail/make.hpp>
#include <hof/detail/static_const_var.hpp>
#include <hof/detail/compressed_pair.hpp>
#include <hof/detail/result_type.hpp>
#include <hof/apply_eval.hpp>

namespace boost { namespace hof {

namespace detail {

template<class T, class Projection>
struct project_eval
{
    T&& x;
    const Projection& p;

    template<class X, class P>
    constexpr project_eval(X&& xp, const P& pp) : x(HOF_FORWARD(X)(xp)), p(pp)
    {}

    constexpr auto operator()() const HOF_RETURNS
    (p(HOF_FORWARD(T)(x)));
};

template<class T, class Projection>
constexpr project_eval<T, Projection> make_project_eval(T&& x, const Projection& p)
{
    return project_eval<T, Projection>(HOF_FORWARD(T)(x), p);
}

template<class T, class Projection>
struct project_void_eval
{
    T&& x;
    const Projection& p;

    template<class X, class P>
    constexpr project_void_eval(X&& xp, const P& pp) : x(HOF_FORWARD(X)(xp)), p(pp)
    {}

    struct void_ {};

    constexpr void_ operator()() const
    {
        return p(HOF_FORWARD(T)(x)), void_();
    }
};

template<class T, class Projection>
constexpr project_void_eval<T, Projection> make_project_void_eval(T&& x, const Projection& p)
{
    return project_void_eval<T, Projection>(HOF_FORWARD(T)(x), p);
}

template<class Projection, class F, class... Ts, 
    class R=decltype(
        std::declval<const F&>()(std::declval<const Projection&>()(std::declval<Ts>())...)
    )>
constexpr R by_eval(const Projection& p, const F& f, Ts&&... xs)
{
    return boost::hof::apply_eval(f, make_project_eval(HOF_FORWARD(Ts)(xs), p)...);
}

#if HOF_NO_ORDERED_BRACE_INIT
#define HOF_BY_VOID_RETURN HOF_ALWAYS_VOID_RETURN
#else
#if HOF_NO_CONSTEXPR_VOID
#define HOF_BY_VOID_RETURN boost::hof::detail::swallow
#else
#define HOF_BY_VOID_RETURN void
#endif
#endif

template<class Projection, class... Ts>
constexpr HOF_ALWAYS_VOID_RETURN by_void_eval(const Projection& p, Ts&&... xs)
{
    return boost::hof::apply_eval(boost::hof::always(), boost::hof::detail::make_project_void_eval(HOF_FORWARD(Ts)(xs), p)...);
}

struct swallow
{
    template<class... Ts>
    constexpr swallow(Ts&&...)
    {}
};

}

template<class Projection, class F=void>
struct proj_adaptor;

template<class Projection, class F>
struct proj_adaptor : detail::compressed_pair<detail::callable_base<Projection>, detail::callable_base<F>>, detail::function_result_type<F>
{
    typedef proj_adaptor fit_rewritable_tag;
    typedef detail::compressed_pair<detail::callable_base<Projection>, detail::callable_base<F>> base;
    template<class... Ts>
    constexpr const detail::callable_base<F>& base_function(Ts&&... xs) const
    {
        return this->second(xs...);;
    }

    template<class... Ts>
    constexpr const detail::callable_base<Projection>& base_projection(Ts&&... xs) const
    {
        return this->first(xs...);
    }

    struct by_failure
    {
        template<class Failure>
        struct apply
        {
            template<class... Ts>
            struct of
            : Failure::template of<decltype(std::declval<detail::callable_base<Projection>>()(std::declval<Ts>()))...>
            {};
        };
    };

    struct failure
    : failure_map<by_failure, detail::callable_base<F>>
    {};

    HOF_INHERIT_CONSTRUCTOR(proj_adaptor, base)

    HOF_RETURNS_CLASS(proj_adaptor);

    template<class... Ts>
    constexpr HOF_SFINAE_RESULT(const detail::callable_base<F>&, result_of<const detail::callable_base<Projection>&, id_<Ts>>...) 
    operator()(Ts&&... xs) const HOF_SFINAE_RETURNS
    (
        boost::hof::detail::by_eval(
            HOF_MANGLE_CAST(const detail::callable_base<Projection>&)(HOF_CONST_THIS->base_projection(xs...)),
            HOF_MANGLE_CAST(const detail::callable_base<F>&)(HOF_CONST_THIS->base_function(xs...)),
            HOF_FORWARD(Ts)(xs)...
        )
    );
};

template<class Projection>
struct proj_adaptor<Projection, void> : detail::callable_base<Projection>
{
    typedef proj_adaptor fit_rewritable1_tag;
    template<class... Ts>
    constexpr const detail::callable_base<Projection>& base_projection(Ts&&... xs) const
    {
        return boost::hof::always_ref(*this)(xs...);
    }

    HOF_INHERIT_DEFAULT(proj_adaptor, detail::callable_base<Projection>)

    template<class P, HOF_ENABLE_IF_CONVERTIBLE(P, detail::callable_base<Projection>)>
    constexpr proj_adaptor(P&& p) 
    : detail::callable_base<Projection>(HOF_FORWARD(P)(p))
    {}

    HOF_RETURNS_CLASS(proj_adaptor);

    template<class... Ts, class=detail::holder<decltype(std::declval<Projection>()(std::declval<Ts>()))...>>
    constexpr HOF_BY_VOID_RETURN operator()(Ts&&... xs) const 
    {
#if HOF_NO_ORDERED_BRACE_INIT
        return boost::hof::detail::by_void_eval(this->base_projection(xs...), HOF_FORWARD(Ts)(xs)...);
#else
#if HOF_NO_CONSTEXPR_VOID
        return
#endif
        boost::hof::detail::swallow{
            (this->base_projection(xs...)(HOF_FORWARD(Ts)(xs)), 0)...
        };
#endif
    }
};

HOF_DECLARE_STATIC_VAR(proj, detail::make<proj_adaptor>);

}} // namespace boost::hof
#endif
