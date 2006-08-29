// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "ParametersLayer.h"
#include "clients_kernel/GlTools_ABC.h"
#include "ShapeHandler_ABC.h"
#include "clients_kernel/Point.h"
#include "clients_kernel/Lines.h"
#include "clients_kernel/Polygon.h"
#include "clients_kernel/Circle.h"
#include "clients_kernel/Path.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: ParametersLayer constructor
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
ParametersLayer::ParametersLayer( const GlTools_ABC& tools )
    : tools_( tools )
    , handler_( 0 )
    , current_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParametersLayer destructor
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
ParametersLayer::~ParametersLayer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParametersLayer::Initialize
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
void ParametersLayer::Initialize( const geometry::Rectangle2f& extent )
{
    world_ = extent;
}

// -----------------------------------------------------------------------------
// Name: ParametersLayer::Paint
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
void ParametersLayer::Paint( const geometry::Rectangle2f& /*viewport*/ )
{
    if( ! current_ )
        return;
    glPushAttrib( GL_CURRENT_BIT | GL_LINE_BIT );
        glColor4d( COLOR_UNDERCONST );
        glLineWidth( 3.f );
        current_->Draw( tools_ );
    glPopAttrib();
}

// -----------------------------------------------------------------------------
// Name: ParametersLayer::HandleKeyPress
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
bool ParametersLayer::HandleKeyPress( QKeyEvent* key )
{
    if( ! current_ )
        return false;
    switch( key->key() )
    {
        case Qt::Key_BackSpace:
        case Qt::Key_Delete:
            current_->PopPoint();
            return true;
        case Qt::Key_Return:
        case Qt::Key_Enter:
            NotifyDone();
            return true;
        case Qt::Key_Escape:
            delete current_;
            current_ = 0;
            NotifyDone();
            return true;
    };
    return false;
}

// -----------------------------------------------------------------------------
// Name: ParametersLayer::HandleMouseMove
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
bool ParametersLayer::HandleMouseMove( QMouseEvent*, const geometry::Point2f& )
{
    return current_ != 0;
}

// -----------------------------------------------------------------------------
// Name: ParametersLayer::HandleMousePress
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
bool ParametersLayer::HandleMousePress( QMouseEvent* mouse, const geometry::Point2f& point )
{
    if( ! current_ )
        return false;

    if( world_.IsInside( point ) )
    {
        if( mouse->button() == Qt::LeftButton && mouse->state() == Qt::NoButton )
            AddPoint( point );

        if( mouse->button() == Qt::RightButton && mouse->state() == Qt::NoButton )
            current_->PopPoint();
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: ParametersLayer::AddPoint
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
void ParametersLayer::AddPoint( const geometry::Point2f& point )
{
    current_->AddPoint( point );
    if( current_->IsDone() )
        NotifyDone();
}

// -----------------------------------------------------------------------------
// Name: ParametersLayer::HandleMouseDoubleClick
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
bool ParametersLayer::HandleMouseDoubleClick( QMouseEvent* mouse, const geometry::Point2f& /*point*/ )
{
    if( ! current_ )
        return false;

    if( mouse->button() == Qt::RightButton && mouse->state() == Qt::NoButton )
        current_->PopPoint();
    if( mouse->button() == Qt::LeftButton && mouse->state() == Qt::NoButton )
        NotifyDone();

    return true;
}

// -----------------------------------------------------------------------------
// Name: ParametersLayer::StartPoint
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
void ParametersLayer::StartPoint( ShapeHandler_ABC& handler )
{
    Start( handler, *new Point() );
}

// -----------------------------------------------------------------------------
// Name: ParametersLayer::StartLine
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
void ParametersLayer::StartLine( ShapeHandler_ABC& handler )
{
    Start( handler, *new Lines() );
}

// -----------------------------------------------------------------------------
// Name: ParametersLayer::StartPolygon
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
void ParametersLayer::StartPolygon( ShapeHandler_ABC& handler )
{
    Start( handler, *new class Polygon() );
}

// -----------------------------------------------------------------------------
// Name: ParametersLayer::StartCircle
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
void ParametersLayer::StartCircle( ShapeHandler_ABC& handler )
{
     Start( handler, *new Circle() );
}

// -----------------------------------------------------------------------------
// Name: ParametersLayer::StartPath
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
void ParametersLayer::StartPath( ShapeHandler_ABC& handler, const Positions& position )
{
    Start( handler, *new Path( position ) );
}

// -----------------------------------------------------------------------------
// Name: ParametersLayer::Start
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
void ParametersLayer::Start( ShapeHandler_ABC& handler, Location_ABC& location )
{
    handler_ = &handler;
    current_ = &location;
}

// -----------------------------------------------------------------------------
// Name: ParametersLayer::NotifyDone
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
void ParametersLayer::NotifyDone()
{
    ShapeHandler_ABC* handler = handler_;
    Location_ABC* location = current_;
    handler_ = 0; current_ = 0;
    if( location )
        handler->Handle( *location );
}
