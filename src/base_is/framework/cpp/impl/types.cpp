/**
  * @file types.cpp
  * @author Artiom N.(cl)2017
  * @brief Types implementation.
  *
  */

#include <tsw/types.h>


namespace tsw
{

//----------------------------------------------------------------------------
// Path
//----------------------------------------------------------------------------

Path get_user_home()
{
#if defined(LINUX) || defined(MACOS)
    Path result = getenv("HOME");
#elif defined(WINDOWS)
    const String::value_type *hd = getenv("HOMEDRIVE");
    const String::value_type *home = getenv("HOMEPATH");
    Path result = home && hd ? Path(hd) / Path(home) : Path();
#endif

    return result;
}

} // namespace tsw
