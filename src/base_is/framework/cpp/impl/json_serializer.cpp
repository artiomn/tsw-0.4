/**
  * @file json_serializer.cpp
  * @author Artiom N.(cl)2017
  * @brief JsonSerializer class implementation.
  *
  */

#include <tsw/json_serializer.h>
#include <tsw/types.h>

#include "json_impl.h"


namespace tsw
{


JsonSerializer::JsonSerializer() : jsrec_(new impl::JsonSerializerImpl)
{}


JsonSerializer::~JsonSerializer()
{}


BinData JsonSerializer::SerializeMessage(const Message &message)
{
    return std::move(Serializer::SerializeMessage<impl::JsonSerializerImpl>(message, *jsrec_));
}


BinData JsonSerializer::SerializeObject(const Object &fields)
{
    jsrec_->clear();

    for (const auto field: fields) jsrec_->append_field(field.first, field.second);

    return jsrec_->get_buffer();
}

} // namespace tsw
