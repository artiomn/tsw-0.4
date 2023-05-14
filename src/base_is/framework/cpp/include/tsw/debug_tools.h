/**
  * @file debug_tools.h
  * @author Artiom N.(cl)2017
  * @brief Routines useful for debugging purposes (i.e., value visualizers).
  *
  */

#ifndef _TSW_DEBUG_TOOLS_H
#define _TSW_DEBUG_TOOLS_H


#include "types.h"


namespace tsw
{

void print_value(const Value& value);
std::string demangle(const char* name) noexcept;

} // namespace tsw

#endif // _TSW_DEBUG_TOOLS_H
