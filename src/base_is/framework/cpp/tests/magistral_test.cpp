/**
  * @file magistral_test.cpp
  * @author Artiom N.(cl)2017
  * @brief Magistral tests.
  *
  */

#include <thread>

#include <tsw/error.h>
#include <tsw/message.h>
#include <tsw/magistral.h>

#include <impl/zmq_magistral_impl.h>


#include "tests_common.h"


class MagistralConnectionTest : public ::testing::Test {
protected:
    void SetUp()
    {

    }

    void TearDown()
    {

    }
};


TEST(Magistral, InitStringCorrectness)
{
    // Incorrect string.
    ASSERT_THROW(tsw::Magistral("incorrect_init_string"), std::runtime_error);
    // Incorrect string, started with correct end-point type "client".
    ASSERT_THROW(tsw::Magistral("client:incorrect_init_string"), tsw::ZMQException);
    // Incorrect string, started with correct end-point type "server".
    ASSERT_THROW(tsw::Magistral("client:incorrect_init_string"), tsw::ZMQException);
    // Incorrect server string, with non-existent host.
    EXPECT_THROW(tsw::Magistral("server:tcp://!no-exisent-host!:123"), tsw::ZMQException);
    // Incorrect client string, with incorrect host.
    EXPECT_THROW(tsw::Magistral("server:tcp://!no-exisent-host!:123"), tsw::ZMQException);
    // Correct server IPC string.
    EXPECT_NO_THROW(tsw::Magistral("server:ipc://filename"));
    // Correct client IPC string.
    EXPECT_NO_THROW(tsw::Magistral("server:ipc://filename"));
}


TEST(Magistral, ServerUseBusyPort)
{
    tsw::Magistral m("server:tcp://127.0.0.1:33221");
    EXPECT_THROW(tsw::Magistral m("server:tcp://*:33221"), tsw::ZMQException);
}


TEST(Magistral, Connection)
{
    tsw::Magistral s("server:tcp://127.0.0.1:33221");
    EXPECT_NO_THROW(tsw::Magistral c("client:tcp://127.0.0.1:33221"));
}

TEST(Magistral, SendRecv)
{
    //! [send_recv]
    tsw::Magistral s("server:tcp://127.0.0.1:33221");
    s.add_message_handler([](const tsw::Message &msg) -> bool
    {
        //ASSERT_EQ(msg.get_type(), tsw::MessageType::Action);
        return false;
    });

    // CLient with an incorrect port.
//    tsw::Magistral c_failure("client:tcp://127.0.0.1:33222");
    tsw::Magistral c("client:tcp://127.0.0.1:33221");
    //sleep(1);
//    tsw::Magistral c("client:ipc://xxx");

    const tsw::NameValueMap fields = {std::make_pair("test", "test_val")};
//    EXPECT_THROW(c_failure.send_message(tsw::MessageType::EchoRequest, fields, 0), tsw::ZMQException);
//    EXPECT_NO_THROW(c.send_message(tsw::MessageType::EchoRequest, fields, 10));
    c.send_message(tsw::MessageType::EchoRequest, fields, 50);
    while (s.active()) std::this_thread::sleep_for(std::chrono::milliseconds(5));
    //s.deactivate();
    c.deactivate();
    std::this_thread::sleep_for(std::chrono::milliseconds(16));
    //! [send_recv]
}

TEST_F(MagistralConnectionTest, SimpleConnection)
{

}
