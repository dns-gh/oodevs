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
#include <tools/Exception.h>

namespace tools
{
// =============================================================================
/** @class  ConnectionError
    @brief  Connection error
*/
// Created: SLI 2013-02-21
// =============================================================================
class ConnectionError : public Exception
{
public:
    //! @name Constructors/Destructor
    //@{
             ConnectionError( const std::string& file, const std::string& function, const int line, const std::string& message ) : Exception( file, function, line, message ) {}
    virtual ~ConnectionError() {}
    //@}
};

class DisconnectionRequest : public ConnectionError
{
public:
             DisconnectionRequest( const std::string& file, const std::string& function, const int line, const std::string& message ) : ConnectionError( file, function, line, message ) {}
    virtual ~DisconnectionRequest() {}
};

}

#define MASA_CONNECTION_ERROR( what ) tools::ConnectionError( __FILE__, __FUNCTION__, __LINE__, what )
#define MASA_DISCONNECTION_REQUEST( what ) tools::DisconnectionRequest( __FILE__, __FUNCTION__, __LINE__, what )
#endif // ConnectionError_h
