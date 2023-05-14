/**
  * @file binary_message.h
  * @author Artiom N.(cl)2017
  * @brief BinaryMessage class definition.
  *
  */

#ifndef _TSW_BINARY_MESSAGE_H
#define _TSW_BINARY_MESSAGE_H

#include "tsw/types.h"

namespace tsw
{

class BinaryMessage: public Message
{
public:
    BinaryMessage(const MessageType message_type, NameValueMap&& fields,
                  const UID receiver_module_uid, String&& receiver_module_name,
                  ModuleClass&& receiver_module_class,
                  const UID sender_module_uid, String&& sender_module_name,
                  ModuleClass&& sender_module_class,
                  Time&& creation_time) :
        Message(message_type, fields, receiver_module_uid, receiver_module_name, receiver_module_class,
                sender_module_uid, sender_module_name, sender_module_class, creation_time)
    {}
    BinaryMessage(const MessageType message_type, const NameValueMap& fields,
                  const UID receiver_module_uid, const String& receiver_module_name,
                  const ModuleClass& receiver_module_class,
                  const UID sender_module_uid, const String& sender_module_name,
                  const ModuleClass& sender_module_class,
                  const Time& creation_time) :
        Message(message_type, fields, receiver_module_uid, receiver_module_name, receiver_module_class,
                sender_module_uid, sender_module_name, sender_module_class, creation_time)
    {}
};

} // namespace tsw

#endif // _TSW_BINARY_MESSAGE_H
