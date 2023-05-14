/**
  * @file types.h
  * @author Artiom N.(cl)2017
  * @brief General TSW types definitions, Value class definition.
  *
  */

#pragma once

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <ctime>
#include <filesystem>
#include <list>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>
#include <variant>

//#include <boost/optional.hpp>
//#include <boost/variant.hpp>

#include <tsw/type_traits.h>

#if defined(_WIN32) || defined(_WIN64)
#   if _WIN64
#       define ENVIRONMENT64
#   else
#       define ENVIRONMENT32
#   endif
#endif

// Check GCC
#if __GNUC__
#   if __x86_64__ || __ppc64__
#       define ENVIRONMENT64
#   else
#       define ENVIRONMENT32
#   endif
#endif

#if defined(__linux) || defined(__linux__) || defined(linux)
#  define LINUX
#elif defined(__APPLE__)
#  define MACOS
#elif defined(_WIN32) || defined(__WIN32__) || defined(WIN32) || defined(_WIN64)
#  define WINDOWS
#endif


namespace tsw
{

/// Value forward declaration.
class Value;

typedef std::string                 String;
typedef std::vector<uint8_t>        BinData;
typedef String                      URL;
typedef uint64_t                    UID;
// Nanoseconds for the compartibility with STL.
typedef std::chrono::nanoseconds    Time;

typedef std::vector<Value>          ValueArray;
typedef std::vector<String>         StringArray;
//typedef std::pair<String, Value>    NameValue;

typedef std::list<Value>            ValueList;
typedef std::list<String>           StringList;
typedef std::map<String, String>    StringStringMap;
typedef std::map<String, Value>     NameValueMap;
typedef NameValueMap                Object;


using std::monostate;

typedef monostate Undefined;
TSW_STRONG_TYPEDEF(monostate, Null)


enum class ValueType
{
    Undefined, Null, Array, BinData, Boolean, DoubleNumber, Int32Number, Int64Number, String, Time, Object
};

// Types ordnung muss to be same with a ValueType ordnung.
// Undefined need to be first: a default-constructed Value is undefined.
/// Base for the Value class
typedef std::variant<Undefined, Null, ValueArray, BinData, bool, double, int32_t, int64_t, String, Time, Object> ValueBase;


/**
 * @brief The Value class, implements common framework value.
 *
 * This class is a container, which can store multiple values, including Values containers.
 *
 * @note
 * Class based on a variant class. It may be either boost::variant or std::variant in C++17 and higher.
 */
class Value : public ValueBase
{
public:
    using ValueBase::ValueBase;
    Value() = default;
    Value(const String::value_type *v) : ValueBase(String(v)) {}

public:
    bool is_array() const { return value_type() == ValueType::Array; }
    bool is_bool() const { return value_type() == ValueType::Boolean; }
    bool is_bindata() const { return value_type() == ValueType::BinData; }
    bool is_double() const { return value_type() == ValueType::DoubleNumber; }
    bool is_int32() const { return value_type() == ValueType::Int32Number; }
    bool is_int64() const { return value_type() == ValueType::Int64Number; }
    bool is_null() const { return value_type() == ValueType::Null; }
    bool is_object() const { return value_type() == ValueType::Object; }
    bool is_string() const { return value_type() == ValueType::String; }
    bool is_time() const { return value_type() == ValueType::Time; }
    bool is_undefined() const { return value_type() == ValueType::Undefined; }

public:
    bool                as_bool() const  { return as<bool>(); }
    BinData             &as_bindata()  { return as<BinData>(); }
    const BinData       &as_bindata() const { return as<BinData>(); }
    double              as_double() const  { return as<double>(); }
    int32_t             as_int32() const  { return as<int32_t>(); }
    int64_t             as_int64() const  { return as<int64_t>(); }
    Object              &as_object()  { return as<Object>(); }
    const Object        &as_object() const { return as<Object>(); }
    String              &as_string() { return as<String>(); }
    const String        &as_string() const { return as<String>(); }
    Time                &as_time()  { return as<Time>(); }
    const Time          &as_time() const { return as<Time>(); }
    ValueArray          &as_array()  { return as<ValueArray>(); }
    const ValueArray    &as_array() const  { return as<ValueArray>(); }

public:
    ValueType     value_type() const  { return static_cast<ValueType>(index()); }

public:
    template <typename T>
    const T& as() const { return std::get<T>(*this); }

    template <typename T>
    T& as() { return std::get<T>(*this); }

    template <typename T>
    const T& as(const T& default_value) const { return as_optional<T>().value_or(default_value); }

    template <typename T>
    T& as(const T& default_value) { return as_optional<T>().value_or(default_value); }

    template <typename T> std::optional<T> as_optional()
    {
        return std::holds_alternative<T>(*this) ? std::make_optional(as<T>()) : std::optional<T>();
    }

private:
    // Force compile error, prevent Variant(bool) to be called
    //Value(void *) = delete;
};


typedef std::filesystem::path Path;
const String path_delimiter = "/";
Path get_user_home();

} // namespace tsw
