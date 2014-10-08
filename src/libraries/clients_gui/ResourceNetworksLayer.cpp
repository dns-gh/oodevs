// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "ResourceNetworksLayer.h"
#include "ResourceNetwork_ABC.h"
#include "Viewport_ABC.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/ObjectType.h"
#include "clients_kernel/UrbanPositions_ABC.h"
#include "clients_kernel/UrbanObject_ABC.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: ResourceNetworksLayer::ResourceNetworksLayer
// Created: LGY 2013-05-15
// -----------------------------------------------------------------------------
ResourceNetworksLayer::ResourceNetworksLayer( kernel::Controllers& controllers, GlTools_ABC& tools, ColorStrategy_ABC& strategy,
                        View_ABC& view, const kernel::Profile_ABC& profile )
    : EntityLayer< kernel::Entity_ABC >( controllers, tools, strategy, view, profile, eLayerTypes_ResourceNetworks )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworksLayer::~ResourceNetworksLayer
// Created: LGY 2013-05-15
// -----------------------------------------------------------------------------
ResourceNetworksLayer::~ResourceNetworksLayer()
{
    // NOTHING
}

namespace
{
    geometry::Point2f GetPosition( const kernel::Entity_ABC& entity )
    {
        if( const kernel::UrbanPositions_ABC* urbanPositions = entity.Retrieve< kernel::UrbanPositions_ABC >() )
            return urbanPositions->Barycenter();
        if( const kernel::Positions* positions = entity.Retrieve< kernel::Positions >() )
            return positions->GetPosition();
        return geometry::Point2f();
    }
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworksLayer::NotifyCreated
// Created: JSR 2013-06-07
// -----------------------------------------------------------------------------
void ResourceNetworksLayer::NotifyCreated( const kernel::Entity_ABC& entity )
{
    if( entity.GetTypeName() == kernel::UrbanObject_ABC::typeName_ ||
        ( entity.GetTypeName() == kernel::Object_ABC::typeName_ && static_cast< const kernel::Object_ABC& >( entity ).GetType().HasResourceNetwork() ) )
        EntityLayer< kernel::Entity_ABC >::NotifyCreated( entity );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworksLayer::NotifyDeleted
// Created: JSR 2013-06-07
// -----------------------------------------------------------------------------
void ResourceNetworksLayer::NotifyDeleted( const kernel::Entity_ABC& entity )
{
    EntityLayer< kernel::Entity_ABC >::NotifyDeleted( entity );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworksLayer::Draw
// Created: LGY 2013-05-15
// -----------------------------------------------------------------------------
void ResourceNetworksLayer::Draw( const kernel::Entity_ABC& entity, Viewport_ABC& viewport, bool /*pickingMode*/ )
{
    if( ShouldDisplay( entity ) && tools_.ShouldDisplay( GetType() ) )
        if( const ResourceNetwork_ABC* attribute = entity.Retrieve< ResourceNetwork_ABC >() )
        {
            const geometry::Point2f position = GetPosition( entity );
            viewport.SetHotpoint( position );
            attribute->Draw( viewport, tools_, position, GetAlpha() );
        }
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworksLayer::IsInside
// Created: LGY 2013-05-15
// -----------------------------------------------------------------------------
bool ResourceNetworksLayer::IsInside( const kernel::Entity_ABC& /*entity*/, const geometry::Rectangle2f& /*rectangle*/ ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworksLayer::IsSelected
// Created: LGY 2013-05-15
// -----------------------------------------------------------------------------
bool ResourceNetworksLayer::IsSelected( const kernel::Entity_ABC& /*entity*/ ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworksLayer::ActivateEntity
// Created: LGY 2013-05-24
// -----------------------------------------------------------------------------
void ResourceNetworksLayer::ActivateEntity( const kernel::Entity_ABC& /*entity*/ )
{
    // NOTHING
}
