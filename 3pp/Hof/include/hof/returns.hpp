/*=============================================================================
    Copyright (c) 2014 Paul Fultz II
    returns.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef HOF_GUARD_RETURNS_H
#define HOF_GUARD_RETURNS_H

/// HOF_RETURNS
/// ===========
/// 
/// Description
/// -----------
/// 
/// The `HOF_RETURNS` macro defines the function as the expression
/// equivalence. It does this by deducing `noexcept` and the return type by
/// using a trailing `decltype`. Instead of repeating the expression for the
/// return type, `noexcept` clause and the function body, this macro will
/// reduce the code duplication from that.
/// 
/// Note: The expression used to deduce the return the type will also
/// constrain the template function and deduce `noexcept` as well, which is
/// different behaviour than using C++14's return type deduction.
/// 
/// Synopsis
/// --------
/// 
///     #define HOF_RETURNS(...) 
/// 
/// 
/// Example
/// -------
/// 
///     #include <hof.hpp>
///     #include <cassert>
/// 
///     template<class T, class U>
///     auto sum(T x, U y) HOF_RETURNS(x+y);
/// 
///     int main() {
///         assert(3 == sum(1, 2));
///     }
/// 
/// 
/// Incomplete this
/// ---------------
/// 
/// Description
/// -----------
/// 
/// On some non-conformant compilers, such as gcc, the `this` variable cannot
/// be used inside the `HOF_RETURNS` macro because it is considered an
/// incomplete type. So the following macros are provided to help workaround
/// the issue.
/// 
/// 
/// Synopsis
/// --------
/// 
///     // Declares the type of the `this` variable
///     #define HOF_RETURNS_CLASS(...) 
///     // Used to refer to the `this` variable in the HOF_RETURNS macro
///     #define HOF_THIS
///     // Used to refer to the const `this` variable in the HOF_RETURNS macro
///     #define HOF_CONST_THIS
/// 
/// 
/// Example
/// -------
/// 
///     #include <hof.hpp>
///     #include <cassert>
/// 
///     struct add_1
///     {
///         int a;
///         add_1() : a(1) {}
///         
///         HOF_RETURNS_CLASS(add_1);
///         
///         template<class T>
///         auto operator()(T x) const 
///         HOF_RETURNS(x+HOF_CONST_THIS->a);
///     };
/// 
///     int main() {
///         assert(3 == add_1()(2));
///     }
/// 
/// 
/// Mangling overloads
/// ------------------
/// 
/// Description
/// -----------
/// 
/// On older compilers some operations done in the expressions cannot be
/// properly mangled. These macros help provide workarounds for these
/// operations on older compilers.
/// 
/// 
/// Synopsis
/// --------
/// 
///     // Explicitly defines the type for name mangling
///     #define HOF_MANGLE_CAST(...) 
///     // C cast for name mangling
///     #define HOF_RETURNS_C_CAST(...) 
///     // Reinterpret cast for name mangling
///     #define HOF_RETURNS_REINTERPRET_CAST(...) 
///     // Static cast for name mangling
///     #define HOF_RETURNS_STATIC_CAST(...) 
///     // Construction for name mangling
///     #define HOF_RETURNS_CONSTRUCT(...) 
/// 


#include <hof/config.hpp>
#include <utility>
#include <hof/detail/forward.hpp>
#include <hof/detail/noexcept.hpp>

#define HOF_EAT(...)
#define HOF_REM(...) __VA_ARGS__

#if HOF_HAS_COMPLETE_DECLTYPE && HOF_HAS_MANGLE_OVERLOAD
#ifdef _MSC_VER
// Since decltype can't be used in noexcept on MSVC, we can't check for noexcept
// move constructors.
#define HOF_RETURNS_DEDUCE_NOEXCEPT(...) HOF_NOEXCEPT(noexcept(__VA_ARGS__))
#else
#define HOF_RETURNS_DEDUCE_NOEXCEPT(...) HOF_NOEXCEPT(noexcept(static_cast<decltype(__VA_ARGS__)>(__VA_ARGS__)))
#endif
#define HOF_RETURNS(...) \
HOF_RETURNS_DEDUCE_NOEXCEPT(__VA_ARGS__) \
-> decltype(__VA_ARGS__) { return __VA_ARGS__; }
#define HOF_THIS this
#define HOF_CONST_THIS this
#define HOF_RETURNS_CLASS(...) \
void fit_returns_class_check() \
{ \
    static_assert(std::is_same<__VA_ARGS__*, decltype(this)>::value, \
        "Returns class " #__VA_ARGS__ " type doesn't match"); \
}

#define HOF_MANGLE_CAST(...) HOF_REM

#define HOF_RETURNS_C_CAST(...) (__VA_ARGS__) HOF_REM
#define HOF_RETURNS_REINTERPRET_CAST(...) reinterpret_cast<__VA_ARGS__>
#define HOF_RETURNS_STATIC_CAST(...) static_cast<__VA_ARGS__>
#define HOF_RETURNS_CONSTRUCT(...) __VA_ARGS__
#else
#include <hof/detail/pp.hpp>

#define HOF_RETURNS_RETURN(...) return HOF_RETURNS_RETURN_X(HOF_PP_WALL(__VA_ARGS__))
#define HOF_RETURNS_RETURN_X(...) __VA_ARGS__

#ifdef _MSC_VER
// Since decltype can't be used in noexcept on MSVC, we can't check for noexcept
// move constructors.
#define HOF_RETURNS_DEDUCE_NOEXCEPT(...) HOF_NOEXCEPT(noexcept(HOF_RETURNS_DECLTYPE_CONTEXT(__VA_ARGS__)))
#else
#define HOF_RETURNS_DEDUCE_NOEXCEPT(...) HOF_NOEXCEPT(noexcept(static_cast<decltype(HOF_RETURNS_DECLTYPE_CONTEXT(__VA_ARGS__))>(HOF_RETURNS_DECLTYPE_CONTEXT(__VA_ARGS__))))
#endif

#define HOF_RETURNS_DECLTYPE(...) \
-> decltype(HOF_RETURNS_DECLTYPE_CONTEXT(__VA_ARGS__))

#define HOF_RETURNS_DECLTYPE_CONTEXT(...) HOF_RETURNS_DECLTYPE_CONTEXT_X(HOF_PP_WALL(__VA_ARGS__))
#define HOF_RETURNS_DECLTYPE_CONTEXT_X(...) __VA_ARGS__

#define HOF_RETURNS_THAT(...) HOF_PP_IIF(HOF_PP_IS_PAREN(HOF_RETURNS_DECLTYPE_CONTEXT(())))(\
    (boost::hof::detail::check_this<__VA_ARGS__, decltype(this)>(), this), \
    std::declval<__VA_ARGS__>() \
)

#define HOF_THIS HOF_PP_RAIL(HOF_RETURNS_THAT)(fit_this_type)
#define HOF_CONST_THIS HOF_PP_RAIL(HOF_RETURNS_THAT)(fit_const_this_type)

#define HOF_RETURNS_CLASS(...) typedef __VA_ARGS__* fit_this_type; typedef const __VA_ARGS__* fit_const_this_type

#define HOF_RETURNS(...) \
HOF_RETURNS_DEDUCE_NOEXCEPT(__VA_ARGS__) \
HOF_RETURNS_DECLTYPE(__VA_ARGS__) \
{ HOF_RETURNS_RETURN(__VA_ARGS__); }


namespace boost { namespace hof { namespace detail {
template<class Assumed, class T>
struct check_this
{
    static_assert(std::is_same<T, Assumed>::value, "Incorret HOF_THIS or HOF_CONST_THIS used.");
};

}}} // namespace boost::hof

#endif


#if HOF_HAS_MANGLE_OVERLOAD

#define HOF_MANGLE_CAST(...) HOF_REM

#define HOF_RETURNS_C_CAST(...) (__VA_ARGS__) HOF_REM
#define HOF_RETURNS_REINTERPRET_CAST(...) reinterpret_cast<__VA_ARGS__>
#define HOF_RETURNS_STATIC_CAST(...) static_cast<__VA_ARGS__>
#define HOF_RETURNS_CONSTRUCT(...) __VA_ARGS__

#else

#define HOF_RETURNS_DERAIL_MANGLE_CAST(...) HOF_PP_IIF(HOF_PP_IS_PAREN(HOF_RETURNS_DECLTYPE_CONTEXT(())))(\
    HOF_REM, \
    std::declval<__VA_ARGS__>() HOF_EAT \
)
#define HOF_MANGLE_CAST HOF_PP_RAIL(HOF_RETURNS_DERAIL_MANGLE_CAST)


#define HOF_RETURNS_DERAIL_C_CAST(...) HOF_PP_IIF(HOF_PP_IS_PAREN(HOF_RETURNS_DECLTYPE_CONTEXT(())))(\
    (__VA_ARGS__) HOF_REM, \
    std::declval<__VA_ARGS__>() HOF_EAT \
)
#define HOF_RETURNS_C_CAST HOF_PP_RAIL(HOF_RETURNS_DERAIL_C_CAST)


#define HOF_RETURNS_DERAIL_REINTERPRET_CAST(...) HOF_PP_IIF(HOF_PP_IS_PAREN(HOF_RETURNS_DECLTYPE_CONTEXT(())))(\
    reinterpret_cast<__VA_ARGS__>, \
    std::declval<__VA_ARGS__>() HOF_EAT \
)
#define HOF_RETURNS_REINTERPRET_CAST HOF_PP_RAIL(HOF_RETURNS_DERAIL_REINTERPRET_CAST)

#define HOF_RETURNS_DERAIL_STATIC_CAST(...) HOF_PP_IIF(HOF_PP_IS_PAREN(HOF_RETURNS_DECLTYPE_CONTEXT(())))(\
    static_cast<__VA_ARGS__>, \
    std::declval<__VA_ARGS__>() HOF_EAT \
)
#define HOF_RETURNS_STATIC_CAST HOF_PP_RAIL(HOF_RETURNS_DERAIL_STATIC_CAST)

#define HOF_RETURNS_DERAIL_CONSTRUCT(...) HOF_PP_IIF(HOF_PP_IS_PAREN(HOF_RETURNS_DECLTYPE_CONTEXT(())))(\
    __VA_ARGS__, \
    std::declval<__VA_ARGS__>() HOF_EAT \
)
#define HOF_RETURNS_CONSTRUCT HOF_PP_RAIL(HOF_RETURNS_DERAIL_CONSTRUCT)

#endif

#define HOF_AUTO_FORWARD(...) HOF_RETURNS_STATIC_CAST(decltype(__VA_ARGS__))(__VA_ARGS__)

#endif
