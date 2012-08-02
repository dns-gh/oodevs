// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "UrbanResourceNetworkAttribute.h"

// -----------------------------------------------------------------------------
// Name: UrbanResourceNetworkAttribute constructor
// Created: JSR 2012-08-02
// -----------------------------------------------------------------------------
UrbanResourceNetworkAttribute::UrbanResourceNetworkAttribute( xml::xistream& xis )
{
    xis >> xml::optional >> xml::start( "resources" )
            >> xml::list( "node", *this, &UrbanResourceNetworkAttribute::ReadNode )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: UrbanResourceNetworkAttribute destructor
// Created: JSR 2012-08-02
// -----------------------------------------------------------------------------
UrbanResourceNetworkAttribute::~UrbanResourceNetworkAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanResourceNetworkAttribute::GetResourceNodes
// Created: JSR 2012-08-02
// -----------------------------------------------------------------------------
const UrbanResourceNetworkAttribute::T_ResourceNodes& UrbanResourceNetworkAttribute::GetResourceNodes() const
{
    return resourceNodes_;
}

// -----------------------------------------------------------------------------
// Name: UrbanResourceNetworkAttribute::ReadNode
// Created: JSR 2012-08-02
// -----------------------------------------------------------------------------
void UrbanResourceNetworkAttribute::ReadNode( xml::xistream& xis )
{
    std::string resource = xis.attribute< std::string >( "resource-type" );
    ResourceNode& node = resourceNodes_[ resource ];
    node.resource_ = resource;
    xis >> xml::optional >> xml::attribute( "enabled", node.isEnabled_ )
        >> xml::optional >> xml::attribute( "production", node.production_ )
        >> xml::optional >> xml::attribute( "consumption", node.consumption_ )
        >> xml::optional >> xml::attribute( "critical-consumption", node.critical_ )
        >> xml::optional >> xml::attribute( "stock", node.maxStock_ )
        >> xml::list( "link", *this, &UrbanResourceNetworkAttribute::ReadLink, node );
}

// -----------------------------------------------------------------------------
// Name: UrbanResourceNetworkAttribute::ReadLink
// Created: JSR 2012-08-02
// -----------------------------------------------------------------------------
void UrbanResourceNetworkAttribute::ReadLink( xml::xistream& xis, ResourceNode& node )
{
    ResourceLink link;
    xis >> xml::attribute( "target", link.id_ )
        >> xml::attribute( "capacity", link.capacity_ );
    node.links_.push_back( link );
}
