// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "VisionConesLayer.h"
#include "gaming/VisionCones.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_gui/Viewport2d.h"
#include <graphics/extensions.h>

// -----------------------------------------------------------------------------
// Name: VisionConesLayer constructor
// Created: SBO 2008-04-14
// -----------------------------------------------------------------------------
VisionConesLayer::VisionConesLayer( kernel::Controllers& controllers, gui::GLView_ABC& view,
    gui::ColorStrategy_ABC& strategy, const kernel::Profile_ABC& profile )
    : gui::EntityLayerBase( controllers, view, strategy, profile, eLayerTypes_VisionCones )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: VisionConesLayer destructor
// Created: SBO 2008-04-14
// -----------------------------------------------------------------------------
VisionConesLayer::~VisionConesLayer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: VisionConesLayer::NotifyCreated
// Created: SBO 2008-04-14
// -----------------------------------------------------------------------------
void VisionConesLayer::NotifyCreated( const kernel::Agent_ABC& entity )
{
    AddEntity( entity );
}

// -----------------------------------------------------------------------------
// Name: VisionConesLayer::NotifyDeleted
// Created: SBO 2008-04-14
// -----------------------------------------------------------------------------
void VisionConesLayer::NotifyDeleted( const kernel::Agent_ABC& entity )
{
    RemoveEntity( entity );
}

// -----------------------------------------------------------------------------
// Name: VisionConesLayer::Draw
// Created: SBO 2008-04-14
// -----------------------------------------------------------------------------
void VisionConesLayer::Draw( const kernel::Entity_ABC& entity, gui::Viewport_ABC& viewport, bool /*pickingMode*/ )
{
    if( const VisionCones* extension = entity.Retrieve< VisionCones >() )
        if( ShouldDisplay( entity ) )
        {
            strategy_.SelectColor( dynamic_cast< const kernel::Agent_ABC& >( entity ) );
            extension->DrawMap( viewport, view_ );
        }
}
