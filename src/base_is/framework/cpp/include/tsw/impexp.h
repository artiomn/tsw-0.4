/**
  * @file impexp.h
  * @author Artiom N.(cl)2017
  * @brief Import attribute definitions.
  *
  */

#ifndef _TSW_IMPEXP_H
#define _TSW_IMPEXP_H

#if TSW_BASE_IS_LIBRARY_COMPILING
#   if _MSC_VER
#       define exported __declspec(dllexport)
#   else
#       define exported __attribute__ ((visibility ("default")))
#   endif
#else
#   if _MSC_VER
#       define exported __declspec(dllimport)
#   else
#       define exported __attribute__ ((visibility ("default")))
#   endif
#endif

#endif // _TSW_IMPEXP_H
