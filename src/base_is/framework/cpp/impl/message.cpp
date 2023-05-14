/**
  * @file message.cpp
  * @author Artiom N.(cl)2017
  * @brief Message class implementation.
  *
  */

#include <chrono>
#include <utility>

#include <tsw/magistral.h>
#include <tsw/message.h>
#include <tsw/module.h>
#include <tsw/types.h>


namespace tsw
{

Message::Message(const MessageType message_type, NameValueMap&& fields) :
   message_type_(message_type),
   sender_module_uid_(-1), sender_module_name_(), sender_module_class_(),
   receiver_module_uid_(-1), receiver_module_name_(), receiver_module_class_(),
   creation_time_(std::chrono::system_clock::now().time_since_epoch()),
   fields_(std::move(fields))
{
}


Message::Message(const MessageType message_type, const NameValueMap& fields) :
   message_type_(message_type),
   sender_module_uid_(-1), sender_module_name_(), sender_module_class_(),
   receiver_module_uid_(-1), receiver_module_name_(), receiver_module_class_(),
   creation_time_(std::chrono::system_clock::now().time_since_epoch()),
   fields_(fields)
{
}


Message::Message(const MessageType message_type, NameValueMap&& fields,
                 const UID& receiver_module_uid) :
    message_type_(message_type),
    sender_module_uid_(-1), sender_module_name_(), sender_module_class_(),
    receiver_module_uid_(receiver_module_uid), receiver_module_name_(), receiver_module_class_(),
    creation_time_(std::chrono::system_clock::now().time_since_epoch()),
    fields_(std::move(fields))
{
}


Message::Message(const MessageType message_type, const NameValueMap& fields,
                 const UID& receiver_module_uid) :
    message_type_(message_type),
    sender_module_uid_(-1), sender_module_name_(), sender_module_class_(),
    receiver_module_uid_(receiver_module_uid), receiver_module_name_(), receiver_module_class_(),
    creation_time_(std::chrono::system_clock::now().time_since_epoch()),
    fields_(fields)
{
}


Message::Message(const MessageType message_type, NameValueMap&& fields,
                 const String& receiver_module_name) :
    message_type_(message_type),
    sender_module_uid_(-1), sender_module_name_(), sender_module_class_(),
    receiver_module_uid_(-1), receiver_module_name_(receiver_module_name), receiver_module_class_(),
    creation_time_(std::chrono::system_clock::now().time_since_epoch()),
    fields_(std::move(fields))
{
}


Message::Message(const MessageType message_type, const NameValueMap& fields,
                 const String& receiver_module_name) :
    message_type_(message_type),
    sender_module_uid_(-1), sender_module_name_(), sender_module_class_(),
    receiver_module_uid_(-1), receiver_module_name_(receiver_module_name), receiver_module_class_(),
    creation_time_(std::chrono::system_clock::now().time_since_epoch()),
    fields_(fields)
{
}


// protected constructor.
Message::Message(const MessageType message_type, NameValueMap&& fields,
                 const UID receiver_module_uid, String&& receiver_module_name,
                 ModuleClass&& receiver_module_class,
                 const UID sender_module_uid, String&& sender_module_name,
                 ModuleClass&& sender_module_class,
                 Time&& creation_time) :
    message_type_(message_type),
    sender_module_uid_(sender_module_uid), sender_module_name_(std::move(sender_module_name)),
    sender_module_class_(std::move(sender_module_class)),
    receiver_module_uid_(receiver_module_uid), receiver_module_name_(std::move(receiver_module_name)),
    receiver_module_class_(std::move(receiver_module_class)),
    creation_time_(std::move(creation_time)),
    fields_(std::move(fields))
{
}


// protected constructor.
Message::Message(const MessageType message_type, const NameValueMap& fields,
                 const UID receiver_module_uid, const String& receiver_module_name,
                 const ModuleClass& receiver_module_class,
                 const UID sender_module_uid, const String& sender_module_name,
                 const ModuleClass& sender_module_class,
                 const Time& creation_time) :
    message_type_(message_type),
    sender_module_uid_(sender_module_uid), sender_module_name_(sender_module_name),
    sender_module_class_(sender_module_class),
    receiver_module_uid_(receiver_module_uid), receiver_module_name_(receiver_module_name),
    receiver_module_class_(receiver_module_class),
    creation_time_(creation_time),
    fields_(fields)
{
}


Message::~Message()
{
}


const Time& Message::get_creation_time() const
{
    return creation_time_;
}


const MessageType& Message::get_type() const
{
    return message_type_;
}


const UID& Message::get_sender_module_uid() const
{
    return sender_module_uid_;
}


const String& Message::get_sender_module_name() const
{
    return sender_module_name_;
}


const ModuleClass& Message::get_sender_module_class() const
{
    return sender_module_class_;
}


const UID& Message::get_receiver_module_uid() const
{
    return receiver_module_uid_;
}


const String& Message::get_receiver_module_name() const
{
    return receiver_module_name_;
}


const ModuleClass& Message::get_receiver_module_class() const
{
    return receiver_module_class_;
}


const NameValueMap& Message::get_fields() const
{
    return fields_;
}


void Message::set_sender_module_uid(const UID& uid)
{
    sender_module_uid_ = uid;
}


void Message::set_sender_module_name(const String& module_name)
{
    sender_module_name_ = module_name;
}


void Message::set_sender_module_class(const ModuleClass& module_class)
{
    sender_module_class_ = module_class;
}

} // namespace tsw
