/**
  * @file field.h
  * @author Artiom N.(cl)2017
  * @brief Field and FieldHeader classes definition.
  *
  */

#ifndef _TSW_FIELD_H
#define _TSW_FIELD_H

#include "types.h"


namespace tsw
{

struct FieldHeader
{
   UID         uid;
   ValueType   type;
   String      name;
   int         position;
   String      description;
};


class Field
{
public:
   Field(const FieldHeader& header, const Value& value);
   const FieldHeader& 	get_header() const;
   const Value& 		get_value() const;
private:
   const FieldHeader header_;
   const Value       value_;
};

} // namespace tsw

#endif // _TSW_FIELD_H
