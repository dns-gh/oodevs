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

#include "CursorStrategy.h"
#include "GlTools_ABC.h"
#include "LocationEditor_ABC.h"
#include "ShapeHandler_ABC.h"
#include "TextEditor.h"
#include "resources.h"

#include "clients_kernel/Circle.h"
#include "clients_kernel/Curve.h"
#include "clients_kernel/Lines.h"
#include "clients_kernel/Path.h"
#include "clients_kernel/Point.h"
#include "clients_kernel/Polygon.h"
#include "clients_kernel/Rectangle.h"
#include "clients_kernel/Text.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: ParametersLayer constructor
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
ParametersLayer::ParametersLayer( kernel::Controllers& controllers, GlTools_ABC& tools, TextEditor& textEditor )
    : Layer( controllers, tools, eLayerTypes_Parameters )
    , textEditor_( textEditor )
    , cursors_( new CursorStrategy( tools_ ) )
    , handler_( 0 )
    , current_( 0 )
    , validation_( false )
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
// Created: SBO 2008-06-03
// -----------------------------------------------------------------------------
void ParametersLayer::Paint( const geometry::Rectangle2f& viewport )
{
    viewport_ = viewport;
    Layer::Paint( viewport );
}

// -----------------------------------------------------------------------------
// Name: ParametersLayer::Paint
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
void ParametersLayer::Paint( Viewport_ABC& /*viewport*/ )
{
    if( ! current_ || !handler_ )
        return;
    glPushAttrib( GL_CURRENT_BIT | GL_LINE_BIT );
        glColor4f( COLOR_UNDERCONST );
        glLineWidth( 3.f );
        if( !validation_ )
            current_->AddPoint( lastPoint_ );
        handler_->Draw( *current_, viewport_, tools_ );
        if( !validation_ )
            current_->PopPoint();
    glPopAttrib();
}

// -----------------------------------------------------------------------------
// Name: ParametersLayer::HandleKeyPress
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
bool ParametersLayer::HandleKeyPress( QKeyEvent* key )
{
    if( !current_ )
        return false;
    switch( key->key() )
    {
        case Qt::Key_Backspace:
        case Qt::Key_Delete:
            current_->PopPoint();
            return true;
        case Qt::Key_Return:
        case Qt::Key_Enter:
            NotifyDone();
            return true;
        case Qt::Key_Escape:
            Reset();
            return true;
    };
    return false;
}

// -----------------------------------------------------------------------------
// Name: ParametersLayer::HandleMouseMove
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
bool ParametersLayer::HandleMouseMove( QMouseEvent*, const geometry::Point2f& point )
{
    lastPoint_ = point;
    return current_ != 0;
}

// -----------------------------------------------------------------------------
// Name: ParametersLayer::HandleMousePress
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
bool ParametersLayer::HandleMousePress( QMouseEvent* mouse, const geometry::Point2f& point )
{
    if( !current_ )
        return false;

    if( world_.IsInside( point ) )
    {
        if( mouse->button() == Qt::LeftButton && mouse->buttons() != Qt::NoButton )
            AddPoint( point );
        if( mouse->button() == Qt::RightButton && mouse->buttons() != Qt::NoButton )
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
// Name: ParametersLayer::IsInsideWorld
// Created: JSR 2012-06-25
// -----------------------------------------------------------------------------
bool ParametersLayer::IsInsideWorld( const geometry::Point2f& point ) const
{
    return world_.IsInside( point );
}

// -----------------------------------------------------------------------------
// Name: ParametersLayer::HandleMouseDoubleClick
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
bool ParametersLayer::HandleMouseDoubleClick( QMouseEvent* mouse, const geometry::Point2f& /*point*/ )
{
    if( ! current_ )
        return false;

    if( mouse->button() == Qt::RightButton )
        current_->PopPoint();
    if( mouse->button() == Qt::LeftButton )
        NotifyDone();

    return true;
}

// -----------------------------------------------------------------------------
// Name: ParametersLayer::StartPoint
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
void ParametersLayer::StartPoint( ShapeHandler_ABC& handler )
{
    cursors_->SelectTool( MAKE_PIXMAP( point_cursor ), true );
    Start( handler, Point() );
}

// -----------------------------------------------------------------------------
// Name: ParametersLayer::StartLine
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
void ParametersLayer::StartLine( ShapeHandler_ABC& handler )
{
    cursors_->SelectTool( MAKE_PIXMAP( line_cursor ), true );
    Start( handler, Lines() );
}

// -----------------------------------------------------------------------------
// Name: ParametersLayer::StartCurve
// Created: LGY 2013-04-12
// -----------------------------------------------------------------------------
void ParametersLayer::StartCurve( ShapeHandler_ABC& handler )
{
    cursors_->SelectTool( MAKE_PIXMAP( line_cursor ), true );
    Start( handler, Curve() );
}

// -----------------------------------------------------------------------------
// Name: ParametersLayer::StartText
// Created: LGY 2014-06-10
// -----------------------------------------------------------------------------
void ParametersLayer::StartText( ShapeHandler_ABC& handler, const QColor& color )
{
    textEditor_.SetFontColor( color );
    cursors_->SelectTool( MAKE_PIXMAP( text_cursor ), true );
    Start( handler, Text() );
}

// -----------------------------------------------------------------------------
// Name: ParametersLayer::StartRectangle
// Created: SLG 2010-03-24
// -----------------------------------------------------------------------------
void ParametersLayer::StartRectangle( ShapeHandler_ABC& handler )
{
    cursors_->SelectTool( MAKE_PIXMAP( rectangle_cursor ), true );
    Start( handler, kernel::Rectangle() );
}

// -----------------------------------------------------------------------------
// Name: ParametersLayer::StartPolygon
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
void ParametersLayer::StartPolygon( ShapeHandler_ABC& handler )
{
    cursors_->SelectTool( MAKE_PIXMAP( polygon_cursor ), true );
    Start( handler, kernel::Polygon() );
}

// -----------------------------------------------------------------------------
// Name: ParametersLayer::StartCircle
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
void ParametersLayer::StartCircle( ShapeHandler_ABC& handler )
{
    cursors_->SelectTool( MAKE_PIXMAP( circle_cursor ), true );
    Start( handler, Circle() );
}

// -----------------------------------------------------------------------------
// Name: ParametersLayer::StartPath
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
void ParametersLayer::StartPath( ShapeHandler_ABC& handler, const Positions& position )
{
    cursors_->SelectTool( MAKE_PIXMAP( path_cursor ), true );
    Start( handler, Path( &position ) );
}

// -----------------------------------------------------------------------------
// Name: ParametersLayer::Start
// Created: AGE 2006-08-09
// -----------------------------------------------------------------------------
void ParametersLayer::Start( ShapeHandler_ABC& handler, const Location_ABC& location )
{
    handler_ = &handler;
    current_ = &location.Clone();
}

// -----------------------------------------------------------------------------
// Name: ParametersLayer::Start
// Created: LGY 2014-06-10
// -----------------------------------------------------------------------------
void ParametersLayer::Start( ShapeHandler_ABC& handler, const Location_ABC& location, const QColor& color )
{
    textEditor_.SetFontColor( color );
    handler_ = &handler;
    current_ = &location.Clone();
}

// -----------------------------------------------------------------------------
// Name: ParametersLayer::Reset
// Created: SBO 2007-03-01
// -----------------------------------------------------------------------------
void ParametersLayer::Reset()
{
    if( handler_ )
        handler_->Reset();
    handler_ = 0;
    delete current_;
    current_ = 0;
    validation_ = false;
    cursors_->SelectTool( QCursor(), false );
}

// -----------------------------------------------------------------------------
// Name: ParametersLayer::NotifyDone
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
void ParametersLayer::NotifyDone()
{
    if( !current_ || current_ && current_->IsValid() )
    {
        ShapeHandler_ABC* handler = handler_;
        Location_ABC* location = current_;

        if( location )
        {
            if( location->GetTypeName() == "text" )
            {
                validation_ = true;
                if( textEditor_.Exec( location ) != QDialog::Accepted )
                {
                    Reset();
                    return;
                }
            }
            handler->Handle( *location );
        }
        validation_ = false;
        handler_ = 0;
        current_ = 0;
        cursors_->SelectTool( QCursor(), false );
    }
}

// -----------------------------------------------------------------------------
// Name: ParametersLayer::SelectRaster
// Created: SLG 2010-03-25
// -----------------------------------------------------------------------------
void ParametersLayer::SelectRaster( ShapeHandler_ABC& handler )
{
    Location_ABC* location = new kernel::Rectangle();
    location->AddPoint( world_.BottomLeft() );
    location->AddPoint( world_.TopRight() );
    handler.Handle( *location );
}

// -----------------------------------------------------------------------------
// Name: ParametersLayer::SetPoint
// Created: ABR 2014-02-26
// -----------------------------------------------------------------------------
void ParametersLayer::SetPoint( ShapeHandler_ABC& handler, const geometry::Point2f& point )
{
    Point location;
    location.AddPoint( point );
    handler.Handle( location.Clone() );
}
