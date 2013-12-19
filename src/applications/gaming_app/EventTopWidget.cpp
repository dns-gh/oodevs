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
#include "clients_gui/EventPresenter.h"
#include "clients_gui/EventViewState.h"
#include "clients_gui/ImageWrapper.h"
#include "clients_gui/RichCheckBox.h"
#include "clients_gui/RichDateTimeEdit.h"
#include "clients_gui/resources.h"
#include "clients_kernel/ActionController.h"
#include "clients_kernel/Time_ABC.h"
#include "clients_kernel/TimelineHelpers.h"
#include "tools/GeneralConfig.h"
#include "ENT/ENT_Tr.h"
#include <timeline/api.h>

namespace
{
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
EventTopWidget::EventTopWidget( gui::EventPresenter& presenter,
                                const kernel::Time_ABC& simulation,
                                kernel::ActionController& actionController )
    : EventWidget_ABC< gui::EventView_ABC >( presenter )
    , simulation_( simulation )
    , actionController_( actionController )
    , saveAction_( 0 )
    , saveAsAction_( 0 )
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
    hasEndDateTimeCheckbox_ = new gui::RichCheckBox( "end-date-checkbox" );
    hasEndDateTimeCheckbox_->setText( tr( "End" ) );
    endDateTimeEdit_ = new gui::RichDateTimeEdit( "end-date" );
    connect( beginDateTimeEdit_, SIGNAL( dateTimeChanged( const QDateTime& ) ), &presenter, SLOT( OnBeginDateChanged( const QDateTime& ) ) );
    connect( endDateTimeEdit_, SIGNAL( dateTimeChanged( const QDateTime& ) ), &presenter, SLOT( OnEndDateChanged( const QDateTime& ) ) );
    connect( hasEndDateTimeCheckbox_, SIGNAL( stateChanged( int ) ), this, SLOT( OnEndDateActivated( int ) ) );

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
    saveAction_ = toolBar->addAction( MAKE_ICON( save ),
        tr( "Save" ), &presenter, SLOT( OnSaveClicked() ) );
    saveAsAction_ = toolBar->addAction( gui::Pixmap( tools::GeneralConfig::BuildResourceChildFile( "images/gui/clone.png" ) ),
        tr( "Save as copy" ), &presenter, SLOT( OnSaveAsClicked() ) );

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
// Name: EventTopWidget::Purge
// Created: ABR 2013-11-21
// -----------------------------------------------------------------------------
void EventTopWidget::Purge()
{
    title_->clear();
    saveAction_->setEnabled( false );
    saveAsAction_->setEnabled( false );
    hasEndDateTimeCheckbox_->setChecked( false );
    beginDateTimeEdit_->setDateTime( simulation_.GetDateTime() );
    endDateTimeEdit_->setDateTime( simulation_.GetDateTime() );
}

// -----------------------------------------------------------------------------
// Name: EventTopWidget::Update
// Created: ABR 2013-11-21
// -----------------------------------------------------------------------------
void EventTopWidget::Update( const gui::EventViewState& state )
{
    E_EventTypes type = ( state.event_ ) ? state.event_->GetType() : eNbrEventTypes;
    const std::string& beginDate = ( state.event_ ) ? state.event_->GetEvent().begin : "";
    const std::string& endDate = ( state.event_ ) ? state.event_->GetEvent().end : "";

    title_->setText( QString::fromStdString( ENT_Tr::ConvertFromEventType( type ) ) );

    saveAction_->setEnabled( state.save_ );
    saveAsAction_->setEnabled( state.saveAs_ );

    bool canHaveEndTime = type == eEventTypes_Task || type == eEventTypes_Multimedia;
    hasEndDateTimeCheckbox_->setVisible( canHaveEndTime );
    endDateTimeEdit_->setVisible( canHaveEndTime );
    startDateLabel_->setText( canHaveEndTime ? tr( "Start" ) : "" );

    beginDateTimeEdit_->setDateTime( beginDate.empty() ? simulation_.GetDateTime() : QDateTime::fromString( QString::fromStdString( beginDate ), EVENT_DATE_FORMAT ) );
    if( endDate.empty() )
    {
        hasEndDateTimeCheckbox_->setCheckState( Qt::Unchecked );
        endDateTimeEdit_->setDateTime( beginDateTimeEdit_->dateTime() );
    }
    else
    {
        hasEndDateTimeCheckbox_->setCheckState( Qt::Checked );
        endDateTimeEdit_->setDateTime( QDateTime::fromString( QString::fromStdString( endDate ), EVENT_DATE_FORMAT ) );
    }
    endDateTimeEdit_->setMinimumDateTime( beginDateTimeEdit_->dateTime() );
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
    presenter_.OnBeginDateChanged( selectedDateTime_ );
}

// -----------------------------------------------------------------------------
// Name: EventTopWidget::OnEndDateTimeSelected
// Created: ABR 2013-06-19
// -----------------------------------------------------------------------------
void EventTopWidget::OnEndDateTimeSelected()
{
    presenter_.OnEndDateActivated( true );
    presenter_.OnEndDateChanged( selectedDateTime_ );
}

// -----------------------------------------------------------------------------
// Name: EventTopWidget::OnEndDateActivated
// Created: ABR 2013-11-22
// -----------------------------------------------------------------------------
void EventTopWidget::OnEndDateActivated( int status )
{
    presenter_.OnEndDateActivated( status == Qt::Checked );
}

// -----------------------------------------------------------------------------
// Name: EventTopWidget::BlockSignals
// Created: ABR 2013-11-22
// -----------------------------------------------------------------------------
void EventTopWidget::BlockSignals( bool blocked )
{
    saveAction_->blockSignals( blocked );
    saveAsAction_->blockSignals( blocked );
    beginDateTimeEdit_->blockSignals( blocked );
    endDateTimeEdit_->blockSignals( blocked );
    hasEndDateTimeCheckbox_->blockSignals( blocked );
}
