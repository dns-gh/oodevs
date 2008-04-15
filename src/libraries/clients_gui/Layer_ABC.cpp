// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "Layer_ABC.h"
#include "Gl3dWidget.h"
#include "GlWidget.h"
#include "MapLayerProxy.h"
#include "Viewport2d.h"
#include "Viewport3d.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: Layer_ABC constructor
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
Layer_ABC::Layer_ABC()
    : alpha_        ( 1 )
    , currentWidget_( 0 )
    , currentProxy_ ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Layer_ABC destructor
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
Layer_ABC::~Layer_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Layer_ABC::Paint
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
void Layer_ABC::Paint( const ViewFrustum& frustum )
{
    Viewport3d viewport( frustum );
    Paint( viewport );
}

// -----------------------------------------------------------------------------
// Name: Layer_ABC::Paint
// Created: AGE 2007-02-23
// -----------------------------------------------------------------------------
void Layer_ABC::Paint( const geometry::Rectangle2f& rectangle )
{
    if( ShouldDrawPass() )
    {
        Viewport2d viewport( rectangle );
        Paint( viewport );
    }
}

// -----------------------------------------------------------------------------
// Name: Layer_ABC::Paint
// Created: AGE 2007-02-23
// -----------------------------------------------------------------------------
void Layer_ABC::Paint( kernel::Viewport_ABC& )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Layer_ABC::RegisterIn
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
void Layer_ABC::RegisterIn( Gl3dWidget& widget )
{
    widget.Register( *new MapLayerProxy( *this ) );
}

// -----------------------------------------------------------------------------
// Name: Layer_ABC::RegisterIn
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
void Layer_ABC::RegisterIn( GlWidget& widget )
{
    currentWidget_ = &widget;
    currentProxy_  = new MapLayerProxy( *this );
    widget.Register( *currentProxy_ );
}

// -----------------------------------------------------------------------------
// Name: Layer_ABC::Reset2d
// Created: AGE 2007-01-19
// -----------------------------------------------------------------------------
void Layer_ABC::Reset2d()
{
    currentWidget_ = 0;
    currentProxy_   = 0;
}

// -----------------------------------------------------------------------------
// Name: Layer_ABC::Reset3d
// Created: AGE 2007-01-19
// -----------------------------------------------------------------------------
void Layer_ABC::Reset3d()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Layer_ABC::SetAlpha
// Created: AGE 2007-02-23
// -----------------------------------------------------------------------------
void Layer_ABC::SetAlpha( float alpha )
{
    alpha_ = alpha;
}

// -----------------------------------------------------------------------------
// Name: Layer_ABC::GetAlpha
// Created: AGE 2007-02-23
// -----------------------------------------------------------------------------
float Layer_ABC::GetAlpha() const
{
    return alpha_;
}

// -----------------------------------------------------------------------------
// Name: Layer_ABC::MoveAbove
// Created: AGE 2007-04-27
// -----------------------------------------------------------------------------
void Layer_ABC::MoveAbove( Layer_ABC& layer )
{
    if( currentWidget_ && currentProxy_ && layer.currentProxy_ )
        currentWidget_->MoveAbove( *currentProxy_, *layer.currentProxy_ );
}

// -----------------------------------------------------------------------------
// Name: Layer_ABC::MoveBelow
// Created: AGE 2007-04-27
// -----------------------------------------------------------------------------
void Layer_ABC::MoveBelow( Layer_ABC& layer )
{
    if( currentWidget_ && currentProxy_ && layer.currentProxy_ )
        currentWidget_->MoveBelow( *currentProxy_, *layer.currentProxy_ );
}

// -----------------------------------------------------------------------------
// Name: Layer2d_ABC::Reset2d
// Created: AGE 2007-02-23
// -----------------------------------------------------------------------------
void Layer2d_ABC::Reset2d()
{
    Layer_ABC::Reset2d();
    Reset();
}

// -----------------------------------------------------------------------------
// Name: Layer2d_ABC::Reset
// Created: AGE 2007-02-23
// -----------------------------------------------------------------------------
void Layer2d_ABC::Reset()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Layer3d_ABC::Reset3d
// Created: AGE 2007-02-23
// -----------------------------------------------------------------------------
void Layer3d_ABC::Reset3d()
{
    Reset();
}

// -----------------------------------------------------------------------------
// Name: Layer3d_ABC::Reset
// Created: AGE 2007-02-23
// -----------------------------------------------------------------------------
void Layer3d_ABC::Reset()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Layer_ABC::SetPasses
// Created: SBO 2008-04-15
// -----------------------------------------------------------------------------
void Layer_ABC::SetPasses( const std::string& passes )
{
    passes_ = passes;
}

// -----------------------------------------------------------------------------
// Name: Layer_ABC::GetCurrentPass
// Created: SBO 2008-04-14
// -----------------------------------------------------------------------------
std::string Layer_ABC::GetCurrentPass() const
{
    return currentWidget_ ? currentWidget_->GetCurrentPass() : "";
}

// -----------------------------------------------------------------------------
// Name: Layer_ABC::ShouldDrawPass
// Created: SBO 2008-04-15
// -----------------------------------------------------------------------------
bool Layer_ABC::ShouldDrawPass() const
{
    return passes_.empty() || GetCurrentPass().empty() || passes_.find( GetCurrentPass() ) != std::string::npos;
}
