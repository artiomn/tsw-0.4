/**
  * @file tests_common.h
  * @author Artiom N.(cl)2017
  * @brief Common test routines and definitions.
  *
  */

#ifndef _TSW_TESTS_COMMON_H
#define _TSW_TESTS_COMMON_H

#include <boost/range.hpp>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <tsw/debug_tools.h>
#include <tsw/types.h>


template<typename T, size_t size>
::testing::AssertionResult ArraysMatch(const T (&expected)[size],
                                       const T (&actual)[size])
{
    for (size_t i = 0; i < size; ++i)
    {
        if (expected[i] != actual[i])
        {
            return ::testing::AssertionFailure() << "array[" << i
                << "] (" << actual[i] << ") != expected[" << i
                << "] (" << expected[i] << ")";
        }
    }

    return ::testing::AssertionSuccess();
}


template<typename T>
::testing::AssertionResult ArraysMatch(const T *expected,
                                       const T *actual,
                                       size_t size)
{
    for (size_t i = 0; i < size; ++i)
    {
        if (expected[i] != actual[i])
        {
            return ::testing::AssertionFailure() << "array[" << i
                << "] (" << actual[i] << ") != expected[" << i
                << "] (" << expected[i] << ")";
        }
    }

    return ::testing::AssertionSuccess();
}

#endif // _TSW_TESTS_COMMON_H
