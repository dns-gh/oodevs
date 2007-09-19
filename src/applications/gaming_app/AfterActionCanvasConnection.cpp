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

// -----------------------------------------------------------------------------
// Name: AfterActionCanvasConnection constructor
// Created: AGE 2007-09-18
// -----------------------------------------------------------------------------
AfterActionCanvasConnection::AfterActionCanvasConnection( AfterActionCanvasItem* from, int index, const QPoint& attach )
    : QCanvasLine( from->canvas() )
    , from_      ( from )
    , fromIndex_ ( index )
    , to_        ( 0 )
    , toIndex_   ( 0 )
{
    setPoints( attach.x(), attach.y(), attach.x(), attach.y() );
    show();
}

// -----------------------------------------------------------------------------
// Name: AfterActionCanvasConnection destructor
// Created: AGE 2007-09-18
// -----------------------------------------------------------------------------
AfterActionCanvasConnection::~AfterActionCanvasConnection()
{
    hide();
}

// -----------------------------------------------------------------------------
// Name: AfterActionCanvasConnection::Disconnect
// Created: AGE 2007-09-18
// -----------------------------------------------------------------------------
void AfterActionCanvasConnection::Disconnect( AfterActionCanvasItem* item )
{
    if( from_ && from_ != item )
        from_->Remove( this );
    if( to_ && to_ != item )
        to_->Remove( this );
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

