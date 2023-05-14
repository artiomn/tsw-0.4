/**
  * @file bson_impl.h
  * @author Artiom N.(cl)2017
  * @brief BSON serializer/deserializer implementation's classes definition.
  *
  */

#pragma once

#include <type_traits>

#include <ejdb/ejdb.h>

#include "tsw/types.h"
#include "tsw/type_traits.h"


namespace tsw
{

namespace impl
{

class BsonSerializerImpl
{
public:
    BsonSerializerImpl();
    ~BsonSerializerImpl();

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

    template<typename IterableType, typename std::enable_if<is_iterable<IterableType>::value, int>::type = 0>
    void append_field(const String &name, const IterableType &data);

public:
    void clear();
    BinData get_buffer();

public:
    void print_buffer();

protected:
    void init(bool need_init = true);

private:
    bson bs_rec_;
};


class BsonDeserializerImpl
{
public:
    BsonDeserializerImpl();
    ~BsonDeserializerImpl();

public:
    NameValueMap Deserialize(const BinData::value_type* bin_data, size_t size, bool raise_on_unknown = true)
    {
        raise_on_unknown_ = raise_on_unknown;
        return DeserializeInternal(bin_data, size);
    }

private:
    NameValueMap DeserializeInternal(const BinData::value_type* bin_data, size_t size);

private:
    bool raise_on_unknown_;
};

} // namespace impl

} // namespace tsw
