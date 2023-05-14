#ifndef _TSW_TYPES_H
#define _TSW_TYPES_H

#include <cstddef>
#include <cstdint>
#include <ctime>
#include <list>
#include <map>
#include <memory>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>


namespace tsw
{

class Object
{
public:
    Object() = delete;
    Object(const Object &other) = default;
    Object(Object &&other);

    Object(const String &name);
    Object(String &&name);
    Object(const String &name, const NameValueMap &fields);
    Object(String &&name, const NameValueMap &fields);
    Object(const String &name, NameValueMap &&fields);
    Object(String &&name, NameValueMap &&fields);

    Object &operator=(const Object &other) = default;
    Object &operator=(Object &&other);

public:
    const String &get_name() const;
    const NameValueMap &get_fields() const;

public:
    bool operator<(const Object &other) const noexcept;
    bool operator>(const Object &other) const noexcept;
    bool operator<=(const Object &other) const noexcept;
    bool operator>=(const Object &other) const noexcept;
    bool operator==(const Object &other) const noexcept;
    bool operator!=(const Object &other) const noexcept;

private:
    String name_;
    NameValueMap fields_;
};


class Value
{
public:
    Value(ValueType type = ValueType::Null);
    Value(bool b);
    Value(double n);
    Value(int64_t n);
    Value(const String &s);
    Value(const Time &t);
    Value(const String::value_type *s);
    Value(const BinData &dt);
    Value(const ValueArray &a);
    Value(const Object &o);
    Value(const Value &other);
    Value(Value &&other);

    ~Value();
public:
    bool        can_convert(ValueType targetTypeId) const;
    bool        is_array() const;
    bool        is_bool() const;
    bool        is_double() const;
    bool        is_int64() const;
    bool        is_null() const;
    bool        is_object() const;
    bool        is_string() const;
    bool        is_bindata() const;
    bool        is_time() const;
    bool        is_undefined() const;
public:
    ValueArray      to_value_array() const;
    bool            to_bool() const;
    double          to_double() const;
    int             to_int() const;
    int64_t         to_int64() const;
    Object          to_object() const;
    String          to_string() const;
    BinData         to_bindata() const;
    Time            to_time() const;
    ValueType       type() const;
public:
    bool            operator<(const Value &other) const;
    bool            operator>(const Value &other) const;
    bool            operator!=(const Value &other) const;
    Value&          operator=(const Value &other);
    bool            operator==(const Value &other) const;
private:
    // force compile error, prevent Variant(bool) to be called
    Value(void *) = delete;
    class ValueImpl;
    std::unique_ptr<ValueImpl> value_;
};

} // namespace tsw

#endif // _TSW_TYPES_H
