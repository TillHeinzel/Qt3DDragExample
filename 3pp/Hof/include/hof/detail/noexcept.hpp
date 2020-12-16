/*=============================================================================
    Copyright (c) 2016 Paul Fultz II
    noexcept.hpp
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef HOF_GUARD_DETAIL_NOEXCEPT_HPP
#define HOF_GUARD_DETAIL_NOEXCEPT_HPP

#include <hof/config.hpp>

#if HOF_HAS_NOEXCEPT_DEDUCTION
#define HOF_NOEXCEPT(...) noexcept(__VA_ARGS__)
#else
#define HOF_NOEXCEPT(...)
#endif

#endif
