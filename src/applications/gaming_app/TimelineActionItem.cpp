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
#include "TimelineRuler.h"
#include "actions/Action_ABC.h"
#include "actions/ActionsModel.h"
#include "gaming/ActionTiming.h"
#include "gaming/Tools.h"
#include "clients_kernel/Controllers.h"
#include <qpainter.h>
#include <qfontmetrics.h>

using namespace actions;

// -----------------------------------------------------------------------------
// Name: TimelineActionItem constructor
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
TimelineActionItem::TimelineActionItem( QCanvas* canvas, const TimelineRuler& ruler, kernel::Controllers& controllers, ActionsModel& model, const Action_ABC& action, const QPalette& palette )
    : QObject         ( canvas )
    , TimelineItem_ABC( canvas )
    , ruler_          ( ruler )
    , controllers_    ( controllers )
    , model_          ( model )
    , action_         ( action )
    , textWidth_      ( 0 )
    , palette_        ( palette )
    , nameEditor_     ( 0 )
{
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
        Update();
}

// -----------------------------------------------------------------------------
// Name: TimelineActionItem::setVisible
// Created: SBO 2007-07-05
// -----------------------------------------------------------------------------
void TimelineActionItem::setVisible( bool visible )
{
    QCanvasRectangle::setVisible( visible );
    if( ActionTiming* timing = const_cast< ActionTiming* >( action_.Retrieve< ActionTiming >() ) )
        if( timing->IsEnabled() != isVisible() )
            timing->ToggleEnabled();
}

// -----------------------------------------------------------------------------
// Name: TimelineActionItem::Move
// Created: SBO 2007-07-06
// -----------------------------------------------------------------------------
void TimelineActionItem::Move( long offset )
{
    if( isEnabled() )
        if( ActionTiming* timing = const_cast< ActionTiming* >( action_.Retrieve< ActionTiming >() ) )
            timing->Shift( ruler_.ConvertToSeconds( offset ) );
}

// -----------------------------------------------------------------------------
// Name: TimelineActionItem::Update
// Created: SBO 2007-07-06
// -----------------------------------------------------------------------------
void TimelineActionItem::Update()
{
    setSize( textWidth_ + 10, height() );
    if( const ActionTiming* timing = action_.Retrieve< ActionTiming >() )
    {
        setX( ruler_.ConvertToPosition( timing->GetTime() ) );
        setEnabled( timing->IsEnabled() );
    }
    update();
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

    void DrawHorizontalGradientBox( QPainter& painter, const QRect& rect, const QColor& leftColor, const QColor& rightColor )
    {
        const int range = 12; //item2->GetPercentage() - item1->GetPercentage();
        if( range < 1 )
            return;
        const int rStep = ( rightColor.red()   - leftColor.red()   ) / range;
        const int gStep = ( rightColor.green() - leftColor.green() ) / range;
        const int bStep = ( rightColor.blue()  - leftColor.blue()  ) / range;
        const float xStep = float( rect.width() ) / range;

        QColor color( leftColor );
        QRect region( rect );
        for( unsigned int i = 0; i < unsigned int( range ); ++i )
        {
            color.setRgb( leftColor.red() + rStep * i, leftColor.green() + gStep * i, leftColor.blue() + bStep * i );
            region.setLeft ( rect.left() + int( xStep * i ) );
            region.setRight( rect.left() + int( xStep * ( i + 1 ) ) );
            painter.fillRect( region, color );
        }
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
    setZ( isSelected() ? std::numeric_limits< double >::max() : x() );
    const QPalette::ColorGroup colorGroup = isEnabled() ? ( isSelected() ? QPalette::Active : QPalette::Inactive ) : QPalette::Disabled;
    const QPen oldPen = painter.pen();

    //painter.fillRect( rect().left(), rect().top() + 1, rect().width(), rect().height() - 2, Qt::white ); // $$$$ SBO 2008-04-22: clear color
    const QRect gradientRect( rect().left(), rect().top(), rect().width() + 40, rect().height() );
    DrawHorizontalGradientBox( painter, gradientRect, palette_.color( colorGroup, QColorGroup::Background ), Qt::white );
    DrawArrow( painter, rect() );
    painter.setPen( oldPen );
    painter.drawText( rect(), Qt::AlignLeft | Qt::AlignVCenter, " " + action_.GetName() );
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
void TimelineActionItem::DisplayContextMenu( QPopupMenu* menu ) const
{
    if( !nameEditor_ )
    {
        nameEditor_ = new QLineEdit( menu->parentWidget() );
        QToolTip::add( nameEditor_, tools::translate( "TimelineActionItem", "Press enter to rename, unfocus field to cancel" ) );
        connect( nameEditor_, SIGNAL( returnPressed() ), SLOT( DoRename() ) );
        connect( nameEditor_, SIGNAL( lostFocus() ), nameEditor_, SLOT( hide() ) );
    }
    menu->insertItem( tools::translate( "TimelineActionItem", "Rename" ), this, SLOT( OnRename() ), Qt::Key_F2 );
    menu->insertItem( tools::translate( "TimelineActionItem", "Delete" ), this, SLOT( OnDelete() ), Qt::Key_Delete );
}

// -----------------------------------------------------------------------------
// Name: TimelineActionItem::Delete
// Created: SBO 2008-04-28
// -----------------------------------------------------------------------------
void TimelineActionItem::Delete()
{
    model_.Destroy( action_ );
}

// -----------------------------------------------------------------------------
// Name: TimelineActionItem::OnDelete
// Created: SBO 2008-04-22
// -----------------------------------------------------------------------------
void TimelineActionItem::OnDelete()
{
     Delete();
}

// -----------------------------------------------------------------------------
// Name: TimelineActionItem::OnRename
// Created: SBO 2008-04-22
// -----------------------------------------------------------------------------
void TimelineActionItem::OnRename()
{
    if( !nameEditor_ )
        return;
    nameEditor_->setText( action_.GetName() );
    int vx = int( x() ), vy = int( y() );
    if( nameEditor_->parentWidget()->inherits( "QScrollView" ) )
    {
        QScrollView* view = (QScrollView*)( nameEditor_->parentWidget() );
        view->contentsToViewport( vx, vy, vx, vy );
    }
    nameEditor_->setGeometry( vx + 4, vy + 2, width(), height() - 4 );
    nameEditor_->selectAll();
    nameEditor_->setFocus();
    nameEditor_->show();
}

// -----------------------------------------------------------------------------
// Name: TimelineActionItem::DoRename
// Created: SBO 2008-08-20
// -----------------------------------------------------------------------------
void TimelineActionItem::DoRename()
{
    const QString name = nameEditor_ ? nameEditor_->text() : "";
    if( !name.isEmpty() && name != action_.GetName() )
        const_cast< Action_ABC& >( action_ ).Rename( name );
    nameEditor_->hide();
}
