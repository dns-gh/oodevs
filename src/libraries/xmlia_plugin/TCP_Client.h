// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************


#ifndef __TCP_Client_h_
#define __TCP_Client_h_

#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

using boost::asio::ip::tcp;

namespace plugins
{
namespace xmlia
{
// =============================================================================
/** @class  Publisher
@brief  Publisher
*/
// Created: SLG 2009-06-02
// =============================================================================

class TCP_Client
{
public:
    TCP_Client( boost::asio::io_service& io_service, const std::string parameter, const std::string webServicePath, const std::string webServiceHost );

    //! @name Accessors
    //@{
    std::string GetContent();
    //@}

private:
    void handle_resolve ( const boost::system::error_code& err, tcp::resolver::iterator endpoint_iterator );
    void handle_connect ( const boost::system::error_code& err, tcp::resolver::iterator endpoint_iterator );
    void handle_write_request ( const boost::system::error_code& err);
    void handle_read_status_line ( const boost::system::error_code& err );
    void handle_read_headers ( const boost::system::error_code& err );
    void handle_read_content ( const boost::system::error_code& err );


    void handle_read    ( const boost::system::error_code& err, size_t number_bytes_read );



private:
    tcp::resolver resolver_;
    tcp::socket socket_;
    std::string parameter_;
    boost::asio::streambuf request_;
    boost::asio::streambuf response_;
    std::istringstream content_stream_;
    std::string strContent_;


    boost::array<char, 128> ReceptionbufferMessage;

};

}
}

#endif // __REST_Client_h_