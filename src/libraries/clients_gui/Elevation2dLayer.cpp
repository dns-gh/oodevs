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
#include "graphics/FixedElevationTextureSet.h"
#include "graphics/ElevationShader.h"
#include "graphics/extensions.h"
#include "graphics/Visitor2d.h"
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
    , hsx_           ( 1 )
    , hsy_           ( 1 )
    , hsStrength_    ( 1 )
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
// Name: Elevation2dLayer::SetHillShadeDirection
// Created: AGE 2007-07-02
// -----------------------------------------------------------------------------
void Elevation2dLayer::SetHillShadeDirection( int angle )
{
    const float convert = std::acos( -1.f ) / 180.f;
    hsx_ = std::cos( angle * convert );
    hsy_ = std::sin( angle * convert );
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
// Name: Elevation2dLayer::SetHillShadeStrength
// Created: AGE 2007-07-02
// -----------------------------------------------------------------------------
void Elevation2dLayer::SetHillShadeStrength( float strength )
{
    hsStrength_ = strength;
}

// -----------------------------------------------------------------------------
// Name: Elevation2dLayer::SetElevations
// Created: AGE 2007-01-17
// -----------------------------------------------------------------------------
void Elevation2dLayer::SetElevations()
{
    CreateShader();
    if( shader_.get() )
    {
        shader_->SetMinimumElevation( minElevation_ );
        shader_->SetMaximumElevation( maxElevation_ );
        shader_->SetHillShade( hsx_, hsy_, hsStrength_ );
        shader_->Use();
    }
}

// -----------------------------------------------------------------------------
// Name: Elevation2dLayer::NotifyUpdated
// Created: SBO 2006-05-24
// -----------------------------------------------------------------------------
void Elevation2dLayer::NotifyUpdated( const ModelLoaded& /*modelLoaded*/ )
{
    layer_.reset();
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

namespace
{
    struct MyVisitor : public Visitor2d
    {
        MyVisitor( ElevationShader& shader ) 
            : shader_( &shader )
            , previousWidth_( 0 )
            , previousHeight_( 0 ){}
        virtual void Visit( const geometry::Rectangle2f& extent, unsigned width, unsigned height )
        {
            if( previousHeight_ != height || previousWidth_ != width )
            {
                previousWidth_  = width;
                previousHeight_ = height;
                shader_->SetTextureSize( width, height );
                shader_->UpdateParameters();
            }
            Visitor2d::Visit( extent, width, height );
        }
        ElevationShader* shader_;
        unsigned previousWidth_, previousHeight_;
    };
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
        CreateTextures();

    if( layer_.get() )
    {
        SetGradient();
        if( ! enabled_ )
        {
            minElevation_ = 0;
            maxElevation_ = elevation_.MaximumElevation();
        }
        else if( viewport != lastViewport_ )
        {
            extrema_->FindExtrema( viewport, minElevation_, maxElevation_ );
            lastViewport_ = viewport;
        }
        SetElevations();
        Visitor2d visitor;
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
        shader_->Unuse();
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

// -----------------------------------------------------------------------------
// Name: Elevation2dLayer::CreateShader
// Created: AGE 2007-04-25
// -----------------------------------------------------------------------------
void Elevation2dLayer::CreateShader()
{
    if( ! shader_.get() && ! ignore_ )
    {
        try
        {
            shader_.reset( new ElevationShader() );
            minElevation_ = 0; maxElevation_ = elevation_.MaximumElevation();
        }
        catch( ... )
        {
            ignore_ = true;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: Elevation2dLayer::CreateTextures
// Created: AGE 2007-04-25
// -----------------------------------------------------------------------------
void Elevation2dLayer::CreateTextures()
{
    extrema_.reset( new ElevationExtrema( elevation_.GetMap() ) );
    CreateShader();
    if( !ignore_ )
        layer_.reset( new ElevationTextureSet( elevation_.GetMap() ) );
    else
        layer_.reset( new FixedElevationTextureSet( elevation_.GetMap() ) );
}
