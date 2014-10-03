// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef CLIENT_H
#define CLIENT_H

#include "Client_ABC.h"

#ifdef _MSC_VER
#   pragma warning( push, 0 )
#   undef _SCL_SECURE_NO_WARNINGS
#endif
#define BOOST_NETWORK_ENABLE_HTTPS
#include <boost/network/protocol/http/client.hpp>
#ifdef _MSC_VER
#pragma warning( pop )
#endif

namespace web
{
// =============================================================================
/** @class  Client
    @brief  Client class definition
*/
// Created: BAX 2012-04-11
// =============================================================================
class Client : public Client_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Client();
    virtual ~Client();
    //@}

    //! @name Methods
    //@{
    virtual T_Response Get( const std::string& host, int port, const std::string& path, const T_Parameters& parameters );
    virtual T_Response Get( const std::string& scheme, const std::string& host, int port, const std::string& path, const T_Parameters& parameters );
    //@}

private:
    boost::network::http::client client_;
};
}

#endif // CLIENT_H
