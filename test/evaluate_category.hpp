// Copyright Cromwell D. Enage 2017.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef LIBS_PARAMETER_TEST_EVALUTE_CATEGORY_HPP
#define LIBS_PARAMETER_TEST_EVALUTE_CATEGORY_HPP

namespace test {

    enum invoked
    {
        passed_by_lvalue_reference_to_const
      , passed_by_lvalue_reference
      , passed_by_rvalue_reference_to_const
      , passed_by_rvalue_reference
    };

    float rvalue_float()
    {
        return 0.0f;
    }

    float const rvalue_const_float()
    {
        return 0.0f;
    }

    float& lvalue_float()
    {
        static float lfloat = 0.0f;
        return lfloat;
    }

    float const& lvalue_const_float()
    {
        static float const clfloat = 0.0f;
        return clfloat;
    }

    char const* rvalue_char_ptr()
    {
        return "foo";
    }

    char const* const rvalue_const_char_ptr()
    {
        return "foo";
    }

    char const*& lvalue_char_ptr()
    {
        static char const* larr = "foo";
        return larr;
    }

    char const* const& lvalue_const_char_ptr()
    {
        static char const* const clarr = "foo";
        return clarr;
    }
} // namespace test

#include <string>

namespace test {

    std::string rvalue_str()
    {
        return std::string("bar");
    }

    std::string const rvalue_const_str()
    {
        return std::string("bar");
    }

    std::string& lvalue_str()
    {
        static std::string lstr = std::string("bar");
        return lstr;
    }

    std::string const& lvalue_const_str()
    {
        static std::string const clstr = std::string("bar");
        return clstr;
    }
} // namespace test

#include <boost/parameter/config.hpp>

namespace test {

    template <typename T>
    struct A
    {
        static invoked evaluate_category(T const&)
        {
            return passed_by_lvalue_reference_to_const;
        }

        static invoked evaluate_category(T&)
        {
            return passed_by_lvalue_reference;
        }

#if defined BOOST_PARAMETER_HAS_PERFECT_FORWARDING
        static invoked evaluate_category(T const&&)
        {
            return passed_by_rvalue_reference_to_const;
        }

        static invoked evaluate_category(T&&)
        {
            return passed_by_rvalue_reference;
        }
#endif
    };
} // namespace test

#endif // LIBS_PARAMETER_TEST_EVALUTE_CATEGORY_HPP

