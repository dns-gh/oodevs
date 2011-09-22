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
#include "clients_kernel/UrbanPositions_ABC.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/Viewport_ABC.h"
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
ResourceNetwork::ResourceNetwork( Controllers& controllers, unsigned int id, const tools::Resolver_ABC< TerrainObjectProxy >& urbanResolver, const tools::Resolver_ABC< Object_ABC >& objectResolver, const tools::StringResolver< ResourceNetworkType >& resourceNetworkResolver, PropertiesDictionary& dico )
    : controllers_            ( controllers )
    , id_                     ( id )
    , urbanResolver_          ( urbanResolver )
    , objectResolver_         ( objectResolver )
    , resourceNetworkResolver_( resourceNetworkResolver )
{
    CreateDictionary( dico );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetwork constructor
// Created: JSR 2010-08-19
// -----------------------------------------------------------------------------
ResourceNetwork::ResourceNetwork( Controllers& controllers, unsigned int id, const tools::Resolver_ABC< TerrainObjectProxy >& urbanResolver, const tools::Resolver_ABC< Object_ABC >& objectResolver, const tools::StringResolver< ResourceNetworkType >& resourceNetworkResolver, const sword::UrbanAttributes_Infrastructures& msg, PropertiesDictionary& dico )
    : controllers_            ( controllers )
    , id_                     ( id )
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
ResourceNetwork::ResourceNetwork( Controllers& controllers, unsigned int id, const tools::Resolver_ABC< gui::TerrainObjectProxy >& urbanResolver, const tools::Resolver_ABC< Object_ABC >& objectResolver, const tools::StringResolver< ResourceNetworkType >& resourceNetworkResolver, const sword::ObjectAttributeResourceNetwork& msg, PropertiesDictionary& dico )
    : controllers_            ( controllers )
    , id_                     ( id )
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
// Created: JSR 2010-08-19
// -----------------------------------------------------------------------------
void ResourceNetwork::Draw( const Viewport_ABC& viewport, const GlTools_ABC& tools ) const
{
    int filter = controllers_.options_.GetOption( "ResourceNetworks", 0 ).To< int >();
    if( filter == 1 )// off
        return;
    if( filter == 3 && !IsSelected() ) // selected outgoing
        return;
    const Entity_ABC* object = FindEntity( id_ );
    if( !object )
        return;
    Point2f from = GetPosition( *object );
    glPushAttrib( GL_LINE_BIT );
    glLineWidth( 1.f );
    for( CIT_ResourceNodes node = resourceNodes_.begin(); node != resourceNodes_.end(); ++node )
    {
        SetColor( node->second.resource_ );
        if( node->second.links_.size() > 0 )
        {
            glEnable( GL_LINE_STIPPLE );
            for( ResourceNetwork_ABC::CIT_ResourceLinks link = node->second.links_.begin(); link != node->second.links_.end(); ++link )
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
        Entity_ABC* entity = objectResolver_.Find( id_ );
        for( int i = 0; i < message.attributes().resource_networks().network_size(); ++i )
            UpdateNetwork( entity, message.attributes().resource_networks().network( i ) );
    }
}

// -----------------------------------------------------------------------------
// Name: ResourceNetwork::DoUpdate
// Created: JSR 2010-09-01
// -----------------------------------------------------------------------------
void ResourceNetwork::DoUpdate( const sword::UrbanUpdate& message )
{
    if( message.attributes().has_infrastructures() )
    {
        Entity_ABC* entity = urbanResolver_.Find( id_ );
        for( int i = 0; i < message.attributes().infrastructures().resource_network_size(); ++i )
            UpdateNetwork( entity, message.attributes().infrastructures().resource_network( i ) );
    }
}

// -----------------------------------------------------------------------------
// Name: ResourceNetwork::UpdateNetwork
// Created: JSR 2010-08-31
// -----------------------------------------------------------------------------
void ResourceNetwork::UpdateNetwork( Entity_ABC* entity, const sword::ResourceNetwork& msg )
{
    std::string resource( msg.resource().name() );
    ResourceNode& node = resourceNodes_[ resource ];
    unsigned int oldMaxStock = node.maxStock_;
    unsigned int oldProduction = node.production_;
    unsigned int oldConsumption = node.consumption_;
    unsigned int oldNeeds = node.needs_;
    float oldSatisfaction = node.satisfaction_;
    bool oldCritical = node.critical_;
    unsigned int oldStock = node.stock_;
    unsigned int oldFlow = node.totalFlow_;
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
        if( node.totalFlow_ != oldFlow && node.links_.size() )
            controllers_.controller_.Update( DictionaryUpdated( *entity, baseName + tools::translate( "ResourceNetwork", "Total flow" ) ) );
        if( node.stock_ != oldStock )
            controllers_.controller_.Update( DictionaryUpdated( *entity, baseName + tools::translate( "ResourceNetwork", "Stock" ) ) );
        if( node.maxStock_ != oldMaxStock )
            controllers_.controller_.Update( DictionaryUpdated( *entity, baseName + tools::translate( "ResourceNetwork", "Maximal stock" ) ) );
        if( node.production_ != oldProduction )
            controllers_.controller_.Update( DictionaryUpdated( *entity, baseName + tools::translate( "ResourceNetwork", "Production" ) ) );
        if( node.consumption_ != oldConsumption )
            controllers_.controller_.Update( DictionaryUpdated( *entity, baseName + tools::translate( "ResourceNetwork", "Consumption" ) ) );
        if( node.critical_ != oldCritical )
            controllers_.controller_.Update( DictionaryUpdated( *entity, baseName + tools::translate( "ResourceNetwork", "Vital consumption" ) ) );
        if( node.needs_ != oldNeeds )
            controllers_.controller_.Update( DictionaryUpdated( *entity, baseName + tools::translate( "ResourceNetwork", "Needs" ) ) );
        if( node.satisfaction_ != oldSatisfaction )
            controllers_.controller_.Update( DictionaryUpdated( *entity, baseName + tools::translate( "ResourceNetwork", "Satisfaction" ) ) );
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
    glColor4f( red, green, blue, 0.5f );
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
// Name: ResourceNetwork::CreateDictionary
// Created: JSR 2010-08-23
// -----------------------------------------------------------------------------
void ResourceNetwork::CreateDictionary( PropertiesDictionary& dico ) const
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
        dico.Register( *this, baseName + tools::translate( "ResourceNetwork", "Needs" ), node->second.needs_ );
        dico.Register( *this, baseName + tools::translate( "ResourceNetwork", "Satisfaction" ), node->second.satisfaction_ );
    }
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
