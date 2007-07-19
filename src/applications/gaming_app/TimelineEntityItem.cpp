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
TimelineEntityItem::TimelineEntityItem( QCanvasView& view, const TimelineItem_ABC* after, kernel::Controllers& controllers, const kernel::Entity_ABC& entity )
    : TimelineItem_ABC( view.canvas(), QRect( 0, 0, itemWidth_, itemHeight_ ) )
    , controllers_( controllers )
    , view_( view )
    , previous_( after )
    , entity_( entity )
{
    SetOverlayed( true );
    setZ( 900 );
    if( previous_ )
        setY( previous_->y() + previous_->height() );
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
    TimelineItem_ABC*& item = childItems_[ &action ];
    if( !item )
    {
        item = new TimelineActionItem( *this, controllers_, action );
        canvas()->resize( canvas()->width(), canvas()->height() + itemHeight_ );
    }
}

// -----------------------------------------------------------------------------
// Name: TimelineEntityItem::RemoveAction
// Created: SBO 2007-07-05
// -----------------------------------------------------------------------------
void TimelineEntityItem::RemoveAction( const Action_ABC& action )
{
    T_Items::iterator it = childItems_.find( &action );
    if( it != childItems_.end() )
    {
        delete it->second;
        childItems_.erase( it );
    }
}

// -----------------------------------------------------------------------------
// Name: TimelineEntityItem::Update
// Created: SBO 2007-07-06
// -----------------------------------------------------------------------------
void TimelineEntityItem::Update()
{
    const QPoint canvasTopLeft = view_.inverseWorldMatrix().map( QPoint( view_.contentsX(), view_.contentsY() ) );
    setX( canvasTopLeft.x() );
    if( previous_ && ! previous_->IsOverlayed() )
        setY( previous_->y() + previous_->height() );
}

// -----------------------------------------------------------------------------
// Name: TimelineEntityItem::draw
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
void TimelineEntityItem::draw( QPainter& painter )
{
    static const QColor fillColor( 240, 240, 240 );
    painter.fillRect( rect().left(), rect().top() + 1, rect().width(), rect().height() - 2, fillColor );
    painter.drawText( rect(), Qt::AlignLeft | Qt::AlignVCenter, " " + entity_.GetName() );
}

// -----------------------------------------------------------------------------
// Name: TimelineEntityItem::DisplayToolTip
// Created: SBO 2007-07-19
// -----------------------------------------------------------------------------
void TimelineEntityItem::DisplayToolTip( QWidget* parent ) const
{
    QString tip = QString( "<nobr><b>%1 - [%2]</b></nobr>" ).arg( entity_.GetName(), QString::number( entity_.GetId() ) );
    if( QToolTip::textFor( parent ) != tip )
    {
        QToolTip::remove( parent );
        QToolTip::add( parent, tip );
    }
}
