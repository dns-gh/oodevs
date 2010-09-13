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
#include "clients_kernel/ResourceNetworkType.h"
#include "clients_kernel/Options.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/Viewport_ABC.h"
#include "protocol/protocol.h"
#include <time.h>

using namespace geometry;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: ResourceNetwork constructor
// Created: JSR 2010-08-19
// -----------------------------------------------------------------------------
ResourceNetwork::ResourceNetwork( kernel::Controllers& controllers, unsigned int id, const tools::Resolver_ABC< TerrainObjectProxy >& urbanResolver, const tools::Resolver_ABC< kernel::Object_ABC >& objectResolver, const tools::StringResolver< kernel::ResourceNetworkType >& resourceNetworkResolver, const MsgsSimToClient::MsgUrbanAttributes_Infrastructures& msg, kernel::PropertiesDictionary& dico )
    : controllers_( controllers )
    , id_                     ( id )
    , isUrban_                ( true )
    , urbanResolver_          ( urbanResolver )
    , objectResolver_         ( objectResolver )
    , resourceNetworkResolver_( resourceNetworkResolver )
{
    for( int i = 0; i < msg.resource_network_size(); ++i )
        UpdateNetwork( 0, msg.resource_network( i ) );
    CreateDictionary( dico );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetwork constructor
// Created: JSR 2010-08-31
// -----------------------------------------------------------------------------
ResourceNetwork::ResourceNetwork( kernel::Controllers& controllers, unsigned int id, const tools::Resolver_ABC< gui::TerrainObjectProxy >& urbanResolver, const tools::Resolver_ABC< kernel::Object_ABC >& objectResolver, const tools::StringResolver< kernel::ResourceNetworkType >& resourceNetworkResolver, const Common::MsgObjectAttributeResourceNetwork& msg, kernel::PropertiesDictionary& dico )
    : controllers_( controllers )
    , id_                     ( id )
    , isUrban_                ( false )
    , urbanResolver_          ( urbanResolver )
    , objectResolver_         ( objectResolver )
    , resourceNetworkResolver_( resourceNetworkResolver )
{
    for( int i = 0; i < msg.network_size(); ++i )
        UpdateNetwork( 0, msg.network( i ) );
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
// Name: ResourceNetwork::Draw
// Created: JSR 2010-08-19
// -----------------------------------------------------------------------------
void ResourceNetwork::Draw( const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const
{
    // TODO utiliser un enum pour les options
    char filter = controllers_.options_.GetOption( "ResourceNetworks", 0 ).To< char >();
    if( filter == 1 )// off
        return;
    if( filter == 3 && !IsSelected() ) // selected outgoing
        return;
    Point2f from = isUrban_ ? urbanResolver_.Get( id_ ).Barycenter()
        : objectResolver_.Get( id_ ).Get< kernel::Positions >().GetPosition();

    glPushAttrib( GL_LINE_BIT );
    glLineWidth( 3.f );

    for( CIT_ResourceNodes node = resourceNodes_.begin(); node != resourceNodes_.end(); ++node )
    {
        SetColor( node->second.resource_ );
        if( node->second.links_.size() > 0 )
        {
            glEnable( GL_LINE_STIPPLE );
            for( std::vector< ResourceLink >::const_iterator link = node->second.links_.begin(); link != node->second.links_.end(); ++link )
            {
                kernel::Entity_ABC* target = link->urban_ ? static_cast< kernel::Entity_ABC* >( urbanResolver_.Find( link->id_ ) ) : objectResolver_.Find( link->id_ );
                if( !target )
                    continue;
                if( filter == 2 )  // selected all
                {
                    ResourceNetwork_ABC* resourceTarget = target->Retrieve< ResourceNetwork_ABC >();
                    if( !resourceTarget || ( !IsSelected() && !resourceTarget->IsSelected() ) )
                        continue;
                }
                Point2f to = link->urban_ ? urbanResolver_.Get( link->id_ ).Barycenter()
                                          : objectResolver_.Get( link->id_ ).Get< kernel::Positions >().GetPosition();
                UpdateStipple( link->flow_ );
                if( viewport.IsVisible( Rectangle2f( from, to ) ) )
                    tools.DrawLine( from, to );
            }
            glDisable( GL_LINE_STIPPLE );
        }
        else
            if( filter == 0 || IsSelected() )
                tools.DrawCircle( from, 20.0 );
    }
    glPopAttrib();
}

// -----------------------------------------------------------------------------
// Name: ResourceNetwork::GetLinkName
// Created: JSR 2010-08-25
// -----------------------------------------------------------------------------
QString ResourceNetwork::GetLinkName( const std::string& resource, unsigned int i ) const
{
    const ResourceNode* node = FindResourceNode( resource );
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
// Name: ResourceNetwork::DoUpdate
// Created: JSR 2010-09-01
// -----------------------------------------------------------------------------
void ResourceNetwork::DoUpdate( const MsgsSimToClient::MsgObjectUpdate& message )
{
    if( message.attributes().has_resource_networks() )
    {
        kernel::Entity_ABC* entity = objectResolver_.Find( id_ );
        for( int i = 0; i < message.attributes().resource_networks().network_size(); ++i )
            UpdateNetwork( entity, message.attributes().resource_networks().network( i ) );
    }
}

// -----------------------------------------------------------------------------
// Name: ResourceNetwork::DoUpdate
// Created: JSR 2010-09-01
// -----------------------------------------------------------------------------
void ResourceNetwork::DoUpdate( const MsgsSimToClient::MsgUrbanUpdate& message )
{
    if( message.attributes().has_infrastructures() )
    {
        kernel::Entity_ABC* entity = urbanResolver_.Find( id_ );
        for( int i = 0; i < message.attributes().infrastructures().resource_network_size(); ++i )
            UpdateNetwork( entity, message.attributes().infrastructures().resource_network( i ) );
    }
}

// -----------------------------------------------------------------------------
// Name: ResourceNetwork::UpdateNetwork
// Created: JSR 2010-08-31
// -----------------------------------------------------------------------------
void ResourceNetwork::UpdateNetwork( kernel::Entity_ABC* entity, const Common::ResourceNetwork& msg )
{
    std::string resource( msg.resource().name() );
    ResourceNode& node = resourceNodes_[ resource ];
    unsigned int oldMaxStock = node.maxStock_;
    unsigned int oldProduction = node.production_;
    unsigned int oldConsumption = node.consumption_;
    bool oldCritical = node.critical_;
    unsigned int oldStock = node.stock_;
    unsigned int oldFlow = node.totalFlow_;
    node.resource_ = resource;
    node.isEnabled_ = msg.enabled();
    node.production_ = msg.has_production() ? msg.production() : 0;
    node.maxStock_ = msg.has_max_stock() ? msg.max_stock() : 0;
    node.stock_ = msg.has_stock() ? msg.stock() : 0;
    node.consumption_ = msg.has_consumption() ? msg.consumption() : 0;
    node.critical_ = msg.has_critical() ? msg.critical() : false;
    node.links_.clear();
    node.totalFlow_ = 0;
    for( int j = 0; j < msg.link_size(); ++j )
    {
        ResourceLink link;
        link.urban_ = msg.link( j ).kind() == Common::ResourceNetwork_Link::urban;
        link.id_ = msg.link( j ).target_id();
        link.capacity_ = msg.link( j ).capacity();
        link.flow_ = msg.link( j ).flow();
        node.totalFlow_ += link.flow_;
        node.links_.push_back( link );
    }
    if( entity )
    {
        const QString baseName = tools::translate( "ResourceNetwork", "Resources Networks" ) + "/" + resource.c_str() + "/";
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
// Name: ResourceNetwork::SetColor
// Created: JSR 2010-08-20
// -----------------------------------------------------------------------------
void ResourceNetwork::SetColor( const std::string& resource ) const
{
    float red, green, blue;
    resourceNetworkResolver_.Get( resource ).GetColor( red, green, blue );
    glColor3f( red, green, blue );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetwork::UpdateStipple
// Created: JSR 2010-08-25
// -----------------------------------------------------------------------------
void ResourceNetwork::UpdateStipple( int value ) const
{
    long time = clock();
    value = std::min( value, 256 );
    unsigned short shift = ( unsigned short ) ( ( ( time * value ) >> 8 ) % 128 ) >> 3;
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
        const QString baseName = tools::translate( "ResourceNetwork", "Resources Networks" ) + "/" + node->second.resource_.c_str() + "/";
        dico.Register( *this, baseName + tools::translate( "ResourceNetwork", "Enabled" ), node->second.isEnabled_ );
        dico.Register( *this, baseName + tools::translate( "ResourceNetwork", "Total flow" ), node->second.totalFlow_ );
        dico.Register( *this, baseName + tools::translate( "ResourceNetwork", "Maximal stock" ), node->second.maxStock_ );
        dico.Register( *this, baseName + tools::translate( "ResourceNetwork", "Stock" ), node->second.stock_ );
        dico.Register( *this, baseName + tools::translate( "ResourceNetwork", "Production" ), node->second.production_ );
        dico.Register( *this, baseName + tools::translate( "ResourceNetwork", "Consumption" ), node->second.consumption_ );
        dico.Register( *this, baseName + tools::translate( "ResourceNetwork", "Vital consumption" ), node->second.critical_ );
    }
}
