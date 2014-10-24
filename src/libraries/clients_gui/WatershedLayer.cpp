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
#include "GLView_ABC.h"
#include "GLOptions.h"
#include "WatershedTexture.h"
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

namespace
{
    const unsigned short gradientSize = 1024;
}

// -----------------------------------------------------------------------------
// Name: WatershedLayer constructor
// Created: SBO 2010-03-23
// -----------------------------------------------------------------------------
WatershedLayer::WatershedLayer( kernel::Controllers& controllers,
                                GLView_ABC& view,
                                const kernel::DetectionMap& elevation )
    : Layer2D( controllers, view, eLayerTypes_Watershed )
    , elevation_( elevation )
    , modelLoaded_( false )
{
    controllers_.controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: WatershedLayer destructor
// Created: SBO 2010-03-23
// -----------------------------------------------------------------------------
WatershedLayer::~WatershedLayer()
{
    controllers_.controller_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: WatershedLayer::NotifyUpdated
// Created: SBO 2006-05-24
// -----------------------------------------------------------------------------
void WatershedLayer::NotifyUpdated( const ModelLoaded& )
{
    modelLoaded_ = true;
}

// -----------------------------------------------------------------------------
// Name: WatershedLayer::NotifyUpdated
// Created: ABR 2014-07-02
// -----------------------------------------------------------------------------
void WatershedLayer::NotifyUpdated( const kernel::ModelUnLoaded& )
{
    modelLoaded_ = false;
    shader_.reset();
    layer_.reset();
}

// -----------------------------------------------------------------------------
// Name: WatershedLayer::Paint
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
void WatershedLayer::Paint( const geometry::Rectangle2f& viewport )
{
    if( !modelLoaded_ ||
        !ShouldDrawPass() ||
        !view_.GetOptions().Get( "Watershed/Enabled" ).To< bool >() ||
        !gl::HasMultiTexturing() )
        return;

    if( !layer_ )
        CreateLayer();

    unsigned int texture = view_.GetOptions().GetWatershedTexture().CreateTexture();
    if( texture == 0 )
        return;

    gl::glActiveTexture( gl::GL_TEXTURE1 );
    glDisable( GL_TEXTURE_2D );
    glEnable( GL_TEXTURE_1D );
    glBindTexture( GL_TEXTURE_1D, texture );
    gl::glActiveTexture( gl::GL_TEXTURE0 );
    glEnable( GL_TEXTURE_2D );

    if( shader_ )
        shader_->Use();
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
// Name: WatershedLayer::Initialize
// Created: ABR 2014-07-03
// -----------------------------------------------------------------------------
void WatershedLayer::CreateLayer()
{
    try
    {
        shader_.reset( new ElevationShader() );
        shader_->SetMinimumElevation( 0 );
        shader_->SetMaximumElevation( elevation_.MaximumElevation() );
        shader_->SetGradientSize( gradientSize );
        layer_.reset( new ElevationTextureSet( elevation_.GetMap() ) );
    }
    catch( ... )
    {
        layer_.reset( new FixedElevationTextureSet( elevation_.GetMap() ) );
    }
}
