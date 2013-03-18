// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef tools_ConnectionError_h
#define tools_ConnectionError_h

#include <stdexcept>

namespace tools
{
// =============================================================================
/** @class  ConnectionError
    @brief  Connection error
*/
// Created: SLI 2013-02-21
// =============================================================================
class ConnectionError : public std::runtime_error
{
public:
    //! @name Constructors/Destructor
    //@{
             ConnectionError( const std::string& message ) : std::runtime_error( message ) {}
    virtual ~ConnectionError() {}
    //@}
};

class DisconnectionRequest : public ConnectionError
{
public:
             DisconnectionRequest( const std::string& message ) : ConnectionError( message ) {}
    virtual ~DisconnectionRequest() {}
};

}

#endif // ConnectionError_h
