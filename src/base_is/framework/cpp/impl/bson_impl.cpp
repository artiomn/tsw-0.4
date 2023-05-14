/**
  * @file bson_impl.cpp
  * @author Artiom N.(cl)2017
  * @brief BSON classes and routines implementation.
  *
  */

#include <iostream>
#include <limits>
#include <memory>
#include <string>
#include <type_traits>

#include "tsw/error.h"

#include "bson_impl.h"


namespace tsw
{

namespace impl
{

class BsonException : public Exception
{
    using Exception::Exception;
};


static void bson_error_handler(const char *errmsg)
{
    TSW_THROW(BsonException, errmsg);
}


//----------------------------------------------------------------------------
// BsonSerializerImpl
//----------------------------------------------------------------------------

BsonSerializerImpl::BsonSerializerImpl()
{
    set_bson_err_handler(&bson_error_handler);
    init();
}


BsonSerializerImpl::~BsonSerializerImpl()
{
    int result = bson_finish(&bs_rec_);
    TSW_ASSERT(result == BSON_OK);

    bson_destroy(&bs_rec_);
}


void BsonSerializerImpl::init(bool need_init)
{
    if (!need_init) return;

    // If result is not Ok, error will be thrown in the error_handler.
    int result = bson_safe_init(&bs_rec_);
    TSW_ASSERT(BSON_OK == result);
}


void BsonSerializerImpl::append_field(const String &name, const String &data)
{
    init(bs_rec_.finished);

    // error_handler will not be called in bson_append_* methods.
    if (bson_append_string_n(&bs_rec_, name.c_str(), data.c_str(), data.size()) != BSON_OK)
    {
        TSW_THROW(BsonException, "Can't append field \"" + name + "\" as a string \"" + data + "\" into a BSON document");
    }
}


void BsonSerializerImpl::append_field(const String &name, const String::value_type *data)
{
    init(bs_rec_.finished);

    if (bson_append_string(&bs_rec_, name.c_str(), data) != BSON_OK)
    {
        TSW_THROW(BsonException, "Can't append field \"" + name + "\" as a char* \"" + String(data) + "\" into a BSON document");
    }
}


void BsonSerializerImpl::append_field(const String &name, int data)
{
    init(bs_rec_.finished);

    if (bson_append_int(&bs_rec_, name.c_str(), data) != BSON_OK)
    {
        TSW_THROW(BsonException, "Can't append field \"" + name + "\" as an integer \"" + std::to_string(data) + "\" into a BSON document");
    }
}


void BsonSerializerImpl::append_field(const String &name, int64_t data)
{
    init(bs_rec_.finished);

    if (bson_append_long(&bs_rec_, name.c_str(), data) != BSON_OK)
    {
        TSW_THROW(BsonException, "Can't append field \"" + name + "\" as an int64 \"" + std::to_string(data) + "\" into a BSON document");
    }
}


void BsonSerializerImpl::append_field(const String &name, double data)
{
    init(bs_rec_.finished);

    if (bson_append_double(&bs_rec_, name.c_str(), data) != BSON_OK)
    {
        TSW_THROW(BsonException, "Can't append field \"" + name + "\" as a double \"" + std::to_string(data) + "\" into a BSON document");
    }
}


void BsonSerializerImpl::append_field(const String &name, bool data)
{
    init(bs_rec_.finished);

    if (bson_append_bool(&bs_rec_, name.c_str(), data) != BSON_OK)
    {
        TSW_THROW(BsonException, "Can't append field \"" + name + "\" as a boolean \"" + std::to_string(data) + "\" into a BSON document");
    }
}


void BsonSerializerImpl::append_field(const String &name, const Value &data)
{
    init(bs_rec_.finished);
    std::visit([&](auto &&element)
    {
        using T = std::decay_t<decltype(element)>;
        if constexpr (std::is_same_v<T, Null>)
        {
            this->append_null_field(name);
        }
        else if constexpr (std::is_same_v<T, Undefined>)
        {
            this->append_undefined_field(name);
        }
        else
        {
            this->append_field(name, element);
        }
    }, static_cast<const ValueBase&>(data));
}


void BsonSerializerImpl::append_field(const String &name, const NameValueMap &data)
{
    init(bs_rec_.finished);

    if (bson_append_start_object2(&bs_rec_, name.c_str(), name.size()) != BSON_OK)
    {
        TSW_THROW(BsonException, "Can't start appending field \"" + name + "\" as a NameValueMap into a BSON document");
    }

    for (const auto field: data) append_field(field.first, field.second);

    if (bson_append_finish_object(&bs_rec_) != BSON_OK)
    {
        TSW_THROW(BsonException, "Can't finish appending field \"" + name + "\" as a NameValueMap into a BSON document");
    }
}


void BsonSerializerImpl::append_field(const String &name, const BinData &data)
{
    init(bs_rec_.finished);

    if (bson_append_binary(&bs_rec_, name.c_str(), BSON_BIN_BINARY,
                           reinterpret_cast<const char*>(data.data()), data.size()) != BSON_OK)
    {
        TSW_THROW(BsonException, "Can't append field \"" + name + "\" as a BinaryData into a BSON document");
    }
}


template<typename IterableType, typename std::enable_if<is_iterable<IterableType>::value, int>::type>
void BsonSerializerImpl::append_field(const String &name, const IterableType &data)
{
    init(bs_rec_.finished);

    if (bson_append_start_array2(&bs_rec_, name.c_str(), name.size()) != BSON_OK)
    {
        TSW_THROW(BsonException, "Can't start appending field \"" + name + "\" as an iterable type into a BSON document");
    }

    size_t field_num = 0;
    for (const auto field: data)
    {
        append_field(std::to_string(field_num++), field);
        TSW_ASSERT(field_num < std::numeric_limits<size_t>::max());
    }

    if (bson_append_finish_array(&bs_rec_) != BSON_OK)
    {
        TSW_THROW(BsonException, "Can't finish appending field \"" + name + "\" as an iterable type into a BSON document");
    }
}


void BsonSerializerImpl::append_field(const String &name, const Time &data)
{
    init(bs_rec_.finished);

    TSW_ASSERT(sizeof(Time) <= sizeof(bson_date_t));
    if (bson_append_date(&bs_rec_, name.c_str(), bson_date_t(data.count())) != BSON_OK)
    {
        TSW_THROW(BsonException, "Can't append field \"" + name + "\" as Time \"" + std::to_string(data.count()) + "\" into a BSON document");
    }
}


void BsonSerializerImpl::append_uid(const String &name, UID data, size_t size)
{
    init(bs_rec_.finished);

    if (bson_append_binary(&bs_rec_, name.c_str(), BSON_BIN_UUID, reinterpret_cast<const char*>(&data), size) != BSON_OK)
    {
        TSW_THROW(BsonException, "Can't append field \"" + name + "\" as an UID \"" + std::to_string(data) + "\" into a BSON document");
    }
}


void BsonSerializerImpl::append_null_field(const String &name)
{
    init(bs_rec_.finished);

    if (bson_append_null(&bs_rec_, name.c_str()) != BSON_OK)
    {
        TSW_THROW(BsonException, "Can't append Null field \"" + name + "\" into a BSON document");
    }
}


void BsonSerializerImpl::append_undefined_field(const String &name)
{
    init(bs_rec_.finished);

    if (bson_append_undefined(&bs_rec_, name.c_str()) != BSON_OK)
    {
        TSW_THROW(BsonException, "Can't append Undefined field \"" + name + "\" into a BSON document");
    }
}


BinData BsonSerializerImpl::get_buffer()
{
    const char* buffer;
    int buffer_size;

    bson_finish(&bs_rec_);

    if (bson_check_duplicate_keys(&bs_rec_))
    {
        // Heavy operation.
        bson bs_out;

        bson_safe_init(&bs_out);
        bson_fix_duplicate_keys(&bs_rec_, &bs_out);
        bson_finish(&bs_out);

        buffer = bson_data2(&bs_out, &buffer_size);
    }
    else buffer = bson_data2(&bs_rec_, &buffer_size);

    TSW_ASSERT(buffer_size > 0);
    TSW_ASSERT(sizeof(buffer_size) <= sizeof(size_t));

    return BinData(buffer, buffer + buffer_size);
}


void BsonSerializerImpl::print_buffer()
{
    bson_print_raw(reinterpret_cast<const char*>(get_buffer().data()), 0);
}


void BsonSerializerImpl::clear()
{
    bson_destroy(&bs_rec_);
    init();
}


void BsonSerializerImpl::append_field(const String &name, const ValueArray &data)
{
    append_field<ValueArray>(name, data);
}


void BsonSerializerImpl::append_field(const String &name, const ValueList &data)
{
    append_field<ValueList>(name, data);
}


//----------------------------------------------------------------------------
// BsonDeserializerImpl
//----------------------------------------------------------------------------

BsonDeserializerImpl::BsonDeserializerImpl()
{
    set_bson_err_handler(&bson_error_handler);
    //init();
}


BsonDeserializerImpl::~BsonDeserializerImpl()
{

}


NameValueMap BsonDeserializerImpl::DeserializeInternal(const BinData::value_type *bin_data, size_t size)
{
    NameValueMap result;
    bson_iterator i;
    bson_type bson_iter_type;

    const char *key;
    //bson_timestamp_t ts;
    //char oidhex[25];
    if (static_cast<size_t>(bson_size2(bin_data)) != size)
        TSW_THROW(BsonException, (std::to_string(bson_size2(bin_data)) + " != " + std::to_string(size)).c_str());

    BSON_ITERATOR_FROM_BUFFER(&i, bin_data);

    while (bson_iterator_next(&i))
    {
        bson_iter_type = BSON_ITERATOR_TYPE(&i);
        if (bson_iter_type == 0) break;

        key = BSON_ITERATOR_KEY(&i);

        switch (bson_iter_type)
        {
            case BSON_DOUBLE:
                result[key] = bson_iterator_double(&i);
            break;
            case BSON_STRING:
            case BSON_SYMBOL:
                result[key] = bson_iterator_string(&i);
            break;
            case BSON_REGEX:
                result[key] = bson_iterator_string(&i);
            break;
            case BSON_BOOL:
                result[key] = bson_iterator_bool(&i) == true;
            break;
            case BSON_DATE:
                // bson_date_t is a milliseconds since epoch UTC.
                // Time is a nanoseconds, but I write Time type and need to return nanoseconds.
                result[key] = std::move(Time(bson_iterator_date(&i)));
            break;
            case BSON_BINDATA:
            {
                int bd_len = bson_iterator_bin_len(&i);
                const char *bd_buffer = bson_iterator_bin_data(&i);
                result[key] = std::move(BinData(bd_buffer, bd_buffer + bd_len));
                break;
            }
            case BSON_UNDEFINED:
                result[key] = Value();
            break;
            case BSON_NULL:
                result[key] = Null();
            break;
            case BSON_INT:
                result[key] = static_cast<int32_t>(bson_iterator_int(&i));
            break;
            case BSON_LONG:
                result[key] = bson_iterator_long(&i);
            break;
            case BSON_OBJECT:
            {
                auto bi_val = bson_iterator_value(&i);
                result[key] = std::move(DeserializeInternal(reinterpret_cast<const BinData::value_type*>(bi_val), bson_size2(bi_val)));
                break;
            }
            case BSON_ARRAY:
            {
                auto bi_val = bson_iterator_value(&i);
                NameValueMap nvm = std::move(DeserializeInternal(reinterpret_cast<const BinData::value_type*>(bi_val), bson_size2(bi_val)));
                ValueArray va;

                va.clear();
                for (auto i: nvm) va.push_back(std::move(i.second));

                result[key] = std::move(va);
                break;
            }
            case BSON_CODE:
                result[key] = bson_iterator_code(&i);
            break;
            case BSON_CODEWSCOPE:
                // This will work, but need too use "bson_iterator_code_scope".
                result[key] = bson_iterator_code(&i);
            break;
            case BSON_OID:
            {
                auto data = reinterpret_cast<const BinData::value_type *>(bson_iterator_oid(&i));
                result[key] = BinData(data, data + sizeof(bson_oid_t));
                break;
            }
            case BSON_TIMESTAMP:
            {
                auto bi_val = bson_iterator_timestamp(&i);
                auto seconds = std::chrono::seconds(bi_val.t);
                // It's not fully correct: i is not milliseconds, it's "operation ordinal".
                result[key] = seconds + std::chrono::milliseconds(bi_val.i);
            }
            default:
                if (raise_on_unknown_) TSW_THROW(BsonException, String("can't deserialize field \"") +
                                                 key + "\" with type: " + std::to_string(bson_iter_type));
            break;
        } // switch
    } // while

    return result;
}

} // namespace impl

} // namespace tsw
