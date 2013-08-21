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

#include <tools/Exception.h>

namespace tools
{
class ConnectionError : public Exception
{
public:
    ConnectionError( const std::string& file, const std::string& function, const int line, const std::string& message )
        : Exception( file, function, line, message )
    {}
};

class DisconnectionRequest : public ConnectionError
{
public:
    DisconnectionRequest( const std::string& file, const std::string& function, const int line, const std::string& message )
        : ConnectionError( file, function, line, message )
    {}
};

}

#endif // ConnectionError_h
