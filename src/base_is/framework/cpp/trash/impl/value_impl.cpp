#include <assert.h>

#include "value_impl.h"


namespace tsw
{
//----------------------------------------------------------------------------
// Value::ValueImpl
//----------------------------------------------------------------------------

Value::ValueImpl::ValueImpl(ValueType type)
{
    create_value(type);
}


Value::ValueImpl::ValueImpl(const ValueImpl &other)
{
    create_value(other.type_);
    from_other_value(other);
}


Value::ValueImpl::ValueImpl(bool b) :
    type_(ValueType::Boolean), m_i(b)
{
}


Value::ValueImpl::ValueImpl(double n) :
    type_(ValueType::DoubleNumber), m_d(n)
{
}


Value::ValueImpl::ValueImpl(int64_t n) :
    type_(ValueType::Int64Number), m_i64(n)
{
}


Value::ValueImpl::ValueImpl(const String &s) :
    type_(ValueType::String), m_s(s)
{
}


Value::ValueImpl::ValueImpl(const Time &t) :
    type_(ValueType::Time), m_t(t)
{
}


Value::ValueImpl::ValueImpl(const BinData &dt) :
    type_(ValueType::BinData), m_dt(dt)
{
}


Value::ValueImpl::ValueImpl(const ValueArray &a) :
    type_(ValueType::Array), m_a(a)
{
}


Value::ValueImpl::ValueImpl(const Object &o) :
    type_(ValueType::Object), m_o(o)
{
}


Value::ValueImpl::ValueImpl(const Value &other)
{
    from_other_value(other);
}


void Value::ValueImpl::destroy_current_value()
{
    switch (type_)
    {
    case ValueType::String:
    {
        m_s.~String();
    }
    break;
    case ValueType::Object:
    {
        m_o.~Object();
    }
    break;
    case ValueType::Array:
    {
        m_a.~ValueArray();
    }
    break;
    case ValueType::BinData:
    {
        m_dt.~BinData();
    }
    break;
    case ValueType::Time:
    {
        m_t.~Time();
    }
    break;
    case ValueType::Boolean:
    case ValueType::Int64Number:
    case ValueType::DoubleNumber:
    case ValueType::Undefined:
    case ValueType::Null:
    break;
    default:
        assert(false); // Unknown value type
    }
}


Value::ValueImpl::~ValueImpl()
{
    destroy_current_value();
}


void Value::ValueImpl::from_value_array(const ValueArray &value)
{
    if (type_ != ValueType::Array)
    {
        destroy_current_value();
        new (&m_a) ValueArray(value);
        type_ = ValueType::Array;
    }
    else m_a = value;
}


void Value::ValueImpl::from_bool(bool value)
{
    if (type_ != ValueType::Boolean)
    {
        destroy_current_value();
        m_i = value;
        type_ = ValueType::Boolean;
    }
    else m_i = value;
}


void Value::ValueImpl::from_double(double value)
{
    if (type_ != ValueType::DoubleNumber)
    {
        destroy_current_value();
        m_d = value;
        type_ = ValueType::DoubleNumber;
    }
    else m_d = value;
}


void Value::ValueImpl::from_int64(int64_t value)
{
    if (type_ != ValueType::Int64Number)
    {
        destroy_current_value();
        m_i64 = value;
        type_ = ValueType::Int64Number;
    }
    else m_i64 = value;
}


void Value::ValueImpl::from_object(const Object &value)
{
    if (type_ != ValueType::Object)
    {
        destroy_current_value();
        new (&m_o) Object(value);
        type_ = ValueType::Object;
    }
    else m_o = value;
}


void Value::ValueImpl::from_string(const String &value)
{
    if (type_ != ValueType::String)
    {
        destroy_current_value();
        new (&m_s) String(value);
        type_ = ValueType::String;
    }
    else m_s = value;
}


void Value::ValueImpl::from_bindata(const BinData &value)
{
    if (type_ != ValueType::BinData)
    {
        destroy_current_value();
        new (&m_dt) BinData(value);
        type_ = ValueType::BinData;
    }
    else m_dt = value;
}


void Value::ValueImpl::from_time(const Time &value)
{
    if (type_ != ValueType::Time)
    {
        destroy_current_value();
        new (&m_t) Time(value);
        type_ = ValueType::Time;
    }
    else m_t = value;
}


void Value::ValueImpl::create_value(ValueType type)
{
    switch (type)
    {
    case ValueType::DoubleNumber:
    {
        new (&m_d) double;
    }
    break;
    case ValueType::String:
    {
        new (&m_s) String();
    }
    break;
    case ValueType::Object:
    {
        new (&m_o) Object();
    }
    break;
    case ValueType::Array:
    {
        new (&m_a) ValueArray();
    }
    break;
    case ValueType::BinData:
    {
        new (&m_dt) BinData();
    }
    break;
    case ValueType::Boolean:
    {
        new (&m_i) bool;
    }
    break;
    case ValueType::Time:
    {
        new (&m_t) Time();
    }
    break;
    case ValueType::Int64Number:
    {
        new (&m_i64) int64_t;
    }
    break;
    case ValueType::Undefined:
    case ValueType::Null:
    break;
    default:
        assert(false); // Unknown value type
    }

    type_ = type;
}


void Value::ValueImpl::from_other_value(const ValueImpl& other)
{
    switch (type_)
    {
    case ValueType::String:
        m_s = other.m_s;
    break;
    case ValueType::Object:
    {
        m_o = other.m_o;
    }
    break;
    case ValueType::Array:
    {
        m_a = other.m_a;
    }
    break;
    case ValueType::BinData:
    {
        m_dt = other.m_dt;
    }
    break;
    case ValueType::Boolean:
        m_i = other.m_i;
    break;
    case ValueType::Int64Number:
        m_i64 = other.m_i64;
    break;
    case ValueType::DoubleNumber:
        m_d = other.m_d;
    break;
    case ValueType::Undefined:
    case ValueType::Null:
    default:
        assert(false); // Unknown value type
    }
}


bool Value::ValueImpl::can_convert(ValueType target_type_id) const
{
    //switch {};

    return false;
}


ValueArray Value::ValueImpl::to_value_array() const
{
    assert(can_convert(ValueType::Array));

    return m_a;
}


bool Value::ValueImpl::to_bool() const
{
    assert(can_convert(ValueType::Boolean));

    return m_i;
}


double Value::ValueImpl::to_double() const
{
    assert(can_convert(ValueType::DoubleNumber));

    return m_d;
}


int64_t Value::ValueImpl::to_int64() const
{
    assert(can_convert(ValueType::Int64Number));

    return m_i64;
}


Object Value::ValueImpl::to_object() const
{
    assert(can_convert(ValueType::Object));

    return m_o;
}


String Value::ValueImpl::to_string() const
{
    assert(can_convert(ValueType::String));

    return m_s;
}


BinData Value::ValueImpl::to_bindata() const
{
    assert(can_convert(ValueType::BinData));

    return m_dt;
}


Time Value::ValueImpl::to_time() const
{
    assert(can_convert(ValueType::Time));

    return m_t;
}


ValueType Value::ValueImpl::type() const
{
    return type_;
}


Value::ValueImpl& Value::ValueImpl::operator=(const ValueImpl &other)
{
    if (other.type_ != type_)
    {
        destroy_current_value();
        create_value(other.type_);
        from_other_value(other);
    }
    else from_other_value(other);

    return *this;
}


bool Value::ValueImpl::operator==(const Value &other) const
{


}


/*
 template<typename F, typename... Ts>
struct variant_helper {
  static const size_t size =
    sizeof(F) > variant_helper<Ts...>::size ? sizeof(F) : variant_helper<Ts...>::size;
};

template<typename F>
struct variant_helper<F>  {
        static const size_t size = sizeof(F);
};*/
} // namespace tsw
