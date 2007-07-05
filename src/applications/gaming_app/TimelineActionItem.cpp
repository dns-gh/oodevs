// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "TimelineActionItem.h"
#include "TimelineEntityItem.h"
#include "gaming/Action_ABC.h"
#include <qpainter.h>

// -----------------------------------------------------------------------------
// Name: TimelineActionItem constructor
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
TimelineActionItem::TimelineActionItem( TimelineEntityItem& parent, const Action_ABC& action )
    : QCanvasRectangle( parent.canvas() )
    , parentItem_( parent )
    , action_( action )
{
    palette_.setColor( QPalette::Inactive, QColorGroup::Background, QColor( 200, 200, 200 ) );
    palette_.setColor( QPalette::Inactive, QColorGroup::Foreground, QColor( 100, 100, 100 ) );
    palette_.setColor( QPalette::Active  , QColorGroup::Background, QColor( 180, 220, 250 ) );
    palette_.setColor( QPalette::Active  , QColorGroup::Foreground, QColor(  50, 120, 200 ) );

    setX( parent.width() + 50 ); // $$$$ SBO 2007-07-05: action time
    setZ( parent.z() - 100 );
    show();
}

// -----------------------------------------------------------------------------
// Name: TimelineActionItem destructor
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
TimelineActionItem::~TimelineActionItem()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TimelineActionItem::moveBy
// Created: SBO 2007-07-05
// -----------------------------------------------------------------------------
void TimelineActionItem::moveBy( double dx, double dy )
{
    if( x() + dx < parentItem_.width() )
        dx = 0;
    QCanvasRectangle::moveBy( dx, dy );
    // $$$$ SBO 2007-07-05: set action time
}

// -----------------------------------------------------------------------------
// Name: TimelineActionItem::draw
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
void TimelineActionItem::draw( QPainter& painter )
{
    const QPalette::ColorGroup colorGroup = isSelected() ? QPalette::Active : QPalette::Inactive;
    const QPen oldPen = painter.pen();

    setY( parentItem_.y() + 3 );
    setSize( 100, parentItem_.height() - 5 );

    painter.fillRect( rect(), palette_.color( colorGroup, QColorGroup::Background ) );
    painter.setPen( palette_.color( colorGroup, QColorGroup::Foreground ) );
    painter.drawRect( rect() );
    painter.drawText( rect(), Qt::AlignLeft | Qt::AlignVCenter, " " + action_.GetName() );

    painter.setPen( oldPen );
}
