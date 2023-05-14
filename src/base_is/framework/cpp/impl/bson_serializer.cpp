/**
  * @file bson_serializer.cpp
  * @author Artiom N.(cl)2017
  * @brief BsonSerializer class implementation.
  *
  */

#include <tsw/bson_serializer.h>
#include <tsw/types.h>

#include "bson_impl.h"


namespace tsw
{

BsonSerializer::BsonSerializer() : bsrec_(new impl::BsonSerializerImpl)
{}


BsonSerializer::~BsonSerializer()
{}


BinData BsonSerializer::SerializeMessage(const Message &message)
{
    return std::move(Serializer::SerializeMessage<impl::BsonSerializerImpl>(message, *bsrec_));
}


BinData BsonSerializer::SerializeObject(const Object &fields)
{
    bsrec_->clear();

    for (const auto field: fields) bsrec_->append_field(field.first, field.second);

    return bsrec_->get_buffer();
}


} // namespace tsw
