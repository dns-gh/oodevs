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
#include <qpainter.h>

// -----------------------------------------------------------------------------
// Name: AfterActionCanvasConnection constructor
// Created: AGE 2007-09-18
// -----------------------------------------------------------------------------
AfterActionCanvasConnection::AfterActionCanvasConnection( const QPalette& palette, AfterActionCanvasItem* from, int index, const QPoint& attach )
    : QCanvasLine( from->canvas() )
    , palette_   ( palette )
    , from_      ( from )
    , fromIndex_ ( index )
    , to_        ( 0 )
    , toIndex_   ( 0 )
{
    setPoints( attach.x(), attach.y(), attach.x(), attach.y() );
    setEnabled( true );
    show();
}

// -----------------------------------------------------------------------------
// Name: AfterActionCanvasConnection destructor
// Created: AGE 2007-09-18
// -----------------------------------------------------------------------------
AfterActionCanvasConnection::~AfterActionCanvasConnection()
{
    Disconnect();
    hide();
    setCanvas( 0 );
}

// -----------------------------------------------------------------------------
// Name: AfterActionCanvasConnection::Disconnect
// Created: AGE 2007-09-18
// -----------------------------------------------------------------------------
void AfterActionCanvasConnection::Disconnect( AfterActionCanvasItem* item )
{
    Remove( from_, item );
    Remove( to_, item );
}

// -----------------------------------------------------------------------------
// Name: AfterActionCanvasConnection::Remove
// Created: AGE 2007-09-19
// -----------------------------------------------------------------------------
void AfterActionCanvasConnection::Remove( AfterActionCanvasItem*& mine, AfterActionCanvasItem* item )
{
    if( mine && mine != item )
        mine->Remove( this );
    mine = 0;
}

// -----------------------------------------------------------------------------
// Name: AfterActionCanvasConnection::IsConnected
// Created: AGE 2007-09-19
// -----------------------------------------------------------------------------
bool AfterActionCanvasConnection::IsConnected( AfterActionCanvasItem* item, int index ) const
{
    return ( item == from_ && index == fromIndex_ )
        || ( item == to_   && index == toIndex_ );
}

// -----------------------------------------------------------------------------
// Name: AfterActionCanvasConnection::Move
// Created: AGE 2007-09-18
// -----------------------------------------------------------------------------
void AfterActionCanvasConnection::Move( const QPoint& to )
{
    setPoints( startPoint().x(), startPoint().y(), to.x(), to.y() );
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
// Name: AfterActionCanvasConnection::Close
// Created: AGE 2007-09-18
// -----------------------------------------------------------------------------
bool AfterActionCanvasConnection::Close( AfterActionCanvasItem* item, int index, const QPoint& attach )
{
    if( !to_ && item != from_ && index * fromIndex_ < 0 )
    {
        setPoints( startPoint().x(), startPoint().y(), attach.x(), attach.y() );
        to_ = item; toIndex_ = index;
        return true;
    }
    return false;
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


