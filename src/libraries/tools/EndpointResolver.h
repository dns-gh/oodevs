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
#include <boost/function.hpp>

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
    //! @name Types
    //@{
    typedef boost::function< void( const std::string&, const boost::system::error_code&, boost::asio::ip::tcp::resolver::iterator ) > T_Callback;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    explicit EndpointResolver( boost::asio::io_service& service );
    virtual ~EndpointResolver();
    //@}

    //! @name Operations
    //@{
    void AsyncResolve( const std::string& endpoint, T_Callback callback );
    void Cancel();
    //@}

private:
    //! @name Member data
    //@{
    boost::asio::ip::tcp::resolver resolver_;
    //@}
};

}

#endif // tools_EndpointResolver_h
