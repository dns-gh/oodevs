// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "RectangleSelectionLayer.h"

#include "GLView_ABC.h"

#include "clients_kernel/ActionController.h"
#include "clients_kernel/Controllers.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: RectangleSelectionLayer constructor
// Created: JSR 2012-05-23
// -----------------------------------------------------------------------------
RectangleSelectionLayer::RectangleSelectionLayer( kernel::Controllers& controllers, gui::GLView_ABC& tools )
    : Layer2D( controllers, tools, eLayerTypes_RectangleSelection )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RectangleSelectionLayer destructor
// Created: JSR 2012-05-23
// -----------------------------------------------------------------------------
RectangleSelectionLayer::~RectangleSelectionLayer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RectangleSelectionLayer::Paint
// Created: JSR 2012-05-23
// -----------------------------------------------------------------------------
void RectangleSelectionLayer::Paint( const geometry::Rectangle2f& /*viewport*/ )
{
    if( !topLeft_ || !bottomRight_ )
        return;
    const geometry::Point2f topRight( bottomRight_->X(), topLeft_->Y() );
    const geometry::Point2f bottomLeft( topLeft_->X(), bottomRight_->Y() );
    glPushAttrib( GL_CURRENT_BIT | GL_LINE_BIT );
    glLineWidth( 2.f );
    view_.DrawStippledLine( *topLeft_, topRight );
    view_.DrawStippledLine( topRight, *bottomRight_ );
    view_.DrawStippledLine( *bottomRight_, bottomLeft );
    view_.DrawStippledLine( bottomLeft, *topLeft_ );
    glPopAttrib();
}

// -----------------------------------------------------------------------------
// Name: RectangleSelectionLayer::Reset
// Created: JSR 2012-05-23
// -----------------------------------------------------------------------------
void RectangleSelectionLayer::Reset()
{
    // TODO
}

// -----------------------------------------------------------------------------
// Name: RectangleSelectionLayer::HandleMousePress
// Created: JSR 2012-05-23
// -----------------------------------------------------------------------------
bool RectangleSelectionLayer::HandleMousePress( QMouseEvent* event, const geometry::Point2f& point )
{
    if( !controllers_.actions_.HasMultipleSelection() )
        return false;
    bottomRight_ = boost::none;
    topLeft_ = boost::none;
    if( !topLeft_ && event->button() == Qt::LeftButton )
        topLeft_ = point;
    return false;
}

// -----------------------------------------------------------------------------
// Name: RectangleSelectionLayer::HandleMouseRelease
// Created: LGY 2013-03-08
// -----------------------------------------------------------------------------
bool RectangleSelectionLayer::HandleMouseRelease( QMouseEvent* event, const geometry::Point2f& /*point*/ )
{
    if( controllers_.actions_.HasMultipleSelection() &&
        event->button() == Qt::LeftButton && event->buttons() == Qt::NoButton &&
        topLeft_ && bottomRight_ )
    {
        controllers_.actions_.NotifyRectangleSelection( *topLeft_, *bottomRight_, event->modifiers() == Qt::ControlModifier );
        bottomRight_ = boost::none;
        topLeft_ = boost::none;
        return true;
    }
    bottomRight_ = boost::none;
    topLeft_ = boost::none;
    return false;
}

// -----------------------------------------------------------------------------
// Name: RectangleSelectionLayer::HandleMouseMove
// Created: JSR 2012-05-23
// -----------------------------------------------------------------------------
bool RectangleSelectionLayer::HandleMouseMove( QMouseEvent* event, const geometry::Point2f& point )
{
    bottomRight_ = boost::none;
    if( controllers_.actions_.HasMultipleSelection() &&
        event->buttons() == Qt::LeftButton &&
        topLeft_ && topLeft_->SquareDistance( point ) > 40000 )
        bottomRight_ = point;
    return bottomRight_;
}
