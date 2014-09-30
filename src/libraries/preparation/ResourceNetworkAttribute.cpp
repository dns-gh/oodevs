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
#include "clients_gui/GlTools_ABC.h"
#include "clients_gui/Viewport_ABC.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/UrbanObject_ABC.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/OptionsController.h"
#include "clients_kernel/OptionVariant.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/UrbanPositions_ABC.h"
#include "clients_kernel/ResourceNetworkType.h"
#include <xeumeuleu/xml.hpp>

using namespace gui;

// -----------------------------------------------------------------------------
// Name: ResourceNetworkAttribute constructor
// Created: JSR 2010-09-07
// -----------------------------------------------------------------------------
ResourceNetworkAttribute::ResourceNetworkAttribute( kernel::Controllers& controllers, xml::xistream& xis, bool isUrban,
                                                    const T_Urbans& urbans, const T_Objects& objects, const T_Resources& resources )
    : controllers_( controllers )
    , isUrban_    ( isUrban )
    , urbans_     ( urbans )
    , objects_    ( objects )
    , resources_  ( resources )
{
    if( isUrban_ )
    {
        if( xis.has_child( "resources" ) )
        {
            xis >> xml::start( "resources" );
            Update( xis );
            xis >> xml::end;
        }
    }
    else
        Update( xis );
    controllers_.controller_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkAttribute constructor
// Created: JSR 2011-02-23
// -----------------------------------------------------------------------------
ResourceNetworkAttribute::ResourceNetworkAttribute( kernel::Controllers& controllers, bool isUrban,
                                                    const T_Urbans& urbans, const T_Objects& objects, const T_Resources& resources )
    : controllers_( controllers )
    , isUrban_    ( isUrban )
    , urbans_     ( urbans )
    , objects_    ( objects )
    , resources_  ( resources )
{
    controllers_.controller_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkAttribute destructor
// Created: JSR 2010-09-07
// -----------------------------------------------------------------------------
ResourceNetworkAttribute::~ResourceNetworkAttribute()
{
    controllers_.controller_.Unregister( *this );
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
        entity = urbans_.Find( link.id_ );
    else
        entity = objects_.Find( link.id_ );
    QString ret;
    if( entity )
    {
        ret = entity->GetName();
        if( ret.isEmpty() )
            ret = QString::number( link.id_ );
    }
    return ret;
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkAttribute::Draw
// Created: LGY 2013-03-07
// -----------------------------------------------------------------------------
void ResourceNetworkAttribute::Draw( const gui::Viewport_ABC& viewport, const gui::GlTools_ABC& tools, const geometry::Point2f& from, float alpha ) const
{
    int filter = controllers_.options_.GetOption( "ResourceNetworks" ).To< int >();
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
            for( auto link = node->second.links_.begin(); link != node->second.links_.end(); ++link )
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
                geometry::Point2f to;
                if( link->urban_ )
                {
                    if( const kernel::UrbanObject_ABC* obj = urbans_.Find( link->id_ ) )
                        to = obj->Get< kernel::UrbanPositions_ABC >().Barycenter();
                    else
                        continue;
                }
                else
                {
                    if( const kernel::Object_ABC* obj = objects_.Find( link->id_ ) )
                        to = obj->Get< kernel::Positions >().GetPosition();
                    else
                        continue;
                }

                if( viewport.IsVisible( geometry::Rectangle2f( from, to ) ) )
                    tools.DrawArrow( from, to );
            }
        else
            if( filter == 0 || IsSelected() )
                tools.DrawCircle( from, 20.0 );
    }
    glPopAttrib();
}

namespace
{
    class IsUrbanDeleted
    {
    public:
        IsUrbanDeleted( const ResourceNetworkAttribute::T_Urbans& urbans )
            : urbans_( &urbans )
        {}
        bool operator() ( const gui::ResourceLink& link )
        {
            return link.urban_ && !urbans_->Find( link.id_ );
        }
    private:
        const ResourceNetworkAttribute::T_Urbans* urbans_;
    };
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkAttribute::CleanLinksToDeletedUrbanBlocks
// Created: LDC 2012-08-08
// -----------------------------------------------------------------------------
bool ResourceNetworkAttribute::CleanLinksToDeletedUrbanBlocks()
{
    bool cleaned = false;
    for( auto node = resourceNodes_.begin(); node != resourceNodes_.end(); ++node )
    {
        T_ResourceLinks& links = node->second.links_;
        size_t size = links.size();
        links.erase( std::remove_if( links.begin(), links.end(), IsUrbanDeleted( urbans_ ) ), links.end() );
        if( size != links.size() )
            cleaned = true;
    }
    return cleaned;
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkAttribute::SerializeAttributes
// Created: JSR 2012-09-04
// -----------------------------------------------------------------------------
void ResourceNetworkAttribute::SerializeAttributes( xml::xostream& xos ) const
{
    SerializeObjectAttributes( xos );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkAttribute::SerializeObjectAttributes
// Created: JSR 2010-09-08
// -----------------------------------------------------------------------------
void ResourceNetworkAttribute::SerializeObjectAttributes( xml::xostream& xos ) const
{
    const_cast< ResourceNetworkAttribute* >( this )->invalidResources_.clear();
    if( !isUrban_ || !resourceNodes_.empty() )
    {
        xos << xml::start( "resources" );
        for( auto it = resourceNodes_.begin(); it != resourceNodes_.end(); ++it )
        {
            const ResourceNode& node = it->second;
            xos << xml::start( "node" )
                << xml::attribute( "resource-type", node.resource_ );
            if( !node.isEnabled_ )
                xos << xml::attribute( "enabled", false );
            if( node.production_ )
                xos << xml::attribute( "production", node.production_ );
            if( node.consumption_ )
                xos << xml::attribute( "consumption", node.consumption_ );
            if( node.critical_ )
                xos << xml::attribute( "critical-consumption", true );
            if( node.maxStock_ )
                xos << xml::attribute( "stock", node.maxStock_ );
            if( node.stock_ )
                xos << xml::attribute( "initial-stock", node.stock_ );
            for( unsigned int i = 0; i < node.links_.size(); ++i )
            {
                xos << xml::start( "link" );
                if( !node.links_[ i ].urban_ )
                    xos << xml::attribute( "kind", "terrain-object" );
                xos << xml::attribute( "target", node.links_[ i ].id_ )
                    << xml::attribute( "capacity", node.links_[ i ].capacity_ )
                    << xml::end;
            }
            xos << xml::end;
        }
        xos << xml::end;
    }
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkAttribute::NotifyDeleted
// Created: JSR 2012-06-08
// -----------------------------------------------------------------------------
void ResourceNetworkAttribute::NotifyDeleted( const kernel::Object_ABC& object )
{
    RemoveLinks( false, object.GetId() );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkAttribute::NotifyDeleted
// Created: JSR 2012-06-08
// -----------------------------------------------------------------------------
void ResourceNetworkAttribute::NotifyDeleted( const kernel::UrbanObject_ABC& object )
{
    RemoveLinks( true, object.GetId() );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkAttribute::NotifyUpdated
// Created: LDC 2012-09-10
// -----------------------------------------------------------------------------
void ResourceNetworkAttribute::NotifyUpdated( const gui::ResourceLinkDeletion& deletion )
{
    RemoveLinks( deletion.isUrban_, deletion.id_, deletion.resource_ );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkAttribute::GetInvalidResources
// Created: JSR 2012-06-28
// -----------------------------------------------------------------------------
const std::set< std::string >& ResourceNetworkAttribute::GetInvalidResources() const
{
    return invalidResources_;
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkAttribute::Update
// Created: JSR 2010-09-08
// -----------------------------------------------------------------------------
void ResourceNetworkAttribute::Update( xml::xistream& xis )
{
    xis >> xml::list( "node", *this, &ResourceNetworkAttribute::ReadNode );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkAttribute::Update
// Created: JSR 2010-09-09
// -----------------------------------------------------------------------------
void ResourceNetworkAttribute::Update( const std::map< std::string, gui::ResourceNode >& nodes )
{
    resourceNodes_ = nodes;
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkAttribute::ReadNode
// Created: JSR 2010-09-07
// -----------------------------------------------------------------------------
void ResourceNetworkAttribute::ReadNode( xml::xistream& xis )
{
    std::string resource = xis.attribute< std::string >( "resource-type" );
    if( resources_.Find( resource ) == 0 )
    {
        invalidResources_.insert( resource );
        return;
    }
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
        link.urban_ = ( xis.attribute< std::string >( "kind" ) != "terrain-object" );
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
void ResourceNetworkAttribute::SetColor( const std::string& resource, float alpha ) const
{
    float red, green, blue;
    resources_.Get( resource ).GetColor( red, green, blue );
    glColor4f( red, green, blue, alpha );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkAttribute::ReplaceLinksUrbanId
// Created: MMC 2013-05-29
// -----------------------------------------------------------------------------
void ResourceNetworkAttribute::ReplaceLinksUrbanId( unsigned long oldId, unsigned long newId )
{
    for( auto node = resourceNodes_.begin(); node != resourceNodes_.end(); ++node )
        node->second.ReplaceLinkId( oldId, newId );
}
