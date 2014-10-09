// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "Layer.h"
#include "moc_Layer.cpp"
#include "Gl3dWidget.h"
#include "GlWidget.h"
#include "MapLayerProxy.h"
#include "Viewport2d.h"
#include "Viewport3d.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: Layer constructor
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
Layer::Layer()
    : alpha_        ( 1 )
    , currentWidget_( 0 )
    , currentProxy_ ( 0 )
    , enabled_      ( true )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Layer destructor
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
Layer::~Layer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Layer::Paint
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
void Layer::Paint( const ViewFrustum& frustum )
{
    Viewport3d viewport( frustum );
    Paint( viewport );
}

// -----------------------------------------------------------------------------
// Name: Layer::Paint
// Created: AGE 2007-02-23
// -----------------------------------------------------------------------------
void Layer::Paint( const geometry::Rectangle2f& rectangle )
{
    if( ShouldDrawPass() )
    {
        Viewport2d viewport( rectangle );
        Paint( viewport );
    }
}

// -----------------------------------------------------------------------------
// Name: Layer::Paint
// Created: AGE 2007-02-23
// -----------------------------------------------------------------------------
void Layer::Paint( Viewport_ABC& )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Layer::RegisterIn
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
void Layer::RegisterIn( Gl3dWidget& widget, kernel::Logger_ABC& logger )
{
    currentProxy_ = new MapLayerProxy( *this, logger );
    widget.Register( *currentProxy_ );
}

// -----------------------------------------------------------------------------
// Name: Layer::RegisterIn
// Created: AGE 2006-03-29
// -----------------------------------------------------------------------------
void Layer::RegisterIn( GlWidget& widget, kernel::Logger_ABC& logger )
{
    currentWidget_ = &widget;
    currentProxy_ = new MapLayerProxy( *this, logger );
    widget.Register( *currentProxy_ );
}

// -----------------------------------------------------------------------------
// Name: Layer::UnregisterIn
// Created: ABR 2012-06-11
// -----------------------------------------------------------------------------
void Layer::UnregisterIn( Gl3dWidget& widget )
{
    widget.Unregister( *currentProxy_ );
}

// -----------------------------------------------------------------------------
// Name: Layer::UnregisterIn
// Created: ABR 2012-06-11
// -----------------------------------------------------------------------------
void Layer::UnregisterIn( GlWidget& widget )
{
    widget.Unregister( *currentProxy_ );
}

// -----------------------------------------------------------------------------
// Name: Layer::Reset2d
// Created: AGE 2007-01-19
// -----------------------------------------------------------------------------
void Layer::Reset2d()
{
    currentWidget_ = 0;
    currentProxy_ = 0;
}

// -----------------------------------------------------------------------------
// Name: Layer::Reset3d
// Created: AGE 2007-01-19
// -----------------------------------------------------------------------------
void Layer::Reset3d()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Layer::SetAlpha
// Created: AGE 2007-02-23
// -----------------------------------------------------------------------------
void Layer::SetAlpha( float alpha )
{
    alpha_ = alpha;
}

// -----------------------------------------------------------------------------
// Name: Layer::GetAlpha
// Created: AGE 2007-02-23
// -----------------------------------------------------------------------------
float Layer::GetAlpha() const
{
    return alpha_;
}

// -----------------------------------------------------------------------------
// Name: Layer::MoveAbove
// Created: AGE 2007-04-27
// -----------------------------------------------------------------------------
void Layer::MoveAbove( Layer& layer )
{
    if( currentWidget_ && currentProxy_ && layer.currentProxy_ )
        currentWidget_->MoveAbove( *currentProxy_, *layer.currentProxy_ );
}

// -----------------------------------------------------------------------------
// Name: Layer::MoveBelow
// Created: AGE 2007-04-27
// -----------------------------------------------------------------------------
void Layer::MoveBelow( Layer& layer )
{
    if( currentWidget_ && currentProxy_ && layer.currentProxy_ )
        currentWidget_->MoveBelow( *currentProxy_, *layer.currentProxy_ );
}

// -----------------------------------------------------------------------------
// Name: Layer2D::Reset2d
// Created: AGE 2007-02-23
// -----------------------------------------------------------------------------
void Layer2D::Reset2d()
{
    Layer::Reset2d();
    Reset();
}

// -----------------------------------------------------------------------------
// Name: Layer2D::Reset
// Created: AGE 2007-02-23
// -----------------------------------------------------------------------------
void Layer2D::Reset()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Layer3D::Reset3d
// Created: AGE 2007-02-23
// -----------------------------------------------------------------------------
void Layer3D::Reset3d()
{
    Reset();
}

// -----------------------------------------------------------------------------
// Name: Layer3D::Reset
// Created: AGE 2007-02-23
// -----------------------------------------------------------------------------
void Layer3D::Reset()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Layer::SetPasses
// Created: SBO 2008-04-15
// -----------------------------------------------------------------------------
void Layer::SetPasses( const std::string& passes )
{
    passes_ = passes;
}

// -----------------------------------------------------------------------------
// Name: Layer::GetCurrentPass
// Created: SBO 2008-04-14
// -----------------------------------------------------------------------------
std::string Layer::GetCurrentPass() const
{
    return currentWidget_ ? currentWidget_->GetCurrentPass() : "";
}

// -----------------------------------------------------------------------------
// Name: Layer::ShouldDrawPass
// Created: SBO 2008-04-15
// -----------------------------------------------------------------------------
bool Layer::ShouldDrawPass() const
{
    bool pickingMode = false;
    if( currentWidget_ )
        pickingMode = currentWidget_->IsPickingMode();
    return ( passes_.empty() || GetCurrentPass().empty() || passes_.find( GetCurrentPass() ) != std::string::npos )
        && ( !pickingMode || ( pickingMode && IsPickable() ) );
}

// -----------------------------------------------------------------------------
// Name: Layer::IsPickable
// Created: LGY 2013-02-25
// -----------------------------------------------------------------------------
bool Layer::IsPickable() const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: Layer::SetVisible
// Created: ABR 2012-05-14
// -----------------------------------------------------------------------------
void Layer::SetVisible( bool /*visible*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Layer::ForceEnabled
// Created: ABR 2012-05-14
// -----------------------------------------------------------------------------
void Layer::ForceEnabled( bool enabled )
{
    enabled_ = enabled;
}

// -----------------------------------------------------------------------------
// Name: Layer::EnsureIsEnabled
// Created: ABR 2012-05-14
// -----------------------------------------------------------------------------
void Layer::EnsureIsEnabled()
{
    enabled_ = true;
}

// -----------------------------------------------------------------------------
// Name: Layer::IsEnabled
// Created: ABR 2012-05-14
// -----------------------------------------------------------------------------
bool Layer::IsEnabled() const
{
    return enabled_;
}

// -----------------------------------------------------------------------------
// Name: Layer::IsVisible
// Created: ABR 2012-05-16
// -----------------------------------------------------------------------------
bool Layer::IsVisible() const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: Layer::GetName
// Created: ABR 2013-01-25
// -----------------------------------------------------------------------------
QString Layer::GetName() const
{
    return "";
}

// -----------------------------------------------------------------------------
// Name: Layer::Select
// Created: ABR 2013-01-28
// -----------------------------------------------------------------------------
void Layer::Select( unsigned int, bool )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Layer::ContextMenu
// Created: ABR 2013-01-28
// -----------------------------------------------------------------------------
void Layer::ContextMenu( const kernel::GraphicalEntity_ABC&, const geometry::Point2f&, const QPoint& )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Layer::ContextMenu
// Created: LGY 2013-04-03
// -----------------------------------------------------------------------------
bool Layer::ContextMenu( const std::vector< const kernel::GraphicalEntity_ABC* >&, const geometry::Point2f&, const QPoint& )
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: Layer::FillContextMenu
// Created: LGY 2013-04-04
// -----------------------------------------------------------------------------
void Layer::FillContextMenu( QMouseEvent* /*mouse*/, kernel::ContextMenu& /*menu*/, const geometry::Point2f& /*point*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Layer::FillContextMenu
// Created: LGY 2013-04-05
// -----------------------------------------------------------------------------
void Layer::FillContextMenu( unsigned int /*id*/, kernel::ContextMenu& /*menu*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Layer::ExtractElements
// Created: ABR 2013-01-25
// -----------------------------------------------------------------------------
void Layer::ExtractElements( T_LayerElements&, const GlTools_ABC::T_ObjectsPicking& )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Layer::ShowTooltip
// Created: LGY 2013-03-08
// -----------------------------------------------------------------------------
bool Layer::ShowTooltip( const T_ObjectPicking& /*selection*/ )
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: Layer::HideTooltip
// Created: LGY 2013-03-08
// -----------------------------------------------------------------------------
void Layer::HideTooltip()
{
    // NOTHING
}
