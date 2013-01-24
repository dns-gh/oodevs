// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "SelectionLayer.h"
#include "clients_gui/GlTools_ABC.h"
#include "clients_kernel/ActionController.h"
#include "clients_kernel/Controllers.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: SelectionLayer constructor
// Created: JSR 2012-05-23
// -----------------------------------------------------------------------------
SelectionLayer::SelectionLayer( kernel::Controllers& controllers, const gui::GlTools_ABC& tools )
    : controllers_  ( controllers )
    , tools_        ( tools )
    , displaying_   ( false )
    , firstPointSet_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SelectionLayer destructor
// Created: JSR 2012-05-23
// -----------------------------------------------------------------------------
SelectionLayer::~SelectionLayer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SelectionLayer::Paint
// Created: JSR 2012-05-23
// -----------------------------------------------------------------------------
void SelectionLayer::Paint( const geometry::Rectangle2f& /*viewport*/ )
{
    if( !displaying_ )
        return;
    const geometry::Point2f topRight( bottomRight_.X(), topLeft_.Y() );
    const geometry::Point2f bottomLeft( topLeft_.X(), bottomRight_.Y() );
    glPushAttrib( GL_CURRENT_BIT | GL_LINE_BIT );
    glLineWidth( 2.f );
    tools_.DrawStippledLine( topLeft_, topRight );
    tools_.DrawStippledLine( topRight, bottomRight_ );
    tools_.DrawStippledLine( bottomRight_, bottomLeft );
    tools_.DrawStippledLine( bottomLeft, topLeft_ );
    glPopAttrib();
}

// -----------------------------------------------------------------------------
// Name: SelectionLayer::Reset
// Created: JSR 2012-05-23
// -----------------------------------------------------------------------------
void SelectionLayer::Reset()
{
    // TODO
}

// -----------------------------------------------------------------------------
// Name: SelectionLayer::HandleMousePress
// Created: JSR 2012-05-23
// -----------------------------------------------------------------------------
bool SelectionLayer::HandleMousePress( QMouseEvent* event, const geometry::Point2f& point )
{
    if( !controllers_.actions_.HasMultipleSelection() )
        return false;
    if( event->button() == Qt::LeftButton && event->buttons() == Qt::NoButton && displaying_ )
    {
        firstPointSet_ = false;
        displaying_ = false;
        controllers_.actions_.NotifyRectangleSelection( topLeft_, bottomRight_, event->modifiers() == Qt::ControlModifier );
       // UpdateMultipleSelection( mouse->modifiers() == Qt::ControlModifier );
    }
    if( event->button() == Qt::LeftButton && event->buttons() == Qt::LeftButton )
    {
        topLeft_ = point;
        bottomRight_ = point;
        /*const urban::TerrainObject_ABC* object = model_.FindBlock( point );
        if( mouse->modifiers() != Qt::ControlModifier )
        {
            controller_.DeselectAll();
            if( object )
                controller_.SetSelected( *object, true );
        }
        else
            if( object )
                ApplyControlSelection( *object );*/
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: SelectionLayer::HandleMouseMove
// Created: JSR 2012-05-23
// -----------------------------------------------------------------------------
bool SelectionLayer::HandleMouseMove( QMouseEvent* event, const geometry::Point2f& point )
{
    if( !controllers_.actions_.HasMultipleSelection() )
        return false;
    if( event->buttons() == Qt::LeftButton )
    {
        //cursorController_.SetCurrentCursor( QCursor( Qt::PointingHandCursor ) );
        if( !firstPointSet_ )
        {
            topLeft_ = point;
            firstPointSet_ = true;
        }
        else
            bottomRight_ = point;
        if( !displaying_ )
            displaying_ = ( topLeft_.SquareDistance( bottomRight_ ) > 400 );
        return displaying_;
    }
    return false;
}
