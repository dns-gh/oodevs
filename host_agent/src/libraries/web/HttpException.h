// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef HTTP_EXCEPTION_H
#define HTTP_EXCEPTION_H

#include <string>

namespace web
{
// -----------------------------------------------------------------------------
// Name: HttpStatus enumeration
// Created: BAX 2012-02-28
// -----------------------------------------------------------------------------
enum HttpStatus
{
    OK,
    BAD_REQUEST,
    UNAUTHORIZED,
    FORBIDDEN,
    NOT_FOUND,
    INTERNAL_SERVER_ERROR,
    NOT_IMPLEMENTED,
    HTTP_STATUS_COUNT,
};

int         GetStatusCode   ( HttpStatus status );
const char* GetStatusMessage( HttpStatus status );

// -----------------------------------------------------------------------------
// Name: HttpException struct
// Created: BAX 2012-04-03
// -----------------------------------------------------------------------------
struct HttpException : public std::exception
{
    explicit HttpException( HttpStatus code, const std::string& msg = std::string() )
        : code( code )
        , msg ( msg )
    {
        // NOTHING
    }

    ~HttpException() throw()
    {
        // NOTHING
    }

    const char* what() const throw()
    {
        return msg.c_str();
    }

    HttpStatus code;
    std::string msg;
};
}

#endif // HTTP_EXCEPTION_H
