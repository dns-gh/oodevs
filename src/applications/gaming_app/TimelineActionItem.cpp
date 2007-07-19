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
#include "gaming/ActionTiming.h"
#include "clients_kernel/Controllers.h"
#include <qpainter.h>

// -----------------------------------------------------------------------------
// Name: TimelineActionItem constructor
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
TimelineActionItem::TimelineActionItem( const TimelineItem_ABC& parent, kernel::Controllers& controllers, const Action_ABC& action )
    : TimelineItem_ABC( parent.canvas() )
    , controllers_( controllers )
    , parentItem_( parent )
    , action_( action )
{
    palette_.setColor( QPalette::Disabled, QColorGroup::Background, QColor( 220, 220, 220 ) );
    palette_.setColor( QPalette::Disabled, QColorGroup::Foreground, QColor( 180, 180, 180 ) );
    palette_.setColor( QPalette::Inactive, QColorGroup::Background, QColor( 200, 240, 215 ) );
    palette_.setColor( QPalette::Inactive, QColorGroup::Foreground, QColor(  50, 200, 105 ) );
    palette_.setColor( QPalette::Active  , QColorGroup::Background, QColor( 200, 215, 240 ) );
    palette_.setColor( QPalette::Active  , QColorGroup::Foreground, QColor(  50, 105, 200 ) );

    setZ( parent.z() - 100 );
    show();
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: TimelineActionItem destructor
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
TimelineActionItem::~TimelineActionItem()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: TimelineActionItem::NotifyUpdated
// Created: SBO 2007-07-05
// -----------------------------------------------------------------------------
void TimelineActionItem::NotifyUpdated( const ActionTiming& timing )
{
    if( &timing == action_.Retrieve< ActionTiming >() )
    {
        setX( parentItem_.width() + timing.GetTime() ); // $$$$ SBO 2007-07-05: 
        setEnabled( timing.IsEnabled() );
        if( x() + rect().width() > canvas()->width() )
            canvas()->resize( x() + rect().width(), canvas()->height() );
    }
}

// -----------------------------------------------------------------------------
// Name: TimelineActionItem::setVisible
// Created: SBO 2007-07-05
// -----------------------------------------------------------------------------
void TimelineActionItem::setVisible( bool visible )
{
    QCanvasRectangle::setVisible( visible );
    if( ActionTiming* timing = const_cast< ActionTiming* >( action_.Retrieve< ActionTiming >() ) )
    {
        if( timing->IsEnabled() != isVisible() )
            timing->ToggleEnabled();
        setX( parentItem_.width() + timing->GetTime() ); // $$$$ SBO 2007-07-05: 
    }
}

// -----------------------------------------------------------------------------
// Name: TimelineActionItem::Shift
// Created: SBO 2007-07-06
// -----------------------------------------------------------------------------
void TimelineActionItem::Shift( long shift )
{
    if( isEnabled() )
        if( ActionTiming* timing = const_cast< ActionTiming* >( action_.Retrieve< ActionTiming >() ) )
            timing->Shift( shift ); // $$$$ SBO 2007-07-06: 
}

// -----------------------------------------------------------------------------
// Name: TimelineActionItem::Update
// Created: SBO 2007-07-06
// -----------------------------------------------------------------------------
void TimelineActionItem::Update()
{
    setY( parentItem_.y() + 3 );
    setSize( 100, parentItem_.height() - 5 );
}

// -----------------------------------------------------------------------------
// Name: TimelineActionItem::draw
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
void TimelineActionItem::draw( QPainter& painter )
{
    Update();
    if( isSelected() )
        setZ( parentItem_.z() - 99 );
    else
        setZ( parentItem_.z() - 100 );
    const QPalette::ColorGroup colorGroup = isEnabled() ? ( isSelected() ? QPalette::Active : QPalette::Inactive ) : QPalette::Disabled;
    const QPen oldPen = painter.pen();
    painter.fillRect( rect(), palette_.color( colorGroup, QColorGroup::Background ) );
    painter.setPen( palette_.color( colorGroup, QColorGroup::Foreground ) );
    painter.drawRect( rect() );
    painter.drawText( rect(), Qt::AlignLeft | Qt::AlignVCenter, " " + action_.GetName() );
    painter.setPen( oldPen );
}

// -----------------------------------------------------------------------------
// Name: TimelineActionItem::DisplayToolTip
// Created: SBO 2007-07-19
// -----------------------------------------------------------------------------
void TimelineActionItem::DisplayToolTip( QWidget* parent ) const
{
    QString tip;
    tip = "<table cellspacing='0' cellpadding='0'><tr><td><img source=\"mission\"></td><td><nobr><b>" + action_.GetName() + "</b></nobr></td></tr></table>";
    if( !isEnabled() )
        tip += "<i>disabled</i>";
    else
        tip += "<table cellspacing='0' cellpadding='0'><tr><td>Start time: </td><td><i>tick " + QString::number( rect().left() - parentItem_.width() ) + "</i></td></tr></table>";
    if( QToolTip::textFor( parent ) != tip )
    {
        QToolTip::remove( parent );
        QToolTip::add( parent, tip );
    }
}
