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
#include "Elevation2dTexture.h"
#include "GLOptions.h"
#include "GLView_ABC.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/DetectionMap.h"
//#include "Gradient.h"
#include "clients_kernel/OptionVariant.h"
#include "ElevationExtrema.h"
#include <graphics/ElevationTextureSet.h>
#include <graphics/FixedElevationTextureSet.h>
#include <graphics/ElevationShader.h>
#include <graphics/extensions.h>
#include <graphics/Visitor2d.h>

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: Elevation2dLayer constructor
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
Elevation2dLayer::Elevation2dLayer( Controllers& controllers,
                                    GLView_ABC& view,
                                    const DetectionMap& elevation )
    : Layer2D( controllers, view, eLayerTypes_Elevation2d )
    , elevation_      ( elevation )
    , modelLoaded_    ( false )
{
    controllers_.controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Elevation2dLayer destructor
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
Elevation2dLayer::~Elevation2dLayer()
{
    controllers_.controller_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: Elevation2dLayer::NotifyUpdated
// Created: SBO 2006-05-24
// -----------------------------------------------------------------------------
void Elevation2dLayer::NotifyUpdated( const ModelLoaded& /*modelLoaded*/ )
{
    modelLoaded_ = true;
}

// -----------------------------------------------------------------------------
// Name: Elevation2dLayer::NotifyUpdated
// Created: ABR 2014-07-03
// -----------------------------------------------------------------------------
void Elevation2dLayer::NotifyUpdated( const kernel::ModelUnLoaded& )
{
    modelLoaded_ = false;
    extrema_.reset();
    shader_.reset();
    layer_.reset();
}

// -----------------------------------------------------------------------------
// Name: Elevation2dLayer::Paint
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
void Elevation2dLayer::Paint( const geometry::Rectangle2f& viewport )
{
    if( !modelLoaded_ ||
        !ShouldDrawPass() ||
        GetAlpha() == 0 ||
        !gl::HasMultiTexturing() )
        return;

    if( !layer_ )
        CreateLayer();

    auto& texture = view_.GetOptions().GetElevation2dTexture();
    unsigned int textureId = texture->CreateTexture( GetAlpha() );
    if( textureId == 0 )
        return;

    if( view_.GetOptions().Get( "Elevation/FitToViewPort" ).To< bool >() &&
        viewport != texture->GetViewport() &&
        extrema_ )
        texture->SetViewport( viewport, *extrema_ );

    gl::glActiveTexture( gl::GL_TEXTURE1 );
    glDisable( GL_TEXTURE_2D );
    glEnable( GL_TEXTURE_1D );
    glBindTexture( GL_TEXTURE_1D, textureId );
    gl::glActiveTexture( gl::GL_TEXTURE0 );
    glEnable( GL_TEXTURE_2D );

    if( shader_ )
        texture->ConfigureShader( *shader_ );
    Visitor2d visitor;
    layer_->Accept( visitor, 0, viewport );
    if( shader_ )
        shader_->Unuse();

    gl::glActiveTexture( gl::GL_TEXTURE1 );
    glBindTexture( GL_TEXTURE_1D, 0 );
    glDisable( GL_TEXTURE_1D );
    gl::glActiveTexture( gl::GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_2D, 0 );
}

// -----------------------------------------------------------------------------
// Name: Elevation2dLayer::CreateLayer
// Created: ABR 2014-07-03
// -----------------------------------------------------------------------------
void Elevation2dLayer::CreateLayer()
{
    try
    {
        extrema_.reset( new ElevationExtrema( elevation_.GetMap() ) );
        shader_.reset( new ElevationShader() );
        layer_.reset( new ElevationTextureSet( elevation_.GetMap() ) );
    }
    catch( ... )
    {
        layer_.reset( new FixedElevationTextureSet( elevation_.GetMap() ) );
    }
}
