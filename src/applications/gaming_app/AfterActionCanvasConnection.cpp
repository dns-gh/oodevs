// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "AfterActionCanvasConnection.h"
#include "AfterActionCanvasItem.h"
#include "gaming/AfterActionItem_ABC.h"
#include <qpainter.h>

// -----------------------------------------------------------------------------
// Name: AfterActionCanvasConnection constructor
// Created: AGE 2007-09-18
// -----------------------------------------------------------------------------
AfterActionCanvasConnection::AfterActionCanvasConnection( const QPalette& palette, AfterActionCanvasItem* to, int index, double x, double y )
    : QCanvasLine( to->canvas() )
    , palette_   ( palette )
    , from_      ( 0 )
    , to_        ( to )
    , toIndex_   ( index )
{
    setPoints( x, y, x, y );
    setEnabled( true );
    show();
}

// -----------------------------------------------------------------------------
// Name: AfterActionCanvasConnection constructor
// Created: AGE 2007-09-20
// -----------------------------------------------------------------------------
AfterActionCanvasConnection::AfterActionCanvasConnection( const QPalette& palette, AfterActionCanvasItem* from, double x, double y )
    : QCanvasLine( from->canvas() )
    , palette_   ( palette )
    , from_      ( from )
    , to_        ( 0 )
    , toIndex_   ( 0 )
{
    setPoints( x, y, x, y );
    setEnabled( true );
    show();
}

// -----------------------------------------------------------------------------
// Name: AfterActionCanvasConnection destructor
// Created: AGE 2007-09-18
// -----------------------------------------------------------------------------
AfterActionCanvasConnection::~AfterActionCanvasConnection()
{
    hide();
    setCanvas( 0 );
    if( from_ ) from_->Remove( this );
    if( to_ )   to_->Remove( this );
}

// -----------------------------------------------------------------------------
// Name: AfterActionCanvasConnection::Close
// Created: AGE 2007-09-21
// -----------------------------------------------------------------------------
void AfterActionCanvasConnection::Close( AfterActionCanvasItem* from, AfterActionCanvasItem* to, int i, double x1, double y1, double x2, double y2 )
{
    if( ( from_ && from_ != from )
    ||  ( to_ && to_ != to && toIndex_ != i ) )
        throw std::runtime_error( "Invalid connection" );
    from_ = from;
    to_   = to;
    toIndex_ = i;
    setPoints( x1, y1, x2, y2 );
}

// -----------------------------------------------------------------------------
// Name: AfterActionCanvasConnection::Move
// Created: AGE 2007-09-18
// -----------------------------------------------------------------------------
void AfterActionCanvasConnection::Move( const QPoint& to )
{
    if( from_ )
        setPoints( startPoint().x(), startPoint().y(), to.x(), to.y() );
    else
        setPoints( to.x(), to.y(), endPoint().x(), endPoint().y() );
}

// -----------------------------------------------------------------------------
// Name: AfterActionCanvasConnection::MoveBy
// Created: AGE 2007-09-18
// -----------------------------------------------------------------------------
void AfterActionCanvasConnection::MoveBy( AfterActionCanvasItem* item, double x, double y )
{
    if( item == from_ )
        setPoints( startPoint().x() + x, startPoint().y() + y, endPoint().x(), endPoint().y() );
    else if( item == to_ )
        setPoints( startPoint().x(), startPoint().y(), endPoint().x() + x, endPoint().y() + y );
}

// -----------------------------------------------------------------------------
// Name: AfterActionCanvasConnection::drawShape
// Created: AGE 2007-09-19
// -----------------------------------------------------------------------------
void AfterActionCanvasConnection::drawShape( QPainter& p )
{
    const QPalette::ColorGroup colorGroup = isEnabled() ? ( isSelected() ? QPalette::Active : QPalette::Inactive ) : QPalette::Disabled;
    p.setPen( palette_.color( colorGroup, QColorGroup::Foreground ) );
    QCanvasLine::drawShape( p );
}

// -----------------------------------------------------------------------------
// Name: AfterActionCanvasConnection::From
// Created: AGE 2007-09-20
// -----------------------------------------------------------------------------
AfterActionCanvasItem* AfterActionCanvasConnection::From() const
{
    return from_;
}

// -----------------------------------------------------------------------------
// Name: AfterActionCanvasConnection::To
// Created: AGE 2007-09-20
// -----------------------------------------------------------------------------
AfterActionCanvasItem* AfterActionCanvasConnection::To() const
{
    return to_;
}

// -----------------------------------------------------------------------------
// Name: AfterActionCanvasConnection::ToIndex
// Created: AGE 2007-09-20
// -----------------------------------------------------------------------------
int AfterActionCanvasConnection::ToIndex() const
{
    return toIndex_;
}
