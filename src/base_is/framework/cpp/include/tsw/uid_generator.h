/**
  * @file uid_generator.h
  * @author Artiom N.(cl)2018
  * @brief UIDGenerator class definition.
  *
  */

#ifndef _TSW_UIDGENERATOR_H
#define _TSW_UIDGENERATOR_H

#include "types.h"


namespace tsw
{

class UIDGenerator
{
public:
    static UID generate_uid();
};

}

#endif // _TSW_UIDGENERATOR_H
