/**
  * @file bson_serializer.h
  * @author Artiom N.(cl)2017
  * @brief BsonSerializer class definition.
  *
  */

#ifndef _TSW_BSON_SERIALIZER_H
#define _TSW_BSON_SERIALIZER_H

#include <memory>

#include "message.h"
#include "serializer.h"
#include "types.h"


namespace tsw
{

namespace impl
{
struct BsonSerializerImpl;
}

class BsonSerializer: public Serializer
{
public:
    BsonSerializer();
    ~BsonSerializer();

public:
    BinData SerializeMessage(const Message &message) override;
    BinData SerializeObject(const Object &fields) override;

private:
    std::unique_ptr<impl::BsonSerializerImpl> bsrec_;
};

} // namespace tsw

#endif // _TSW_BSON_SERIALIZER_H
