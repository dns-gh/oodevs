// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "ParametersLayer.h"
#include "GlTools_ABC.h"
#include "ShapeHandler_ABC.h"

// -----------------------------------------------------------------------------
// Name: ParametersLayer constructor
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
ParametersLayer::ParametersLayer( const GlTools_ABC& tools )
    : tools_( tools )
    , handler_( 0 )
    , type_( GL_LINE_STRIP )
    , expected_( 0 )
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
    if( ! handler_ )
        return;
    glPushAttrib( GL_CURRENT_BIT | GL_LINE_BIT );
        glColor4d( COLOR_UNDERCONST );
        glLineWidth( 3.f );
        if( type_ == GL_POINTS )
            for( T_PointVector::const_iterator it = points_.begin(); it != points_.end(); ++it )
                tools_.DrawCross( *it );
        else 
            tools_.DrawLines( points_ );
        if( type_ == GL_LINE_LOOP && ! points_.empty() )
            tools_.DrawLine( points_.back(), points_.front() );
    glPopAttrib();
}

// -----------------------------------------------------------------------------
// Name: ParametersLayer::HandleKeyPress
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
bool ParametersLayer::HandleKeyPress( QKeyEvent* key )
{
    if( ! handler_ )
        return false;
    switch( key->key() )
    {
        case Qt::Key_BackSpace:
        case Qt::Key_Delete:
            if( ! points_.empty() )
                points_.pop_back();
            return true;
        case Qt::Key_Return:
        case Qt::Key_Enter:
            NotifyDone();
            return true;
        case Qt::Key_Escape:
            points_.clear();
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
    return handler_; // $$$$ AGE 2006-03-23: trouver autre chose pour gerer les focus events...
}

// -----------------------------------------------------------------------------
// Name: ParametersLayer::HandleMousePress
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
bool ParametersLayer::HandleMousePress( QMouseEvent* mouse, const geometry::Point2f& point )
{
    if( ! handler_ )
        return false;

    if( world_.IsInside( point ) )
    {
        if( mouse->button() == Qt::LeftButton && mouse->state() == Qt::NoButton )
        {
            points_.push_back( point );
            if( IsDone() )
                NotifyDone();
        }

        if( mouse->button() == Qt::RightButton && mouse->state() == Qt::NoButton && ! points_.empty() )
            points_.pop_back();
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: ParametersLayer::HandleMouseDoubleClick
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
bool ParametersLayer::HandleMouseDoubleClick( QMouseEvent* mouse, const geometry::Point2f& /*point*/ )
{
    if( ! handler_ )
        return false;

    if( mouse->button() == Qt::RightButton && mouse->state() == Qt::NoButton && ! points_.empty() )
        points_.pop_back();
    if( mouse->button() == Qt::LeftButton && mouse->state() == Qt::NoButton )
        NotifyDone();

    return true;
}

// -----------------------------------------------------------------------------
// Name: ParametersLayer::Start
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
void ParametersLayer::Start( ShapeHandler_ABC& handler, int type, unsigned expected )
{
    points_.clear();
    handler_ = &handler;
    type_     = type;
    expected_ = expected;
}

// -----------------------------------------------------------------------------
// Name: ParametersLayer::IsDone
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
bool ParametersLayer::IsDone() const
{
    return points_.size() == expected_;
}

// -----------------------------------------------------------------------------
// Name: ParametersLayer::NotifyDone
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
void ParametersLayer::NotifyDone()
{
    ShapeHandler_ABC* handler = handler_;
    handler_ = 0;
    handler->Handle( points_ );
    points_.clear();
}
