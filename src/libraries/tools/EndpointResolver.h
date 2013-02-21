// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef tools_EndpointResolver_h
#define tools_EndpointResolver_h

#include "asio.h"

namespace tools
{
// =============================================================================
/** @class  EndpointResolver
    @brief  Endpoint resolver
*/
// Created: SLI 2013-02-20
// =============================================================================
class EndpointResolver
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit EndpointResolver( boost::asio::io_service& service );
    virtual ~EndpointResolver();
    //@}

    //! @name Operations
    //@{
    boost::asio::ip::tcp::resolver::iterator Resolve( const std::string& endpoint, boost::system::error_code& error );
    //@}

private:
    //! @name Member data
    //@{
    boost::asio::ip::tcp::resolver resolver_;
    //@}
};

}

#endif // tools_EndpointResolver_h
