/**
  * @file field.cpp
  * @author Artiom N.(cl)2017
  * @brief Field class implementation.
  *
  */

#include <tsw/field.h>
#include <tsw/types.h>


namespace tsw
{

Field::Field(const FieldHeader& header, const Value& value) : header_(header), value_(value)
{
}


const FieldHeader& Field::get_header() const
{
    return header_;
}


const Value& Field::get_value() const
{
    return value_;
}

} // namespace tsw
