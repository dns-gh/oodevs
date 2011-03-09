// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "tools_pch.h"

#include "FileMatcherRootNode.h"
#include <xeumeuleu/xml.hpp>
#include <boost/bind.hpp>

using namespace tools;

// -----------------------------------------------------------------------------
// Name: FileMatcherRootNode constructor
// Created: NLD 2011-03-08
// -----------------------------------------------------------------------------
FileMatcherRootNode::FileMatcherRootNode( xml::xistream& xis )
    : rootNodeToMatch_( xis.attribute< std::string >( "root-node" ) )
    , assignedSchema_ ( xis.attribute< std::string >( "assign-schema" ) )
{
}

// -----------------------------------------------------------------------------
// Name: FileMatcherRootNode destructor
// Created: NLD 2011-03-08
// -----------------------------------------------------------------------------
FileMatcherRootNode::~FileMatcherRootNode()
{
}

namespace
{
    void ExtractRootNode( const std::string& nodeName, xml::xistream& , std::string& rootNode )
    {
        rootNode = nodeName;
    }
}

// -----------------------------------------------------------------------------
// Name: FileMatcherRootNode destructor
// Created: NLD 2011-03-08
// -----------------------------------------------------------------------------
bool FileMatcherRootNode::MatchAndReturnNewSchema( const std::string& /*inputFileName*/, xml::xistream& xis, std::string& outputSchema ) const
{
    outputSchema.clear();

    std::string rootNode;
    xis >> xml::list( boost::bind( &ExtractRootNode, _2, _3, boost::ref( rootNode ) ) );

    if( rootNodeToMatch_ != rootNode )
        return false;
    outputSchema = assignedSchema_;
    return true;
}
