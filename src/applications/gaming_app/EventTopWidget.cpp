// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "EventTopWidget.h"
#include "moc_EventTopWidget.cpp"
#include "clients_kernel/ActionController.h"
#include "clients_kernel/Time_ABC.h"
#include "clients_kernel/ContextMenu.h"
#include "clients_gui/RichDateTimeEdit.h"
#include "ENT/ENT_Tr.h"
#include "gaming/Event.h"
#include "timeline/api.h"

namespace
{
    QWidget* Wrap( QWidget* parent, QWidget* widget )
    {
        QWidget* result = new QWidget( parent );
        QVBoxLayout* layout = new QVBoxLayout( result );
        layout->addWidget( widget );
        return result;
    }
}

// -----------------------------------------------------------------------------
// Name: EventTopWidget constructor
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
EventTopWidget::EventTopWidget( const kernel::Time_ABC& simulation, kernel::ActionController& actionController )
    : EventWidget_ABC()
    , simulation_( simulation )
    , actionController_( actionController )
{
    // Header
    QFont font( "Arial", 12, QFont::Bold );
    title_ = new QLabel();
    title_->setFont( font );
    source_ = new QLabel();
    source_->setFont( font );

    QWidget* header = new QWidget();
    QHBoxLayout* headerLayout = new QHBoxLayout( header );
    headerLayout->addWidget( title_ );
    headerLayout->addStretch( 1 );
    headerLayout->addWidget( source_ );

    // Date time
    QToolBar* toolbar = new QToolBar();
    beginDateTimeEdit_ = new gui::RichDateTimeEdit( "begin-date" );
    hasEndDateTimeCheckbox_ = new QCheckBox();
    hasEndDateTimeCheckbox_->setText( tr( "End date" ) );
    endDateTimeEdit_ = new gui::RichDateTimeEdit( "end-date" );
    connect( beginDateTimeEdit_, SIGNAL( dateTimeChanged( const QDateTime& ) ), this, SLOT( OnBeginDateTimeChanged( const QDateTime& ) ) );
    connect( hasEndDateTimeCheckbox_, SIGNAL( stateChanged( int ) ), this, SLOT( OnHasEndTimeChanged( int ) ) );

    QWidget* empty = new QWidget( toolbar );
    empty->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Preferred );

    toolbar->addWidget( Wrap( toolbar, new QLabel( tr( "Start date" ) ) ) );
    toolbar->addWidget( Wrap( toolbar, beginDateTimeEdit_ ) );
    toolbar->addWidget( empty );
    toolbar->addWidget( Wrap( toolbar, hasEndDateTimeCheckbox_ ) );
    toolbar->addWidget( Wrap( toolbar, endDateTimeEdit_ ) );
    mainLayout_->addWidget( header );
    mainLayout_->addWidget( toolbar );

    actionController_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: EventTopWidget destructor
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
EventTopWidget::~EventTopWidget()
{
    actionController_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: EventTopWidget::Fill
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
void EventTopWidget::Fill( const Event& event )
{
    title_->setText( QString::fromStdString( ENT_Tr::ConvertFromEventType( event.GetType() ) ) );
    source_->setText( "Source" );

    // Date time
    bool canHaveEndTime = ( event.GetType() == eEventTypes_Task || event.GetType() == eEventTypes_Multimedia );
    hasEndDateTimeCheckbox_->setVisible( canHaveEndTime );
    endDateTimeEdit_->setVisible( canHaveEndTime );

    const timeline::Event& timelineEvent = event.GetEvent();
    beginDateTimeEdit_->setDateTime( ( timelineEvent.begin.empty() ) ? simulation_.GetDateTime() : QDateTime::fromString( QString::fromStdString( timelineEvent.begin ), EVENT_DATE_FORMAT ) );
    if( timelineEvent.end.empty() )
    {
        hasEndDateTimeCheckbox_->setCheckState( Qt::Unchecked );
        endDateTimeEdit_->setEnabled( false );
        endDateTimeEdit_->setDateTime( beginDateTimeEdit_->dateTime() );
    }
    else
    {
        hasEndDateTimeCheckbox_->setCheckState( Qt::Checked );
        endDateTimeEdit_->setEnabled( true );
        endDateTimeEdit_->setDateTime( QDateTime::fromString( QString::fromStdString( timelineEvent.end ), EVENT_DATE_FORMAT ) );
    }
}

// -----------------------------------------------------------------------------
// Name: EventTopWidget::Commit
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
void EventTopWidget::Commit( timeline::Event& event ) const
{
    event.begin = beginDateTimeEdit_->dateTime().toString( EVENT_DATE_FORMAT ).toStdString();
    event.end = ( hasEndDateTimeCheckbox_->checkState() == Qt::Checked ) ? endDateTimeEdit_->dateTime().toString( EVENT_DATE_FORMAT ).toStdString() : "";
}

// -----------------------------------------------------------------------------
// Name: EventTopWidget::OnBeginDateTimeChanged
// Created: ABR 2013-05-31
// -----------------------------------------------------------------------------
void EventTopWidget::OnBeginDateTimeChanged( const QDateTime& dateTime )
{
    endDateTimeEdit_->setMinimumDateTime( dateTime );
}

// -----------------------------------------------------------------------------
// Name: EventTopWidget::OnHasEndTimeChanged
// Created: ABR 2013-05-31
// -----------------------------------------------------------------------------
void EventTopWidget::OnHasEndTimeChanged( int state )
{
    bool hasEndTime = state == Qt::Checked;
    endDateTimeEdit_->setEnabled( hasEndTime );
    if( hasEndTime )
        endDateTimeEdit_->setDateTime( beginDateTimeEdit_->dateTime() );
}

// -----------------------------------------------------------------------------
// Name: EventTopWidget::NotifyContextMenu
// Created: ABR 2013-06-19
// -----------------------------------------------------------------------------
void EventTopWidget::NotifyContextMenu( const QDateTime& dateTime, kernel::ContextMenu& menu )
{
    selectedDateTime_ = dateTime;
    if( !isVisible() )
        return;
    menu.InsertItem( "Parameter", tr( "Start date" ), this, SLOT( OnBeginDateTimeSelected() ) );
    if( endDateTimeEdit_->isVisible() && dateTime.secsTo( beginDateTimeEdit_->dateTime() ) < 0 )
        menu.InsertItem( "Parameter", tr( "End date" ), this, SLOT( OnEndDateTimeSelected() ) );
}

// -----------------------------------------------------------------------------
// Name: EventTopWidget::OnBeginDateTimeSelected
// Created: ABR 2013-06-19
// -----------------------------------------------------------------------------
void EventTopWidget::OnBeginDateTimeSelected()
{
    beginDateTimeEdit_->setDateTime( selectedDateTime_ );
}

// -----------------------------------------------------------------------------
// Name: EventTopWidget::OnEndDateTimeSelected
// Created: ABR 2013-06-19
// -----------------------------------------------------------------------------
void EventTopWidget::OnEndDateTimeSelected()
{
    hasEndDateTimeCheckbox_->setCheckState( Qt::Checked );
    endDateTimeEdit_->setDateTime( selectedDateTime_ );
}

// -----------------------------------------------------------------------------
// Name: EventTopWidget::SetBeginDateTime
// Created: ABR 2013-06-19
// -----------------------------------------------------------------------------
void EventTopWidget::SetBeginDateTime( const QDateTime& dateTime )
{
    beginDateTimeEdit_->setDateTime( dateTime );
}
