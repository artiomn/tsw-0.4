/**
  * @file functional_helper.h
  * @author Artiom N.(cl)2017
  * @brief Specific functions, extending std::functional library.
  *
  */

#ifndef _TSW_FUNCTIONAL_HELPER_H
#define _TSW_FUNCTIONAL_HELPER_H

#include <functional>


namespace tsw
{

namespace impl
{

// Return target address from the std::function<>.
// Need to compare objects.
template<typename T, typename... U>
void *get_target_address(std::function<T(U...)> f)
{
    typedef T (fnType)(U...);
    fnType ** fn_pointer = f.template target<fnType*>();
    return (void*)*fn_pointer;
}

template<typename T, typename... U>
bool compare_function_addresses(std::function<T(U...)> f1, std::function<T(U...)> f2)
{
    return get_target_address(f1) == get_target_address(f2);
}

template<typename T, typename... U>
bool operator==(std::function<T(U...)> f1, std::function<T(U...)> f2)
{
    return get_target_address(f1) == get_target_address(f2);
}

} // namespace impl

} // namespace tsw

#endif // _TSW_FUNCTIONAL_HELPER_H
