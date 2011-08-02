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
#include "actions/ActionTasker.h"
#include "actions/ActionTiming.h"
#include "gaming/Tools.h"
#include "clients_kernel/ActionController.h"
#include "clients_kernel/Controllers.h"

using namespace actions;

// -----------------------------------------------------------------------------
// Name: TimelineActionItem constructor
// Created: SBO 2007-07-04
// -----------------------------------------------------------------------------
TimelineActionItem::TimelineActionItem( Q3Canvas* canvas, const TimelineRuler& ruler, kernel::Controllers& controllers, ActionsModel& model, const Action_ABC& action )
    : QObject         ( canvas )
    , TimelineItem_ABC( canvas )
    , ruler_          ( ruler )
    , controllers_    ( controllers )
    , model_          ( model )
    , action_         ( action )
    , textWidth_      ( 0 )
    , nameEditor_     ( 0 )
{
    const ActionTasker* tasker = action_.Retrieve< ActionTasker >();
    setActive( !tasker || !tasker->IsSimulation() );
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
// Name: TimelineActionItem::setSelected
// Created: SBO 2010-04-30
// -----------------------------------------------------------------------------
void TimelineActionItem::setSelected( bool selected )
{
    if( isSelected() != selected )
    {
        TimelineItem_ABC::setSelected( selected );
        if( selected )
            action_.Select( controllers_.actions_ );
    }
}

// -----------------------------------------------------------------------------
// Name: TimelineActionItem::setEnabled
// Created: SBO 2010-05-04
// -----------------------------------------------------------------------------
void TimelineActionItem::setEnabled( bool enabled )
{
    TimelineItem_ABC::setEnabled( enabled );
    if( ActionTiming* timing = const_cast< ActionTiming* >( action_.Retrieve< ActionTiming >() ) )
        if( timing->IsEnabled() != enabled )
            timing->ToggleEnabled();
}

// -----------------------------------------------------------------------------
// Name: TimelineActionItem::Move
// Created: SBO 2007-07-06
// -----------------------------------------------------------------------------
void TimelineActionItem::Move( long offset )
{
    if( isActive() )
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
        setEnabled( isActive() && timing->IsEnabled() );
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
        painter.drawLine( rect.left(), rect.top(), rect.left(), rect.bottom() );
        painter.drawLine( rect.left() + 1, rect.top() +  8, rect.left() + 1, rect.top() + 16 );
        painter.drawLine( rect.left() + 2, rect.top() + 10, rect.left() + 2, rect.top() + 14 );
        painter.drawPoint( rect.left() + 3, rect.top() + 12 );
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

namespace
{
    QColor SelectForegroundColor( Q3CanvasItem& item, bool valid )
    {
        if( item.isSelected() )
            return QColor( 50, 105, 200 );
        if( !valid )
            return QColor( 0, 0, 0 );
        if( !item.isActive() )
            return QColor( 100, 100, 100 );
        if( !item.isEnabled() )
            return QColor( 170, 170, 170 );
        return QColor( 40, 40, 40 );
    }

    QColor SelectBackgroundColor( Q3CanvasItem& item, bool valid )
    {
        if( !valid )
        {
            if( item.isSelected() )
                return QColor( 200, 40, 40 );
            return QColor( 255, 0, 0 );
        }
        if( item.isSelected() )
            return QColor( 200, 215, 240 );
        if( !item.isActive() )
            return QColor( 255, 240, 20 );
        if( !item.isEnabled() )
            return QColor( 220, 220, 220 );
        return QColor( 163, 219, 135 );
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
    const QColor backgroundColor( SelectBackgroundColor( *this, action_.IsValid() ) );
    const QPen oldPen = painter.pen();
    painter.fillRect( rect().left(), rect().top() + 1, rect().width(), rect().height() - 2, Qt::white ); // $$$$ SBO 2008-04-22: clear color
    const QRect gradientRect( rect().left(), rect().top() + 20, rect().width() + 40, rect().height() - 21 );
    DrawHorizontalGradientBox( painter, gradientRect, backgroundColor, Qt::white );
    painter.setPen( backgroundColor );
    DrawArrow( painter, rect() );
    painter.setPen( SelectForegroundColor( *this , action_.IsValid() ) );
    painter.drawText( rect(), Qt::AlignLeft | Qt::AlignVCenter, "  " + action_.GetName() );
    painter.setPen( oldPen );
}

// -----------------------------------------------------------------------------
// Name: TimelineActionItem::DisplayToolTip
// Created: SBO 2007-07-19
// -----------------------------------------------------------------------------
void TimelineActionItem::DisplayToolTip( QWidget* parent ) const
{
    QString tip;
    tip = tools::translate( "TimelineActionItem", "<table cellspacing='0' cellpadding='0'><tr><td><img source=\"mission\"></td><td><nobr><b>%1</b></nobr></td></tr></table>" )
        .arg( action_.GetName() );
    if( !isActive() )
        tip += tools::translate( "TimelineActionItem", "<i>issued by automat</i>" );
    else if( !isEnabled() )
        tip += tools::translate( "TimelineActionItem", "<i>disabled</i>" );
    else
        tip += tools::translate( "TimelineActionItem", "<table cellspacing='0' cellpadding='0'><tr><td>Starts: </td><td><i>%1</i></td></tr></table>" )
              .arg( action_.Get< ActionTiming >().GetTime().toString() );
    if( parent->toolTip() != tip )
    {
        QToolTip::remove( parent );
        QToolTip::add( parent, tip );
    }
}

// -----------------------------------------------------------------------------
// Name: TimelineActionItem::DisplayContextMenu
// Created: SBO 2008-04-22
// -----------------------------------------------------------------------------
void TimelineActionItem::DisplayContextMenu( Q3PopupMenu* menu ) const
{
    if( !nameEditor_ )
    {
        nameEditor_ = new QLineEdit( menu->parentWidget() );
        QToolTip::add( nameEditor_, tools::translate( "TimelineActionItem", "Press enter to rename, unfocus field to cancel" ) );
        connect( nameEditor_, SIGNAL( returnPressed() ), SLOT( DoRename() ) );
        connect( nameEditor_, SIGNAL( lostFocus() ), nameEditor_, SLOT( hide() ) );
    }
    menu->setItemEnabled( menu->insertItem( tools::translate( "TimelineActionItem", "Rename" ), this, SLOT( OnRename() ), Qt::Key_F2 ), isActive() );
    menu->setItemEnabled( menu->insertItem( tools::translate( "TimelineActionItem", "Delete" ), this, SLOT( OnDelete() ), Qt::Key_Delete ), isActive() );
    const QString toggleEnable = isEnabled() ? tools::translate( "TimelineActionItem", "Disable" ) : tools::translate( "TimelineActionItem", "Enable" );
    menu->setItemEnabled( menu->insertItem( toggleEnable, this, SLOT( OnToggleEnabled() ), Qt::Key_X ), isActive() );
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
        Q3ScrollView* view = (Q3ScrollView*)( nameEditor_->parentWidget() );
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

// -----------------------------------------------------------------------------
// Name: TimelineActionItem::OnToggleEnabled
// Created: SBO 2010-05-04
// -----------------------------------------------------------------------------
void TimelineActionItem::OnToggleEnabled()
{
    setEnabled( !isEnabled() );
}
