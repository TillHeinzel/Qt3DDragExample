/*=============================================================================
    Copyright (c) 2016 Paul Fultz II
    using.hpp
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef HOF_GUARD_USING_HPP
#define HOF_GUARD_USING_HPP

#include <hof/config.hpp>

#if HOF_HAS_TEMPLATE_ALIAS
#define HOF_USING(name, ...) using name = __VA_ARGS__
#define HOF_USING_TYPENAME(name, ...) using name = typename __VA_ARGS__
#else
#define HOF_USING(name, ...) struct name : std::enable_if<true, __VA_ARGS__>::type {}
#define HOF_USING_TYPENAME(name, ...) struct name : __VA_ARGS__ {}
#endif

#endif
