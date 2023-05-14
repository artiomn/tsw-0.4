/**
  * @file deserializer.cpp
  * @author Artiom N.(cl)2017
  * @brief Deserializer implementation.
  *
  */

#include <tsw/deserializer.h>
#include <tsw/types.h>

#include "binary_message.h"


namespace tsw
{

Message Deserializer::MessageFromObject(Object &&obj)
{
    return BinaryMessage(
                std::move(static_cast<MessageType>(obj["type"].as_int32())),
                std::move(obj["fields"].as_object()),
                std::move(static_cast<UID>(obj["receiver_m_uid"].as_int64())),
                std::move(obj["receiver_m_name"].as_string()),
                std::move(obj["receiver_m_class"].as_string()),
                std::move(static_cast<UID>(obj["sender_m_uid"].as_int64())),
                std::move(obj["sender_m_name"].as_string()),
                std::move(obj["sender_m_class"].as_string()),
                std::move(obj["c_time"].as_time())
         );
}

} // namespace tsw
