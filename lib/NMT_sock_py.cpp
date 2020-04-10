/** 
 *  @file      NMT_sock.cpp
 *  @brief     Wrapper Library sockets
 *  @details   This library is designed to facilate in socket Communication
 *  @author    Nitin Mohan
 *  @date      Feb 22, 2019
 *  @copyright 2020 - NM Technologies
 */

/*--------------------------------------------------/
/                   System Imports                  /
/--------------------------------------------------*/
#include <boost/python.hpp>
#include "NMT_sock.hpp"

boost::python::tuple NMT_read_socket_py(NMT_sock_multicast &nmt_sock_multicast)
{
    /*!
     *  @brief     A python wrapper function for NMT_read_socket
     *  @param[in] nmt_sock_multicast
     *  @return    NMT_result, message
     */

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

BOOST_PYTHON_MODULE(NMT_sock)
{
    /*!
     *  @brief     Function to expose C++ functions to python
     *  @param[in] libNMT_sock
     *  @return    N/A
     */

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
                                                       sock_mode,
                                                       unsigned int>())
      .def("NMT_write_socket", &NMT_sock_multicast::NMT_write_socket)
      .def("NMT_read_socket", NMT_read_socket_py)
      .def("NMT_get_result", &NMT_sock_multicast::NMT_get_result);
}
