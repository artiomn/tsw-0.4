/**
  * @file base64.cpp
  * @author Artiom N.(cl)2017
  * @brief Base64 routines implementation.
  *
  */
#include <iostream>

#include <algorithm>
#include <cstring>
#include <iterator>
#include <regex>

#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/insert_linebreaks.hpp>
#include <boost/archive/iterators/remove_whitespace.hpp>
#include <boost/archive/iterators/transform_width.hpp>

#include <tsw/base64.h>
#include <tsw/error.h>
#include <tsw/types.h>


namespace tsw
{

static constexpr size_t line_break_length = 72;
// String length.
static constexpr size_t base64_prefix_size = sizeof(base64_prefix);
static const std::regex base64_regex(R"((?:[A-Za-z0-9+/]{4}|\n|(?:\r\n))*(?:[A-Za-z0-9+/]{2}==|[A-Za-z0-9+/]{3}=)?)",
                                     std::regex::ECMAScript );


bool is_base64(const uint8_t* data, size_t size)
{
    using namespace boost::archive::iterators;

    if (size < base64_prefix_size || memcmp(base64_prefix, data, base64_prefix_size)) return false;
    /*
    size_t spaces_count = 0;
    for (size_t i = 0; i < size; ++i)
    {
        if (data[i] == ' ') ++spaces_count;
    }

    if ((size - spaces_count - base64_prefix_size) % 4 != 0) return false;*/

    const auto *data_start = data + base64_prefix_size;
    const auto *data_end = data_start + size - base64_prefix_size;

    return std::regex_match(reinterpret_cast<const char*>(data_start),
                            reinterpret_cast<const char*>(data_end), base64_regex);
}


using namespace boost::archive::iterators;
// Convert binary values to base64 characters.
typedef base64_from_binary<transform_width<const tsw::BinData::value_type*, 6, 8>> base_64_text_without_lb;


template<typename Converter>
String to_base64_converter(const uint8_t* data, size_t size)
{
    String result(base64_prefix, base64_prefix + base64_prefix_size);

    std::copy(
       Converter(data),
       Converter(data + size),
       std::back_inserter(result)
    );

    return result;
}


inline void fill_result(size_t rs, String &result)
{
    while (rs++ % 4)
    {
        result.push_back('=');
    }
}


String to_base64(const uint8_t* data, size_t size)
{
    // Insert line breaks every line_break_length characters.
    typedef insert_linebreaks<base_64_text_without_lb, line_break_length> base64_text;

    String result = std::move(to_base64_converter<base64_text>(data, size));

    fill_result(result.size() - base64_prefix_size - result.size() / line_break_length, result);

    return std::move(result);
}


String to_base64_no_lb(const uint8_t* data, size_t size)
{
    String result = std::move(to_base64_converter<base_64_text_without_lb>(data, size));

    fill_result(result.size() - base64_prefix_size, result);

    return std::move(result);
}


BinData from_base64(const char* data, size_t size)
{
    using namespace boost::archive::iterators;
    BinData result;

    typedef transform_width<binary_from_base64<remove_whitespace<const char*>>, 8, 6> bin_base64;

    const char *data_end = static_cast<const char*>(memchr(data, '=', size));
    const char *data_start = ::memcmp(reinterpret_cast<const char*>(base64_prefix), data,
                                      std::min(base64_prefix_size, size)) == 0 ? data + base64_prefix_size : data;

    if (!data_end) data_end = data_start + size;

    std::copy(
       bin_base64(data_start),
       bin_base64(data_end),
       std::back_inserter(result)
    );

    return result;
}

} // namespace tsw
