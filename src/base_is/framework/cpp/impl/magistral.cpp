/**
  * @file magistral.cpp
  * @author Artiom N.(cl)2017
  * @brief Magistral class implementation.
  *
  */

#include <functional>

#include <boost/range/adaptor/map.hpp>

#include <tsw/error.h>
#include <tsw/magistral.h>
#include <tsw/message.h>
#include <tsw/bson_deserializer.h>
#include <tsw/bson_serializer.h>
#include <tsw/types.h>

#include "functional_helper.h"
#include "zmq_magistral_impl.h"


namespace tsw
{

using namespace std::placeholders;
using namespace tsw::impl;


Magistral::Magistral(const String& magistral_param_string, bool activate_on_creation,
                     std::shared_ptr<Serializer> serializer, std::shared_ptr<Deserializer> deserializer) :
    serializer_(serializer),
    deserializer_(deserializer),
    magistral_(new MagistralImpl(magistral_param_string))
{
    magistral_->set_recv_handler(std::bind(&Magistral::default_recv_handler, this, _1));
    if (activate_on_creation) magistral_->activate();
}


Magistral::Magistral(const String& magistral_param_string, bool activate_on_creation) :
    serializer_(new BsonSerializer()),
    deserializer_(new BsonDeserializer()),
    magistral_(new MagistralImpl(magistral_param_string))
{
    magistral_->set_recv_handler(std::bind(&Magistral::default_recv_handler, this, _1));
    if (activate_on_creation) magistral_->activate();
}


Magistral::~Magistral()
{
}


void Magistral::activate()
{
    magistral_->activate();
}


void Magistral::deactivate()
{
    magistral_->deactivate();
}


bool Magistral::active() const
{
    return magistral_->active();
}


void Magistral::send_message(const Message& msg, int timeout)
{
    magistral_->send_data(serializer_->SerializeMessage(msg), timeout);
}


void Magistral::send_message(const MessageType message_type, NameValueMap&& fields, int timeout)
{
    magistral_->send_data(serializer_->SerializeMessage(Message(message_type, fields, -1)), timeout);
}

void Magistral::send_message(const MessageType message_type, const NameValueMap& fields, int timeout)
{
    magistral_->send_data(serializer_->SerializeMessage(Message(message_type, fields, -1)), timeout);
}


void Magistral::send_message(const MessageType message_type, NameValueMap&& fields, UID receiver_module_uid, int timeout)
{
    magistral_->send_data(serializer_->SerializeMessage(Message(message_type, fields, receiver_module_uid)), timeout);
}


void Magistral::send_message(const MessageType message_type, const NameValueMap& fields, UID receiver_module_uid, int timeout)
{
    magistral_->send_data(serializer_->SerializeMessage(Message(message_type, fields, receiver_module_uid)), timeout);
}


void Magistral::add_message_handler(MessageHandler handler)
{
    for (uint16_t i = static_cast<uint16_t>(MessageType::Action); static_cast<MessageType>(i) != MessageType::ZLast; ++i )
    {
        add_message_handler(static_cast<MessageType>(i), handler);
    }
}


void Magistral::add_message_handler(MessageType message_type, MessageHandler handler)
{
    message_handlers_[message_type].push_back(handler);
}


void Magistral::remove_message_handler(MessageHandler handler)
{
    for (auto const &msg_type : boost::adaptors::keys(message_handlers_))
    {
        remove_message_handler(msg_type, handler);
    }
}


void Magistral::remove_message_handler(MessageType message_type, MessageHandler handler)
{
    auto &handlers = message_handlers_[message_type];

    for (auto i = handlers.begin(); i != handlers.end(); ++i)
    {
        if (*i == handler) handlers.erase(i);
    }
}


bool Magistral::default_recv_handler(const BinData& data)
{
    auto plain_message = deserializer_->DeserializeMessage(data);

    for (auto mh: message_handlers_[plain_message.get_type()])
    {
        if (!mh(plain_message)) return false;
    }

    return default_message_handler(deserializer_->DeserializeMessage(data));
}


bool Magistral::default_message_handler(const Message& message)
{
    if (message.get_type() == MessageType::EchoRequest)
    {
        send_message(MessageType::EchoReply, Object());
    }
    return true;
}


MessageHandler Magistral::get_default_message_handler()
{
    return std::bind(&Magistral::default_message_handler, this, _1);
}

} // namespace tsw
