// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "RasterLayer.h"
#include "clients_kernel/ModelLoaded.h"
#include "clients_kernel/Controllers.h"
#include "tools/ExerciseConfig.h"
#include "MT_Tools/MT_Logger.h"
#include <graphics/TextureSet.h>
#include <graphics/Visitor2d.h>

using namespace gui;

// -----------------------------------------------------------------------------
// Name: RasterLayer constructor
// Created: AGE 2007-01-04
// -----------------------------------------------------------------------------
RasterLayer::RasterLayer( kernel::Controllers& controllers, GLView_ABC& tools )
    : Layer2D( controllers, tools, eLayerTypes_Raster )
    , ignore_( false )
    , texture_( "usrp.texture" )
{
    controllers_.Update( *this );
}

RasterLayer::RasterLayer( kernel::Controllers& controllers,
                          GLView_ABC& tools,
                          const tools::Path& texture,
                          const QString& userName )
    : Layer2D( controllers, tools, eLayerTypes_RasterDynamic )
    , ignore_( false )
{
    name_ = name_.arg( userName );
    GenerateTexture( texture );
    controllers_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: RasterLayer destructor
// Created: AGE 2007-01-04
// -----------------------------------------------------------------------------
RasterLayer::~RasterLayer()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: RasterLayer::Paint
// Created: AGE 2007-01-04
// -----------------------------------------------------------------------------
void RasterLayer::Paint( const geometry::Rectangle2f& viewport )
{
    if( !ShouldDrawPass() )
        return;
    if( !ignore_ && !textures_.get() && !directory_.IsEmpty() )
        GenerateTexture( directory_ / texture_ );
    if( textures_.get() )
    {
        glColor4f( 1, 1, 1, GetAlpha() );
        Visitor2d visitor;
        textures_->Accept( visitor, 0, viewport );
        glBindTexture( GL_TEXTURE_2D, 0 );
        glDisable( GL_TEXTURE_GEN_S );
        glDisable( GL_TEXTURE_GEN_T );
    }
}

// -----------------------------------------------------------------------------
// Name: RasterLayer::GenerateTexture
// Created: ABR 2012-06-12
// -----------------------------------------------------------------------------
void RasterLayer::GenerateTexture( const tools::Path& texture )
{
    try
    {
        textures_.reset( new TextureSet( texture ) );
    }
    catch( ... )
    {
        ignore_ = true;
        MT_LOG_WARNING_MSG( "failed to load raster " << texture );
    }
}

// -----------------------------------------------------------------------------
// Name: RasterLayer::NotifyUpdated
// Created: AGE 2007-01-04
// -----------------------------------------------------------------------------
void RasterLayer::NotifyUpdated( const kernel::ModelLoaded& modelLoaded )
{
    Reset();
    directory_ = modelLoaded.config_.GetGraphicsDirectory();
}

// -----------------------------------------------------------------------------
// Name: RasterLayer::Reset
// Created: AGE 2007-01-19
// -----------------------------------------------------------------------------
void RasterLayer::Reset()
{
    directory_.Clear();
    textures_.reset();
    ignore_ = false;
}
