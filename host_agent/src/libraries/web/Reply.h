// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef REPLY_H
#define REPLY_H

#include <boost/property_tree/ptree_fwd.hpp>
#include <string>
#include <vector>

namespace web
{
    typedef boost::property_tree::ptree Tree;
}

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

// =============================================================================
/** @class  Reply
    @brief  Reply struct definition
*/
// Created: BAX 2012-03-07
// =============================================================================
struct Reply
{
    const HttpStatus status;
    const std::string data;
    explicit Reply( const std::string& data );
    explicit Reply( HttpStatus status, const std::string& data = std::string() );
    explicit Reply( const Tree& tree );
    explicit Reply( const std::vector< Tree >& list );
    virtual ~Reply() {}
};
}

#endif // REPLY_H
