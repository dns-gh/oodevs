// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __Connection_h_
#define __Connection_h_

#ifndef _WIN32_WINNT
#   define _WIN32_WINNT 0x0501
#endif
#pragma warning( push, 0 )
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#pragma warning( pop )
#include <string>

using boost::asio::ip::tcp;

namespace edxl
{

class Connection
{
public:
    //! @name Constructors/Destructor
    //@{
             Connection( const std::string& host, bool useSsl);
    virtual ~Connection();
    //@}

    //! @name I/O Operators
    //@{
    size_t Write( boost::asio::streambuf& request );
    size_t ReadUntil( boost::asio::streambuf& response, const std::string& delimiter );
    size_t Read( boost::asio::streambuf& response, boost::system::error_code& error );
    //@}

private:
    //! @name Helpers
    //@{
    void ResolveHandler( tcp::resolver::query& query, boost::system::error_code& error );
    //@}

private:
    //! @name Network Member data
    //@{
    boost::asio::io_service io_service_;
    tcp::resolver resolver_;
    tcp::socket socket_;
    boost::asio::ssl::context ctx_;
    boost::asio::ssl::stream<tcp::socket&> sslSocket_;
    bool useSsl_;
    //@}
};

}

#endif // __Connection_h_
