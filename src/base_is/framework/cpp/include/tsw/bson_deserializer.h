/**
  * @file bson_deserializer.h
  * @author Artiom N.(cl)2017
  * @brief BsonDeserializer class definition.
  *
  */

#ifndef _TSW_BSONDESERIALIZER_H
#define _TSW_BSONDESERIALIZER_H

#include <memory>

#include "deserializer.h"
#include "types.h"


namespace tsw
{

namespace impl
{
struct BsonDeserializerImpl;
}

class BsonDeserializer : public Deserializer
{
public:
    BsonDeserializer();
    ~BsonDeserializer();

public:
    Message DeserializeMessage(const BinData &data) override;
    Object DeserializeObject(const BinData &data) override;
    Message DeserializeMessage(const BinData::value_type *data, size_t size) override;
    Object DeserializeObject(const BinData::value_type *data, size_t size) override;

private:
    std::unique_ptr<impl::BsonDeserializerImpl> bsrec_;
};

} // namespace tsw

#endif // _TSW_BSONDESERIALIZER_H
