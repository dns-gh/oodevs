// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "DefaultLayer.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/ActionController.h"
#include "clients_kernel/Types.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: DefaultLayer constructor
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
DefaultLayer::DefaultLayer( Controllers& controllers )
    : controllers_( controllers )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DefaultLayer destructor
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
DefaultLayer::~DefaultLayer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DefaultLayer::Paint
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
void DefaultLayer::Paint( Viewport_ABC& )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DefaultLayer::HandleMousePress
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
bool DefaultLayer::HandleMousePress( QMouseEvent* mouse, const geometry::Point2f& point )
{
    if( mouse && mouse->buttons() != Qt::NoButton )
    {
        point_ = point;
        if( controllers_.actions_.HasMultipleSelection() && mouse->button() == Qt::LeftButton && ( mouse->modifiers() & Qt::ControlModifier ) == 0)
        {
            controllers_.actions_.DeselectAll();
            controllers_.actions_.Select( point_ );
        }
        else if( mouse->button() == Qt::RightButton )
            controllers_.actions_.ContextMenu( point_, kernel::Nothing(), mouse->globalPos() );
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: DefaultLayer::HandleMoveDragEvent
// Created: ABR 2011-10-28
// -----------------------------------------------------------------------------
bool DefaultLayer::HandleMoveDragEvent( QDragMoveEvent* /*event*/, const geometry::Point2f& point )
{
    point_ = point;
    controllers_.actions_.OverFly( point_ );
    return false;
}

// -----------------------------------------------------------------------------
// Name: DefaultLayer::HandleMouseMove
// Created: ABR 2011-10-28
// -----------------------------------------------------------------------------
bool DefaultLayer::HandleMouseMove( QMouseEvent* /*mouse*/, const geometry::Point2f& point )
{
    point_ = point;
    controllers_.actions_.OverFly( point_ );
    return false;
}
