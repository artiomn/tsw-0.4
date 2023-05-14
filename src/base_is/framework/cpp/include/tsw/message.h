/**
  * @file message.h
  * @author Artiom N.(cl)2017
  * @brief Message class definition, MessageHandler typedef and MessageType enum.
  *
  */

#ifndef _TSW_MESSAGE_H
#define _TSW_MESSAGE_H

#include <functional>
#include <type_traits>

#include "module.h"
#include "types.h"


namespace tsw
{

enum class MessageType : uint16_t
{
    Action,
    AnnounceModuleActions,
    AnnounceModuleEvents,
    DenounceModuleActions,
    DenounceModuleEvents,
    DesintegrateModule,
    EchoReply,
    EchoRequest,
    Error,
    Event,
    IntegrateModule,
    ReceiveActionsList,
    ReceiveEventsList,
    Reply,
    SubscribeToEvents,
    SystemStarted,
    UnsubscribeFromEvents,
    ZLast
};


class Magistral;
class Message;
typedef std::function<bool(const Message& message)> MessageHandler;


class Message
{
public:
    Message(const MessageType message_type, NameValueMap&& fields);
    Message(const MessageType message_type, const NameValueMap& fields);
    Message(const MessageType message_type, NameValueMap&& fields,
            const UID& receiver_module_uid);
    Message(const MessageType message_type, const NameValueMap& fields,
            const UID& receiver_module_uid);
    Message(const MessageType message_type, NameValueMap&& fields,
            const String& receiver_module_name);
    Message(const MessageType message_type, const NameValueMap& fields,
            const String& receiver_module_name);
    virtual ~Message();

public:
   const Time&          get_creation_time() const;
   const MessageType&   get_type() const;
   const UID&           get_sender_module_uid() const;
   const String&        get_sender_module_name() const;
   const ModuleClass&   get_sender_module_class() const;
   const UID&           get_receiver_module_uid() const;
   const String&        get_receiver_module_name() const;
   const ModuleClass&   get_receiver_module_class() const;
   const NameValueMap&  get_fields() const;

protected:
   Message(const MessageType message_type, NameValueMap&& fields,
           const UID receiver_module_uid, String&& receiver_module_name,
           ModuleClass&& receiver_module_class,
           const UID sender_module_uid, String&& sender_module_name,
           ModuleClass&& sender_module_class,
           Time&& creation_time);
   Message(const MessageType message_type, const NameValueMap& fields,
           const UID receiver_module_uid, const String& receiver_module_name,
           const ModuleClass& receiver_module_class,
           const UID sender_module_uid, const String& sender_module_name,
           const ModuleClass& sender_module_class,
           const Time& creation_time);
   void set_sender_module_uid(const UID& uid);
   void set_sender_module_name(const String& module_name);
   void set_sender_module_class(const ModuleClass& module_class);

private:
   const MessageType    message_type_;
   UID                  sender_module_uid_;
   String               sender_module_name_;
   ModuleClass          sender_module_class_;
   const UID            receiver_module_uid_;
   const String         receiver_module_name_;
   const ModuleClass    receiver_module_class_;
   const Time           creation_time_;
   const NameValueMap   fields_;
};

} // namespace tsw

#endif //_MESSAGE_H
