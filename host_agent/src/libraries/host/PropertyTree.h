// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef PROPERTY_TREE_H
#define PROPERTY_TREE_H

#include <boost/property_tree/ptree_fwd.hpp>

namespace host
{
    std::string ToJson( const boost::property_tree::ptree& tree, bool isPretty = false );
    boost::property_tree::ptree FromJson( const std::string& data );
    std::string ToXml( const boost::property_tree::ptree& tree );
    boost::property_tree::ptree FromXml( const std::string& data );
}

#endif // PROPERTY_TREE_H
