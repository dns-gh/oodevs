// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "NodeProperties.h"
#include "ResourceNetworkModel.h"
#include "ResourceTools_ABC.h"
#include "protocol/protocol.h"
#include <boost/bind.hpp>
#include <urban/ResourceNetworkAttribute.h>
#include <xeumeuleu/xml.hpp>

using namespace resource;

// -----------------------------------------------------------------------------
// Name: NodeProperties constructor
// Created: JSR 2010-08-13
// -----------------------------------------------------------------------------
NodeProperties::NodeProperties( const ResourceTools_ABC& tools )
    : isFunctional_( true )
    , tools_       ( tools )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NodeProperties constructor
// Created: JSR 2010-08-13
// -----------------------------------------------------------------------------
NodeProperties::NodeProperties( xml::xistream& xis, const ResourceTools_ABC& tools )
    : isFunctional_( true )
    , tools_       ( tools )
{
    Update( xis );
}

// -----------------------------------------------------------------------------
// Name: NodeProperties constructor
// Created: JSR 2010-09-17
// -----------------------------------------------------------------------------
NodeProperties::NodeProperties( const urban::ResourceNetworkAttribute& urbanAttribute, const ResourceTools_ABC& tools )
    : isFunctional_( true )
    , tools_       ( tools )
{
    const urban::ResourceNetworkAttribute::T_ResourceNodes& nodes = urbanAttribute.GetResourceNodes();
    for( urban::ResourceNetworkAttribute::CIT_ResourceNodes it = nodes.begin(); it != nodes.end(); ++it )
    {
        unsigned long resourceId = tools_.GetResourceId( it->second.resource_ );
        Register( resourceId, *new NodeElement( it->second, resourceId ) );
    }
}

// -----------------------------------------------------------------------------
// Name: NodeProperties constructor
// Created: JSR 2010-08-13
// -----------------------------------------------------------------------------
NodeProperties::NodeProperties( const NodeProperties& from )
    : isFunctional_( true )
    , tools_       ( from.tools_ )
{
    for( CIT_Elements it = from.elements_.begin(); it != from.elements_.end(); ++it )
        Register( it->first, *new NodeElement( *it->second ) );
}

// -----------------------------------------------------------------------------
// Name: NodeProperties destructor
// Created: JSR 2010-08-13
// -----------------------------------------------------------------------------
NodeProperties::~NodeProperties()
{
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: NodeProperties::SetModel
// Created: JSR 2010-08-16
// -----------------------------------------------------------------------------
void NodeProperties::SetModel( const ResourceNetworkModel& model )
{
    Apply( boost::bind( &NodeElement::SetModel, _1, boost::cref( model ) ) );
}

// -----------------------------------------------------------------------------
// Name: NodeProperties::Update
// Created: JSR 2010-08-16
// -----------------------------------------------------------------------------
void NodeProperties::Update( xml::xistream& xis )
{
    xis >> xml::list( "node", *this, &NodeProperties::ReadNode );
}

// -----------------------------------------------------------------------------
// Name: NodeProperties::Update
// Created: JSR 2010-08-16
// -----------------------------------------------------------------------------
void NodeProperties::Update()
{
    // update intermediate stocks
    Apply( boost::bind( &NodeElement::UpdateImmediateStock, _1, isFunctional_ ) );
    // apply consumptions
    isFunctional_ = true;
    Apply( boost::bind( &NodeElement::Consume, _1, boost::ref( isFunctional_ ) ) );
    Apply( boost::bind( &NodeElement::DistributeResource, _1, isFunctional_ ) );
}

// -----------------------------------------------------------------------------
// Name: NodeProperties::Push
// Created: JSR 2010-08-16
// -----------------------------------------------------------------------------
void NodeProperties::Push( int quantity, unsigned long resourceId )
{
    NodeElement* element = Find( resourceId );
    if( element )
        element->Push( quantity );
}

// -----------------------------------------------------------------------------
// Name: NodeProperties::SetModifier
// Created: JSR 2010-08-31
// -----------------------------------------------------------------------------
void NodeProperties::SetModifier( unsigned int modifier )
{
    Apply( boost::bind( &NodeElement::SetModifier, _1, modifier ) );
}

// -----------------------------------------------------------------------------
// Name: NodeProperties::Serialize
// Created: JSR 2010-08-17
// -----------------------------------------------------------------------------
void NodeProperties::Serialize( MsgsSimToClient::MsgUrbanAttributes_Infrastructures& msg ) const
{
    // TODO sérialiser isFunctional_?
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
        it->second->Serialize( *msg.add_resource_network() );
}

// -----------------------------------------------------------------------------
// Name: NodeProperties::Serialize
// Created: JSR 2010-08-31
// -----------------------------------------------------------------------------
void NodeProperties::Serialize( Common::MsgObjectAttributeResourceNetwork& msg ) const
{
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
        it->second->Serialize( *msg.add_network() );
}

// -----------------------------------------------------------------------------
// Name: NodeProperties::Update
// Created: JSR 2010-08-26
// -----------------------------------------------------------------------------
void NodeProperties::Update( const Common::MsgMissionParameter_Value& msg )
{
    for( int i = 0; i< msg.list_size(); ++i )
    {
        Common::MsgMissionParameter_Value node = msg.list( i );
        unsigned int id = tools_.GetResourceId( node.list( 0 ).acharstr() );
        NodeElement* element = Find( id );
        if( element )
            element->Update( node );
    }
}

// -----------------------------------------------------------------------------
// Name: NodeProperties::ReadNode
// Created: JSR 2010-08-13
// -----------------------------------------------------------------------------
void NodeProperties::ReadNode( xml::xistream& xis )
{
    std::string resourceName = xis.attribute< std::string >( "resource-type" );
    unsigned long resourceId = tools_.GetResourceId( resourceName );
    NodeElement* element = Find( resourceId );
    if( element )
        element->Update( xis );
    else
        Register( resourceId, *new NodeElement( xis, resourceId, resourceName ) );
}
