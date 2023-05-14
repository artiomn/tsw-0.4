/**
  * @file json_serializer.cpp
  * @author Artiom N.(cl)2017
  * @brief JsonSerializer class implementation.
  *
  */

#include <tsw/json_deserializer.h>
#include <tsw/types.h>

#include "json_impl.h"


namespace tsw
{

JsonDeserializer::JsonDeserializer() : jsrec_(new impl::JsonDeserializerImpl)
{}


JsonDeserializer::~JsonDeserializer()
{}


Message JsonDeserializer::DeserializeMessage(const BinData &data)
{
    return DeserializeMessage(data.data(), data.size());
}


Object JsonDeserializer::DeserializeObject(const BinData &data)
{
    return jsrec_->Deserialize(data.data(), data.size());
}


Message JsonDeserializer::DeserializeMessage(const BinData::value_type *data, size_t size)
{
    auto obj = jsrec_->Deserialize(data, size);

    return MessageFromObject(std::move(obj));
}


Object JsonDeserializer::DeserializeObject(const BinData::value_type *data, size_t size)
{
    return jsrec_->Deserialize(data, size);
}

} // namespace tsw
