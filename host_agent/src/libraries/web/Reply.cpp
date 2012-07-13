// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "Reply.h"

#include "runtime/PropertyTree.h"

#include <boost/foreach.hpp>

using namespace web;
using namespace property_tree;

// -----------------------------------------------------------------------------
// Name: Reply::Reply
// Created: BAX 2012-07-12
// -----------------------------------------------------------------------------
Reply::Reply( const std::string& data )
    : status( OK )
    , data  ( data )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Reply::Reply
// Created: BAX 2012-07-12
// -----------------------------------------------------------------------------
Reply::Reply( HttpStatus status, const std::string& data )
    : status( status )
    , data  ( data )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Reply::Reply
// Created: BAX 2012-07-12
// -----------------------------------------------------------------------------
Reply::Reply( const Tree& tree )
    : status( OK )
    , data  ( ToJson( tree ) )
{
    // NOTHING
}

namespace
{
std::string Join( const std::vector< Tree >& list )
{
    std::string json;
    BOOST_FOREACH( const Tree& it, list )
        json += ToJson( it ) + ",";
    return "[" + json.substr( 0, json.size() - 1 ) + "]";
}
}

// -----------------------------------------------------------------------------
// Name: Reply::Reply
// Created: BAX 2012-07-12
// -----------------------------------------------------------------------------
Reply::Reply( const std::vector< Tree >& list )
    : status( OK )
    , data  ( Join( list ) )
{
    // NOTHING
}