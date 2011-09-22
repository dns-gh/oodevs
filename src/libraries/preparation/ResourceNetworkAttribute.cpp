// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "ResourceNetworkAttribute.h"
#include "clients_gui/TerrainObjectProxy.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Options.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/UrbanPositions_ABC.h"
#include "clients_kernel/ResourceNetworkType.h"
#include "clients_kernel/Viewport_ABC.h"
#include <urban/ResourceNetworkAttribute.h>
#include <xeumeuleu/xml.hpp>

using namespace geometry;

// -----------------------------------------------------------------------------
// Name: ResourceNetworkAttribute constructor
// Created: JSR 2010-09-07
// -----------------------------------------------------------------------------
ResourceNetworkAttribute::ResourceNetworkAttribute( kernel::Controllers& controllers, xml::xistream& xis, const geometry::Point2f position
                                                  , const T_Urbans& urbans, const T_Objects& objects, const T_Resources& resources )
    : controllers_( controllers )
    , position_   ( position )
    , urbans_     ( urbans )
    , objects_    ( objects )
    , resources_  ( resources )
    , needSaving_ ( true )
{
    xis >> xml::list( "node", *this, &ResourceNetworkAttribute::ReadNode );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkAttribute constructor
// Created: JSR 2010-09-20
// -----------------------------------------------------------------------------
ResourceNetworkAttribute::ResourceNetworkAttribute( kernel::Controllers& controllers, const urban::ResourceNetworkAttribute* network, const geometry::Point2f position
                                                  , const T_Urbans& urbans, const T_Objects& objects, const T_Resources& resources )
    : controllers_( controllers )
    , position_   ( position )
    , urbans_     ( urbans )
    , objects_    ( objects )
    , resources_  ( resources )
    , needSaving_ ( false )
{
    if( network )
    {
        const urban::ResourceNetworkAttribute::T_ResourceNodes& nodes = network->GetResourceNodes();
        for( urban::ResourceNetworkAttribute::CIT_ResourceNodes it = nodes.begin(); it != nodes.end(); ++it )
        {
            ResourceNode& node = resourceNodes_[ it->second.resource_ ];
            node.resource_ = it->second.resource_;
            node.isEnabled_ = it->second.isEnabled_;
            node.production_ = it->second.production_;
            node.consumption_ = it->second.consumption_;
            node.critical_ = it->second.critical_;
            node.maxStock_ = it->second.maxStock_;
            for( std::vector< urban::ResourceNetworkAttribute::ResourceLink >::const_iterator itLink = it->second.links_.begin(); itLink != it->second.links_.end(); ++itLink )
            {
                ResourceLink link;
                link.id_ = itLink->id_;
                link.capacity_ = itLink->capacity_;
                link.urban_ = true;
                node.links_.push_back( link );
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkAttribute constructor
// Created: JSR 2011-02-23
// -----------------------------------------------------------------------------
ResourceNetworkAttribute::ResourceNetworkAttribute( kernel::Controllers& controllers, const geometry::Point2f position, const T_Urbans& urbans, const T_Objects& objects, const T_Resources& resources )
    : controllers_( controllers )
    , position_   ( position )
    , urbans_     ( urbans )
    , objects_    ( objects )
    , resources_  ( resources )
    , needSaving_ ( true )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkAttribute destructor
// Created: JSR 2010-09-07
// -----------------------------------------------------------------------------
ResourceNetworkAttribute::~ResourceNetworkAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkAttribute::GetLinkName
// Created: JSR 2010-09-07
// -----------------------------------------------------------------------------
QString ResourceNetworkAttribute::GetLinkName( const std::string& resource, unsigned int i ) const
{
    const ResourceNode* node = FindResourceNode( resource );
    if( node == 0 || i >= node->links_.size() )
        return "";
    const ResourceLink& link = node->links_[ i ];
    kernel::Entity_ABC* entity = 0;
    if( link.urban_ )
        entity = &urbans_.Get( link.id_ );
    else
        entity = &objects_.Get( link.id_ );
    QString ret = entity->GetName();
    if( ret.isEmpty() )
        ret = QString::number( link.id_ );
    return ret;
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkAttribute::FindOrCreateResourceNode
// Created: JSR 2011-02-25
// -----------------------------------------------------------------------------
kernel::ResourceNetwork_ABC::ResourceNode& ResourceNetworkAttribute::FindOrCreateResourceNode( std::string resource )
{
    needSaving_ = true;
    return kernel::ResourceNetwork_ABC::FindOrCreateResourceNode( resource );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkAttribute::Draw
// Created: JSR 2010-09-07
// -----------------------------------------------------------------------------
void ResourceNetworkAttribute::Draw( const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const
{
    int filter = controllers_.options_.GetOption( "ResourceNetworks", 0 ).To< int >();
    if( filter == 1 )// off
        return;
    if( filter == 3 && !IsSelected() ) // selected outgoing
        return;
    Point2f from = position_;

    glPushAttrib( GL_LINE_BIT );
    glLineWidth( 1.f );

    for( CIT_ResourceNodes node = resourceNodes_.begin(); node != resourceNodes_.end(); ++node )
    {
        SetColor( node->second.resource_ );
        if( node->second.links_.size() > 0 )
            for( ResourceNetwork_ABC::CIT_ResourceLinks link = node->second.links_.begin(); link != node->second.links_.end(); ++link )
            {
                kernel::Entity_ABC* target = link->urban_ ? static_cast< kernel::Entity_ABC* >( urbans_.Find( link->id_ ) ) : objects_.Find( link->id_ );
                if( !target )
                    continue;
                if( filter == 2 )  // selected all
                {
                    ResourceNetwork_ABC* resourceTarget = target->Retrieve< ResourceNetwork_ABC >();
                    if( !resourceTarget || ( !IsSelected() && !resourceTarget->IsSelected() ) )
                        continue;
                }
                Point2f to = link->urban_ ? urbans_.Get( link->id_ ).Get< kernel::UrbanPositions_ABC >().Barycenter()
                                          : objects_.Get( link->id_ ).Get< kernel::Positions >().GetPosition();
                if( viewport.IsVisible( Rectangle2f( from, to ) ) )
                    tools.DrawArrow( from, to );
            }
        else
            if( filter == 0 || IsSelected() )
                tools.DrawCircle( from, 20.0 );
    }
    glPopAttrib();
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkAttribute::SerializeAttributes
// Created: JSR 2010-09-08
// -----------------------------------------------------------------------------
void ResourceNetworkAttribute::SerializeAttributes( xml::xostream& xos ) const
{
    if( needSaving_ )
    {
        xos << xml::start( "resources" );
        for( CIT_ResourceNodes it = resourceNodes_.begin(); it != resourceNodes_.end(); ++it )
        {
            const ResourceNetwork_ABC::ResourceNode& node = it->second;
            xos << xml::start( "node" )
                << xml::attribute( "resource-type", node.resource_ )
                << xml::attribute( "enabled", node.isEnabled_ )
                << xml::attribute( "production", node.production_ )
                << xml::attribute( "consumption", node.consumption_ )
                << xml::attribute( "stock", node.maxStock_ )
                << xml::attribute( "initial-stock", node.stock_ )
                << xml::attribute( "critical-consumption", node.critical_ );
                for( unsigned int i = 0; i < node.links_.size(); ++i )
                {
                    xos << xml::start( "link" )
                        << xml::attribute( "kind", node.links_[ i ].urban_ ? "urban-object" : "terrain-object" )
                        << xml::attribute( "target", node.links_[ i ].id_ )
                        << xml::attribute( "capacity", node.links_[ i ].capacity_ )
                        << xml::end();
                }
            xos << xml::end();
        }
        xos << xml::end;
    }
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkAttribute::SetOverriden
// Created: JSR 2010-09-09
// -----------------------------------------------------------------------------
void ResourceNetworkAttribute::SetOverriden( bool& overriden ) const
{
    if( needSaving_ )
        overriden = true;
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkAttribute::Update
// Created: JSR 2010-09-08
// -----------------------------------------------------------------------------
void ResourceNetworkAttribute::Update( xml::xistream& xis )
{
    xis >> xml::list( "node", *this, &ResourceNetworkAttribute::ReadNode );
    needSaving_ = true;
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkAttribute::Update
// Created: JSR 2010-09-09
// -----------------------------------------------------------------------------
void ResourceNetworkAttribute::Update( const kernel::ResourceNetwork_ABC::T_ResourceNodes& nodes )
{
    resourceNodes_ = nodes;
    needSaving_ = true;
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkAttribute::ReadNode
// Created: JSR 2010-09-07
// -----------------------------------------------------------------------------
void ResourceNetworkAttribute::ReadNode( xml::xistream& xis )
{
    std::string resource = xis.attribute< std::string >( "resource-type" );
    ResourceNode& node = resourceNodes_[ resource ];
    node.resource_ = resource;
    xis >> xml::optional >> xml::attribute( "enabled", node.isEnabled_ )
        >> xml::optional >> xml::attribute( "production", node.production_ )
        >> xml::optional >> xml::attribute( "stock", node.maxStock_ )
        >> xml::optional >> xml::attribute( "initial-stock", node.stock_ )
        >> xml::optional >> xml::attribute( "consumption", node.consumption_ )
        >> xml::optional >> xml::attribute( "critical-consumption", node.critical_ )
        >> xml::list( "link", *this, &ResourceNetworkAttribute::ReadLink, node );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkAttribute::ReadLink
// Created: JSR 2010-09-07
// -----------------------------------------------------------------------------
void ResourceNetworkAttribute::ReadLink( xml::xistream& xis, ResourceNode& node )
{
    ResourceLink link;
    link.id_ = xis.attribute< unsigned int >( "target" );
    link.capacity_ = xis.attribute< int >( "capacity" );
    link.urban_ = true;
    if( xis.has_attribute( "kind" ) )
        link.urban_ = ( xis.attribute< std::string >( "kind" ) == "urban-object" );
    for( unsigned int i = 0; i < node.links_.size(); ++i )
        if( node.links_[ i ].id_ == link.id_ && node.links_[ i ].urban_ == link.urban_ )
        {
            node.links_[ i ].capacity_ = link.capacity_;
            return;
        }
    node.links_.push_back( link );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkAttribute::SetColor
// Created: JSR 2010-08-20
// -----------------------------------------------------------------------------
void ResourceNetworkAttribute::SetColor( const std::string& resource ) const
{
    float red, green, blue;
    resources_.Get( resource ).GetColor( red, green, blue );
    glColor4f( red, green, blue, 0.5f );
}
