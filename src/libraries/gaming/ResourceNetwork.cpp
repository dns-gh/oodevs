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
#include "Tools.h"
#include "clients_gui/TerrainObjectProxy.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/DictionaryUpdated.h"
#include "clients_kernel/Options.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/Viewport_ABC.h"
#include "protocol/protocol.h"
#include <time.h>

using namespace geometry;
using namespace gui;
using namespace resource;

namespace
{
    QString ConvertFromResourceType( E_ResourceType type )
    {
        switch( type )
        {
        case eResourceType_Water:
            return tools::translate( "ResourceNetwork", "Water" );
        case eResourceType_Gaz:
            return tools::translate( "ResourceNetwork", "Gaz" );
        case eResourceType_Electricity:
            return tools::translate( "ResourceNetwork", "Electricity" );
        default:
            throw std::exception( "Unknown resource type" ); 
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ResourceNetwork constructor
// Created: JSR 2010-08-19
// -----------------------------------------------------------------------------
ResourceNetwork::ResourceNetwork( kernel::Controllers& controllers, unsigned int id, bool isUrban, const tools::Resolver_ABC< TerrainObjectProxy >& urbanResolver, const tools::Resolver_ABC< kernel::Object_ABC >& objectResolver, const MsgsSimToClient::MsgUrbanAttributes_Infrastructures& msg, kernel::PropertiesDictionary& dico )
    : controllers_( controllers )
    , id_( id )
    , isUrban_( isUrban )
    , urbanResolver_( urbanResolver )
    , objectResolver_( objectResolver )
{
    Update( msg );
    CreateDictionary( dico );
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
    kernel::Entity_ABC* entity = 0;
    if( isUrban_ )
        entity = &urbanResolver_.Get( id_ );
    else
        entity = &objectResolver_.Get( id_ );
    for( int i = 0; i < message.resource_network_size(); ++i )
    {
        const MsgsSimToClient::MsgUrbanAttributes_Infrastructures_ResourceNetwork& network = message.resource_network( i );
        E_ResourceType type;
        switch( network.type() )
        {
        case MsgsSimToClient::MsgUrbanAttributes_Infrastructures_ResourceNetwork::water:
            type = eResourceType_Water;
            break;
        case MsgsSimToClient::MsgUrbanAttributes_Infrastructures_ResourceNetwork::gaz:
            type = eResourceType_Gaz;
            break;
        case MsgsSimToClient::MsgUrbanAttributes_Infrastructures_ResourceNetwork::electricity:
            type = eResourceType_Electricity;
            break;
        default:
            throw std::exception( "Bad resource type" );
        }
        ResourceNode& node = resourceNodes_[ type ];
        unsigned int oldMaxStock = node.maxStock_;
        unsigned int oldProduction = node.production_;
        unsigned int oldConsumption = node.consumption_;
        bool oldCritical = node.critical_;
        unsigned int oldStock = node.stock_;
        unsigned int oldFlow = node.totalFlow_;
        node.type_ = type;
        node.isEnabled_ = network.enabled();
        node.production_ = network.has_production() ? network.production() : 0;
        node.maxStock_ = network.has_max_stock() ? network.max_stock() : 0;
        node.stock_ = network.has_stock() ? network.stock() : 0;
        node.consumption_ = network.has_consumption() ? network.consumption() : 0;
        node.critical_ = network.has_critical() ? network.critical() : false;
        node.links_.clear();
        node.totalFlow_ = 0;
        for( int j = 0; j < network.link_size(); ++j )
        {
            ResourceLink link;
            link.urban_ = network.link( j ).kind() == MsgsSimToClient::MsgUrbanAttributes_Infrastructures_ResourceNetwork_Link::urban;
            link.id_ = network.link( j ).target_id();
            link.capacity_ = network.link( j ).capacity();
            link.flow_ = network.link( j ).flow();
            node.totalFlow_ += link.flow_;
            node.links_.push_back( link );
        }
        const QString baseName = tools::translate( "ResourceNetwork", "Resources Networks" ) + "/" + ConvertFromResourceType( type ) + "/";
        if( node.totalFlow_ != oldFlow && node.links_.size() )
            controllers_.controller_.Update( kernel::DictionaryUpdated( *entity, baseName + tools::translate( "ResourceNetwork", "Total flow" ) ) );
        if( node.stock_ != oldStock )
            controllers_.controller_.Update( kernel::DictionaryUpdated( *entity, baseName + tools::translate( "ResourceNetwork", "Stock" ) ) );
        if( node.maxStock_ != oldMaxStock )
            controllers_.controller_.Update( kernel::DictionaryUpdated( *entity, baseName + tools::translate( "ResourceNetwork", "Maximal stock" ) ) );
        if( node.production_ != oldProduction )
            controllers_.controller_.Update( kernel::DictionaryUpdated( *entity, baseName + tools::translate( "ResourceNetwork", "Production" ) ) );
        if( node.consumption_ != oldConsumption )
            controllers_.controller_.Update( kernel::DictionaryUpdated( *entity, baseName + tools::translate( "ResourceNetwork", "Consumption" ) ) );
        if( node.critical_ != oldCritical )
            controllers_.controller_.Update( kernel::DictionaryUpdated( *entity, baseName + tools::translate( "ResourceNetwork", "Vital consumption" ) ) );
    }
}

// -----------------------------------------------------------------------------
// Name: ResourceNetwork::Draw
// Created: JSR 2010-08-19
// -----------------------------------------------------------------------------
void ResourceNetwork::Draw( const Point2f&, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const
{
    // TODO utiliser un enum pour les options
    char filter = controllers_.options_.GetOption( "ResourceNetworks", 0 ).To< char >();
    if( filter == 1 )// off
        return;
    Point2f from;
    Point2f to;
    bool selected = true;
    if( isUrban_ )
    {
        TerrainObjectProxy& proxy = urbanResolver_.Get( id_ );
        selected = proxy.IsSelected();
        if( filter == 3 && !selected ) // selected outgoing
            return;
        from = proxy.Barycenter();
    }
    else
        from = objectResolver_.Get( id_ ).Get< kernel::Positions >().GetPosition();

    glPushAttrib( GL_LINE_BIT );
    glLineWidth( 3.f );

    for( CIT_ResourceNodes node = resourceNodes_.begin(); node != resourceNodes_.end(); ++node )
    {
        SetColor( node->second.type_ );
        if( node->second.links_.size() > 0 )
        {
            glEnable( GL_LINE_STIPPLE );
            for( std::vector< ResourceLink >::const_iterator link = node->second.links_.begin(); link != node->second.links_.end(); ++link )
            {
                UpdateStipple( link->flow_ );
                if( link->urban_ )
                {
                    TerrainObjectProxy* proxy = urbanResolver_.Find( link->id_ );
                    if( !proxy || ( filter == 2 && !proxy->IsSelected() && !selected ) ) // Selected all
                        continue;
                    to = proxy->Barycenter();
                }
                else
                    to = objectResolver_.Get( link->id_ ).Get< kernel::Positions >().GetPosition();
                if( viewport.IsVisible( Rectangle2f( from, to ) ) )
                    tools.DrawLine( from, to );
            }
            glDisable( GL_LINE_STIPPLE );
        }
        else
            if( filter == 0 || selected )
                tools.DrawCircle( from, 20.0 );
    }
    glPopAttrib();
}

// -----------------------------------------------------------------------------
// Name: ResourceNetwork::GetLinkName
// Created: JSR 2010-08-25
// -----------------------------------------------------------------------------
QString ResourceNetwork::GetLinkName( E_ResourceType type, unsigned int i ) const
{
    const ResourceNode* node = FindResourceNode( type );
    if( node == 0 || i >= node->links_.size() )
        return "";
    const ResourceLink& link = node->links_[ i ];
    kernel::Entity_ABC* entity = 0;
    if( link.urban_ )
        entity = &urbanResolver_.Get( link.id_ );
    else
        entity = &objectResolver_.Get( link.id_ );
    QString ret = entity->GetName();
    if( ret.isEmpty() )
        ret = QString::number( link.id_ );
    return ret;
}

// -----------------------------------------------------------------------------
// Name: ResourceNetwork::SetColor
// Created: JSR 2010-08-20
// -----------------------------------------------------------------------------
void ResourceNetwork::SetColor( E_ResourceType type ) const
{
    // TODO régler les couleurs
    switch( type )
    {
    default:
    case eResourceType_Water:
        glColor4f( 0.2f, 0.2f, 0.6f,  1.0f );
        break;
    case eResourceType_Gaz:
        glColor4f( 0.4f, 0.4f, 0.4f,  1.0f );
        break;
    case eResourceType_Electricity:
        glColor4f( 0.4f, 0.4f, 0.7f,  1.0f );
        break;
    }
}

// -----------------------------------------------------------------------------
// Name: ResourceNetwork::UpdateStipple
// Created: JSR 2010-08-25
// -----------------------------------------------------------------------------
void ResourceNetwork::UpdateStipple( int value ) const
{
    long time = clock();
    unsigned short shift = ( unsigned short ) ( ( long ) ( time * value / 256 ) % 128 ) / 8;
    glLineStipple( 1, 0x00FF << shift | 0x00FF >> ( 16-shift ) );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetwork::CreateDictionary
// Created: JSR 2010-08-23
// -----------------------------------------------------------------------------
void ResourceNetwork::CreateDictionary( kernel::PropertiesDictionary& dico ) const
{
    for( CIT_ResourceNodes node = resourceNodes_.begin(); node != resourceNodes_.end(); ++node )
    {
        const QString baseName = tools::translate( "ResourceNetwork", "Resources Networks" ) + "/" + ConvertFromResourceType( node->second.type_ ) + "/";
        dico.Register( *this, baseName + tools::translate( "ResourceNetwork", "Enabled" ), node->second.isEnabled_ );
        dico.Register( *this, baseName + tools::translate( "ResourceNetwork", "Total flow" ), node->second.totalFlow_ );
        dico.Register( *this, baseName + tools::translate( "ResourceNetwork", "Maximal stock" ), node->second.maxStock_ );
        dico.Register( *this, baseName + tools::translate( "ResourceNetwork", "Stock" ), node->second.stock_ );
        dico.Register( *this, baseName + tools::translate( "ResourceNetwork", "Production" ), node->second.production_ );
        dico.Register( *this, baseName + tools::translate( "ResourceNetwork", "Consumption" ), node->second.consumption_ );
        dico.Register( *this, baseName + tools::translate( "ResourceNetwork", "Vital consumption" ), node->second.critical_ );
    }
}
