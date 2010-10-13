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
#include "clients_kernel/ResourceNetworkType.h"
#include "clients_kernel/Options.h"
#include "clients_kernel/Viewport_ABC.h"
#include <boost/bind.hpp>
#include <urban/ResourceNetworkAttribute.h>
#include <xeumeuleu/xml.hpp>

using namespace geometry;

// -----------------------------------------------------------------------------
// Name: ResourceNetworkAttribute constructor
// Created: JSR 2010-09-07
// -----------------------------------------------------------------------------
ResourceNetworkAttribute::ResourceNetworkAttribute( kernel::Controllers& controllers, xml::xistream& xis, unsigned int id, const tools::Resolver_ABC< gui::TerrainObjectProxy >& urbanResolver, const tools::StringResolver< kernel::ResourceNetworkType >& resourceNetworkResolver )
    : controllers_( controllers )
    , id_( id )
    , urbanResolver_( urbanResolver )
    , resourceNetworkResolver_( resourceNetworkResolver )
    , needSaving_( false )
{
    xis >> xml::list( "node", *this, &ResourceNetworkAttribute::ReadNode );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkAttribute constructor
// Created: JSR 2010-09-20
// -----------------------------------------------------------------------------
ResourceNetworkAttribute::ResourceNetworkAttribute( kernel::Controllers& controllers, const urban::ResourceNetworkAttribute& network, unsigned int id, const tools::Resolver_ABC< gui::TerrainObjectProxy >& urbanResolver, const tools::StringResolver< kernel::ResourceNetworkType >& resourceNetworkResolver )
    : controllers_( controllers )
    , id_( id )
    , urbanResolver_( urbanResolver )
    , resourceNetworkResolver_( resourceNetworkResolver )
    , needSaving_( false )
{
    const urban::ResourceNetworkAttribute::T_ResourceNodes& nodes = network.GetResourceNodes();
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
            link.id_ = ( *itLink ).id_;
            link.capacity_ = ( *itLink ).capacity_;
            link.urban_ = true;
            node.links_.push_back( link );
        }
    }
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
        entity = &urbanResolver_.Get( link.id_ );
    // TODO
    /*else
        entity = &objectResolver_.Get( link.id_ );*/
    else 
        return QString::number( link.id_ );
    QString ret = entity->GetName();
    if( ret.isEmpty() )
        ret = QString::number( link.id_ );
    return ret;
}
    
// -----------------------------------------------------------------------------
// Name: ResourceNetworkAttribute::Draw
// Created: JSR 2010-09-07
// -----------------------------------------------------------------------------
void ResourceNetworkAttribute::Draw( const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const
{
    // TODO les objets ne sont pas g�r�s : il y a diff�rents objectResolver, un par Team. Refactoriser �a pour n'avoir qu'un seul r�solver comme dans Gaming (faire une extension Objets, s'inspirer de Populations).

    // TODO utiliser un enum pour les options
    char filter = controllers_.options_.GetOption( "ResourceNetworks", 0 ).To< char >();
    if( filter == 1 )// off
        return;
    if( filter == 3 && !IsSelected() ) // selected outgoing
        return;
    // TODO
    /*Point2f from = isUrban_ ? urbanResolver_.Get( id_ ).Barycenter()
        : objectResolver_.Get( id_ ).Get< kernel::Positions >().GetPosition();*/
    Point2f from = urbanResolver_.Get( id_ ).Barycenter();

    glPushAttrib( GL_LINE_BIT );
    glLineWidth( 1.f );

    for( CIT_ResourceNodes node = resourceNodes_.begin(); node != resourceNodes_.end(); ++node )
    {
        SetColor( node->second.resource_ );
        if( node->second.links_.size() > 0 )
        {
            for( std::vector< ResourceLink >::const_iterator link = node->second.links_.begin(); link != node->second.links_.end(); ++link )
            {
                // TODO
                //kernel::Entity_ABC* target = link->urban_ ? static_cast< kernel::Entity_ABC* >( urbanResolver_.Find( link->id_ ) ) : objectResolver_.Find( link->id_ );
                kernel::Entity_ABC* target = link->urban_ ? static_cast< kernel::Entity_ABC* >( urbanResolver_.Find( link->id_ ) ) : 0;
                if( !target )
                    continue;
                if( filter == 2 )  // selected all
                {
                    ResourceNetwork_ABC* resourceTarget = target->Retrieve< ResourceNetwork_ABC >();
                    if( !resourceTarget || ( !IsSelected() && !resourceTarget->IsSelected() ) )
                        continue;
                }
                // TODO
                /*Point2f to = link->urban_ ? urbanResolver_.Get( link->id_ ).Barycenter()
                                          : objectResolver_.Get( link->id_ ).Get< kernel::Positions >().GetPosition();*/
                Point2f to = urbanResolver_.Get( link->id_ ).Barycenter();
                if( viewport.IsVisible( Rectangle2f( from, to ) ) )
                    tools.DrawArrow( from, to );
            }
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
        >> xml::list( "link", *this, &ResourceNetworkAttribute::ReadLink, boost::ref( node ) );
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
    {
        if( node.links_[ i ].id_ == link.id_ && node.links_[ i ].urban_ == link.urban_ )
        {
            node.links_[ i ].capacity_ = link.capacity_;
            return;
        }
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
    resourceNetworkResolver_.Get( resource ).GetColor( red, green, blue );
    glColor4f( red, green, blue, 0.5f );
}
