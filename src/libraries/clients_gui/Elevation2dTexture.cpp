// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "Elevation2dTexture.h"
#include "GradientPreferences.h"
#include "clients_gui/ElevationExtrema.h"
#include "clients_gui/Gradient.h"
#include "clients_kernel/DetectionMap.h"
#include "clients_kernel/Options.h"
#include "clients_kernel/OptionVariant.h"
#include "graphics/ElevationShader.h"
#include "ENT/ENT_Tr.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: Elevation2dTexture constructor
// Created: ABR 2014-07-03
// -----------------------------------------------------------------------------
Elevation2dTexture::Elevation2dTexture( const kernel::DetectionMap& map )
    : map_( map )
    , gradientPreferences_( new GradientPreferences() )
    , texture_( 0 )
    , hsEnabled_( true )
    , hsx_( 0 )
    , hsy_( 0 )
    , hsStrength_( 1 )
    , minElevation_( 0 )
    , maxElevation_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Elevation2dTexture destructor
// Created: ABR 2014-07-03
// -----------------------------------------------------------------------------
Elevation2dTexture::~Elevation2dTexture()
{
    Purge();
}

// -----------------------------------------------------------------------------
// Name: Elevation2dTexture::Load
// Created: ABR 2014-08-05
// -----------------------------------------------------------------------------
void Elevation2dTexture::Load( const kernel::Options& options )
{
    UpdateHillShadeValues( options );
    gradientPreferences_->Load( options );
    Purge();
}

// -----------------------------------------------------------------------------
// Name: Elevation2dTexture::UpdateHillShadeValues
// Created: ABR 2014-10-17
// -----------------------------------------------------------------------------
void Elevation2dTexture::UpdateHillShadeValues( const kernel::Options& options )
{
    hsEnabled_ = options.Get( "HillShade/Enabled" ).To< bool >();
    hsAngle_ = options.Get( "HillShade/Direction" ).To< int >();
    const float convert = std::acos( -1.f ) / 180.f;
    hsx_ = std::cos( hsAngle_ * convert );
    hsy_ = std::sin( hsAngle_ * convert );
    hsStrength_ = pow( 1.1f, options.Get( "HillShade/Strength" ).To< int >() );
    minElevation_ = 0;
    maxElevation_ = map_.MaximumElevation();
}

// -----------------------------------------------------------------------------
// Name: Elevation2dTexture::ConfigureShader
// Created: ABR 2014-08-05
// -----------------------------------------------------------------------------
void Elevation2dTexture::ConfigureShader( ElevationShader& shader )
{
    auto gradient = gradientPreferences_->GetCurrent();
    shader.SetGradientSize( unsigned short( gradient->Length() ), gradient->UsedRatio() );
    shader.SetMinimumElevation( minElevation_ );
    shader.SetMaximumElevation( maxElevation_ );
    const float delta = std::min( 1.f, 100.f / float( maxElevation_ - minElevation_ ) );
    shader.SetHillShade( hsx_, hsy_, delta * ( hsEnabled_ ? hsStrength_ : 0 ) );
    shader.Use();
}

// -----------------------------------------------------------------------------
// Name: Elevation2dTexture::Purge
// Created: ABR 2014-08-05
// -----------------------------------------------------------------------------
void Elevation2dTexture::Purge()
{
    glDeleteTextures( 1, &texture_ );
    texture_ = 0;
    viewport_ = geometry::Rectangle2f();
}

// -----------------------------------------------------------------------------
// Name: Elevation2dTexture::CreateTexture
// Created: ABR 2014-07-03
// -----------------------------------------------------------------------------
unsigned int Elevation2dTexture::CreateTexture( float alpha )
{
    if( texture_ && alpha_ == alpha )
        return texture_;
    alpha_ = alpha;
    auto gradient = gradientPreferences_->GetCurrent();
    if( map_.MaximumElevation() == 0 || !gradient )
        return 0;
    glGenTextures( 1, &texture_ );
    glBindTexture( GL_TEXTURE_1D, texture_ );
    gradient->MakeGlTexture( alpha );
    return texture_;
}

// -----------------------------------------------------------------------------
// Name: Elevation2dTexture::GetViewport
// Created: ABR 2014-07-03
// -----------------------------------------------------------------------------
const geometry::Rectangle2f& Elevation2dTexture::GetViewport() const
{
    return viewport_;
}

// -----------------------------------------------------------------------------
// Name: Elevation2dTexture::SetViewport
// Created: ABR 2014-07-03
// -----------------------------------------------------------------------------
void Elevation2dTexture::SetViewport( const geometry::Rectangle2f& viewport,
                                      const ElevationExtrema& extrema )
{
    viewport_ = viewport;
    extrema.FindExtrema( viewport, minElevation_, maxElevation_ );
}

// -----------------------------------------------------------------------------
// Name: Elevation2dTexture::GetGradient
// Created: ABR 2014-07-03
// -----------------------------------------------------------------------------
std::shared_ptr< Gradient > Elevation2dTexture::GetGradient() const
{
    return gradientPreferences_->GetCurrent();
}

// -----------------------------------------------------------------------------
// Name: Elevation2dTexture::GetPreferences
// Created: ABR 2014-07-28
// -----------------------------------------------------------------------------
std::shared_ptr< GradientPreferences > Elevation2dTexture::GetPreferences()
{
    return gradientPreferences_;
}
