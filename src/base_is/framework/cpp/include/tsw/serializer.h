/**
  * @file serializer.h
  * @author Artiom N.(cl)2017
  * @brief Common serializer interface.
  *
  */

#ifndef _TSW_SERIALIZER_H
#define _TSW_SERIALIZER_H

#include "message.h"
#include "types.h"


namespace tsw
{

class Serializer
{
public:
    virtual BinData SerializeMessage(const Message &message) = 0;
    virtual BinData SerializeObject(const Object &object) = 0;

public:
    virtual ~Serializer() = default;

protected:
    template<typename Serializer>
    BinData SerializeMessage(const Message &message, Serializer &serializer)
    {
        serializer.clear();

        serializer.append_field("type", static_cast<int>(message.get_type()));
        serializer.append_field("sender_m_uid", static_cast<int64_t>(message.get_sender_module_uid()));
        serializer.append_field("sender_m_name", message.get_sender_module_name());
        serializer.append_field("sender_m_class", message.get_sender_module_class());
        serializer.append_field("receiver_m_uid", static_cast<int64_t>(message.get_sender_module_uid()));
        serializer.append_field("receiver_m_name", message.get_sender_module_name());
        serializer.append_field("receiver_m_class", message.get_sender_module_class());
        serializer.append_field("c_time", message.get_creation_time());
        serializer.append_field("fields", message.get_fields());

        return serializer.get_buffer();
    }


};

} // namespace tsw

#endif // _TSW_SERIALIZER_H
