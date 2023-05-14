#include <tsw/types.h>

#include "value_impl.h"


namespace tsw
{

//----------------------------------------------------------------------------
// Value
//----------------------------------------------------------------------------

Value::Value(ValueType type) : value_(new ValueImpl(type)) {}


Value::Value(bool b) : value_(new ValueImpl(b)) {}


Value::Value(double n) : value_(new ValueImpl(n)) {}


Value::Value(int64_t n) : value_(new ValueImpl(ValueType::Int64Number))
{
    value_->from_int64(n);
}


Value::Value(const String &s) : value_(new ValueImpl(s)) {}


Value::Value(const Time &t) : value_(new ValueImpl(t)) {}


Value::Value(const String::value_type *s) : value_(new ValueImpl(String(s))) {}


Value::Value(const BinData &dt) : value_(new ValueImpl(dt)) {}


Value::Value(const ValueArray &a) : value_(new ValueImpl(a)) {}


Value::Value(const Object &o) : value_(new ValueImpl(o)) {}


Value::Value(const Value &other) : value_(new ValueImpl(*other.value_)) {}


Value::Value(Value &&other) : value_(std::move(other.value_)) {}


Value::~Value()
{
}


bool Value::can_convert(ValueType target_type_id) const
{
   return value_->can_convert(target_type_id);
}


bool Value::is_array() const
{
   return value_->type() == ValueType::Array;
}


bool Value::is_bool() const
{
   return value_->type() == ValueType::Boolean;
}


bool Value::is_double() const
{
    return value_->type() == ValueType::DoubleNumber;
}


bool Value::is_int64() const
{
    return value_->type() == ValueType::Int64Number;
}


bool Value::is_null() const
{
    return value_->type() == ValueType::Null;
}


bool Value::is_object() const
{
    return value_->type() == ValueType::Object;
}


bool Value::is_string() const
{
    return value_->type() == ValueType::String;
}


bool Value::is_bindata() const
{
    return value_->type() == ValueType::BinData;
}


bool Value::is_time() const
{
    return value_->type() == ValueType::Time;
}


bool Value::is_undefined() const
{
    return value_->type() == ValueType::Undefined;
}


ValueArray Value::to_value_array() const
{
    return value_->to_value_array();
}


bool Value::to_bool() const
{
    return value_->to_bool();
}


double Value::to_double() const
{
    return value_->to_double();
}


int64_t Value::to_int64() const
{
    return value_->to_int64();
}


Object Value::to_object() const
{
    return value_->to_object();
}


String Value::to_string() const
{
    return value_->to_string();
}


BinData Value::to_bindata() const
{
    return value_->to_bindata();
}


Time Value::to_time() const
{
    return value_->to_time();
}


ValueType Value::type() const
{
    return value_->type();
}


bool Value::operator<(const Value &other) const
{

}


bool Value::operator>(const Value &other) const
{

}


bool Value::operator!=(const Value &other) const
{

}


Value& Value::operator=(const Value &other)
{

}


bool Value::operator==(const Value &other) const
{

}

} // namespace tsw
