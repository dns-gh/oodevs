// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "Json.h"

#include <boost/property_tree/json_parser.hpp>

// -----------------------------------------------------------------------------
// Name: ToJson
// Created: BAX 2012-04-18
// -----------------------------------------------------------------------------
std::string host::ToJson( const boost::property_tree::ptree& tree )
{
    std::ostringstream out;
    boost::property_tree::write_json( out, tree, false );
    return out.str();
}

// -----------------------------------------------------------------------------
// Name: FromJson
// Created: BAX 2012-04-18
// -----------------------------------------------------------------------------
boost::property_tree::ptree host::FromJson( const std::string& data )
{
    std::istringstream input( data );
    boost::property_tree::ptree tree;
    boost::property_tree::read_json( input, tree );
    return tree;
}
