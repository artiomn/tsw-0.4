/**
  * @file zmq_magistral_impl.h
  * @author Artiom N.(cl)2017
  * @brief ZeroMQ-based Magistral implementation header.
  *
  */

#ifndef _TSW_MAGISTRAL_IMPL_H
#define _TSW_MAGISTRAL_IMPL_H

#include <array>
#include <atomic>
#include <future>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>

#include <zmq.h>

#include <tsw/deserializer.h>
#include <tsw/serializer.h>
#include <tsw/types.h>


namespace tsw
{

// Debug stuff.
void tsw_zmq_dump (void *socket, int options);


//----------------------------------------------------------------------------
// ZMQException
//----------------------------------------------------------------------------

class ZMQException : public Exception
{
public:
    using Exception::Exception;
    ZMQException() : Exception(zmq_strerror(zmq_errno()))
    {}
};


class Magistral::MagistralImpl
{
public:
    typedef std::function<bool(const BinData& message)> RecvHandler;

public:
    MagistralImpl(const String& param_string );
    ~MagistralImpl() noexcept;

public:
    // In milliseconds.
    const int connect_timeout = 15;

    // In milliseconds.
    const int zmq_poll_delay = 1;

    // In milliseconds.
    const int zmq_linger_delay = 5;

public:
    void activate();
    void deactivate();

    /**
     * @brief Send message to a specific module and block until respond will be received or timeout exceed.
     * @param message_type
     * @param fields
     * @param timeout
     * @param receiver_module_uid
     * @return response, received from the magistral
     */
    void send_data(const BinData& dat, int timeout);

    /**
     * @brief Set user message handler.
     * @param handler
     */
    void set_recv_handler(RecvHandler handler);

    /**
     * @brief Get user message handler.
     * @return handler
     */
    RecvHandler get_recv_handler() const;

    bool active() const { return reader_thread_active_; }

private:
    // ZeroMQ client identifier size (ROUTER/DEALER pattern).
    static const int max_zmq_id_size_ = 255;

private:
    String set_params_by_init_string(const String &s);
    int create_client();
    int create_server();
    const uint8_t *read_id();

private:
    void reader_thread_func();
    void recv_multipart(zmq_msg_t &zmsg);

private:
    bool is_client_;
    std::atomic_bool reader_thread_active_;
    String connection_string_;

    std::array<uint8_t, max_zmq_id_size_ + 1> zmq_client_id_;
    void *ZMQContext_;
    void *ZMQSocket_;
    int recv_timeout_;
    int send_timeout_;

    RecvHandler recv_handler_;
    std::promise<void> reader_promise_;

    BinData msg_to_send_;
    std::mutex mutex_;

    std::thread reader_thread_;
};

} // namespace tsw

#endif // _TSW_MAGISTRAL_IMPL_H
