// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "Elevation2dLayer.h"
#include "clients_kernel/GLTools_ABC.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/DetectionMap.h"
#include "graphics/ElevationTextureSet.h"
#include "graphics/ElevationShader.h"
#include "graphics/extensions.h"
#include "graphics/Visitor2d.h"
#include "graphics/ShaderProgram.h"
#include "ElevationExtrema.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: Elevation2dLayer constructor
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
Elevation2dLayer::Elevation2dLayer( Controller& controller, const DetectionMap& elevation )
    : controller_    ( controller )
    , elevation_     ( elevation )
    , modelLoaded_   ( false )
    , ignore_        ( false )
    , min_           ( Qt::white )
    , max_           ( Qt::black )
    , updateGradient_( true )
    , gradient_      ( 0 )
    , enabled_       ( true )
    , minElevation_  ( 0 )
    , maxElevation_  ( 0 )
{
    controller_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: Elevation2dLayer destructor
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
Elevation2dLayer::~Elevation2dLayer()
{
    controller_.Remove( *this );
}

// -----------------------------------------------------------------------------
// Name: Elevation2dLayer::SetColors
// Created: AGE 2007-01-17
// -----------------------------------------------------------------------------
void Elevation2dLayer::SetColors( const QColor& min, const QColor& max )
{
    min_ = min;
    max_ = max;
    updateGradient_ = true;
}

// -----------------------------------------------------------------------------
// Name: Elevation2dLayer::EnableVariableGradient
// Created: AGE 2007-01-18
// -----------------------------------------------------------------------------
void Elevation2dLayer::EnableVariableGradient( bool enable )
{
    lastViewport_ = geometry::Rectangle2f();
    enabled_ = enable;
}

// -----------------------------------------------------------------------------
// Name: Elevation2dLayer::SetElevations
// Created: AGE 2007-01-17
// -----------------------------------------------------------------------------
void Elevation2dLayer::SetElevations( unsigned short min, unsigned short max )
{
    if( shader_.get() )
    {
        shader_->SetMinimumElevation( min );
        shader_->SetMaximumElevation( max );
    }
}

// -----------------------------------------------------------------------------
// Name: Elevation2dLayer::NotifyUpdated
// Created: SBO 2006-05-24
// -----------------------------------------------------------------------------
void Elevation2dLayer::NotifyUpdated( const ModelLoaded& /*modelLoaded*/ )
{
    layer_.reset(); // $$$$ SBO 2007-03-21: hack because ElevationExtrema/TextureSet keeps a reference on elevation_.GetMap() which has been reset
    modelLoaded_ = true;
}

// -----------------------------------------------------------------------------
// Name: Elevation2dLayer::SetAlpha
// Created: AGE 2007-02-23
// -----------------------------------------------------------------------------
void Elevation2dLayer::SetAlpha( float alpha )
{
    Layer2d_ABC::SetAlpha( alpha );
    updateGradient_ = true;
}

// -----------------------------------------------------------------------------
// Name: Elevation2dLayer::Paint
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
void Elevation2dLayer::Paint( const geometry::Rectangle2f& viewport )
{
    if( GetAlpha() == 0 )
        return;
    gl::Initialize();
    if( !layer_.get() && modelLoaded_ )
    {
        extrema_.reset( new ElevationExtrema( elevation_.GetMap() ) );
        layer_.reset( new ElevationTextureSet( elevation_.GetMap() ) );
    }
    if( layer_.get() )
    {
        SetGradient();
        SetShader();
        Visitor2d visitor;
        short min = minElevation_;
        short max = maxElevation_;
        if( ! enabled_ )
        {
            min = minElevation_ = 0;
            max = maxElevation_ = elevation_.MaximumElevation();
        }
        else if( viewport != lastViewport_ )
        {
            extrema_->FindExtrema( viewport, min, max );
            minElevation_ = min; maxElevation_ = max; 
            lastViewport_ = viewport;
        }
        SetElevations( min, max );
        layer_->Accept( visitor, 0, viewport );
        Cleanup();
    }
}

// -----------------------------------------------------------------------------
// Name: Elevation2dLayer::Paint
// Created: AGE 2007-02-23
// -----------------------------------------------------------------------------
void Elevation2dLayer::Paint( kernel::Viewport_ABC& )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Elevation2dLayer::Cleanup
// Created: AGE 2007-01-17
// -----------------------------------------------------------------------------
void Elevation2dLayer::Cleanup()
{
    if( shader_.get() )
        gl::ShaderProgram::Unuse();
    gl::glActiveTexture( gl::GL_TEXTURE1 );
    glBindTexture( GL_TEXTURE_1D, 0 );
    glDisable( GL_TEXTURE_1D );
    gl::glActiveTexture( gl::GL_TEXTURE0 );
}

// -----------------------------------------------------------------------------
// Name: Elevation2dLayer::SetGradient
// Created: AGE 2007-01-17
// -----------------------------------------------------------------------------
void Elevation2dLayer::SetGradient()
{
    gl::Initialize();
    gl::glActiveTexture( gl::GL_TEXTURE1 );
    glDisable( GL_TEXTURE_2D );
    glEnable( GL_TEXTURE_1D );
    if( updateGradient_ )
    {
        glDeleteTextures( 1, &gradient_ );
        glGenTextures( 1, &gradient_ );
        glBindTexture( GL_TEXTURE_1D, gradient_ );
        unsigned char gradient[] = 
            { min_.red(), min_.green(), min_.blue(), 255 * GetAlpha(),
              max_.red(), max_.green(), max_.blue(), 255 * GetAlpha() };
        glTexImage1D( GL_TEXTURE_1D, 0, GL_RGBA, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, gradient ) ;
        glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_WRAP_S , gl::GL_CLAMP_TO_EDGE );
        updateGradient_ = false;
    }
    glBindTexture( GL_TEXTURE_1D, gradient_ );
    gl::glActiveTexture( gl::GL_TEXTURE0 );
    glEnable( GL_TEXTURE_2D );
}

// -----------------------------------------------------------------------------
// Name: Elevation2dLayer::SetShader
// Created: AGE 2007-01-17
// -----------------------------------------------------------------------------
void Elevation2dLayer::SetShader()
{
    if( ! shader_.get() && ! ignore_ )
    {
        try
        {
            shader_.reset( new ElevationShader() );
            SetElevations( 0, elevation_.MaximumElevation() );
        }
        catch( ... )
        {
            ignore_ = true;
        }
    }
    if( shader_.get() )
        shader_->Use();
}

// -----------------------------------------------------------------------------
// Name: Elevation2dLayer::Reset
// Created: AGE 2007-01-19
// -----------------------------------------------------------------------------
void Elevation2dLayer::Reset()
{
    extrema_.reset();
    shader_.reset();
    modelLoaded_ = false;
    ignore_ = false;
    layer_.reset();
    glDeleteTextures( 1, &gradient_ );
    gradient_ = 0;
    updateGradient_ = true;
    lastViewport_ = geometry::Rectangle2f();
}
