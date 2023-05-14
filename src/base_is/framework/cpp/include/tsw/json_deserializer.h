/**
  * @file json_deserializer.h
  * @author Artiom N.(cl)2017
  * @brief JsonDeserializer class definition.
  *
  */

#ifndef _TSW_JSONDESERIALIZER_H
#define _TSW_JSONDESERIALIZER_H

#include "deserializer.h"
#include "types.h"


namespace tsw
{

namespace impl
{
struct JsonDeserializerImpl;
}

class JsonDeserializer : public Deserializer
{
public:
    JsonDeserializer();
    ~JsonDeserializer();

public:
    Message DeserializeMessage(const BinData &data) override;
    Object DeserializeObject(const BinData &data) override;
    Message DeserializeMessage(const BinData::value_type *data, size_t size) override;
    Object DeserializeObject(const BinData::value_type *data, size_t size) override;

private:
    std::unique_ptr<impl::JsonDeserializerImpl> jsrec_;
};

} // namespace tsw

#endif // _TSW_JSONDESERIALIZER_H
