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

const unsigned int itemHeight_ = 30;
const unsigned int itemWidth_  = 200;

// -----------------------------------------------------------------------------
// Name: TimelineEntityItem constructor
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
TimelineEntityItem::TimelineEntityItem( QCanvasView& view, const QCanvasItem* after, const kernel::Entity_ABC& entity )
    : QCanvasRectangle( 0, 0, itemWidth_, itemHeight_, view.canvas() )
    , view_( view )
    , previous_( after )
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
// Name: TimelineEntityItem::AddAction
// Created: SBO 2007-07-05
// -----------------------------------------------------------------------------
void TimelineEntityItem::AddAction( const Action_ABC& action )
{
    TimelineActionItem*& item = items_[ &action ];
    if( !item )
        item = new TimelineActionItem( *this, action );
}

// -----------------------------------------------------------------------------
// Name: TimelineEntityItem::RemoveAction
// Created: SBO 2007-07-05
// -----------------------------------------------------------------------------
void TimelineEntityItem::RemoveAction( const Action_ABC& action )
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
    if( previous_ )
        setY( previous_->y() + itemHeight_ );
    else
        setY( 15 );
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
