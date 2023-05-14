/**
  * @file json_impl.cpp
  * @author Artiom N.(cl)2017
  * @brief JSON classes and routines implementation.
  *
  */

#include <chrono>
#include <ctime>
#include <iomanip>
#include <limits>
#include <memory>
#include <sstream>
#include <string>
#include <type_traits>

#include "tsw/error.h"
#include "tsw/base64.h"

#include "json_impl.h"


namespace tsw
{

namespace impl
{

class JsonException: public Exception
{
public:
    using Exception::Exception;
    JsonException(int code, int offset) :
        Exception(String("Parse failed with code ") + std::to_string(code) + ", on offset: " + std::to_string(offset)) {}
    JsonException(const char *file, int line) :
        Exception("Json exception: " + std::string(file) + ":" + std::to_string(line)) {}
};


//----------------------------------------------------------------------------
// JsonSerializerImpl
//----------------------------------------------------------------------------

JsonSerializerImpl::JsonSerializerImpl() : need_init_(false), writer_(sb_)
{
    writer_.StartObject();
}


JsonSerializerImpl::~JsonSerializerImpl()
{
}


void JsonSerializerImpl::write_name(const String &name)
{
#if RAPIDJSON_HAS_STDSTRING
    writer_.String(name);
#else
    writer_.String(name.c_str(), static_cast<rj::SizeType>(name.length()));
#endif
}

void JsonSerializerImpl::write_field(const String &data)
{
#if RAPIDJSON_HAS_STDSTRING
    writer_.String(data);
#else
    writer_.String(data.c_str(), static_cast<rj::SizeType>(data.length()));
#endif
}


void JsonSerializerImpl::write_field(const String::value_type *data)
{
    writer_.String(data);
}


void JsonSerializerImpl::write_field(int data)
{
    writer_.Int(data);
}


void JsonSerializerImpl::write_field(int64_t data)
{
    writer_.Int64(data);
}


void JsonSerializerImpl::write_field(double data)
{
    writer_.Double(data);
}


void JsonSerializerImpl::write_field(bool data)
{
    writer_.Bool(data);
}


void JsonSerializerImpl::write_field(const NameValueMap &data)
{
    writer_.StartObject();
    for (const auto field: data) append_field(field.first, field.second);
    writer_.EndObject();
}


void JsonSerializerImpl::write_field(const BinData &data)
{
    write_field(std::move(to_base64_no_lb(data)));
}


void JsonSerializerImpl::write_field(Time data)
{
    std::stringstream ss;

    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(data);
    auto ms = (data - seconds).count();
    auto tm = seconds.count();

    ss << std::put_time(std::gmtime(&tm), datetime_format) << "." << ms << "Z";
    write_field(ss.str());
}


void JsonSerializerImpl::write_field(const Value &data)
{
    std::visit([&](auto &&element)
    {
        using T = std::decay_t<decltype(element)>;
        if constexpr (std::is_same_v<T, Null>)
        {
            this->write_null_field();
        }
        else if constexpr (std::is_same_v<T, Undefined>)
        {
            this->write_undefined_field();
        }
        else
        {
            this->write_field(element);
        }
    }, static_cast<const ValueBase&>(data));
}


template<typename IterableType, typename std::enable_if<is_iterable<IterableType>::value, int>::type>
inline void JsonSerializerImpl::write_field(const IterableType &data)
{
    writer_.StartArray();
    size_t field_num = 0;
    for (const auto& field: data)
    {
        write_field(field);
        TSW_ASSERT(field_num < std::numeric_limits<size_t>::max());
    }
    writer_.EndArray();
}


void JsonSerializerImpl::write_uid(UID data, size_t size)
{
    writer_.Int64(data);
}


void JsonSerializerImpl::write_null_field()
{
    writer_.Null();
}


void JsonSerializerImpl::write_undefined_field()
{
    writer_.Null();
}


void JsonSerializerImpl::append_field(const String &name, const String &data)
{
    append_field<const String&>(name, data);
}


void JsonSerializerImpl::append_field(const String &name, const String::value_type *data)
{
    append_field<const String::value_type *>(name, data);
}


void JsonSerializerImpl::append_field(const String &name, int data)
{
    append_field<int>(name, data);
}


void JsonSerializerImpl::append_field(const String &name, int64_t data)
{
    append_field<int64_t>(name, data);
}


void JsonSerializerImpl::append_field(const String &name, double data)
{
    append_field<double>(name, data);
}


void JsonSerializerImpl::append_field(const String &name, bool data)
{
    append_field<bool>(name, data);
}


void JsonSerializerImpl::append_field(const String &name, const Value &data)
{
    append_field<const Value&>(name, data);
}


void JsonSerializerImpl::append_field(const String &name, const NameValueMap &data)
{
    append_field<const NameValueMap&>(name, data);
}


void JsonSerializerImpl::append_field(const String &name, const BinData &data)
{
    append_field<const BinData&>(name, data);
}


void JsonSerializerImpl::append_field(const String &name, const Time &data)
{
    append_field<Time>(name, data);
}


void JsonSerializerImpl::append_uid(const String &name, UID data, size_t size)
{
    write_name(name);
    writer_.Int64(data);
}


void JsonSerializerImpl::append_null_field(const String &name)
{
    write_name(name);
    write_null_field();
}


void JsonSerializerImpl::append_undefined_field(const String &name)
{
    write_name(name);
    write_undefined_field();
}


template<typename T>
void JsonSerializerImpl::append_field(const String &name, const T data)
{
    if (need_init_) clear();
    write_name(name);
    write_field(data);
}


BinData JsonSerializerImpl::get_buffer()
{
    if (!writer_.IsComplete())
    {
        writer_.EndObject();
        need_init_ = true;
    }
    const char *data = sb_.GetString();
    return BinData(data, data + sb_.GetLength());
}


void JsonSerializerImpl::clear()
{
    sb_.Clear();
    writer_.Reset(sb_);
    writer_.StartObject();
    need_init_ = false;
}


void JsonSerializerImpl::append_field(const String &name, const ValueArray &data)
{
    append_field<ValueArray>(name, data);
}


void JsonSerializerImpl::append_field(const String &name, const ValueList &data)
{
    append_field<ValueList>(name, data);
}


//----------------------------------------------------------------------------
// JsonDeserializerImpl
//----------------------------------------------------------------------------

// R"_(^P?(?:(\d+)[Y-]{1})?(?:(\d+)[M-]{1})?(?:(\d+)D?)?T(?:(\d+)[H:]{1})?(?:(\d+)[M:]{1})?(?:(\d+)S?)?(?:\.(\d+))?Z?$)_"
// All groups are mandatory.
const std::regex JsonDeserializerImpl::iso8601_check_regex_(R"_(^P?(?:(\d+)[Y-]{1})(?:(\d+)[M-]{1})(?:(\d+)D?)T(?:(\d+)[H:]{1})(?:(\d+)[M:]{1})(?:(\d+)S?)(?:\.(\d+))Z$)_");

enum class iso8601_regex_group
{
    year = 1,
    month = 2,
    day = 3,
    hour = 4,
    minute = 5,
    second = 6,
    fraction = 7
};


JsonDeserializerImpl::JsonDeserializerImpl()
{
}


JsonDeserializerImpl::~JsonDeserializerImpl()
{
}


NameValueMap JsonDeserializerImpl::Deserialize(const BinData::value_type* bin_data, size_t size, bool raise_on_unknown)
{
    NameValueMap result;
    rj::Document doc;

    raise_on_unknown_ = raise_on_unknown;

    rj::ParseResult parse_result = doc.Parse<rj::kParseStopWhenDoneFlag>(reinterpret_cast<const char*>(bin_data), size);

    if (!parse_result) TSW_THROW(JsonException, parse_result.Code(), parse_result.Offset());

    for (rj::Value::ConstMemberIterator iter = doc.MemberBegin(); iter != doc.MemberEnd(); ++iter)
    {
        const rj::Value &v = iter->value;
        const String &name = iter->name.GetString();

        cur_value_name_ = &name;
        result[name] = std::move(ParseValue(v));
    }

    return result;
}


Value JsonDeserializerImpl::ParseValue(const rj::Value &value)
{
    switch (value.GetType())
    {
        case rj::Type::kNumberType:
            if (value.IsInt()) return value.GetInt();
            else if (value.IsInt64()) return value.GetInt64();
            else if (value.IsDouble() || value.IsLosslessDouble()) return value.GetDouble();
        break;
        case rj::Type::kStringType:
        {
            const String &s = value.GetString();
            std::smatch matches;

            if (is_base64(s)) return std::move(from_base64(s));
            else if (std::regex_search(s, matches, iso8601_check_regex_))
            {
                // Date/time.
                std::tm timeinfo;
                Time frac;
                memset(&timeinfo, 0, sizeof(tm));

                for (size_t i = 0; i < matches.size(); ++i )
                {
                    const auto &mi = matches[i];
                    if (!mi.matched) continue;
                    switch (static_cast<const iso8601_regex_group>(i))
                    {
                    case iso8601_regex_group::year:
                    {
                        timeinfo.tm_year = stoi(mi.str()) - 1900;
                        break;
                    }
                    case iso8601_regex_group::month:
                    {
                        timeinfo.tm_mon = stoi(mi.str()) - 1;
                        break;
                    }
                    case iso8601_regex_group::day:
                    {
                        timeinfo.tm_mday = stoi(mi.str());
                        break;
                    }
                    case iso8601_regex_group::hour:
                    {
                        timeinfo.tm_hour = stoi(mi.str());
                        break;
                    }
                    case iso8601_regex_group::minute:
                    {
                        timeinfo.tm_min = stoi(mi.str());
                        break;
                    }
                    case iso8601_regex_group::second:
                    {
                        timeinfo.tm_sec = stoi(mi.str());
                        break;
                    }
                    case iso8601_regex_group::fraction:
                    {
                        frac = Time(stoi(mi.str()));
                        break;
                    }
                    default:
                    {
                        // TODO: warning.
                        break;
                    }
                    }
                }

                timeinfo.tm_isdst = -1;
                // _mkgmtime on win
                auto tp = std::chrono::high_resolution_clock::from_time_t(timegm(&timeinfo)) + frac;
                return std::chrono::duration_cast<Time>(tp.time_since_epoch());
            }
            else return std::move(value.GetString());
            break;
        }
        case rj::Type::kArrayType:
        {
            ValueArray va;
            for (const auto& v : value.GetArray())
                va.push_back(std::move(ParseValue(v)));
            return va;
        }
        case rj::Type::kObjectType:
        {
//            for (const auto& v : value.GetObject())
//                nmv[v.name.GetString()] = ParseValue(v);
            NameValueMap nmv;
            const rj::Value::ConstObject &co = value.GetObject();
            for (rj::Value::ConstMemberIterator iter = co.MemberBegin(); iter != co.MemberEnd(); ++iter)
                nmv[iter->name.GetString()] = std::move(ParseValue(iter->value));
            return nmv;
        }
        case rj::Type::kNullType:
            return Null();
        break;
        case rj::Type::kTrueType:
        case rj::Type::kFalseType:
            return value.GetBool();
        break;
        default:
            if (raise_on_unknown_) TSW_THROW(JsonException, String("Can't deserialize field \"") +
                                             *cur_value_name_ + "\" with a type: " +
                                             std::to_string(value.GetType()));
        ;
    }

    return Value();
}

} // namespace impl

} // namespace tsw
