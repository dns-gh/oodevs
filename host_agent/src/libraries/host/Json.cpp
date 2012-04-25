// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "Json.h"

#ifdef _MSC_VER
#   pragma warning( push )
#   pragma warning( disable : 4100 4512 )
#endif
#include <boost/property_tree/json_parser.hpp>
#ifdef _MSC_VER
#pragma warning( pop )
#endif

// -----------------------------------------------------------------------------
// Name: ToJson
// Created: BAX 2012-04-18
// -----------------------------------------------------------------------------
std::string host::ToJson( const boost::property_tree::ptree& tree, bool isPretty )
{
    std::ostringstream out;
    boost::property_tree::write_json( out, tree, isPretty );
    const std::string json = out.str();
    return json.substr( 0, json.size() - 1 );
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
