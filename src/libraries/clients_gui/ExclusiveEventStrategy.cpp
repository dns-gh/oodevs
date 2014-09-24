// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "ExclusiveEventStrategy.h"
#include "Layer_ABC.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: ExclusiveEventStrategy constructor
// Created: AGE 2006-09-04
// -----------------------------------------------------------------------------
ExclusiveEventStrategy::ExclusiveEventStrategy( EventStrategy_ABC& forward )
    : forward_( forward )
    , layer_  ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ExclusiveEventStrategy destructor
// Created: AGE 2006-09-04
// -----------------------------------------------------------------------------
ExclusiveEventStrategy::~ExclusiveEventStrategy()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ExclusiveEventStrategy::TakeExclusiveFocus
// Created: AGE 2006-09-04
// -----------------------------------------------------------------------------
void ExclusiveEventStrategy::TakeExclusiveFocus( const std::shared_ptr< Layer_ABC >& layer )
{
    layer_ = layer;
}

// -----------------------------------------------------------------------------
// Name: ExclusiveEventStrategy::ReleaseExclusiveFocus
// Created: AGE 2006-09-04
// -----------------------------------------------------------------------------
void ExclusiveEventStrategy::ReleaseExclusiveFocus()
{
    layer_ = 0;
}

// -----------------------------------------------------------------------------
// Name: ExclusiveEventStrategy::HandleKeyPress
// Created: AGE 2006-09-04
// -----------------------------------------------------------------------------
void ExclusiveEventStrategy::HandleKeyPress( QKeyEvent* key )
{
    if( layer_ )
        layer_->HandleKeyPress( key );
    else
        forward_.HandleKeyPress( key );
}

// -----------------------------------------------------------------------------
// Name: ExclusiveEventStrategy::HandleKeyRelease
// Created: LGY 2011-10-06
// -----------------------------------------------------------------------------
void ExclusiveEventStrategy::HandleKeyRelease( QKeyEvent* /*key*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ExclusiveEventStrategy::HandleMousePress
// Created: AGE 2006-09-04
// -----------------------------------------------------------------------------
void ExclusiveEventStrategy::HandleMousePress( QMouseEvent* mouse, const geometry::Point2f& point )
{
    if( layer_ )
        layer_->HandleMousePress( mouse, point );
    else
        forward_.HandleMousePress( mouse, point );
}

// -----------------------------------------------------------------------------
// Name: ExclusiveEventStrategy::HandleMouseRelease
// Created: LGY 2013-03-08
// -----------------------------------------------------------------------------
void ExclusiveEventStrategy::HandleMouseRelease( QMouseEvent* mouse, const geometry::Point2f& point )
{
    if( layer_ )
        layer_->HandleMouseRelease( mouse, point );
    else
        forward_.HandleMouseRelease( mouse, point );
}

// -----------------------------------------------------------------------------
// Name: ExclusiveEventStrategy::HandleMouseDoubleClick
// Created: AGE 2006-09-04
// -----------------------------------------------------------------------------
void ExclusiveEventStrategy::HandleMouseDoubleClick( QMouseEvent* mouse, const geometry::Point2f& point )
{
    if( layer_ )
        layer_->HandleMouseDoubleClick( mouse, point );
    else
        forward_.HandleMouseDoubleClick( mouse, point );
}

// -----------------------------------------------------------------------------
// Name: ExclusiveEventStrategy::HandleMouseMove
// Created: AGE 2006-09-04
// -----------------------------------------------------------------------------
void ExclusiveEventStrategy::HandleMouseMove( QMouseEvent* mouse, const geometry::Point2f& point )
{
    if( layer_ )
        layer_->HandleMouseMove( mouse, point );
    else
        forward_.HandleMouseMove( mouse, point );
}

// -----------------------------------------------------------------------------
// Name: ExclusiveEventStrategy::HandleMouseWheel
// Created: AGE 2006-11-21
// -----------------------------------------------------------------------------
void ExclusiveEventStrategy::HandleMouseWheel( QWheelEvent* mouse, const geometry::Point2f& point )
{
    if( layer_ )
        layer_->HandleMouseWheel( mouse, point );
    else
        forward_.HandleMouseWheel( mouse, point );
}

// -----------------------------------------------------------------------------
// Name: ExclusiveEventStrategy::HandleDropEvent
// Created: AGE 2006-09-04
// -----------------------------------------------------------------------------
void ExclusiveEventStrategy::HandleDropEvent( QDropEvent* event, const geometry::Point2f& point )
{
    if( layer_ )
        layer_->HandleDropEvent( event, point );
    else
        forward_.HandleDropEvent( event, point );
}

// -----------------------------------------------------------------------------
// Name: ExclusiveEventStrategy::HandleEnterDragEvent
// Created: AGE 2006-09-04
// -----------------------------------------------------------------------------
void ExclusiveEventStrategy::HandleEnterDragEvent( QDragEnterEvent* event, const geometry::Point2f& point )
{
    if( layer_ )
        layer_->HandleEnterDragEvent( event, point );
    else
        forward_.HandleEnterDragEvent( event, point );
}

// -----------------------------------------------------------------------------
// Name: ExclusiveEventStrategy::HandleMoveDragEvent
// Created: ABR 2011-10-26
// -----------------------------------------------------------------------------
void ExclusiveEventStrategy::HandleMoveDragEvent( QDragMoveEvent* event, const geometry::Point2f& point )
{
    if( layer_ )
        layer_->HandleMoveDragEvent( event, point );
    else
        forward_.HandleMoveDragEvent( event, point );
}

// -----------------------------------------------------------------------------
// Name: ExclusiveEventStrategy::HandleLeaveDragEvent
// Created: JSR 2012-06-26
// -----------------------------------------------------------------------------
void ExclusiveEventStrategy::HandleLeaveDragEvent( QDragLeaveEvent* event )
{
    if( layer_ )
        layer_->HandleLeaveDragEvent( event );
    else
        forward_.HandleLeaveDragEvent( event );
}
