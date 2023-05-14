/**
  * @file magistral.h
  * @author Artiom N.(cl)2017
  * @brief Magistral class definition.
  *
  */

#ifndef _TSW_MAGISTRAL_H
#define _TSW_MAGISTRAL_H

#include <map>
#include <memory>
#include <list>

#include "deserializer.h"
#include "message.h"
#include "serializer.h"
#include "types.h"


namespace tsw
{

/**
 * @brief The Magistral class, implements main bus for modules communication.
 *
 * This class encapsulates bus implementation: ZeroMQ in a common cause.
 * It's low-level layer to exchange messages between modules.
 *
 * @note
 * Usually, module has only one bus, created inside System instance, to exchange with a core and communicate with another modules through core.
 * User doesn't need to create Magistral instance, use System object, instead.
 */

class Magistral
{
public:
    /**
     * @brief Magistral class constructor
     * @param magistral_param_string string, consisting of two parts as follows:<br>
     *   `type:transport://address` (similar to ZeroMQ endpoint string).<br>
     *   transport: tcp, ipc.<br>
     *   type: client, server.
     * @param serializer serializer to create binary data from a message.
     * @param parser parser to deserialize binary data to a message.
     * @code
     * Magistral m("client:tcp://localhost:12345");
     * Magistral magistral("server:ipc:///tmp/feeds/0");
     * @endcode
     */
    Magistral(const String& magistral_param_string, bool activate_on_creation,
              std::shared_ptr<Serializer> serializer, std::shared_ptr<Deserializer> deserializer);
    Magistral(const String& magistral_param_string, bool activate_on_creation = true);
    Magistral() = delete;
    Magistral(Magistral& magistral) = delete;
    virtual ~Magistral();

public:
    void activate();
    void deactivate();
    bool active() const;

   /**
    * @brief Send message and block until respond will be received or timeout exceed.
    * @param msg
    * @param timeout
    */
   void send_message(const Message& msg, int timeout = -1);

   /**
    * @brief Send message and block until respond will be received or timeout exceed.
    * @param message_type
    * @param fields
    * @param timeout
    */
   void send_message(const MessageType message_type, NameValueMap&& fields, int timeout = -1);
   void send_message(const MessageType message_type, const NameValueMap& fields, int timeout = -1);

   /**
    * @brief Send message to a specific module and block until respond will be received or timeout exceed.
    * @param message_type
    * @param fields
    * @param timeout
    * @param receiver_module_uid
    */
   void send_message(const MessageType message_type, NameValueMap&& fields, UID receiver_module_uid, int timeout = -1);
   void send_message(const MessageType message_type, const NameValueMap& fields, UID receiver_module_uid, int timeout = -1);

   /**
    * @brief Add user message handler.
    * @param handler
    */
   void add_message_handler(MessageHandler handler);
   void add_message_handler(MessageType message_type, MessageHandler handler);

   /**
    * @brief Remove user message handler.
    * @param handler
    */
   void remove_message_handler(MessageHandler handler);
   void remove_message_handler(MessageType message_type, MessageHandler handler);

   /**
    * @brief Return default message handler.
    * @return message handler address.
   */
   MessageHandler get_default_message_handler();

private:
   bool default_recv_handler(const BinData& reply);
   bool default_message_handler(const Message& reply);

private:
   class MagistralImpl;
   std::shared_ptr<Serializer> serializer_;
   std::shared_ptr<Deserializer> deserializer_;
   std::map<MessageType, std::list<MessageHandler>> message_handlers_;
   std::unique_ptr<MagistralImpl> magistral_;
};

} // namespace tsw

#endif // _TSW_MAGISTRAL_H
