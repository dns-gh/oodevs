// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef shield_Logger_h
#define shield_Logger_h

#include "DebugInfo.h"
#include "ClientListener_ABC.h"
#include <boost/function.hpp>
#include <string>

namespace shield
{
// =============================================================================
/** @class  Logger
    @brief  Logs received messages
*/
// Created: MCO 2011-07-06
// =============================================================================
template< typename T >
class Logger
{
private:
    typedef boost::function< void( const std::string&, const T& ) > T_Callback;

public:
    Logger( ClientListener_ABC& listener, T_Callback callback )
        : pListener_( &listener )
        , callback_ ( callback )
    {}

    void operator()( const std::string& link, const T& message ) const
    {
        pListener_->Debug( DebugInfo< T >( "Shield received : ", message ) );
        callback_( link, message );
    }

private:
    ClientListener_ABC* pListener_;
    T_Callback callback_;
};

}

#endif // shield_Logger_h
