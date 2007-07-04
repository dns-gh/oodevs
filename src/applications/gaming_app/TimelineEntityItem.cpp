// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "TimelineEntityItem.h"
#include "TimelineActionItem.h"
#include "clients_kernel/Entity_ABC.h"
#include <qpainter.h>

// -----------------------------------------------------------------------------
// Name: TimelineEntityItem constructor
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
TimelineEntityItem::TimelineEntityItem( QCanvas* canvas, QCanvasView* view, const kernel::Entity_ABC& entity )
    : QCanvasRectangle( 0, 0, 200, 30, canvas )
    , view_( *view )
    , entity_( entity )
{
    setZ( 900 );
    show();
}

// -----------------------------------------------------------------------------
// Name: TimelineEntityItem destructor
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
TimelineEntityItem::~TimelineEntityItem()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TimelineEntityItem::SetYOffset
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
void TimelineEntityItem::SetYOffset( unsigned int lineIndex )
{
    setY( lineIndex * height() + 15 ); // $$$$ SBO 2007-07-04: ruler height !!
}

// -----------------------------------------------------------------------------
// Name: TimelineEntityItem::NotifyCreated
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
void TimelineEntityItem::NotifyCreated( const Action_ABC& action )
{
    TimelineActionItem*& item = items_[ &action ];
    if( !item )
        item = new TimelineActionItem( this, action );
}

// -----------------------------------------------------------------------------
// Name: TimelineEntityItem::NotifyUpdated
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
void TimelineEntityItem::NotifyUpdated( const Action_ABC& action )
{
//    CIT_Items it = items_.find( &action );
//    if( it != items_.end() )
//        it->second->Update( action );
}

// -----------------------------------------------------------------------------
// Name: TimelineEntityItem::NotifyDeleted
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
void TimelineEntityItem::NotifyDeleted( const Action_ABC& action )
{
    T_Items::iterator it = items_.find( &action );
    if( it != items_.end() )
    {
        delete it->second;
        items_.erase( it );
    }
}

// -----------------------------------------------------------------------------
// Name: TimelineEntityItem::draw
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
void TimelineEntityItem::draw( QPainter& painter )
{
    const QPoint canvasTopLeft = view_.inverseWorldMatrix().map( QPoint( view_.contentsX(), view_.contentsY() ) );
    setX( canvasTopLeft.x() );
    painter.fillRect( rect(), QColor( 240, 240, 240 ) );
    painter.drawText( rect(), Qt::AlignLeft | Qt::AlignVCenter, " " + entity_.GetName() );
}

// -----------------------------------------------------------------------------
// Name: TimelineEntityItem::chunks
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
QPointArray TimelineEntityItem::chunks() const
{
    // $$$$ SBO 2007-07-04: this is a hack to force redraw on each canvas update => item is "always present in viewport"
    QPointArray array = QCanvasRectangle::chunks();
    array.resize( array.size() + 1 );
    array[int( array.size() - 1 )] = view_.inverseWorldMatrix().map( QPoint( view_.contentsX() + 10, view_.contentsY() + 10 ) );
    return array;
}
