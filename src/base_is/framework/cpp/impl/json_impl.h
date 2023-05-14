/**
  * @file json_impl.h
  * @author Artiom N.(cl)2017
  * @brief BSON serializer/deserializer implementation's classes definition.
  *
  */

#ifndef _TSW_JSON_IMPL_H
#define _TSW_JSON_IMPL_H

#include <regex>
#include <type_traits>

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>

#include "tsw/types.h"
#include "tsw/type_traits.h"


namespace tsw
{

namespace impl
{

namespace rj = rapidjson;

class JsonSerializerImpl
{
public:
    const char *datetime_format = "%FT%T";
public:
    JsonSerializerImpl();
    ~JsonSerializerImpl();

public:
    void append_field(const String &name, const String &data);
    void append_field(const String &name, const String::value_type *data);
    void append_field(const String &name, int data);
    void append_field(const String &name, int64_t data);
    void append_field(const String &name, double data);
    void append_field(const String &name, bool data);
    void append_field(const String &name, const NameValueMap &data);
    void append_field(const String &name, const BinData &data);
    void append_field(const String &name, const Time &data);
    void append_field(const String &name, const Value &data);
    void append_field(const String &name, const ValueArray &data);
    void append_field(const String &name, const ValueList &data);

    void append_uid(const String &name, UID data, size_t size);
    void append_null_field(const String &name);
    void append_undefined_field(const String &name);

public:
    void clear();
    BinData get_buffer();

public:
    void print_buffer();

private:
    template<typename T>
    void append_field(const String &name, const T data);

private:
    inline void write_name(const String &name);

    inline void write_field(const String &data);
    inline void write_field(const String::value_type *data);
    inline void write_field(int data);
    inline void write_field(int64_t data);
    inline void write_field(double data);
    inline void write_field(bool data);
    inline void write_field(const NameValueMap &data);
    inline void write_field(const BinData &data);
    inline void write_field(Time data);
    inline void write_field(const Value &data);

    template<typename IterableType, typename std::enable_if<is_iterable<IterableType>::value, int>::type = 0>
    inline void write_field(const IterableType &data);

    inline void write_uid(UID data, size_t size);
    inline void write_null_field();
    inline void write_undefined_field();

private:
    // Fucking China programmer. RAPIDJson has private member hasRoot_ without getter.
    bool need_init_;
    rj::StringBuffer sb_;
    rj::PrettyWriter<rj::StringBuffer> writer_;
};


class JsonDeserializerImpl
{
public:
    JsonDeserializerImpl();
    ~JsonDeserializerImpl();

public:
    NameValueMap Deserialize(const BinData::value_type* bin_data, size_t size, bool raise_on_unknown = false);

private:
    Value ParseValue(const rj::Value &value);

private:
    const static std::regex iso8601_check_regex_;
    bool raise_on_unknown_;
    // Need for the exception message.
    const String *cur_value_name_;
};

} // namespace impl

} // namespace tsw

#endif // _TSW_JSON_IMPL_H
