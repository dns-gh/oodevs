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
#include "protocol/protocol.h"
#include <boost/bind.hpp>
#include <xeumeuleu/xml.hpp>

using namespace resource;

// -----------------------------------------------------------------------------
// Name: NodeProperties constructor
// Created: JSR 2010-08-13
// -----------------------------------------------------------------------------
NodeProperties::NodeProperties()
    : isFunctional_( true )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NodeProperties constructor
// Created: JSR 2010-08-13
// -----------------------------------------------------------------------------
NodeProperties::NodeProperties( xml::xistream& xis )
    : isFunctional_( true )
{
    // Read from urban.xml
    xis >> xml::list( "node", *this, &NodeProperties::ReadNode );
}

// -----------------------------------------------------------------------------
// Name: NodeProperties constructor
// Created: JSR 2010-08-13
// -----------------------------------------------------------------------------
NodeProperties::NodeProperties( const NodeProperties& from )
    : isFunctional_( true )
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
    std::string name; // See what to do with it
    std::string type;
    xis >> xml::start( "network" )
            >> xml::attribute( "name", name )
            >> xml::attribute( "type", type );
    NodeElement& element = Get( NodeElement::ConvertToResourceType( type ) ); // should exist
    element.Update( xis );
    xis  >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: NodeProperties::Update
// Created: JSR 2010-08-16
// -----------------------------------------------------------------------------
void NodeProperties::Update()
{
    // update intermediate stocks
    Apply( boost::bind( &NodeElement::UpdateImmediateStock, _1, isFunctional_ ) );
    // get total consumption by resource type
    NodeElement::T_Consumptions consumptions;
    Apply( boost::bind( &NodeElement::AddConsumptions, _1, boost::ref( consumptions ) ) ); // TODO optimise and put in method above?
    // apply consumptions
    isFunctional_ = true;
    for( NodeElement::CIT_Consumptions it = consumptions.begin(); it != consumptions.end(); ++it )
    {
        NodeElement* element = Find( it->first );
        if( element )
        {
            if( !element->Consume( it->second.amount_ ) && it->second.critical_ )
                isFunctional_ = false;
        }
        else
            if( it->second.critical_ )
                isFunctional_ = false;
    }
    Apply( boost::bind( &NodeElement::DistributeResource, _1, isFunctional_ ) );
}

// -----------------------------------------------------------------------------
// Name: NodeProperties::Push
// Created: JSR 2010-08-16
// -----------------------------------------------------------------------------
void NodeProperties::Push( int quantity, E_ResourceType resourceType )
{
    NodeElement* element = Find( resourceType );
    if( element )
        element->Push( quantity );
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
// Name: NodeProperties::Update
// Created: JSR 2010-08-26
// -----------------------------------------------------------------------------
void NodeProperties::Update( const Common::MsgMissionParameter_Value& msg )
{
    for( int i = 0; i< msg.list_size(); ++i )
    {
        Common::MsgMissionParameter_Value node = msg.list( i );
        E_ResourceType resourceType = static_cast< E_ResourceType >( node.list( 0 ).identifier() );
        NodeElement* element = Find( resourceType );
        if( element )
            element->Update( node.list( 1 ) );
    }
}

// -----------------------------------------------------------------------------
// Name: NodeProperties::ReadNode
// Created: JSR 2010-08-13
// -----------------------------------------------------------------------------
void NodeProperties::ReadNode( xml::xistream& xis )
{
    E_ResourceType type = NodeElement::ConvertToResourceType( xis.attribute< std::string >( "resource" ) );
    Register( type, *new NodeElement( xis, type ) );
}
