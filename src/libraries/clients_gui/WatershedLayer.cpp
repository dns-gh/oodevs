// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "WatershedLayer.h"
#include "GLTools_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/DetectionMap.h"
#include "clients_kernel/OptionVariant.h"
#include <graphics/ElevationTextureSet.h>
#include <graphics/FixedElevationTextureSet.h>
#include <graphics/ElevationShader.h>
#include <graphics/extensions.h>
#include <graphics/Visitor2d.h>

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: WatershedLayer constructor
// Created: SBO 2010-03-23
// -----------------------------------------------------------------------------
WatershedLayer::WatershedLayer( kernel::Controllers& controllers, const kernel::DetectionMap& elevation )
    : controllers_    ( controllers )
    , elevation_      ( elevation )
    , reset_          ( false )
    , modelLoaded_    ( false )
    , ignore_         ( false )
    , updateGradient_ ( true )
    , gradientTexture_( 0 )
    , gradientSize_   ( 0 )
    , height_         ( 0 )
    , enabled_        ( false )
    , inverted_       ( false )
    , color_          ( 20, 164, 218 )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: WatershedLayer destructor
// Created: SBO 2010-03-23
// -----------------------------------------------------------------------------
WatershedLayer::~WatershedLayer()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: WatershedLayer::NotifyUpdated
// Created: SBO 2006-05-24
// -----------------------------------------------------------------------------
void WatershedLayer::NotifyUpdated( const ModelLoaded& /*modelLoaded*/ )
{
    // $$$$ AGE 2008-01-23: les ressources opengl doivent etre detruites quand le bon contexte opengl est actif
    // $$$$ AGE 2008-01-23: ie en pratique dans le Paint. C'est pénible
    reset_       = true;
    modelLoaded_ = true;
}

// -----------------------------------------------------------------------------
// Name: WatershedLayer::SetAlpha
// Created: AGE 2007-02-23
// -----------------------------------------------------------------------------
void WatershedLayer::SetAlpha( float alpha )
{
    Layer2D::SetAlpha( alpha );
    updateGradient_ = true;
}

// -----------------------------------------------------------------------------
// Name: WatershedLayer::OptionChanged
// Created: SBO 2010-03-23
// -----------------------------------------------------------------------------
void WatershedLayer::OptionChanged( const std::string& name, const kernel::OptionVariant& value )
{
    if( name == "WatershedEnabled" )
        enabled_ = value.To< bool >();
    else if( name == "WatershedHeight" )
    {
        height_ = unsigned short( value.To< int >() );
        updateGradient_ = true;
    }
    else if( name == "WatershedInverse" )
    {
        inverted_ = value.To< bool >();
        updateGradient_ = true;
    }
    else if( name == "WatershedColor" )
    {
        color_.setNamedColor( value.To< QString >() );
        updateGradient_ = true;
    }
}

// -----------------------------------------------------------------------------
// Name: WatershedLayer::Paint
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
void WatershedLayer::Paint( const geometry::Rectangle2f& viewport )
{
    if( !ShouldDrawPass() || !enabled_ )
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
        CreateShader();
        if( shader_.get() )
            shader_->Use();
        Visitor2d visitor;
        layer_->Accept( visitor, 0, viewport );
        Cleanup();
    }
}

// -----------------------------------------------------------------------------
// Name: WatershedLayer::SetGradient
// Created: AGE 2007-01-17
// -----------------------------------------------------------------------------
void WatershedLayer::SetGradient()
{
    gl::Initialize();
    if( !gl::HasMultiTexturing() )
        return;
    gl::glActiveTexture( gl::GL_TEXTURE1 );
    glDisable( GL_TEXTURE_2D );
    glEnable( GL_TEXTURE_1D );
    if( updateGradient_ )
    {
        glBindTexture( GL_TEXTURE_1D, gradientTexture_ );
        MakeGlTexture();
        if( shader_.get() )
            shader_->SetGradientSize( gradientSize_ );
        updateGradient_ = false;
    }
    glBindTexture( GL_TEXTURE_1D, gradientTexture_ );
    gl::glActiveTexture( gl::GL_TEXTURE0 );
    glEnable( GL_TEXTURE_2D );
}

// -----------------------------------------------------------------------------
// Name: WatershedLayer::Cleanup
// Created: AGE 2007-01-17
// -----------------------------------------------------------------------------
void WatershedLayer::Cleanup()
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

namespace
{
    struct Color
    {
        Color() : r( 0 ), g( 0 ), b( 0 ), a( 0 ) {}
        float r, g, b, a;
    };
}

// -----------------------------------------------------------------------------
// Name: WatershedLayer::MakeGlTexture
// Created: SBO 2010-03-23
// -----------------------------------------------------------------------------
void WatershedLayer::MakeGlTexture()
{
    unsigned short maxElevation = elevation_.MaximumElevation();
    if( maxElevation == 0 )
        return;
    gradientSize_ = 1024;
    while( maxElevation > gradientSize_ )
        gradientSize_ *= 2;
    int maxGlTextureSize = 0;
    glGetIntegerv( GL_MAX_TEXTURE_SIZE, &maxGlTextureSize );
    if( maxGlTextureSize !=  0 && gradientSize_ > maxGlTextureSize )
        gradientSize_ = static_cast< unsigned short >( maxGlTextureSize );
    height_ = std::min( height_, maxElevation );
    std::vector< Color > colors;
    colors.resize( gradientSize_ );
    Color color;
    color.r = color_.red() / 256.f;
    color.g = color_.green() / 256.f;
    color.b = color_.blue() / 256.f;
    color.a = GetAlpha();

    const unsigned short heightIndex = gradientSize_ * height_ / maxElevation;
    const unsigned short lowerBound = inverted_ ? heightIndex : 0;
    const unsigned short upperBound = inverted_ ? gradientSize_ : heightIndex;
    std::fill( colors.begin() + lowerBound, colors.begin() + upperBound, color );
    glTexImage1D( GL_TEXTURE_1D, 0, GL_RGBA, gradientSize_, 0, GL_RGBA, GL_FLOAT, &colors.front() ) ;
    glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, gl::GL_CLAMP_TO_EDGE );
}

// -----------------------------------------------------------------------------
// Name: WatershedLayer::Reset
// Created: AGE 2007-01-19
// -----------------------------------------------------------------------------
void WatershedLayer::Reset()
{
    shader_.reset();
    modelLoaded_ = false;
    ignore_ = false;
    layer_.reset();
    glDeleteTextures( 1, &gradientTexture_ );
    gradientTexture_ = 0;
    updateGradient_ = true;
}

// -----------------------------------------------------------------------------
// Name: WatershedLayer::CreateShader
// Created: AGE 2007-04-25
// -----------------------------------------------------------------------------
void WatershedLayer::CreateShader()
{
    if( ! shader_.get() && ! ignore_ )
    {
        try
        {
            shader_.reset( new ElevationShader() );
            shader_->SetMinimumElevation( 0 );
            shader_->SetMaximumElevation( elevation_.MaximumElevation() );
        }
        catch( ... )
        {
            ignore_ = true;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: WatershedLayer::CreateTextures
// Created: AGE 2007-04-25
// -----------------------------------------------------------------------------
void WatershedLayer::CreateTextures()
{
    glGenTextures( 1, &gradientTexture_ );
    CreateShader();
    if( !ignore_ )
        layer_.reset( new ElevationTextureSet( elevation_.GetMap() ) );
    else
        layer_.reset( new FixedElevationTextureSet( elevation_.GetMap() ) );
}
