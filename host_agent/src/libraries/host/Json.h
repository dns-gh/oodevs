// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef JSON_H
#define JSON_H

#include <boost/property_tree/ptree_fwd.hpp>

namespace host
{
    std::string ToJson( const boost::property_tree::ptree& tree, bool isPretty = false );
    boost::property_tree::ptree FromJson( const std::string& data );
}

#endif // JSON_H
