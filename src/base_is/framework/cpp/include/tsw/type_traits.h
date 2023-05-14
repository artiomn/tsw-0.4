/**
  * @file type_traits.h
  * @author Artiom N.(cl)2017
  * @brief Type traits and type manpulation routines.
  *
  */

#pragma once

#include <iterator>
#include <type_traits>
#include <variant>
#include <boost/tti/has_member_function.hpp>


namespace tsw
{

/////////////////////////////////////////////////////
// copied and reduced from boost/strong_typedef.hpp
#define TSW_STRONG_TYPEDEF(T, D)                                    \
struct D                                                            \
    /*: boost::totally_ordered1< D           */                     \
    /*, boost::totally_ordered2< D, T        */                     \
    /*> >                                    */                     \
{                                                                   \
    typedef T orig_type;                                            \
    typedef D new_type;                                             \
    T t;                                                            \
    explicit D(const T t_) : t(t_) {}                               \
    D(){}                                                           \
    D(const D & t_) : t(t_.t){}                                     \
    D & operator=(const D & rhs) { t = rhs.t; return *this;}        \
    D & operator=(const T & rhs) { t = rhs; return *this;}          \
    explicit operator const T & () const {return t; }               \
    explicit operator T & () { return t; }                          \
    bool operator==(const D & rhs) const { return t == rhs.t; }     \
    bool operator<(const D & rhs) const { return t < rhs.t; }       \
};


BOOST_TTI_HAS_MEMBER_FUNCTION(begin)
BOOST_TTI_HAS_MEMBER_FUNCTION(end)

namespace detail
{
    // To allow ADL with custom begin/end
    using std::begin;
    using std::end;

    template <typename T>
    auto is_iterable_impl(int)
    -> decltype (
        begin(std::declval<T&>()) != end(std::declval<T&>()), // begin/end and operator !=
        void(), // Handle evil operator ,
        ++std::declval<decltype(begin(std::declval<T&>()))&>(), // operator ++
        void(*begin(std::declval<T&>())), // operator*
        std::true_type{});

    template <typename T>
    std::false_type is_iterable_impl(...);

}

template <typename T>
using is_iterable = decltype(detail::is_iterable_impl<T>(0));

} // namespace tsw
