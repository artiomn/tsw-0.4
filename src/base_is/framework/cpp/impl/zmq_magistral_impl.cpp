/**
  * @file zmq_magistral_impl.cpp
  * @author Artiom N.(cl)2017
  * @brief ZeroMQ-based Magistral implementation.
  *
  */


#include <iomanip>
#include <iostream>
#include <memory>

#include <tsw/error.h>
#include <tsw/logger.h>
#include <tsw/magistral.h>
#include <tsw/message.h>
#include <tsw/types.h>

#include "zmq_magistral_impl.h"


namespace tsw
{

void tsw_zmq_dump(void *socket, int options)
{
    int rc;

    zmq_msg_t zmsg;
    rc = zmq_msg_init(&zmsg);
    TSW_ASSERT(rc == 0);

    std::cout << "----------------------------------------\n";

    //  Process all parts of the message
    do
    {
        int size = zmq_msg_recv(&zmsg, socket, options);
        TSW_ASSERT(size >= 0);

        // Dump the message as text or binary.
        char *data = static_cast<char*>(zmq_msg_data(&zmsg));
        TSW_ASSERT(data != 0);

        bool is_text = true;
        int char_nbr;
        for (char_nbr = 0; char_nbr < size; ++char_nbr)
        {
            if (data[char_nbr] < 32 || data[char_nbr] > 126)
            {
                is_text = false;
                break;
            }
        }

        std::cout << "[" << std::dec << std::setw(3) << size << "]";
        for (char_nbr = 0; char_nbr < size; ++char_nbr)
        {
            if (is_text)
            {
                std::cout << static_cast<char>(data[char_nbr]);
            }
            else
            {
                std::cout << std::hex << std::setw(2) << static_cast<unsigned char>(data[char_nbr]);
            }
        }
        std::cout << std::endl;
    } while (zmq_msg_more(&zmsg));

    rc = zmq_msg_close(&zmsg);
    TSW_ASSERT(rc == 0);
}


Magistral::MagistralImpl::MagistralImpl(const String& param_string ) :
    connection_string_(set_params_by_init_string(param_string)),
    ZMQContext_(nullptr), ZMQSocket_(nullptr), recv_timeout_(-1), send_timeout_(-1)
{

}


Magistral::MagistralImpl::~MagistralImpl() noexcept
{
    deactivate();
}


void Magistral::MagistralImpl::activate()
{
    if (ZMQContext_) deactivate();

    std::lock_guard<std::mutex> socket_guard(mutex_);

    ZMQContext_ = zmq_ctx_new();
    if (!ZMQContext_) TSW_THROW(ZMQException);

    // Use ROUTER/DEALER ZeroMQ model to bidirectional unrestricted communication between modules.

    ZMQSocket_ = zmq_socket(ZMQContext_, is_client_ ? ZMQ_DEALER : ZMQ_ROUTER);
    if (!ZMQSocket_)
    {
        zmq_ctx_destroy(ZMQContext_);
        ZMQContext_ = nullptr;
        TSW_THROW(ZMQException);
    }

    //ZMQ_HEARTBEAT_IVL
    //ZMQ_HEARTBEAT_TIMEOUT
    //ZMQ_TOS
    //ZMQ_RECONNECT_IVL
    //ZMQ_RECONNECT_IVL_MAX
    int result;
    result = (is_client_) ? create_client() : create_server();

    if (result == -1)
    {
        zmq_close(ZMQSocket_);
        ZMQSocket_ = nullptr;
        zmq_ctx_destroy(ZMQContext_);
        ZMQContext_ = nullptr;

        TSW_THROW(ZMQException);
    }

    reader_thread_active_ = true;
    reader_thread_ = std::move(std::thread(&Magistral::MagistralImpl::reader_thread_func, this));
    reader_thread_.detach();
}


void Magistral::MagistralImpl::deactivate()
{
    reader_thread_active_ = false;

    std::lock_guard<std::mutex> socket_guard(mutex_);
    int rc;

    if (ZMQSocket_)
    {
        rc = zmq_close(ZMQSocket_);
        ZMQSocket_ = nullptr;
        TSW_ASSERT(rc == 0);
    }

    if (ZMQContext_)
    {
        rc = zmq_ctx_shutdown(ZMQContext_);
        TSW_ASSERT(rc == 0);
        rc = zmq_ctx_term(ZMQContext_);
        ZMQContext_ = nullptr;
        TSW_ASSERT(rc == 0);
    }
}


int Magistral::MagistralImpl::create_client()
{
    int opt = 1;
    // ZMQ_IMMEDIATE is unstable.
    //zmq_setsockopt(ZMQSocket_, ZMQ_IMMEDIATE, &opt, sizeof(opt));
    zmq_setsockopt(ZMQSocket_, ZMQ_LINGER, &zmq_linger_delay, sizeof(zmq_linger_delay));
    zmq_setsockopt(ZMQSocket_, ZMQ_PROBE_ROUTER, &opt, sizeof(opt));
    zmq_setsockopt(ZMQSocket_, ZMQ_CONNECT_TIMEOUT, &connect_timeout, sizeof(connect_timeout));
    // opt = 0;
    // zmq_setsockopt(ZMQSocket_, ZMQ_STREAM_NOTIFY, &opt, sizeof(opt));

    return zmq_connect(ZMQSocket_, connection_string_.c_str());
}


int Magistral::MagistralImpl::create_server()
{
    //int opt = 0;
    //zmq_setsockopt(ZMQSocket_, ZMQ_ROUTER_MANDATORY, &opt, sizeof(opt));
    zmq_setsockopt(ZMQSocket_, ZMQ_LINGER, &zmq_linger_delay, sizeof(zmq_linger_delay));
    int result = zmq_bind(ZMQSocket_, connection_string_.c_str());

    return result;
}


const uint8_t *Magistral::MagistralImpl::read_id()
{
    int size = zmq_recv(ZMQSocket_, &zmq_client_id_[0], max_zmq_id_size_, ZMQ_DONTWAIT);
    if (size == -1) return nullptr;
    zmq_client_id_[size] = 0;

    return zmq_client_id_.data();
}


void Magistral::MagistralImpl::send_data(const BinData &data, int timeout)
{
    TSW_ASSERT(ZMQContext_ != nullptr);
    TSW_ASSERT(ZMQSocket_ != nullptr);

    // Guard need to be there, before msg_to_send_ fill, because I can use this method
    // from different threads.
    std::lock_guard<std::mutex> socket_guard(mutex_);

    LOG_TO("magistral", LogLevel::Debug, "Data with size = %ld will be send during %ld ms...",
           data.size(), timeout);

    zmq_msg_t zmsg;

//    if (zmq_msg_init_data(&zmsg, msg_to_send_.data(), msg_to_send_.size(), nullptr, nullptr) == -1)
//    {
//        // Not enough memory? Maybe cleaning data vector will help and I can to throw exception.
//        msg_to_send_.clear();
//        TSW_THROW(ZMQException);
//    }

    // Another way, without global message buufer:
    if (zmq_msg_init_size(&zmsg, data.size()) == -1)
    {
        // Not enough memory? Maybe cleaning data vector will help and I can to throw exception.
        msg_to_send_.clear();
        TSW_THROW(ZMQException);
    }
    memcpy(zmq_msg_data(&zmsg), data.data(), data.size());

    if (timeout != send_timeout_)
    {
        send_timeout_ = timeout;
        if (zmq_setsockopt(ZMQSocket_, ZMQ_SNDTIMEO, &send_timeout_, sizeof(send_timeout_)))
        {
            TSW_THROW(ZMQException);
        }
    }

    int msg_size = zmq_msg_size(&zmsg);

    /*zmq_pollitem_t poll_items[] =
    {
        { ZMQSocket_, 0, ZMQ_POLLOUT, 0 }
    };

    int rc = 0;
    while (!rc)
    {
        rc = zmq_poll(poll_items, sizeof(poll_items) / sizeof(zmq_pollitem_t), 1000);
        std::cout << "rc = " << rc << std::endl;
        if (rc > 0) break;
        if (rc < 0) throw ZMQException();
    }*/

    if (zmq_msg_send(&zmsg, ZMQSocket_, 0) != msg_size)
    {
        TSW_THROW(ZMQException);
    }

    // I don't need to close message after successfull `zmq_msg_send` (man zmq_msg_send).
}


void Magistral::MagistralImpl::set_recv_handler(RecvHandler handler)
{
    recv_handler_ = handler;
}


Magistral::MagistralImpl::RecvHandler Magistral::MagistralImpl::get_recv_handler() const
{
    return recv_handler_;
}


void Magistral::MagistralImpl::recv_multipart(zmq_msg_t &zmsg)
{
    BinData serialized_message;
    int recv_res = 0;

    serialized_message.clear();

    do
    {
        recv_res = zmq_msg_recv(&zmsg, ZMQSocket_, ZMQ_DONTWAIT);

        if (recv_res > 0)
        {
            auto zs = zmq_msg_size(&zmsg);
            auto zd = static_cast<const BinData::value_type*>(zmq_msg_data(&zmsg));
            serialized_message.insert(serialized_message.end(), zd, zd + zs);
        }
        else if (recv_res == 0)
        {
            // Router probe.
            //LOG_TO("magistral", LogLevel::Debug, "Router probe from a client with id = %d", zmq_client_id_);
        }
        else // res < 0, error.
        {
            TSW_THROW(ZMQException);
        }
    } while(recv_res > 0 && zmq_msg_more(&zmsg) && reader_thread_active_);

    //LOG_TO("magistral", LogLevel::Debug, "Multipart message size = %d from a client with id = %d",
    //       serialized_message.size(), client_id);
    if (serialized_message.size() && recv_handler_)
    {
        recv_handler_(serialized_message);
    }
}


void Magistral::MagistralImpl::reader_thread_func()
{
    zmq_pollitem_t poll_item = { ZMQSocket_, 0, ZMQ_POLLIN, 0 };

    zmq_msg_t zmsg;
    int recv_res = 0;

    try
    {
        std::unique_lock<std::mutex> socket_guard(mutex_, std::defer_lock_t());

        while (reader_thread_active_)
        {
            socket_guard.lock();

            int rc = zmq_poll(&poll_item, 1, zmq_poll_delay);

            if (!rc)
            {
                socket_guard.unlock();
                std::this_thread::yield();
                continue;
            }

            if (rc < 0)
            {
                LOG_TO("magistral", LogLevel::Error, "%s poll() error: %s, magistral reading cycle will be broken",
                       (is_client_) ? "cln" : "srv", zmq_strerror(zmq_errno()) );
                break;
                //!!!TSW_THROW(ZMQException);
            }

            TSW_ASSERT(poll_item.revents & ZMQ_POLLIN);

            const uint8_t *client_id = read_id();

            TSW_ASSERT(client_id != nullptr);

            if (zmq_msg_init(&zmsg)) TSW_THROW(ZMQException);

            std::shared_ptr<zmq_msg_t> message_guard(&zmsg, zmq_msg_close);

            recv_res = zmq_msg_recv(&zmsg, ZMQSocket_, ZMQ_DONTWAIT);

            if (!zmq_msg_more(&zmsg))
            {
                if (recv_res > 0)
                {
                    // Normal one-part message.
                    if (recv_handler_)
                    {
                        auto data = static_cast<const BinData::value_type*>(zmq_msg_data(&zmsg));
                        size_t size = zmq_msg_size(&zmsg);
                        LOG_TO("magistral", LogLevel::Debug, "0x%x received %ld bytes of data from a client with id = 0x%x",
                               (is_client_) ? "cln" : "srv", size, *client_id );
                        if (!recv_handler_(BinData(data, data + size))) break;
                    }
                }
                else if (recv_res == 0)
                {
                    // Router probe.
                    LOG_TO("magistral", LogLevel::Debug, "Router probe from a client with id = 0x%x", *client_id);
                }
                else // res < 0, error.
                {
                    TSW_THROW(ZMQException);
                }
            }
            else
            {
                // It's multipart message. Seldom case.
                LOG_TO("magistral", LogLevel::Debug, "Multipart message was received from a client with id = 0x%x...", *client_id);
                recv_multipart(zmsg);
            }
            socket_guard.unlock();
        } // while
    }
    catch(std::exception &e)
    {
        if (reader_thread_active_) deactivate();
        //reader_promise_.set_exception(std::current_exception());
        LOG_TO("magistral", LogLevel::Critical, "Exception with a message: \"%s\"!", e.what());
    }

    if (reader_thread_active_) deactivate();
}


String Magistral::MagistralImpl::set_params_by_init_string(const String &s)
{
    const String delimiter = ":";
    String::size_type mag_type_delimiter_pos = s.find(delimiter.c_str());
    String magistral_type = s.substr(0, mag_type_delimiter_pos);

    if (magistral_type == "client")
    {
        is_client_ = true;
    }
    else if (magistral_type == "server")
    {
        is_client_ = false;
    }
    else
    {
        TSW_THROW(std::runtime_error,
                  String("Unknown magistral endpoint type ") + magistral_type + " [correct types: client, server]!");
    }

    return s.substr(mag_type_delimiter_pos + delimiter.length());
}

} // namespace tsw
