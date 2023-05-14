/**
  * @file bson_deserializer.cpp
  * @author Artiom N.(cl)2017
  * @brief BSON deserializer implementation.
  *
  */

#include <tsw/bson_deserializer.h>
#include <tsw/types.h>

#include "bson_impl.h"


namespace tsw
{

BsonDeserializer::BsonDeserializer() : bsrec_(new impl::BsonDeserializerImpl)
{}


BsonDeserializer::~BsonDeserializer()
{}


Message BsonDeserializer::DeserializeMessage(const BinData &data)
{
    return DeserializeMessage(data.data(), data.size());
}


Object BsonDeserializer::DeserializeObject(const BinData &data)
{
    return bsrec_->Deserialize(data.data(), data.size());
}


Message BsonDeserializer::DeserializeMessage(const BinData::value_type *data, size_t size)
{
    auto obj = bsrec_->Deserialize(data, size);

    return MessageFromObject(std::move(obj));
}


Object BsonDeserializer::DeserializeObject(const BinData::value_type *data, size_t size)
{
    return bsrec_->Deserialize(data, size);
}

} // namespace tsw
