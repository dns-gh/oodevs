// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "EventBottomWidget.h"
#include "moc_EventBottomWidget.cpp"
#include "clients_gui/ImageWrapper.h"
#include "clients_gui/RichDateTimeEdit.h"
#include "clients_kernel/ActionController.h"
#include "clients_kernel/Time_ABC.h"
#include "gaming/Event.h"
#include "timeline/api.h"
#include "tools/ExerciseConfig.h"

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
// Name: EventBottomWidget constructor
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
EventBottomWidget::EventBottomWidget( const kernel::Time_ABC& simulation, kernel::ActionController& actionController, const tools::ExerciseConfig& config )
    : EventWidget_ABC()
    , simulation_( simulation )
    , actionController_( actionController )
    , switchAction_( 0 )
    , detailAction_( 0 )
    , saveAction_( 0 )
    , editing_( false )
    , triggerText_( tr( "Trigger" ) )
    , saveText_( tr( "Save" ) )
    , triggerAsCopyText_( tr( "Copy and trigger" ) )
    , saveAsCopyText_( tr( "Save as copy" ) )
{
    // Dates
    beginDateTimeEdit_ = new gui::RichDateTimeEdit( "begin-date" );
    hasEndDateTimeCheckbox_ = new QCheckBox();
    hasEndDateTimeCheckbox_->setText( tr( "End date" ) );
    endDateTimeEdit_ = new gui::RichDateTimeEdit( "end-date" );
    connect( beginDateTimeEdit_, SIGNAL( dateTimeChanged( const QDateTime& ) ), this, SLOT( OnBeginDateTimeChanged( const QDateTime& ) ) );
    connect( hasEndDateTimeCheckbox_, SIGNAL( stateChanged( int ) ), this, SLOT( OnHasEndTimeChanged( int ) ) );

    QHBoxLayout* startDateLayout = new QHBoxLayout();
    startDateLayout->addWidget( new QLabel( tr( "Start date" ) ) );
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
    switchAction_ = toolBar->addAction( MakePixmap( "actions_designmode" ), tr( "Planning" ), this, SLOT( OnSwitchToggled( bool ) ) );
    switchAction_->setCheckable( true );
    toolBar->addWidget( CreateStretcher() );
    planningActions_.push_back( toolBar->addWidget( dateWidget ) );
    planningActions_.push_back( toolBar->addWidget( CreateStretcher() ) );
    if( config.HasTimeline() )
    {
        detailAction_ = toolBar->addAction( qApp->style()->standardIcon( QStyle::SP_FileDialogInfoView ), "Details", this, SIGNAL( ShowDetail() ) );
        detailAction_->setCheckable( true );
        planningActions_.push_back( detailAction_ );
        planningActions_.push_back( toolBar->addSeparator() );
    }
    saveAction_ = toolBar->addAction( qApp->style()->standardIcon( QStyle::SP_DialogSaveButton ), saveText_, this, SIGNAL( Save() ) );
    planningActions_.push_back( saveAction_ );
    planningActions_.push_back( toolBar->addSeparator() );
    toolBar->addAction( qApp->style()->standardIcon( QStyle::SP_DialogCloseButton ), tr( "Discard" ), this, SIGNAL( Discard() ) );
    toolBar->setIconSize( QSize( 30, 30 ) );

    triggerButton_ = new QToolButton();
    triggerButton_->setIcon( qApp->style()->standardIcon( QStyle::SP_MediaPlay ) );
    triggerButton_->setToolTip( triggerText_ );
    triggerButton_->setFixedSize( QSize( 80, 40 ) );
    connect( triggerButton_, SIGNAL( clicked() ), this, SIGNAL( Trigger() ) );

    toolBar->addWidget( triggerButton_ );

    //add toolbar
    mainLayout_->addWidget( toolBar );

    // Initialization
    actionController_.Register( *this );
    OnSwitchToggled( false );
}

// -----------------------------------------------------------------------------
// Name: EventBottomWidget destructor
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
EventBottomWidget::~EventBottomWidget()
{
    actionController_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: EventBottomWidget::Fill
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
void EventBottomWidget::Fill( const Event& event )
{
    triggerButton_->setToolTip( event.GetEvent().done ? triggerAsCopyText_ : triggerText_ );
    saveAction_->setText( event.GetEvent().done ? saveAsCopyText_ : saveText_ );
    // $$$$ ABR 2013-07-04: TODO Change icon when "as copy"

    if( detailAction_ )
        detailAction_->setChecked( false );

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
// Name: EventBottomWidget::Commit
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
void EventBottomWidget::Commit( timeline::Event& event ) const
{
    event.done = false;
    event.begin = beginDateTimeEdit_->dateTime().toString( EVENT_DATE_FORMAT ).toStdString();
    event.end = ( hasEndDateTimeCheckbox_->checkState() == Qt::Checked ) ? endDateTimeEdit_->dateTime().toString( EVENT_DATE_FORMAT ).toStdString() : "";
}

// -----------------------------------------------------------------------------
// Name: EventBottomWidget::OnSwitchToggled
// Created: ABR 2013-07-04
// -----------------------------------------------------------------------------
void EventBottomWidget::OnSwitchToggled( bool checked )
{
    if( editing_ && !checked )
        switchAction_->setChecked( true );
    else
        for( auto it = planningActions_.begin(); it != planningActions_.end(); ++it )
            if( *it )
                ( *it )->setVisible( checked );
}

// -----------------------------------------------------------------------------
// Name: EventBottomWidget::OnEditingChanged
// Created: ABR 2013-07-04
// -----------------------------------------------------------------------------
void EventBottomWidget::OnEditingChanged( bool editing )
{
    editing_ = editing;
    switchAction_->setChecked( editing );
    OnSwitchToggled( editing );
}

// -----------------------------------------------------------------------------
// Name: EventBottomWidget::OnBeginDateTimeChanged
// Created: ABR 2013-05-31
// -----------------------------------------------------------------------------
void EventBottomWidget::OnBeginDateTimeChanged( const QDateTime& dateTime )
{
    endDateTimeEdit_->setMinimumDateTime( dateTime );
}

// -----------------------------------------------------------------------------
// Name: EventBottomWidget::OnHasEndTimeChanged
// Created: ABR 2013-05-31
// -----------------------------------------------------------------------------
void EventBottomWidget::OnHasEndTimeChanged( int state )
{
    bool hasEndTime = state == Qt::Checked;
    endDateTimeEdit_->setEnabled( hasEndTime );
    if( hasEndTime )
        endDateTimeEdit_->setDateTime( beginDateTimeEdit_->dateTime() );
}

// -----------------------------------------------------------------------------
// Name: EventBottomWidget::NotifyContextMenu
// Created: ABR 2013-06-19
// -----------------------------------------------------------------------------
void EventBottomWidget::NotifyContextMenu( const QDateTime& dateTime, kernel::ContextMenu& menu )
{
    selectedDateTime_ = dateTime;
    if( !isVisible() )
        return;
    menu.InsertItem( "Parameter", tr( "Start date" ), this, SLOT( OnBeginDateTimeSelected() ) );
    if( endDateTimeEdit_->isVisible() && dateTime.secsTo( beginDateTimeEdit_->dateTime() ) < 0 )
        menu.InsertItem( "Parameter", tr( "End date" ), this, SLOT( OnEndDateTimeSelected() ) );
}

// -----------------------------------------------------------------------------
// Name: EventBottomWidget::OnBeginDateTimeSelected
// Created: ABR 2013-06-19
// -----------------------------------------------------------------------------
void EventBottomWidget::OnBeginDateTimeSelected()
{
    beginDateTimeEdit_->setDateTime( selectedDateTime_ );
}

// -----------------------------------------------------------------------------
// Name: EventBottomWidget::OnEndDateTimeSelected
// Created: ABR 2013-06-19
// -----------------------------------------------------------------------------
void EventBottomWidget::OnEndDateTimeSelected()
{
    hasEndDateTimeCheckbox_->setCheckState( Qt::Checked );
    endDateTimeEdit_->setDateTime( selectedDateTime_ );
}

// -----------------------------------------------------------------------------
// Name: EventBottomWidget::SetBeginDateTime
// Created: ABR 2013-06-19
// -----------------------------------------------------------------------------
void EventBottomWidget::SetBeginDateTime( const QDateTime& dateTime )
{
    beginDateTimeEdit_->setDateTime( dateTime );
}
