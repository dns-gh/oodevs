// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "PropertyTree.h"

#ifdef _MSC_VER
#   pragma warning( push )
#   pragma warning( disable : 4100 4512 )
#endif
#include <boost/property_tree/json_parser.hpp>
#ifdef _MSC_VER
#pragma warning( pop )
#endif

#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/property_tree/xml_parser.hpp>

namespace
{
template< typename T >
boost::property_tree::ptree Read( const std::string& data, const T& functor )
{
    std::istringstream input( data );
    boost::property_tree::ptree tree;
    functor( input, tree );
    return tree;
}

template< typename T >
std::string Write( const boost::property_tree::ptree& tree, const T& functor )
{
    std::ostringstream out;
    functor( out, tree );
    return out.str();
}
}

// -----------------------------------------------------------------------------
// Name: ToJson
// Created: BAX 2012-04-18
// -----------------------------------------------------------------------------
std::string host::ToJson( const boost::property_tree::ptree& tree, bool isPretty )
{
    const std::string reply = Write( tree, boost::bind( &boost::property_tree::write_json< boost::property_tree::ptree >, _1, _2, isPretty ) );
    return reply.substr( 0, reply.size() - 1 );
}

// -----------------------------------------------------------------------------
// Name: FromJson
// Created: BAX 2012-04-18
// -----------------------------------------------------------------------------
boost::property_tree::ptree host::FromJson( const std::string& data )
{
    return Read( data, boost::bind( &boost::property_tree::read_json< boost::property_tree::ptree >, _1, _2 ) );
}

// -----------------------------------------------------------------------------
// Name: ToXml
// Created: BAX 2012-05-21
// -----------------------------------------------------------------------------
std::string host::ToXml( const boost::property_tree::ptree& tree )
{
    return Write( tree, boost::bind( &boost::property_tree::write_xml< boost::property_tree::ptree >,
                  _1, _2, boost::property_tree::xml_writer_make_settings( ' ', 4 ) ) );
}

// -----------------------------------------------------------------------------
// Name: FromXml
// Created: BAX 2012-05-21
// -----------------------------------------------------------------------------
boost::property_tree::ptree host::FromXml( const std::string& data )
{
    return Read( data, boost::bind( &boost::property_tree::read_xml< boost::property_tree::ptree >, _1, _2, 0 ) );
}
