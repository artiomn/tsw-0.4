/**
  * @file base64.h
  * @author Artiom N.(cl)2017
  * @brief BASE64 conversion routines.
  *
  */

#ifndef _TSW_BASE64_H
#define _TSW_BASE64_H

#include "types.h"

namespace tsw
{

const BinData::value_type base64_prefix[] = {'B', 'A', 'S', 'E', '6', '4', ':', '\n'};

bool is_base64(const BinData::value_type* data, size_t size);
inline bool is_base64(const BinData &data) { return is_base64(data.data(), data.size()); }
inline bool is_base64(const String &data) { return is_base64(reinterpret_cast<const BinData::value_type*>(data.c_str()), data.size()); }

String to_base64(const BinData::value_type* data, size_t size);
String to_base64_no_lb(const BinData::value_type* data, size_t size);

inline String to_base64(const BinData &data) { return to_base64(data.data(), data.size()); }
inline String to_base64(const String &data) { return to_base64(reinterpret_cast<const BinData::value_type*>(data.c_str()), data.size()); }

inline String to_base64_no_lb(const BinData &data) { return to_base64_no_lb(data.data(), data.size()); }
inline String to_base64_no_lb(const String &data) { return to_base64_no_lb(reinterpret_cast<const BinData::value_type*>(data.c_str()), data.size()); }

BinData from_base64(const char* data, size_t size);
inline BinData from_base64(const BinData::value_type* data, size_t size) { return from_base64(reinterpret_cast<const char*>(data), size); }
inline BinData from_base64(const BinData &data) { return from_base64(reinterpret_cast<const char*>(data.data()), data.size()); }
inline BinData from_base64(const String &data) { return from_base64(data.c_str(), data.size()); }

}

#endif // _TSW_BASE64_H
