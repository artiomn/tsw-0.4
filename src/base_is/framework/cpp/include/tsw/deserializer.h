/**
  * @file deserializer.h
  * @author Artiom N.(cl)2017
  * @brief Common deserializer interface.
  *
  */

#ifndef _TSW_DESERIALIZER_H
#define _TSW_DESERIALIZER_H

#include "message.h"
#include "types.h"


namespace tsw
{

class Deserializer
{
public:
    virtual Message DeserializeMessage(const BinData &data) = 0;
    virtual Object DeserializeObject(const BinData &data) = 0;
    virtual Message DeserializeMessage(const BinData::value_type *data, size_t size) = 0;
    virtual Object DeserializeObject(const BinData::value_type *data, size_t size) = 0;

public:
    virtual ~Deserializer() = default;

protected:
    Message MessageFromObject(Object &&obj);
};

} // namespace tsw

#endif // _TSW_DESERIALIZER_H
