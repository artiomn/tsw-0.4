#ifndef _TSW_VALUE_IMPL_H
#define _TSW_VALUE_IMPL_H

#include <tsw/types.h>


namespace tsw
{

class Value::ValueImpl
{
public:
    ValueImpl(ValueType type);
    ValueImpl(const ValueImpl &other);
    ValueImpl(bool b);
    ValueImpl(double n);
    ValueImpl(int64_t n);
    ValueImpl(const String &s);
    ValueImpl(const Time &t);
    ValueImpl(const BinData &dt);
    ValueImpl(const ValueArray &a);
    ValueImpl(const Object &o);
    ValueImpl(const Value &other);
    ~ValueImpl();

    bool can_convert(ValueType targetTypeId) const;
public:
    void from_value_array(const ValueArray &value);
    void from_bool(bool value);
    void from_double(double value);
    void from_int64(int64_t value);
    void from_object(const Object &value);
    void from_string(const String &value);
    void from_bindata(const BinData &value);
    void from_time(const Time &value);
public:
    ValueArray      to_value_array() const;
    bool            to_bool() const;
    double          to_double() const;
    int64_t         to_int64() const;
    Object          to_object() const;
    String          to_string() const;
    BinData         to_bindata() const;
    Time            to_time() const;
    ValueType       type() const;
public:
    Value::ValueImpl&   operator=(const ValueImpl &other);
    bool                operator!=(const Value &other) const;
    bool                operator==(const Value &other) const;

private:
    void create_value(ValueType type);
    void from_other_value(const ValueImpl& other);
    void destroy_current_value();
private:
    union
    {
        double      m_d;
        int         m_i;
        int64_t     m_i64;
        String      m_s;
        Object      m_o;
        Time        m_t;
        ValueArray  m_a;
        BinData     m_dt;
    };
    ValueType type_;
};

} // namespace tsw

#endif // _TSW_VALUE_IMPL_H
