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
#include "clients_gui/GlTools_ABC.h"
#include "clients_gui/Viewport_ABC.h"
#include "clients_kernel/UrbanObject_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_gui/DictionaryUpdated.h"
#include "clients_kernel/ResourceNetworkType.h"
#include "clients_kernel/Options.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/UrbanPositions_ABC.h"
#include "clients_kernel/Positions.h"
#include "clients_gui/PropertiesDictionary.h"
#include "clients_kernel/Tools.h"
#include "protocol/Protocol.h"
#include <time.h>

using namespace geometry;
using namespace gui;
using namespace kernel;

int ResourceNetwork::maxFlow_ = 0;
double ResourceNetwork::stippleFactor_ = 1;

// -----------------------------------------------------------------------------
// Name: ResourceNetwork constructor
// Created: JSR 2011-03-04
// -----------------------------------------------------------------------------
ResourceNetwork::ResourceNetwork( Controllers& controllers, kernel::Entity_ABC& entity, const tools::Resolver_ABC< UrbanObject_ABC >& urbanResolver, const tools::Resolver_ABC< Object_ABC >& objectResolver, const tools::StringResolver< ResourceNetworkType >& resourceNetworkResolver, gui::PropertiesDictionary* dico )
    : controllers_( controllers )
    , entity_( entity )
    , urbanResolver_( urbanResolver )
    , objectResolver_( objectResolver )
    , resourceNetworkResolver_( resourceNetworkResolver )
    , dico_( dico )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ResourceNetwork constructor
// Created: JSR 2010-08-19
// -----------------------------------------------------------------------------
ResourceNetwork::ResourceNetwork( Controllers& controllers, kernel::Entity_ABC& entity, const tools::Resolver_ABC< UrbanObject_ABC >& urbanResolver, const tools::Resolver_ABC< Object_ABC >& objectResolver, const tools::StringResolver< ResourceNetworkType >& resourceNetworkResolver, const sword::UrbanAttributes_Infrastructures& msg, gui::PropertiesDictionary* dico )
    : controllers_( controllers )
    , entity_( entity )
    , urbanResolver_( urbanResolver )
    , objectResolver_( objectResolver )
    , resourceNetworkResolver_( resourceNetworkResolver )
    , dico_( dico )
{
    UpdateDictionary( msg.resource_network() );
    for( int i = 0; i < msg.resource_network_size(); ++i )
        UpdateNetwork( 0, msg.resource_network( i ) );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetwork constructor
// Created: JSR 2010-08-31
// -----------------------------------------------------------------------------
ResourceNetwork::ResourceNetwork( Controllers& controllers, kernel::Entity_ABC& entity, const tools::Resolver_ABC< UrbanObject_ABC >& urbanResolver, const tools::Resolver_ABC< Object_ABC >& objectResolver, const tools::StringResolver< ResourceNetworkType >& resourceNetworkResolver, const sword::ObjectAttributeResourceNetwork& msg, gui::PropertiesDictionary* dico )
    : controllers_( controllers )
    , entity_( entity )
    , urbanResolver_( urbanResolver )
    , objectResolver_( objectResolver )
    , resourceNetworkResolver_( resourceNetworkResolver )
    , dico_( dico )
{
    UpdateDictionary( msg.network() );
    for( int i = 0; i < msg.network_size(); ++i )
        UpdateNetwork( 0, msg.network( i ) );
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
// Name: ResourceNetwork::FindEntity
// Created: JSR 2011-01-18
// -----------------------------------------------------------------------------
const Entity_ABC* ResourceNetwork::FindEntity( unsigned int id ) const
{
    const Entity_ABC* object = urbanResolver_.Find( id );
    if( !object )
        object = objectResolver_.Find( id );
    return object;
}

// -----------------------------------------------------------------------------
// Name: ResourceNetwork::Draw
// Created: LGY 2013-03-07
// -----------------------------------------------------------------------------
void ResourceNetwork::Draw( const gui::Viewport_ABC& viewport, const gui::GlTools_ABC& tools, const geometry::Point2f& from, float alpha ) const
{
    int filter = controllers_.options_.GetOption( "ResourceNetworks", 0 ).To< int >();
    if( filter == 1 )// off
        return;
    if( filter == 3 && !IsSelected() ) // selected outgoing
        return;
    glPushAttrib( GL_LINE_BIT );
    glLineWidth( 1.f );
    for( auto node = resourceNodes_.begin(); node != resourceNodes_.end(); ++node )
    {
        SetColor( node->second.resource_, alpha );
        if( node->second.links_.size() > 0 )
        {
            glEnable( GL_LINE_STIPPLE );
            for( auto link = node->second.links_.begin(); link != node->second.links_.end(); ++link )
            {
                const Entity_ABC* target = FindEntity( link->id_ );
                if( !target )
                    continue;
                if( filter == 2 )  // selected all
                {
                    const ResourceNetwork_ABC* resourceTarget = target->Retrieve< ResourceNetwork_ABC >();
                    if( !resourceTarget || ( !IsSelected() && !resourceTarget->IsSelected() ) )
                        continue;
                }
                Point2f to = GetPosition( *target );
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
    const Entity_ABC* entity = FindEntity( link.id_ );
    if( !entity )
        return QString();
    QString ret = entity->GetName();
    if( ret.isEmpty() )
        ret = QString::number( link.id_ );
    return ret;
}

// -----------------------------------------------------------------------------
// Name: ResourceNetwork::DoUpdate
// Created: JSR 2010-09-01
// -----------------------------------------------------------------------------
void ResourceNetwork::DoUpdate( const sword::ObjectUpdate& message )
{
    if( message.attributes().has_resource_networks() )
    {
        UpdateDictionary( message.attributes().resource_networks().network() );
        for( int i = 0; i < message.attributes().resource_networks().network_size(); ++i )
            UpdateNetwork( &entity_, message.attributes().resource_networks().network( i ) );
    }
    controllers_.controller_.Update( static_cast< const gui::ResourceNetwork_ABC& >( *this ) );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetwork::DoUpdate
// Created: JSR 2010-09-01
// -----------------------------------------------------------------------------
void ResourceNetwork::DoUpdate( const sword::UrbanUpdate& message )
{
    if( message.attributes().has_infrastructures() )
    {
        UpdateDictionary( message.attributes().infrastructures().resource_network() );
        for( int i = 0; i < message.attributes().infrastructures().resource_network_size(); ++i )
            UpdateNetwork( &entity_, message.attributes().infrastructures().resource_network( i ) );
    }
    controllers_.controller_.Update( static_cast< const gui::ResourceNetwork_ABC& >( *this ) );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetwork::UpdateNetwork
// Created: JSR 2010-08-31
// -----------------------------------------------------------------------------
void ResourceNetwork::UpdateNetwork( Entity_ABC* entity, const sword::ResourceNetwork& msg )
{
    std::string resource( msg.resource().name() );
    ResourceNode& node = resourceNodes_[ resource ];
    node.resource_ = resource;
    node.isEnabled_ = msg.enabled();
    node.production_ = msg.has_production() ? msg.production() : 0;
    node.maxStock_ = msg.has_max_stock() ? msg.max_stock() : 0;
    node.stock_ = msg.has_stock() ? msg.stock() : 0;
    node.consumption_ = msg.has_consumption() ? msg.consumption() : 0;
    node.needs_ = msg.has_max_consumption() ? msg.max_consumption() : 0;
    unsigned int currentConsumption = msg.has_current_consumption() ? msg.current_consumption() : 0;
    node.satisfaction_ = node.needs_ != 0 ? ( static_cast< float >( currentConsumption ) / static_cast< float >( node.needs_ ) ) : 1.f;
    node.critical_ = msg.has_critical() ? msg.critical() : false;
    node.links_.clear();
    node.totalFlow_ = 0;
    for( int j = 0; j < msg.link_size(); ++j )
    {
        ResourceLink link;
        link.id_ = msg.link( j ).object().id();
        link.urban_ = ( urbanResolver_.Find( link.id_ ) != 0 );
        link.capacity_ = msg.link( j ).capacity();
        link.flow_ = msg.link( j ).flow();
        node.totalFlow_ += link.flow_;
        node.links_.push_back( link );
    }
    if( entity )
    {
        const QString baseName = tools::translate( "ResourceNetwork", "Resources Networks" ) + "/" + resource.c_str() + "/";
        controllers_.controller_.Update( gui::DictionaryUpdated( *entity, baseName ) );
    }
}

// -----------------------------------------------------------------------------
// Name: ResourceNetwork::SetColor
// Created: JSR 2010-08-20
// -----------------------------------------------------------------------------
void ResourceNetwork::SetColor( const std::string& resource, float alpha ) const
{
    float red, green, blue;
    resourceNetworkResolver_.Get( resource ).GetColor( red, green, blue );
    glColor4f( red, green, blue, alpha );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetwork::UpdateStipple
// Created: JSR 2010-08-25
// -----------------------------------------------------------------------------
void ResourceNetwork::UpdateStipple( int value ) const
{
    if( maxFlow_ < value )
    {
        maxFlow_ = value;
        stippleFactor_ = 128. / maxFlow_;
    }
    long time = clock();
    if( value )
        value = std::max( 5, static_cast< int >( stippleFactor_ * value ) );
    unsigned short shift = ( unsigned short ) ( ( ( time * value ) >> 8 ) % 128 ) >> 3;
    glLineStipple( 1, 0x00FF << shift | 0x00FF >> ( 16-shift ) );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetwork::UpdateDictionary
// Created: JSR 2013-05-24
// -----------------------------------------------------------------------------
void ResourceNetwork::UpdateDictionary( const google::protobuf::RepeatedPtrField< sword::ResourceNetwork >& networks )
{
    if( !dico_ )
        return;
    std::vector< std::string > dicoResources;
    std::swap( dicoResources, dicoResources_ );
    for( auto it = networks.begin(); it != networks.end(); ++it )
    {
        const std::string resource = it->resource().name();
        auto findIt = std::find( dicoResources.begin(), dicoResources.end(), resource );
        if( findIt != dicoResources.end() )
        {
            dicoResources_.push_back( resource );
            dicoResources.erase( findIt );
        }
        else
        {
            const ResourceNode& node = resourceNodes_[ resource ];
            const QString baseName = tools::translate( "ResourceNetwork", "Resources Networks" ) + "/" + resource.c_str() + "/";
            dico_->Register( entity_, baseName + tools::translate( "ResourceNetwork", "Enabled" ), node.isEnabled_ );
            dico_->Register( entity_, baseName + tools::translate( "ResourceNetwork", "Total flow" ), node.totalFlow_ );
            dico_->Register( entity_, baseName + tools::translate( "ResourceNetwork", "Maximal stock" ), node.maxStock_ );
            dico_->Register( entity_, baseName + tools::translate( "ResourceNetwork", "Stock" ), node.stock_ );
            dico_->Register( entity_, baseName + tools::translate( "ResourceNetwork", "Production" ), node.production_ );
            dico_->Register( entity_, baseName + tools::translate( "ResourceNetwork", "Consumption" ), node.consumption_ );
            dico_->Register( entity_, baseName + tools::translate( "ResourceNetwork", "Vital consumption" ), node.critical_ );
            dico_->Register( entity_, baseName + tools::translate( "ResourceNetwork", "Needs" ), node.needs_ );
            dico_->Register( entity_, baseName + tools::translate( "ResourceNetwork", "Satisfaction" ), node.satisfaction_ );
            dicoResources_.push_back( resource );
        }
    }
    for( auto it = dicoResources.begin(); it != dicoResources.end(); ++it )
        dico_->Remove( tools::translate( "ResourceNetwork", "Resources Networks" ) + "/" + it->c_str() + "/" );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetwork::GetPosition
// Created: LGY 2011-04-18
// -----------------------------------------------------------------------------
geometry::Point2f ResourceNetwork::GetPosition( const kernel::Entity_ABC& entity ) const
{
    if( const UrbanPositions_ABC* positions = entity.Retrieve< UrbanPositions_ABC >() )
        return positions->Barycenter();
    return entity.Get< Positions >().GetPosition();
}
