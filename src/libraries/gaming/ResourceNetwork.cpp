// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ResourceNetwork.h"
#include "clients_gui/TerrainObjectProxy.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/Viewport_ABC.h"
#include "protocol/protocol.h"

using namespace geometry;
using namespace gui;
using namespace resource;

// -----------------------------------------------------------------------------
// Name: ResourceNetwork constructor
// Created: JSR 2010-08-19
// -----------------------------------------------------------------------------
ResourceNetwork::ResourceNetwork( kernel::Controller& controller, unsigned int id, bool isUrban, const tools::Resolver_ABC< TerrainObjectProxy >& urbanResolver, const tools::Resolver_ABC< kernel::Object_ABC >& objectResolver, const MsgsSimToClient::MsgUrbanAttributes_Infrastructures& msg )
    : controller_( controller ) // useful?
    , id_( id )
    , isUrban_( isUrban )
    , urbanResolver_( urbanResolver )
    , objectResolver_( objectResolver )
{
    Update( msg );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetwork destructor
// Created: JSR 2010-08-19
// -----------------------------------------------------------------------------
ResourceNetwork::~ResourceNetwork()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ResourceNetwork::Update
// Created: JSR 2010-08-19
// -----------------------------------------------------------------------------
void ResourceNetwork::Update( const MsgsSimToClient::MsgUrbanAttributes_Infrastructures& message )
{
    // TODO Ne faire les update que si ça a vraiment changé dans la sim
    for( int i = 0; i < message.resource_network_size(); ++i )
    {
        const MsgsSimToClient::MsgUrbanAttributes_Infrastructures_ResourceNetwork& network = message.resource_network( i );
        EResourceType type;
        switch( network.type() )
        {
        case MsgsSimToClient::MsgUrbanAttributes_Infrastructures_ResourceNetwork::water:
            type = eResourceTypeWater;
            break;
        case MsgsSimToClient::MsgUrbanAttributes_Infrastructures_ResourceNetwork::gaz:
            type = eResourceTypeGaz;
            break;
        case MsgsSimToClient::MsgUrbanAttributes_Infrastructures_ResourceNetwork::electricity:
            type = eResourceTypeElectricity;
            break;
        default:
            throw std::exception( "Bad resource type" );
        }
        ResourceNode& node = resourceNodes_[ type ];
        node.isProducer_ = network.producer();
        node.type_ = type;
        node.isEnabled_ = network.enabled();
        node.hasStock_ =  network.has_stock();
        if( node.hasStock_ )
            node.stock_ = network.stock();
        node.links_.clear();
        for( int j = 0; j < network.link_size(); ++j )
        {
            ResourceLink link;
            link.urban_ = network.link( j ).kind() == MsgsSimToClient::MsgUrbanAttributes_Infrastructures_ResourceNetwork_Link::urban;
            link.id_ = network.link( j ).target_id();
            link.capacity_ = network.link( j ).capacity();
            node.links_.push_back( link );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ResourceNetwork::Display
// Created: JSR 2010-08-19
// -----------------------------------------------------------------------------
void ResourceNetwork::Display( kernel::Displayer_ABC& displayer, kernel::Displayer_ABC& itemDisplayer ) const
{
    // TODO?
}

// -----------------------------------------------------------------------------
// Name: ResourceNetwork::Draw
// Created: JSR 2010-08-19
// -----------------------------------------------------------------------------
void ResourceNetwork::Draw( const Point2f&, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const
{
    /*if( !tools.ShouldDisplay( "ResourceNetwork" ) )
        return;*/
    Point2f from;
    Point2f to;
    if( isUrban_ )
        from = urbanResolver_.Get( id_ ).Barycenter();
    else
    {
        // TODO
    }

    glPushAttrib( GL_LINE_BIT | GL_CURRENT_BIT );
    glLineWidth( 3.f );

    for( std::map< resource::EResourceType, ResourceNode >::const_iterator node = resourceNodes_.begin(); node != resourceNodes_.end(); ++node )
    {
        glColor4f( COLOR_YELLOW ); // TODO à faire en fonction de la ressource
        for( std::vector< ResourceLink >::const_iterator link = node->second.links_.begin(); link != node->second.links_.end(); ++link )
        {
            if( link->urban_ )
                to = urbanResolver_.Get( link->id_ ).Barycenter();
            else
            {
                // TODO
            }
            if( viewport.IsVisible( Rectangle2f( from, to ) ) )
                tools.DrawCurvedArrow( from, to, 0.5f );
        }
    }
    glPopAttrib();
}
