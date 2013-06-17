// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "EventDialog.h"
#include "moc_EventDialog.cpp"

#include "EventBottomWidget.h"
#include "EventDetailWidget.h"
#include "EventMultimediaWidget.h"
#include "EventOrderWidget.h"
#include "EventReportWidget.h"
#include "EventSupervisorActionWidget.h"
#include "EventTopWidget.h"
#include "EventTaskWidget.h"

#include "clients_kernel/Time_ABC.h"

#include "gaming/Event.h"
#include "gaming/EventAction.h"
#include "gaming/EventFactory.h"
#include "gaming/Model.h"
#include "gaming/StaticModel.h"

#include "timeline/api.h"

// -----------------------------------------------------------------------------
// Name: EventDialog constructor
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
EventDialog::EventDialog( QWidget* parent, kernel::Controllers& controllers, Model& model, const tools::ExerciseConfig& config,
                          const kernel::Time_ABC& simulation, actions::gui::InterfaceBuilder_ABC& interfaceBuilder,
                          const kernel::Profile_ABC& profile, gui::GlTools_ABC& tools )
    : QDialog( parent )
    , factory_( model.eventFactory_ )
    , simulation_( simulation )
    , topWidget_( 0 )
    , bottomWidget_( 0 )
    , stack_( 0 )
    , currentWidget_( 0 )
    , editing_( false )
{
    setObjectName( "event-dialog" );
    setModal( false );
    setMinimumWidth( 600 );
    setMinimumHeight( 600 );

    topWidget_ = new EventTopWidget( simulation_ );
    bottomWidget_ = new EventBottomWidget();

    connect( bottomWidget_, SIGNAL( Trigger() ),        this, SLOT( OnTrigger() ) );
    connect( bottomWidget_, SIGNAL( CreateReminder() ), this, SLOT( OnCreateReminder() ) );
    connect( bottomWidget_, SIGNAL( Discard() ),        this, SLOT( OnDiscard() ) );
    connect( bottomWidget_, SIGNAL( Save() ),           this, SLOT( OnSave() ) );
    connect( bottomWidget_, SIGNAL( ShowDetail() ),     this, SLOT( OnShowDetail() ) );

    // Content
    EventOrderWidget* orderWidget = new EventOrderWidget( controllers, model, config, interfaceBuilder, profile, tools );
    EventSupervisorActionWidget* supervisorWidget = new EventSupervisorActionWidget();
    EventReportWidget* reportWidget = new EventReportWidget();
    EventTaskWidget* taskWidget = new EventTaskWidget();
    EventMultimediaWidget* multimediaWidget = new EventMultimediaWidget();
    detailWidget_ = new EventDetailWidget();

    stack_ = new QStackedWidget();
    stack_->insertWidget( eEventTypes_Order,            orderWidget );
    stack_->insertWidget( eEventTypes_SupervisorAction, supervisorWidget );
    stack_->insertWidget( eEventTypes_Report,           reportWidget );
    stack_->insertWidget( eEventTypes_Task,             taskWidget );
    stack_->insertWidget( eEventTypes_Multimedia,       multimediaWidget );
    stack_->insertWidget( eNbrEventTypes,               detailWidget_ );

    // Layout
    QVBoxLayout* mainLayout = new QVBoxLayout( this );
    mainLayout->setMargin( 0 );
    mainLayout->setSpacing( 0 );
    mainLayout->addWidget( topWidget_ );
    mainLayout->addWidget( stack_, 1 );
    mainLayout->addWidget( bottomWidget_ );
}

// -----------------------------------------------------------------------------
// Name: EventDialog destructor
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
EventDialog::~EventDialog()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EventDialog::Create
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
void EventDialog::Create( E_EventTypes type )
{
    setCaption( tr( "Event creation" ) );
    editing_ = false;
    event_.reset( factory_.Create( type ) );
    SetEventType( type );
    Purge();
    Fill();
    show();
}

// -----------------------------------------------------------------------------
// Name: EventDialog::Edit
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
void EventDialog::Edit( Event& event )
{
    setCaption( tr( "Event edition" ) );
    editing_ = true;
    event_.reset( event.Clone() );
    SetEventType( event.GetType() );
    Purge();
    Fill();
    show();
}

// -----------------------------------------------------------------------------
// Name: EventDialog::SetType
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
void EventDialog::SetEventType( E_EventTypes type )
{
    stack_->setCurrentIndex( type );
    currentWidget_ = static_cast< EventWidget_ABC* >( stack_->widget( type ) );
}

// -----------------------------------------------------------------------------
// Name: EventDialog::closeEvent
// Created: ABR 2013-06-11
// -----------------------------------------------------------------------------
void EventDialog::closeEvent( QCloseEvent * event )
{
    Purge();
    QDialog::closeEvent( event );
}

// -----------------------------------------------------------------------------
// Name: EventDialog::Purge
// Created: ABR 2013-06-06
// -----------------------------------------------------------------------------
void EventDialog::Purge()
{
    topWidget_->Purge();
    currentWidget_->Purge();
    bottomWidget_->Purge();
    detailWidget_->Purge();
}

// -----------------------------------------------------------------------------
// Name: EventDialog::Fill
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
void EventDialog::Fill()
{
    assert( event_.get() );
    topWidget_->Fill( *event_ );
    currentWidget_->Fill( *event_ );
    bottomWidget_->Fill( *event_ );
    detailWidget_->Fill( *event_ );
}

// -----------------------------------------------------------------------------
// Name: EventDialog::Commit
// Created: ABR 2013-05-31
// -----------------------------------------------------------------------------
void EventDialog::Commit( timeline::Event& event )
{
    topWidget_->Commit( event );
    currentWidget_->Commit( event );
    bottomWidget_->Commit( event );
}

// -----------------------------------------------------------------------------
// Name: EventDialog::OnTrigger
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
void EventDialog::OnTrigger()
{
    if( currentWidget_->IsValid() )
        currentWidget_->Trigger();
    else
    {
        currentWidget_->Warn();
        QMessageBox::warning( this, tr( "Warning" ), tr( "This event is incomplete so it can't be triggered.") );
    }
}

// -----------------------------------------------------------------------------
// Name: EventDialog::OnCreateReminder
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
void EventDialog::OnCreateReminder()
{
    QMessageBox::question( this, "Reminder", "Reminder creation !" );
}

// -----------------------------------------------------------------------------
// Name: EventDialog::OnShowDetail
// Created: ABR 2013-05-31
// -----------------------------------------------------------------------------
void EventDialog::OnShowDetail()
{
    if( stack_->currentIndex() == eNbrEventTypes )
        stack_->setCurrentIndex( lastCurrentIndex_ );
    else
    {
        lastCurrentIndex_ = stack_->currentIndex();
        stack_->setCurrentIndex( eNbrEventTypes );
    }
}

// -----------------------------------------------------------------------------
// Name: EventDialog::OnDiscard
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
void EventDialog::OnDiscard()
{
    Fill();
}

// -----------------------------------------------------------------------------
// Name: EventDialog::OnSave
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
void EventDialog::OnSave()
{
    timeline::Event event;
    Commit( event );
    event_->Update( event );
    if( editing_ )
        emit EditEvent( event );
    else
    {
        emit CreateEvent( event );
        editing_ = true;
    }
}

// -----------------------------------------------------------------------------
// Name: EventDialog::Draw
// Created: ABR 2013-06-11
// -----------------------------------------------------------------------------
void EventDialog::Draw( gui::Viewport_ABC& viewport )
{
    if( currentWidget_ )
        currentWidget_->Draw( viewport );
}
