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

#include "GLView_ABC.h"
#include "Gradient.h"
#include "GradientPreferences.h"
#include "ElevationExtrema.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/DetectionMap.h"
#include "clients_kernel/OptionVariant.h"
#include "clients_kernel/OptionsController.h"

#include <graphics/ElevationShader.h>
#include <graphics/ElevationTextureSet.h>
#include <graphics/extensions.h>
#include <graphics/FixedElevationTextureSet.h>
#include <graphics/Visitor2d.h>

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: Elevation2dLayer constructor
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
Elevation2dLayer::Elevation2dLayer( Controllers& controllers,
                                    GLView_ABC& tools,
                                    const DetectionMap& elevation,
                                    const std::shared_ptr< GradientPreferences >& preferences )
    : Layer2D( controllers, tools, eLayerTypes_Elevation2d )
    , elevation_      ( elevation )
    , preferences_    ( preferences )
    , reset_          ( false )
    , modelLoaded_    ( false )
    , ignore_         ( false )
    , updateGradient_ ( true )
    , gradientTexture_( 0 )
    , enabled_        ( true )
    , minElevation_   ( 0 )
    , maxElevation_   ( 0 )
    , hsx_            ( 1 )
    , hsy_            ( 1 )
    , hsStrength_     ( 1 )
{
    controllers_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Elevation2dLayer destructor
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
Elevation2dLayer::~Elevation2dLayer()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: Elevation2dLayer::UpdateGradient
// Created: AGE 2007-07-03
// -----------------------------------------------------------------------------
void Elevation2dLayer::UpdateGradient()
{
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
// Name: Elevation2dLayer::EnableHillShade
// Created: ABR 2014-10-03
// -----------------------------------------------------------------------------
void Elevation2dLayer::EnableHillShade( bool enable )
{
    hillShade_ = enable;
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
        float delta = std::min( 1.f, 100.f / float( maxElevation_ - minElevation_ ) );
        float hillShade = hillShade_ ? delta * hsStrength_ : 0.f;
        shader_->SetHillShade( hsx_, hsy_, hillShade );
        shader_->Use();
    }
}

// -----------------------------------------------------------------------------
// Name: Elevation2dLayer::NotifyUpdated
// Created: SBO 2006-05-24
// -----------------------------------------------------------------------------
void Elevation2dLayer::NotifyUpdated( const ModelLoaded& /*modelLoaded*/ )
{
    // $$$$ AGE 2008-01-23: les ressources opengl doivent etre detruites quand le bon contexte opengl est actif
    // $$$$ AGE 2008-01-23: ie en pratique dans le Paint. C'est pénible
    reset_       = true;
    modelLoaded_ = true;
}

// -----------------------------------------------------------------------------
// Name: Elevation2dLayer::SetAlpha
// Created: AGE 2007-02-23
// -----------------------------------------------------------------------------
void Elevation2dLayer::SetAlpha( float alpha )
{
    Layer2D::SetAlpha( alpha );
    updateGradient_ = true;
}

// -----------------------------------------------------------------------------
// Name: Elevation2dLayer::Paint
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
void Elevation2dLayer::Paint( const geometry::Rectangle2f& viewport )
{
    if( !ShouldDrawPass() )
        return;
    if( reset_ )
    {
        bool modelLoaded = modelLoaded_;
        Reset();
        reset_ = false;
        modelLoaded_ = modelLoaded;
    }

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
// Name: Elevation2dLayer::Cleanup
// Created: AGE 2007-01-17
// -----------------------------------------------------------------------------
void Elevation2dLayer::Cleanup()
{
    if( shader_.get() )
        shader_->Unuse();
    if( gl::HasMultiTexturing() )
    {
        gl::glActiveTexture( gl::GL_TEXTURE1 );
        glBindTexture( GL_TEXTURE_1D, 0 );
        glDisable( GL_TEXTURE_1D );
        gl::glActiveTexture( gl::GL_TEXTURE0 );
        glBindTexture( GL_TEXTURE_2D, 0 );
    }
}

// -----------------------------------------------------------------------------
// Name: Elevation2dLayer::SetGradient
// Created: AGE 2007-01-17
// -----------------------------------------------------------------------------
void Elevation2dLayer::SetGradient()
{
    gl::Initialize();
    if( !gl::HasMultiTexturing() )
        return;
    gl::glActiveTexture( gl::GL_TEXTURE1 );
    glDisable( GL_TEXTURE_2D );
    glEnable( GL_TEXTURE_1D );
    if( updateGradient_ )
        if( auto gradient = preferences_->GetCurrent() )
        {
            EnableVariableGradient( controllers_.options_.GetOption( "Elevation/FitToViewPort" ).To< bool >() );
            glBindTexture( GL_TEXTURE_1D, gradientTexture_ );
            gradient->MakeGlTexture( GetAlpha() );
            if( shader_.get() )
                shader_->SetGradientSize( unsigned short( gradient->Length() ), gradient->UsedRatio() );
            updateGradient_ = false;
        }
    glBindTexture( GL_TEXTURE_1D, gradientTexture_ );
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
    glDeleteTextures( 1, &gradientTexture_ );
    gradientTexture_ = 0;
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
    glGenTextures( 1, &gradientTexture_ );
    extrema_.reset( new ElevationExtrema( elevation_.GetMap() ) );
    CreateShader();
    if( !ignore_ )
        layer_.reset( new ElevationTextureSet( elevation_.GetMap() ) );
    else
        layer_.reset( new FixedElevationTextureSet( elevation_.GetMap() ) );
}
