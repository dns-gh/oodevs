// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "WeaponRangeLayer.h"
#include "gaming/Weapons.h"
#include "clients_kernel/Agent_ABC.h"

// -----------------------------------------------------------------------------
// Name: WeaponRangeLayer constructor
// Created: SBO 2008-04-14
// -----------------------------------------------------------------------------
WeaponRangeLayer::WeaponRangeLayer( kernel::Controllers& controllers, gui::GlTools_ABC& tools, gui::ColorStrategy_ABC& strategy,
                    gui::View_ABC& view, const kernel::Profile_ABC& profile )
    : gui::EntityLayerBase( controllers, tools, strategy, view, profile, eLayerTypes_WeaponRanges )
    , controllers_( controllers )
    , strategy_( strategy )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: WeaponRangeLayer destructor
// Created: SBO 2008-04-14
// -----------------------------------------------------------------------------
WeaponRangeLayer::~WeaponRangeLayer()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: WeaponRangeLayer::NotifyCreated
// Created: SBO 2008-04-14
// -----------------------------------------------------------------------------
void WeaponRangeLayer::NotifyCreated( const kernel::Agent_ABC& entity )
{
    AddEntity( entity );
}

// -----------------------------------------------------------------------------
// Name: WeaponRangeLayer::NotifyDeleted
// Created: SBO 2008-04-14
// -----------------------------------------------------------------------------
void WeaponRangeLayer::NotifyDeleted( const kernel::Agent_ABC& entity )
{
    RemoveEntity( entity );
}

// -----------------------------------------------------------------------------
// Name: WeaponRangeLayer::Draw
// Created: SBO 2008-04-14
// -----------------------------------------------------------------------------
void WeaponRangeLayer::Draw( const kernel::Entity_ABC& entity, gui::Viewport_ABC& viewport, bool /*pickingMode*/ )
{
    // SelectColor actually controls the result of ShouldDisplay
    strategy_.SelectColor( static_cast< const kernel::Agent_ABC& >( entity ) );
    const geometry::Point2f position = GetPosition( entity );
    if( ShouldDisplay( entity ) && tools_.ShouldDisplay( type_ ) )
        if( const Weapons* extension = entity.Retrieve< Weapons >() )
            extension->Draw( position, viewport, tools_ );
}
