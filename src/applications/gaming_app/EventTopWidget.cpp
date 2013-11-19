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
#include "clients_gui/ImageWrapper.h"
#include "clients_gui/RichDateTimeEdit.h"
#include "clients_gui/resources.h"
#include "clients_kernel/ActionController.h"
#include "clients_kernel/Event.h"
#include "clients_kernel/Time_ABC.h"
#include "ENT/ENT_Tr.h"
#include "tools/GeneralConfig.h"
#include <timeline/api.h>

namespace
{
    QPixmap MakePixmap( const tools::Path& name )
    {
        return gui::Pixmap( tools::GeneralConfig::BuildResourceChildFile( tools::Path( "images/gaming" ) / name + ".png" ) );
    }
    QWidget* CreateStretcher()
    {
        QWidget* empty = new QWidget();
        empty->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
        return empty;
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
    , saveAction_( 0 )
    , saveAsAction_( 0 )
    , editing_( false )
{
    // Header
    QFont font( "Arial", 12, QFont::Bold );
    title_ = new QLabel();
    title_->setFont( font );

    QHBoxLayout* headerLayout = new QHBoxLayout();
    headerLayout->setSpacing( 5 );
    headerLayout->addWidget( title_ );
    headerLayout->addStretch( 1 );

    // Dates
    beginDateTimeEdit_ = new gui::RichDateTimeEdit( "begin-date" );
    hasEndDateTimeCheckbox_ = new QCheckBox();
    hasEndDateTimeCheckbox_->setText( tr( "End" ) );
    endDateTimeEdit_ = new gui::RichDateTimeEdit( "end-date" );
    connect( beginDateTimeEdit_, SIGNAL( dateTimeChanged( const QDateTime& ) ), this, SLOT( OnBeginDateTimeChanged( const QDateTime& ) ) );
    connect( hasEndDateTimeCheckbox_, SIGNAL( stateChanged( int ) ), this, SLOT( OnHasEndTimeChanged( int ) ) );

    QHBoxLayout* startDateLayout = new QHBoxLayout();
    startDateLabel_ = new QLabel();
    startDateLayout->addWidget( startDateLabel_ );
    startDateLayout->addWidget( beginDateTimeEdit_ );
    QHBoxLayout* endDateLayout = new QHBoxLayout();
    endDateLayout->addWidget( hasEndDateTimeCheckbox_ );
    endDateLayout->addWidget( endDateTimeEdit_ );

    QWidget* dateWidget = new QWidget();
    QVBoxLayout* dateLayout = new QVBoxLayout( dateWidget );
    dateLayout->setMargin( 0 );
    dateLayout->setSpacing( 5 );
    dateLayout->addLayout( startDateLayout );
    dateLayout->addLayout( endDateLayout );

    // ToolBar
    QToolBar* toolBar = new QToolBar();
    toolBar->setStyleSheet("QToolBar { border: 0px }");
    toolBar->setIconSize( QSize( 30, 30 ) );
    toolBar->addWidget( CreateStretcher() );
    toolBar->addWidget( dateWidget );
    toolBar->addWidget( CreateStretcher() );
    saveAction_ = toolBar->addAction( MAKE_ICON( save ), tr( "Save" ), this, SIGNAL( Save() ) );
    saveAsAction_ = toolBar->addAction( MAKE_ICON( saveas ), tr( "Save as copy" ), this, SIGNAL( SaveAs() ) );

    // Layout
    mainLayout_->addLayout( headerLayout );
    mainLayout_->addWidget( toolBar );

    // Initialization
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
void EventTopWidget::Fill( const kernel::Event& event )
{
    title_->setText( QString::fromStdString( ENT_Tr::ConvertFromEventType( event.GetType() ) ) );
    saveAction_->setEnabled( !event.GetEvent().done );
    saveAsAction_->setEnabled( editing_ );

    // Date time
    bool canHaveEndTime = ( event.GetType() == eEventTypes_Task || event.GetType() == eEventTypes_Multimedia );
    hasEndDateTimeCheckbox_->setVisible( canHaveEndTime );
    endDateTimeEdit_->setVisible( canHaveEndTime );
    startDateLabel_->setText( canHaveEndTime ? tr( "Start" ) : "" );

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
void EventTopWidget::Commit( timeline::Event& event )
{
    event.done = false;
    event.begin = beginDateTimeEdit_->dateTime().toString( EVENT_DATE_FORMAT ).toStdString();
    event.end = ( hasEndDateTimeCheckbox_->checkState() == Qt::Checked ) ? endDateTimeEdit_->dateTime().toString( EVENT_DATE_FORMAT ).toStdString() : "";
}

// -----------------------------------------------------------------------------
// Name: EventTopWidget::OnEditingChanged
// Created: ABR 2013-07-04
// -----------------------------------------------------------------------------
void EventTopWidget::OnEditingChanged( bool editing )
{
    editing_ = editing;
    saveAsAction_->setEnabled( editing_ );
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
    bool hasEndDateTime = endDateTimeEdit_->isVisible();
    menu.InsertItem( "Parameter", hasEndDateTime ? tr( "Set event start date" ) : tr( "Set event date" ), this, SLOT( OnBeginDateTimeSelected() ) );
    if( hasEndDateTime && dateTime.secsTo( beginDateTimeEdit_->dateTime() ) < 0 )
        menu.InsertItem( "Parameter", tr( "Set event end date" ), this, SLOT( OnEndDateTimeSelected() ) );
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
