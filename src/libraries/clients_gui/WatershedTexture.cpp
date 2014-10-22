// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "WatershedTexture.h"
#include "clients_kernel/DetectionMap.h"
#include "clients_kernel/Options.h"
#include "clients_kernel/OptionVariant.h"
#include "ENT/ENT_Tr.h"
#include <graphics/extensions.h>

using namespace gui;

namespace
{
    const unsigned short gradientSize = 1024;
}

// -----------------------------------------------------------------------------
// Name: WatershedTexture constructor
// Created: ABR 2014-07-02
// -----------------------------------------------------------------------------
WatershedTexture::WatershedTexture( const kernel::DetectionMap& map )
    : map_( map )
    , texture_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: WatershedTexture destructor
// Created: ABR 2014-07-02
// -----------------------------------------------------------------------------
WatershedTexture::~WatershedTexture()
{
    Reset();
}

// -----------------------------------------------------------------------------
// Name: WatershedTexture::Load
// Created: ABR 2014-08-05
// -----------------------------------------------------------------------------
void WatershedTexture::Load( const kernel::Options& options )
{
    const auto alphaOptionName = "Layers/" + ENT_Tr::ConvertFromLayerTypes( eLayerTypes_WeatherComposite, ENT_Tr::eToSim ) + "/Alpha";
    if( !options.Has( alphaOptionName ) )
        return;
    const unsigned short height = static_cast< unsigned short >( options.Get( "Watershed/Height" ).To< int >() );
    const bool inverse = options.Get( "Watershed/Inverse" ).To< bool >();
    const QColor color = QColor( options.Get( "Watershed/Color" ).To< QString >() );
    const float alpha = options.Get( alphaOptionName ).To< float >();
    if( height == height_ && inverse == inverse_ && color == color_ && alpha == alpha_ )
        return;
    Reset();
    height_ = height;
    inverse_ = inverse;
    color_ = color;
    alpha_ = alpha;
}

// -----------------------------------------------------------------------------
// Name: WatershedTexture::Reset
// Created: ABR 2014-07-02
// -----------------------------------------------------------------------------
void WatershedTexture::Reset()
{
    glDeleteTextures( 1, &texture_ );
    texture_ = 0;
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
// Name: WatershedTexture::CreateTexture
// Created: ABR 2014-07-03
// -----------------------------------------------------------------------------
unsigned int WatershedTexture::CreateTexture()
{
    if( texture_ )
        return texture_;
    unsigned short maxElevation = map_.MaximumElevation();
    if( maxElevation == 0 )
        return 0;
    height_ = std::min( height_, maxElevation );
    std::vector< Color > colors( gradientSize );
    Color color;
    color.r = color_.red() / 256.f;
    color.g = color_.green() / 256.f;
    color.b = color_.blue() / 256.f;
    color.a = alpha_;

    const unsigned short heightIndex = gradientSize * height_ / maxElevation;
    const unsigned short lowerBound = inverse_ ? heightIndex : 0;
    const unsigned short upperBound = inverse_ ? gradientSize : heightIndex;
    std::fill( colors.begin() + lowerBound, colors.begin() + upperBound, color );

    glGenTextures( 1, &texture_ );
    glBindTexture( GL_TEXTURE_1D, texture_ );
    glTexImage1D( GL_TEXTURE_1D, 0, GL_RGBA, gradientSize, 0, GL_RGBA, GL_FLOAT, &colors.front() ) ;
    glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, gl::GL_CLAMP_TO_EDGE );
    return texture_;
}
