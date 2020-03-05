/** 
 *  @file      NMT_sock.hpp
 *  @brief     Header file for NMT_sock.cpp
 *  @details   Header file for NMT socket wrapper library
 *  @author    Nitin Mohan
 *  @date      Feb 22, 2019
 *  @copyright 2020 - NM Technologies
 */

#ifndef DEF_CAM_MOTOR_CTRL
#define DEF_CAM_MOTOR_CTRL
/*--------------------------------------------------/
/                   System Imports                  /
/--------------------------------------------------*/
#include <boost/python.hpp>
#include <arpa/inet.h>

/*--------------------------------------------------/
/                   Local Imports                   /
/--------------------------------------------------*/
#include "NMT_stdlib.h"


typedef enum sock_mode {SOCK_CLIENT, SOCK_SERVER}sock_mode;

class NMT_sock_multicast
{
    public:


        /* Constructor */
        NMT_sock_multicast(unsigned int port, std::string multicast_ip,
                           sock_mode socket_mode);

        /* Function to send message over the socket */
        NMT_result NMT_write_socket(char *message);

        /* Function to read message on the socket */
        NMT_result NMT_read_socket(char **message);

        /* Getter function to return the result */
        NMT_result NMT_get_result() {return this->result;}

    private:

        /** @var result
         *  Varible to set the overall state of the object */
        NMT_result result = OK;

        /** @var port
         *  Port to communicate on */
        unsigned int port;

        /** @var multicast_ip
         *  IP Address to send multicast messages to */
       std::string  multicast_ip;

        /** @var sock
         *  Socket object */
        int sock; 

        /** @var mode
         *  Mode of socket */
        sock_mode mode;

        /** @var my_address
         *  Object which contains current systems attributes*/
        struct sockaddr_in my_address; 

        /* Function to initialize the multicast protocol */
        NMT_result NMT_init_multicast_server();

        /* Function to initialize the multicast protocol */
        NMT_result NMT_init_multicast_client();

};

boost::python::tuple NMT_read_socket_py(NMT_sock_multicast &nmt_sock_multicast)
{
    char *message;
    NMT_result result = OK;
    result = nmt_sock_multicast.NMT_read_socket(&message);
    
    if (result == OK)
    {
        return boost::python::make_tuple(result, std::string(message));
    }
    else 
    {
        return boost::python::make_tuple(result, "");
    }
        
}
/* Opening Interface for Python */
BOOST_PYTHON_MODULE(libNMT_sock)
{
    using namespace boost::python;

    /* NMT_result ENUM for Python */
    enum_<NMT_result>("NMT_result")
        .value("OK", OK)
        .value("NOK", NOK);

    /* sock_mode ENUM for Python*/
    enum_<sock_mode>("sock_mode")
        .value("SOCK_CLIENT", SOCK_CLIENT)
        .value("SOCK_SERVER", SOCK_SERVER);

    /* Class Declerations for Python */
    class_<NMT_sock_multicast>("NMT_sock_multicast", init<unsigned int,
                                                       std::string,
                                                       sock_mode>())
      .def("NMT_write_socket", &NMT_sock_multicast::NMT_write_socket)
      .def("NMT_read_socket", NMT_read_socket_py)
      .def("NMT_get_result", &NMT_sock_multicast::NMT_get_result);
}

#endif
