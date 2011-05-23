// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __asio_h_
#define __asio_h_

#pragma warning( push, 0 )
#pragma warning( disable : 4512 4702 )
#ifndef _WIN32_WINNT
#   define _WIN32_WINNT 0x0501
#endif
#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>
#pragma warning( pop )
#undef min
#undef max

namespace tools
{

inline
std::string ToString( const boost::asio::ip::tcp::endpoint& endpoint )
{
    return endpoint.address().to_string()
        + ":" +  boost::lexical_cast< std::string >( endpoint.port() );
}

}

#endif // __asio_h_
