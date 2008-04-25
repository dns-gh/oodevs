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
#include "moc_TimelineActionItem.cpp"
#include "TimelineView.h"
#include "TimelineEntityItem.h"
#include "gaming/Action_ABC.h"
#include "gaming/ActionTiming.h"
#include "clients_kernel/Controllers.h"
#include <qpainter.h>
#include <qfontmetrics.h>

// -----------------------------------------------------------------------------
// Name: TimelineActionItem constructor
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
TimelineActionItem::TimelineActionItem( const TimelineView& view, const TimelineItem_ABC& parent, kernel::Controllers& controllers, const Action_ABC& action )
    : QObject( view.canvas() )
    , TimelineItem_ABC( view.canvas() )
    , view_( view )
    , controllers_( controllers )
    , parentItem_( parent )
    , action_( action )
    , textWidth_( 0 )
{
    palette_.setColor( QPalette::Disabled, QColorGroup::Background, QColor( 220, 220, 220 ) );
    palette_.setColor( QPalette::Disabled, QColorGroup::Foreground, QColor( 180, 180, 180 ) );
    palette_.setColor( QPalette::Inactive, QColorGroup::Background, QColor( 200, 240, 215 ) );
    palette_.setColor( QPalette::Inactive, QColorGroup::Foreground, QColor(  50, 200, 105 ) );
    palette_.setColor( QPalette::Active  , QColorGroup::Background, QColor( 200, 215, 240 ) );
    palette_.setColor( QPalette::Active  , QColorGroup::Foreground, QColor(  50, 105, 200 ) );

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
        setX( view_.ConvertToPosition( timing.GetTime() ) );
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
        setX( view_.ConvertToPosition( timing->GetTime() ) );
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
            timing->Shift( view_.ConvertToSeconds( shift ) );
}

// -----------------------------------------------------------------------------
// Name: TimelineActionItem::Update
// Created: SBO 2007-07-06
// -----------------------------------------------------------------------------
void TimelineActionItem::Update()
{
    setY( parentItem_.y() + 1 );
    setSize( textWidth_ + 10, parentItem_.height() - 1 );
}

// -----------------------------------------------------------------------------
// Name: TimelineActionItem::ComputeTextWidth
// Created: SBO 2008-04-22
// -----------------------------------------------------------------------------
void TimelineActionItem::ComputeTextWidth( QPainter& painter )
{
    QFontMetrics metrics( painter.font() );
    textWidth_ = metrics.width( action_.GetName() );
}

namespace
{
    void DrawArrow( QPainter& painter, const QRect& rect )
    {
        painter.drawLine( rect.left()    , rect.top()    , rect.left()    , rect.bottom() );
        painter.drawLine( rect.left() - 2, rect.top() + 2, rect.left()    , rect.top() );
        painter.drawLine( rect.left() + 2, rect.top() + 2, rect.left()    , rect.top() );
        painter.drawLine( rect.left() + 2, rect.top() + 2, rect.left() - 2, rect.top() + 2 );
    }

    void DrawBox( QPainter& painter, const QRect& rect, const QColor& background, const QColor& foreground )
    {
        painter.fillRect( rect, background );
        painter.setPen( foreground );
        painter.drawRect( rect );
    }
}

// -----------------------------------------------------------------------------
// Name: TimelineActionItem::draw
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
void TimelineActionItem::draw( QPainter& painter )
{
    if( !textWidth_ )
        ComputeTextWidth( painter );
    Update();
    if( isSelected() )
        setZ( parentItem_.z() );
    else
        setZ( parentItem_.z() - 1 );
    const QPalette::ColorGroup colorGroup = isEnabled() ? ( isSelected() ? QPalette::Active : QPalette::Inactive ) : QPalette::Disabled;
    const QPen oldPen = painter.pen();

    painter.fillRect( rect().left(), rect().top() + 1, rect().width(), rect().height() - 2, Qt::white ); // $$$$ SBO 2008-04-22: clear color
    const QRect bottomRect( rect().left(), rect().bottom() - 3, 30, 4 );
    DrawBox( painter, bottomRect, palette_.color( colorGroup, QColorGroup::Background ), palette_.color( colorGroup, QColorGroup::Foreground ) );
    DrawArrow( painter, rect() );
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
        tip += "<table cellspacing='0' cellpadding='0'><tr><td>Starts: </td><td><i>"
        + action_.Get< ActionTiming >().GetTime().toString() + "</i></td></tr></table>";
    if( QToolTip::textFor( parent ) != tip )
    {
        QToolTip::remove( parent );
        QToolTip::add( parent, tip );
    }
}

// -----------------------------------------------------------------------------
// Name: TimelineActionItem::DisplayContextMenu
// Created: SBO 2008-04-22
// -----------------------------------------------------------------------------
void TimelineActionItem::DisplayContextMenu( QWidget* parent, const QPoint& pos  ) const
{
    QPopupMenu* popup = new QPopupMenu( parent );
    popup->insertItem( tr( "Rename" ), this, SLOT( OnRename() ) );
    popup->insertItem( tr( "Delete" ), this, SLOT( OnDelete() ) );
    popup->popup( pos );
}

// -----------------------------------------------------------------------------
// Name: TimelineActionItem::OnDelete
// Created: SBO 2008-04-22
// -----------------------------------------------------------------------------
void TimelineActionItem::OnDelete()
{
     // $$$$ SBO 2008-04-22: 
}

// -----------------------------------------------------------------------------
// Name: TimelineActionItem::OnRename
// Created: SBO 2008-04-22
// -----------------------------------------------------------------------------
void TimelineActionItem::OnRename()
{
     // $$$$ SBO 2008-04-22: 
}
