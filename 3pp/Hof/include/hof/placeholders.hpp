/*=============================================================================
    Copyright (c) 2014 Paul Fultz II
    placeholders.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef HOF_GUARD_FUNCTION_PLACEHOLDERS_H
#define HOF_GUARD_FUNCTION_PLACEHOLDERS_H

/// placeholders
/// ============
/// 
/// Description
/// -----------
/// 
/// The placeholders provide `std::bind` compatible placeholders that
/// additionally provide basic C++ operators that creates bind expressions.
/// Each bind expression supports `constexpr` function evaluation.
/// 
/// Synopsis
/// --------
/// 
///     namespace placeholders {
///         placeholder<1> _1 = {};
///         placeholder<2> _2 = {};
///         placeholder<3> _3 = {};
///         placeholder<4> _4 = {};
///         placeholder<5> _5 = {};
///         placeholder<6> _6 = {};
///         placeholder<7> _7 = {};
///         placeholder<8> _8 = {};
///         placeholder<9> _9 = {};
///     }
/// 
/// Operators
/// ---------
/// 
/// * Binary operators: +,-,*,/,%,>>,<<,>,<,<=,>=,==,!=,&,^,|,&&,||
/// * Assign operators: +=,-=,*=,/=,%=,>>=,<<=,&=,|=,^=
/// * Unary operators: !,~,+,-,*,++,--
/// 
/// 
/// Example
/// -------
/// 
///     #include <hof.hpp>
///     #include <cassert>
///     using namespace boost::hof;
/// 
///     int main() {
///         auto sum = _1 + _2;
///         assert(3 == sum(1, 2));
///     }
/// 
/// 
/// unamed placeholder
/// ==================
/// 
/// Description
/// -----------
/// 
/// The unamed placeholder can be used to build simple functions from C++
/// operators. 
/// 
/// Note: The function produced by the unamed placeholder is not a bind expression.
/// 
/// Synopsis
/// --------
/// 
///     namespace placeholders {
///         /* unspecified */ _ = {};
///     }
/// 
/// Example
/// -------
/// 
///     #include <hof.hpp>
///     #include <cassert>
///     using namespace boost::hof;
/// 
///     int main() {
///         auto sum = _ + _;
///         assert(3 == sum(1, 2));
///     }
/// 

#include <hof/returns.hpp>
#include <hof/lazy.hpp>
#include <hof/protect.hpp>

#if defined(_MSC_VER) && _MSC_VER >= 1910
#include <hof/detail/pp.hpp>
#endif

namespace boost { namespace hof { namespace detail {
    template<int N>
    struct simple_placeholder
    {};
}}} // namespace boost::hof

namespace std {
    template<int N>
    struct is_placeholder<boost::hof::detail::simple_placeholder<N>>
    : std::integral_constant<int, N>
    {};
}


namespace boost { namespace hof {

#define HOF_FOREACH_BINARY_OP(m) \
    m(+, add) \
    m(-, subtract) \
    m(*, multiply) \
    m(/, divide) \
    m(%, remainder) \
    m(>>, shift_right) \
    m(<<, shift_left) \
    m(>, greater_than) \
    m(<, less_than) \
    m(<=, less_than_equal) \
    m(>=, greater_than_equal) \
    m(==, equal) \
    m(!=, not_equal) \
    m(&, bit_and) \
    m(^, xor_) \
    m(|, bit_or) \
    m(&&, and_) \
    m(||, or_)

#define HOF_FOREACH_ASSIGN_OP(m) \
    m(+=, assign_add) \
    m(-=, assign_subtract) \
    m(*=, assign_multiply) \
    m(/=, assign_divide) \
    m(%=, assign_remainder) \
    m(>>=, assign_right_shift) \
    m(<<=, assign_left_shift) \
    m(&=, assign_bit_and) \
    m(|=, assign_bit_or) \
    m(^=, assign_xor)

#ifndef _MSC_VER
#define HOF_FOREACH_UNARY_OP(m) \
    m(!, not_) \
    m(~, compl_) \
    m(+, unary_plus) \
    m(-, unary_subtract) \
    m(*, dereference) \
    m(++, increment) \
    m(--, decrement)
#else
#define HOF_FOREACH_UNARY_OP(m) \
    m(!, not_) \
    m(~, compl_) \
    m(+, unary_plus) \
    m(-, unary_subtract) \
    m(*, dereference)
#endif

namespace operators {

struct call
{
    template<class F, class... Ts>
    constexpr auto operator()(F&& f, Ts&&... xs) const HOF_RETURNS
    (f(HOF_FORWARD(Ts)(xs)...));
};

// MSVC 2017 ICEs on && and || in conxtexpr, so we fallback on bitwise operators
#if defined(_MSC_VER) && _MSC_VER >= 1910
#define HOF_BINARY_OP_SKIP_and_ ()
#define HOF_BINARY_OP_SKIP_or_ ()

struct and_
{
    template<class T, class U>
    constexpr auto operator()(T&& x, U&& y) const 
    noexcept(noexcept(HOF_FORWARD(T)(x) && HOF_FORWARD(U)(y)))
    -> decltype(HOF_FORWARD(T)(x) && HOF_FORWARD(U)(y)) 
    { return HOF_FORWARD(T)(x) & HOF_FORWARD(U)(y); }
};

struct or_
{
    template<class T, class U>
    constexpr auto operator()(T&& x, U&& y) const 
    noexcept(noexcept(HOF_FORWARD(T)(x) || HOF_FORWARD(U)(y)))
    -> decltype(HOF_FORWARD(T)(x) || HOF_FORWARD(U)(y)) 
    { return HOF_FORWARD(T)(x) | HOF_FORWARD(U)(y); }
};

#define HOF_BINARY_OP_IMPL(op, name) \
    struct name \
    { \
        template<class T, class U> \
        HOF_USING(ex_failure, decltype(std::declval<T>() op std::declval<U>())); \
        struct failure : as_failure<ex_failure> {}; \
        template<class T, class U> \
        constexpr auto operator()(T&& x, U&& y) const HOF_RETURNS \
        (HOF_FORWARD(T)(x) op HOF_FORWARD(U)(y)); \
    };

#define HOF_BINARY_OP(op, name) \
    HOF_PP_IIF(HOF_PP_IS_PAREN(HOF_PP_CAT(HOF_BINARY_OP_SKIP_, name))) \
    (HOF_PP_EMPTY, HOF_BINARY_OP_IMPL)(op, name)

#else

#define HOF_BINARY_OP(op, name) \
    struct name \
    { \
        template<class T, class U> \
        constexpr auto operator()(T&& x, U&& y) const HOF_RETURNS \
        (HOF_FORWARD(T)(x) op HOF_FORWARD(U)(y)); \
    };

#endif

HOF_FOREACH_BINARY_OP(HOF_BINARY_OP)
HOF_FOREACH_ASSIGN_OP(HOF_BINARY_OP)

#define HOF_UNARY_OP(op, name) \
    struct name \
    { \
        template<class T> \
        constexpr auto operator()(T&& x) const HOF_RETURNS \
        (op(HOF_FORWARD(T)(x))); \
    };


HOF_FOREACH_UNARY_OP(HOF_UNARY_OP)


}

template<int N>
struct placeholder
{
#if HOF_HAS_MANGLE_OVERLOAD
    template<class... Ts>
    constexpr auto operator()(Ts&&... xs) const HOF_RETURNS 
    ( boost::hof::lazy(operators::call())(detail::simple_placeholder<N>(), HOF_FORWARD(Ts)(xs)...) );
#else
    template<class... Ts>
    struct result_call
    { typedef decltype(boost::hof::lazy(operators::call())(detail::simple_placeholder<N>(), std::declval<Ts>()...)) type; };
    template<class... Ts>
    constexpr typename result_call<Ts...>::type operator()(Ts&&... xs) const 
    { return boost::hof::lazy(operators::call())(detail::simple_placeholder<N>(), HOF_FORWARD(Ts)(xs)...); };

#endif

#define HOF_PLACEHOLDER_UNARY_OP(op, name) \
    constexpr auto operator op () const HOF_RETURNS \
    ( boost::hof::lazy(operators::name())(detail::simple_placeholder<N>()) );

HOF_FOREACH_UNARY_OP(HOF_PLACEHOLDER_UNARY_OP)

#define HOF_PLACEHOLDER_ASSIGN_OP(op, name) \
    template<class T> \
    constexpr auto operator op (T&& x) const HOF_RETURNS \
    ( boost::hof::lazy(operators::name())(detail::simple_placeholder<N>(), HOF_FORWARD(T)(x)) );

HOF_FOREACH_ASSIGN_OP(HOF_PLACEHOLDER_ASSIGN_OP)

};

#if HOF_HAS_MANGLE_OVERLOAD

#define HOF_PLACEHOLDER_BINARY_OP(op, name) \
    template<class T, int N> \
    constexpr inline auto operator op (const placeholder<N>&, T&& x) HOF_RETURNS \
    ( boost::hof::lazy(operators::name())(detail::simple_placeholder<N>(), HOF_FORWARD(T)(x)) ); \
    template<class T, int N> \
    constexpr inline auto operator op (T&& x, const placeholder<N>&) HOF_RETURNS \
    ( boost::hof::lazy(operators::name())(HOF_FORWARD(T)(x), detail::simple_placeholder<N>()) ); \
    template<int N, int M> \
    constexpr inline auto operator op (const placeholder<N>&, const placeholder<M>&) HOF_RETURNS \
    ( boost::hof::lazy(operators::name())(detail::simple_placeholder<N>(), detail::simple_placeholder<M>()) );

#else

#define HOF_PLACEHOLDER_BINARY_OP(op, name) \
    template<class T, class U> \
    struct result_ ## name \
    { typedef decltype(boost::hof::lazy(operators::name())(std::declval<T>(), std::declval<U>())) type; }; \
    template<class T, int N> \
    constexpr inline typename result_ ## name<detail::simple_placeholder<N>, T>::type operator op (const placeholder<N>&, T&& x) \
    { return boost::hof::lazy(operators::name())(detail::simple_placeholder<N>(), HOF_FORWARD(T)(x)); } \
    template<class T, int N> \
    constexpr inline typename result_ ## name<T, detail::simple_placeholder<N>>::type operator op (T&& x, const placeholder<N>&) \
    { return boost::hof::lazy(operators::name())(HOF_FORWARD(T)(x), detail::simple_placeholder<N>()); } \
    template<int N, int M> \
    constexpr inline typename result_ ## name<detail::simple_placeholder<N>, detail::simple_placeholder<M>>::type operator op (const placeholder<N>&, const placeholder<M>&) \
    { return boost::hof::lazy(operators::name())(detail::simple_placeholder<N>(), detail::simple_placeholder<M>()); }

#endif

HOF_FOREACH_BINARY_OP(HOF_PLACEHOLDER_BINARY_OP)

namespace placeholders {
HOF_DECLARE_STATIC_VAR(_1, placeholder<1>);
HOF_DECLARE_STATIC_VAR(_2, placeholder<2>);
HOF_DECLARE_STATIC_VAR(_3, placeholder<3>);
HOF_DECLARE_STATIC_VAR(_4, placeholder<4>);
HOF_DECLARE_STATIC_VAR(_5, placeholder<5>);
HOF_DECLARE_STATIC_VAR(_6, placeholder<6>);
HOF_DECLARE_STATIC_VAR(_7, placeholder<7>);
HOF_DECLARE_STATIC_VAR(_8, placeholder<8>);
HOF_DECLARE_STATIC_VAR(_9, placeholder<9>);
}

using placeholders::_1;
using placeholders::_2;
using placeholders::_3;
using placeholders::_4;
using placeholders::_5;
using placeholders::_6;
using placeholders::_7;
using placeholders::_8;
using placeholders::_9;

namespace detail {



struct unamed_placeholder
{
template<class T, class Invoker>
struct partial_ap
{
    T val;

    HOF_INHERIT_DEFAULT_EMPTY(partial_ap, T)

    template<class X, class... Xs, HOF_ENABLE_IF_CONSTRUCTIBLE(T, X&&, Xs&&...)>
    constexpr partial_ap(X&& x, Xs&&... xs) : val(HOF_FORWARD(X)(x), HOF_FORWARD(Xs)(xs)...)
    {}

    HOF_RETURNS_CLASS(partial_ap);

    struct partial_ap_failure
    {
        template<class Failure>
        struct apply
        {
            template<class... Xs>
            struct of;

            template<class X>
            struct of<X>
            : Failure::template of<typename std::add_const<T>::type, X>
            {};
        };
    };

    struct failure
    : failure_map<partial_ap_failure, Invoker>
    {};

    template<class X>
    constexpr HOF_SFINAE_RESULT(const Invoker&, id_<T>, id_<X>) 
    operator()(X&& x) const HOF_SFINAE_RETURNS
    (
        Invoker()(HOF_CONST_THIS->val, HOF_FORWARD(X)(x))
    );
};

template<class Invoker, class T>
static constexpr partial_ap<T, Invoker> make_partial_ap(T&& x)
{
    return {HOF_FORWARD(T)(x)};
}

template<class Op>
struct left
{
    struct failure
    : failure_for<Op>
    {};
    template<class T, class X>
    constexpr HOF_SFINAE_RESULT(const Op&, id_<T>, id_<X>) 
    operator()(T&& val, X&& x) const HOF_SFINAE_RETURNS
    (Op()(HOF_FORWARD(T)(val), HOF_FORWARD(X)(x)));
};

template<class Op>
struct right
{
    struct right_failure
    {
        template<class Failure>
        struct apply
        {
            template<class T, class U, class... Ts>
            struct of
            : Failure::template of<U, T, Ts...>
            {};
        };
    };

    struct failure
    : failure_map<right_failure, Op>
    {};

    template<class T, class X>
    constexpr HOF_SFINAE_RESULT(const Op&, id_<X>, id_<T>) 
    operator()(T&& val, X&& x) const HOF_SFINAE_RETURNS
    (Op()(HOF_FORWARD(X)(x), HOF_FORWARD(T)(val)));
};

#define HOF_UNAMED_PLACEHOLDER_UNARY_OP(op, name) \
    constexpr auto operator op () const HOF_RETURNS \
    ( operators::name() );

HOF_FOREACH_UNARY_OP(HOF_UNAMED_PLACEHOLDER_UNARY_OP)

#define HOF_UNAMED_PLACEHOLDER_ASSIGN_OP(op, name) \
    template<class T> \
    constexpr auto operator op (const T& x) const HOF_RETURNS \
    ( partial_ap<T, left<operators::name>>(x) );

HOF_FOREACH_ASSIGN_OP(HOF_UNAMED_PLACEHOLDER_ASSIGN_OP)
};
#define HOF_UNAMED_PLACEHOLDER_BINARY_OP(op, name) \
    template<class T> \
    constexpr inline auto operator op (const unamed_placeholder&, const T& x) HOF_RETURNS \
    ( unamed_placeholder::make_partial_ap<unamed_placeholder::right<operators::name>>(boost::hof::decay(x)) ); \
    template<class T> \
    constexpr inline auto operator op (const T& x, const unamed_placeholder&) HOF_RETURNS \
    ( unamed_placeholder::make_partial_ap<unamed_placeholder::left<operators::name>>(boost::hof::decay(x)) ); \
    constexpr inline auto operator op (const unamed_placeholder&, const unamed_placeholder&) HOF_RETURNS \
    ( operators::name() );

HOF_FOREACH_BINARY_OP(HOF_UNAMED_PLACEHOLDER_BINARY_OP)
}

namespace placeholders {
HOF_DECLARE_STATIC_VAR(_, detail::unamed_placeholder);
}

using placeholders::_;

}} // namespace boost::hof

namespace std {
    template<int N>
    struct is_placeholder<boost::hof::placeholder<N>>
    : std::integral_constant<int, N>
    {};
}

namespace boost {

    template<class T> 
    struct is_placeholder;

    template<int N>
    struct is_placeholder<boost::hof::placeholder<N>>
    : std::integral_constant<int, N>
    {};


}

#endif
