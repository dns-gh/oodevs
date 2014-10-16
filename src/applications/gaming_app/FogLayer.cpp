// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "FogLayer.h"
#include "gaming/VisionCones.h"
#include "clients_gui/Viewport2d.h"
#include "clients_kernel/Agent_ABC.h"
#include <graphics/extensions.h>

// -----------------------------------------------------------------------------
// Name: FogLayer constructor
// Created: SBO 2008-04-14
// -----------------------------------------------------------------------------
FogLayer::FogLayer( kernel::Controllers& controllers,
                    gui::GLView_ABC& view,
                    gui::ColorStrategy_ABC& strategy,
                    const kernel::Profile_ABC& profile )
    : gui::EntityLayerBase( controllers, view, strategy, profile, eLayerTypes_Fog )
    , controllers_( controllers )
    , texture_( 0 )
{
    controllers_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: FogLayer destructor
// Created: SBO 2008-04-14
// -----------------------------------------------------------------------------
FogLayer::~FogLayer()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: FogLayer::NotifyCreated
// Created: SBO 2008-04-14
// -----------------------------------------------------------------------------
void FogLayer::NotifyCreated( const kernel::Agent_ABC& entity )
{
    AddEntity( entity );
}

// -----------------------------------------------------------------------------
// Name: FogLayer::NotifyDeleted
// Created: SBO 2008-04-14
// -----------------------------------------------------------------------------
void FogLayer::NotifyDeleted( const kernel::Agent_ABC& entity )
{
    RemoveEntity( entity );
}

// -----------------------------------------------------------------------------
// Name: FogLayer::Paint
// Created: AGE 2008-05-13
// -----------------------------------------------------------------------------
void FogLayer::Paint( const ViewFrustum& )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FogLayer::Paint
// Created: SBO 2008-04-14
// -----------------------------------------------------------------------------
void FogLayer::Paint( const geometry::Rectangle2f& rect )
{
    if( !ShouldDrawPass() )
        return;
    if( !texture_ )
        CreateTexture();
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LEQUAL );
    glDisable( GL_TEXTURE_2D );
    glEnable( GL_TEXTURE_1D );
    glBindTexture( GL_TEXTURE_1D, texture_ );
    gui::Viewport2d viewport( rect );
    gui::EntityLayerBase::Paint( viewport );
    glBindTexture( GL_TEXTURE_1D, 0 );
    glDisable( GL_TEXTURE_1D );
    glDisable( GL_DEPTH_TEST );
}

// -----------------------------------------------------------------------------
// Name: FogLayer::Draw
// Created: SBO 2008-04-14
// -----------------------------------------------------------------------------
void FogLayer::Draw( const kernel::Entity_ABC& entity, gui::Viewport_ABC& viewport, bool /*pickingMode*/ )
{
    if( ShouldDisplay( entity ) )
        if( const VisionCones* extension = entity.Retrieve< VisionCones >() )
            extension->DrawFill( viewport );
}

// -----------------------------------------------------------------------------
// Name: FogLayer::CreateTexture
// Created: SBO 2008-04-14
// -----------------------------------------------------------------------------
void FogLayer::CreateTexture()
{
    glGenTextures( 1, &texture_ );
    glBindTexture( GL_TEXTURE_1D, texture_ );

    float colors[] = { 1, 1, 1, 1,
                       1, 1, 1, 1,
                       0.8f, 0.8f, 0.8f, 1,
                       0, 0, 0, 1 };

    glTexImage1D( GL_TEXTURE_1D, 0, GL_RGBA, 4, 0, GL_RGBA, GL_FLOAT, colors ) ;
    glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_WRAP_S , gl::GL_CLAMP_TO_EDGE );
}

// -----------------------------------------------------------------------------
// Name: FogLayer::Reset2d
// Created: SBO 2008-04-15
// -----------------------------------------------------------------------------
void FogLayer::Reset2d()
{
    glDeleteTextures( 1, &texture_ );
    texture_ = 0;
}
