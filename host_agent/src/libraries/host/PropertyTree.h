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
    typedef boost::property_tree::ptree Tree;
    std::string ToJson( const Tree& tree, bool isPretty = false );
    Tree FromJson( const std::string& data );
    std::string ToXml( const Tree& tree );
    Tree FromXml( const std::string& data );
}

#endif // PROPERTY_TREE_H
