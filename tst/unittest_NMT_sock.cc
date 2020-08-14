/** 
 *  @file      unittest_NMT_sock.cc
 *  @brief     Unittests for NMT Sock Library
 *  @details   Test all NMT Sock Functionality
 *  @author    Nitin Mohan
 *  @date      June 11, 2020
 *  @copyright 2020 - NM Technologies
 */

/*--------------------------------------------------/
/                   System Imports                  /
/--------------------------------------------------*/ 
#include <iostream>
#include <gtest/gtest.h>

/*--------------------------------------------------/
/                   Local Imports                   /
/--------------------------------------------------*/
#include "NMT_sock_multi.hpp"
#include "NMT_sock_tcp.hpp"
#include "NMT_stdlib.h"
#include "NMT_log.h"

/* @class MyEnvironment
 *  Environment Setup for Test */
class MyEnvironment: public ::testing::Environment
{
public:
  virtual ~MyEnvironment() = default;

  virtual void SetUp() 
  {
      NMT_log_init((char *)"/tmp/", false);
  }

  virtual void TearDown() {NMT_log_finish();}
};

/* @class NMT_sock_Test_Fixture_multi
 * Test Fixture for unittests */
class NMT_sock_Test_Fixture_multi : public ::testing::Test
{
    public:
        /*Initialize Varibles */
        int port = 5500;
        std::string ip_address = "224.3.29.71";
        NMT_result result = OK;
        int client = -1;
};

/* @class NMT_sock_Test_Fixture_tcp
 * Test Fixture for unittests */
class NMT_sock_Test_Fixture_tcp : public ::testing::Test
{
    public:
        /*Initialize Varibles */
        int time_out = 10;
        std::string ip_address = "127.0.0.1";
        NMT_result result = OK;
};

/*--------------------------------------------------/
/                   Start of Tests                  /
/--------------------------------------------------*/


using namespace testing;
TEST_F(NMT_sock_Test_Fixture_multi, GW_Test_txrx)
{
   /**
    * @test Test that client is able to recieve data that the server sends
    * @step 1. Create 3 instances of the socket object (Server and 2 Clients)
    * @step 2. Server sends messages on socket
    * @step 3. Client is able to recieve the message that was sent by the server
    */
   
    /* Initialize Varibles */
    std::string out_message1;
    std::string out_message2;
    std::string test_message = "This is a test message";

    /* Setup Client/Server Objects */
    NMT_sock_multi multi_server1(port, ip_address, SOCK_SERVER);
    NMT_sock_multi multi_server2(port, ip_address, SOCK_SERVER);
    NMT_sock_multi multi_client(port, ip_address, SOCK_CLIENT);

    /* Write to Socket */
    result = multi_client.NMT_write_socket((char *)test_message.c_str());
    ASSERT_EQ(result, OK);

    /* Read from Socket */
    tie(result, out_message1) = multi_server1.NMT_read_socket();
    ASSERT_EQ(result, OK);

    tie(result, out_message2) = multi_server2.NMT_read_socket();
    ASSERT_EQ(result, OK);

    /*Verify Message */
    ASSERT_EQ(out_message1, test_message);
    ASSERT_EQ(out_message2, test_message);
}

TEST_F(NMT_sock_Test_Fixture_multi, BW_ServerTimeout)
{
    /**
     * @test Test scenario where no message is recieved on the socket
     * @step 1. Create 2 instances of the socket object (Server and Client)
     * @step 2. Client listens on the socket for 100ms and returns NOk as nothing is recieved
     */

    /* Initialize Varibles */
    std::string out_message = "";
    unsigned int timeOut = 1;

    /* Setup Server Object */
    NMT_sock_multi multi_server(port, ip_address, SOCK_SERVER, timeOut);

    /* Read from Socket */
    tie(result, out_message) = multi_server.NMT_read_socket();

    /* Verify Results */
    ASSERT_EQ(result, NOK);
    ASSERT_EQ(out_message, "");
}

TEST_F(NMT_sock_Test_Fixture_multi, BW_PortMisMatch)
{
    /**
     * @test Test Client does not rx anything when it joins with the incorrect port number
     * @step 1. Create 2 instances Server and Client (Client subscribes to incorrect port)
     * @step 2. Send Message
     * @step 3. Client timesout and does not recieve the message sent by the server
     */

    /* Initialize Varibles */
    std::string out_message1 = "";
    int wrong_port = port + 1;
    unsigned int timeOut = 1;
    std::string test_message = "This is a test message";

    /* Setup Client/Server Objects */
    NMT_sock_multi multi_server1(wrong_port, ip_address, SOCK_SERVER, timeOut);
    NMT_sock_multi multi_client(port, ip_address, SOCK_CLIENT);

    /* Write to Socket */
    result = multi_client.NMT_write_socket((char *)test_message.c_str());
    ASSERT_EQ(result, OK);

    /* Read from Socket */
    tie(result, out_message1) = multi_server1.NMT_read_socket();
    ASSERT_EQ(result, NOK);

    /*Verify Message */
    ASSERT_EQ(out_message1, "");
}

TEST_F(NMT_sock_Test_Fixture_tcp, GW_Test_tx_to_client)
{
   /**
    * @test Test that the server is able to send messages to the client
    * @step 1. Create 3 instances of the socket object (Server and 2 Clients)
    * @step 2. Server sends messages on socket
    * @step 3. Client is able to recieve the message that was sent by the server
    */
   
    /* Initialize Varibles */
    std::string out_message;
    std::string client_message1 = "Client Message1";
    std::string client_message2 = "Client Message2";
    std::string test_message1 = "This is a test message1";
    std::string test_message2 = "This is a test message2";
    int client_id1;
    int client_id2;
    int server_id;
    int port = 5501;

    /* Setup Client/Server Objects */
    NMT_sock_tcp tcp_server(port,  ip_address, SOCK_SERVER, time_out);
    NMT_sock_tcp tcp_client1(port, ip_address, SOCK_CLIENT);
    NMT_sock_tcp tcp_client2(port, ip_address, SOCK_CLIENT);

    /* Client1 Writes to Socket */
    result = tcp_client1.NMT_write_socket(client_message1);
    ASSERT_EQ(result, OK);

    /* Read from Socket */
    tie(result, out_message, client_id1) = tcp_server.NMT_read_socket();
    ASSERT_EQ(result, OK);
    ASSERT_EQ(out_message, client_message1);

    /* Client2 Writes to Socket */
    result = tcp_client2.NMT_write_socket(client_message2);
    ASSERT_EQ(result, OK);

    /* Read from Socket */
    tie(result, out_message, client_id2) = tcp_server.NMT_read_socket();
    ASSERT_EQ(result, OK);
    ASSERT_EQ(out_message, client_message2);

    /* Write to Client 1 */
    result = tcp_server.NMT_write_socket(test_message1, client_id1);
    ASSERT_EQ(result, OK);

    /* Write to Client 2 */
    result = tcp_server.NMT_write_socket(test_message2, client_id2);
    ASSERT_EQ(result, OK);

    /* Read on First Client */
    tie(result, out_message, server_id) = tcp_client1.NMT_read_socket();
    ASSERT_EQ(result, OK);
    ASSERT_EQ(out_message, test_message1);

    /* Read on Second Client */
    tie(result, out_message, server_id) = tcp_client2.NMT_read_socket();
    ASSERT_EQ(result, OK);
    ASSERT_EQ(out_message, test_message2);
}

int main(int argc, char **argv) 
{
    testing::InitGoogleTest(&argc, argv);
    MyEnvironment* env = new MyEnvironment(); 
    ::testing::AddGlobalTestEnvironment(env);
    return RUN_ALL_TESTS();
}

