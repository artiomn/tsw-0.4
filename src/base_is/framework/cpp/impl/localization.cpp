/**
  * @file localization.cpp
  * @author Artiom N.(cl)2017
  * @brief Localization, language-translation and internatiolization routines implementation.
  *
  */

#include <libintl.h>

#include <tsw/localization.h>
#include <tsw/types.h>


namespace tsw
{

const String tr(const String& text)
{
    return gettext(text.c_str());
}


const String _(const String& text)
{
    return gettext(text.c_str());
}

} // namespace tsw
