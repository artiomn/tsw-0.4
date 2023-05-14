/**
  * @file localization.h
  * @author Artiom N.(cl)2017
  * @brief Localization, language translation and internatiolization routines.
  *
  */

#ifndef _TSW_LOCALIZATION_H
#define _TSW_LOCALIZATION_H

#include "types.h"


namespace tsw
{

#if !defined(_T)
#   define _T(x) L##x
#endif


/**
 * @param const text
 */
const String tr(const String& text);

/**
 * @param const text
*/
const String _(const String& text);

} // namespace tsw

#endif // _TRANSLATOR_H
