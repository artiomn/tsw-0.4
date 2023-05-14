/**
  * @file json_serializer.h
  * @author Artiom N.(cl)2017
  * @brief JsonSerializer class definition.
  *
  */

#ifndef _TSW_JSON_SERIALIZER_H
#define _TSW_JSON_SERIALIZER_H

#include <memory>

#include "message.h"
#include "serializer.h"
#include "types.h"


namespace tsw
{

namespace impl
{
struct JsonSerializerImpl;
}

class JsonSerializer: public Serializer
{
    JsonSerializer();
    ~JsonSerializer();

public:
    BinData SerializeMessage(const Message &message) override;
    BinData SerializeObject(const Object &fields) override;

private:
    std::unique_ptr<impl::JsonSerializerImpl> jsrec_;
};

} // namespace tsw

#endif // _TSW_JSON_SERIALIZER_H
