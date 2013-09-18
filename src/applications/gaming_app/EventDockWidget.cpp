// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "EventDockWidget.h"
#include "moc_EventDockWidget.cpp"

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

#include <timeline/api.h>

#include <boost/lexical_cast.hpp>
#pragma warning( push )
#pragma warning( disable : 4724 )
#include <boost/uuid/random_generator.hpp>
#pragma warning( pop )
#include <boost/uuid/uuid_io.hpp>

namespace
{
    void ShowWidget( QWidget* w )
    {
        w->setVisible( true );
        w->raise();
    }
}

// -----------------------------------------------------------------------------
// Name: EventDockWidget constructor
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
EventDockWidget::EventDockWidget( QWidget* parent, kernel::Controllers& controllers, Model& model, const tools::ExerciseConfig& config,
                                  const kernel::Time_ABC& simulation, actions::gui::InterfaceBuilder_ABC& interfaceBuilder,
                                  const kernel::Profile_ABC& profile, gui::GlTools_ABC& tools )
    : gui::RichDockWidget( controllers, parent, "event-dock-widget" )
    , factory_( model.eventFactory_ )
    , simulation_( simulation )
    , topWidget_( 0 )
    , bottomWidget_( 0 )
    , stack_( 0 )
    , currentWidget_( 0 )
    , editing_( false )
    , event_( 0 )
    , selected_( controllers )
{
    setWindowTitle( tr( "Event edition" ) );
    setFloating( true );
    hide();

    // Header / footer
    topWidget_ = new EventTopWidget();
    bottomWidget_ = new EventBottomWidget( simulation_, controllers.actions_, config );

    // Content
    EventOrderWidget* orderWidget = new EventOrderWidget( controllers, model, config, interfaceBuilder, profile, tools, simulation_ );
    connect( bottomWidget_, SIGNAL( PlanningModeToggled( bool ) ), orderWidget, SLOT( OnPlanningModeToggled( bool ) ) );

    EventSupervisorActionWidget* supervisorWidget = new EventSupervisorActionWidget();
    EventReportWidget* reportWidget = new EventReportWidget();
    EventTaskWidget* taskWidget = new EventTaskWidget();
    EventMultimediaWidget* multimediaWidget = new EventMultimediaWidget();
    detailWidget_ = new EventDetailWidget();

    //stack

    stack_ = new QStackedWidget();
    stack_->insertWidget( eEventTypes_Order,            orderWidget );
    stack_->insertWidget( eEventTypes_SupervisorAction, supervisorWidget );
    stack_->insertWidget( eEventTypes_Report,           reportWidget );
    stack_->insertWidget( eEventTypes_Task,             taskWidget );
    stack_->insertWidget( eEventTypes_Multimedia,       multimediaWidget );
    stack_->insertWidget( eNbrEventTypes,               detailWidget_ );

    QScrollArea* stackArea = new QScrollArea();
    QHBoxLayout* stackAreaLayout = new QHBoxLayout( stackArea );
    stackArea->setFrameStyle( QFrame::Box | QFrame::Sunken );
    stackArea->setWidgetResizable( true );
    stackArea->setMinimumSize( 300, 160 );
    stackAreaLayout->addWidget( stack_ );

    // Layout
    QWidget* mainWidget = new QWidget();
    QVBoxLayout* mainLayout = new QVBoxLayout( mainWidget );
    mainLayout->setMargin( 5 );
    mainLayout->setSpacing( 5 );
    mainLayout->addWidget( topWidget_ );
    mainLayout->addWidget( stackArea, 1 );
    mainLayout->addWidget( bottomWidget_ );
    setWidget( mainWidget );
    SetContentVisible( false );

    // Connections
    connect( this, SIGNAL( BeginDateChanged( const QDateTime& ) ), bottomWidget_, SLOT( SetBeginDateTime( const QDateTime& ) ) );
    connect( this, SIGNAL( EditingChanged( bool ) ), bottomWidget_, SLOT( OnEditingChanged( bool ) ) );
    connect( bottomWidget_, SIGNAL( Trigger() ),        this, SLOT( OnTrigger() ) );
    connect( bottomWidget_, SIGNAL( Discard() ),        this, SLOT( OnDiscard() ) );
    connect( bottomWidget_, SIGNAL( Save() ),           this, SLOT( OnSave() ) );
    connect( bottomWidget_, SIGNAL( ShowDetail() ),     this, SLOT( OnShowDetail() ) );
    connect( orderWidget, SIGNAL( EnableTriggerEvent( bool ) ), bottomWidget_, SLOT( OnEnableTriggerEvent( bool ) ) );
    connect( orderWidget, SIGNAL( StartCreation( E_EventTypes, const QDateTime& ) ), this, SLOT( StartCreation( E_EventTypes, const QDateTime& ) ) );
    connect( orderWidget, SIGNAL( UpdateCreation( E_EventTypes, const QDateTime& ) ), this, SLOT( UpdateCreation( E_EventTypes, const QDateTime& ) ) );
}

// -----------------------------------------------------------------------------
// Name: EventDockWidget destructor
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
EventDockWidget::~EventDockWidget()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EventDockWidget::StartCreation
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
void EventDockWidget::StartCreation( E_EventTypes type, const QDateTime& dateTime )
{
    event_.reset( factory_.Create( type ) );
    if( dateTime.isValid() )
        emit BeginDateChanged( dateTime );
    Configure( type, false, true );
    ShowWidget( this );
}

// -----------------------------------------------------------------------------
// Name: EventDockWidget::UpdateCreation
// Created: LGY 2013-08-20
// -----------------------------------------------------------------------------
void EventDockWidget::UpdateCreation( E_EventTypes type, const QDateTime& dateTime )
{
    event_.reset( factory_.Create( type ) );
    if( dateTime.isValid() )
        emit BeginDateChanged( dateTime );
    Configure( type, false, false );
}

// -----------------------------------------------------------------------------
// Name: EventDockWidget::StartEdition
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
void EventDockWidget::StartEdition( const Event& event )
{
    event_.reset( event.Clone() );
    Configure( event.GetType(), true, true );
}

// -----------------------------------------------------------------------------
// Name: EventDockWidget::Configure
// Created: ABR 2013-08-21
// -----------------------------------------------------------------------------
void EventDockWidget::Configure( E_EventTypes type, bool editing, bool purge )
{
    SetEventType( type );
    if( purge )
        Purge();
    SetEditing( editing );
    Fill();
    SetContentVisible( true );
}

// -----------------------------------------------------------------------------
// Name: EventDockWidget::SetType
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
void EventDockWidget::SetEventType( E_EventTypes type )
{
    stack_->setCurrentIndex( type );
    currentWidget_ = static_cast< EventWidget_ABC* >( stack_->widget( type ) );
}

// -----------------------------------------------------------------------------
// Name: EventDockWidget::closeEvent
// Created: ABR 2013-06-11
// -----------------------------------------------------------------------------
void EventDockWidget::closeEvent( QCloseEvent * event )
{
    Purge();
    gui::RichDockWidget::closeEvent( event );
}

// -----------------------------------------------------------------------------
// Name: EventDockWidget::SetContentVisible
// Created: ABR 2013-07-03
// -----------------------------------------------------------------------------
void EventDockWidget::SetContentVisible( bool visible )
{
    topWidget_->setVisible( visible );
    stack_->setVisible( visible );
    bottomWidget_->setVisible( visible );
}

// -----------------------------------------------------------------------------
// Name: EventDockWidget::Purge
// Created: ABR 2013-06-06
// -----------------------------------------------------------------------------
void EventDockWidget::Purge()
{
    topWidget_->Purge();
    if( currentWidget_ )
        currentWidget_->Purge();
    bottomWidget_->Purge();
    detailWidget_->Purge();
    SetContentVisible( false );
}

// -----------------------------------------------------------------------------
// Name: EventDockWidget::Fill
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
void EventDockWidget::Fill()
{
    assert( event_.get() );
    topWidget_->Fill( *event_ );
    currentWidget_->Fill( *event_ );
    bottomWidget_->Fill( *event_ );
    detailWidget_->Fill( *event_ );
}

// -----------------------------------------------------------------------------
// Name: EventDockWidget::Commit
// Created: ABR 2013-05-31
// -----------------------------------------------------------------------------
void EventDockWidget::Commit( timeline::Event& event )
{
    topWidget_->Commit( event );
    currentWidget_->Commit( event );
    bottomWidget_->Commit( event );
}

// -----------------------------------------------------------------------------
// Name: EventDockWidget::OnTrigger
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
void EventDockWidget::OnTrigger()
{
    if( currentWidget_->IsValid() )
    {
        if( editing_ && event_.get() && !event_->GetEvent().done )
            emit DeleteEvent( event_->GetEvent().uuid );
        currentWidget_->Trigger();
    }
    else
    {
        currentWidget_->Warn();
        QMessageBox::warning( this, tr( "Warning" ), tr( "This event is incomplete so it can't be triggered.") );
    }
}

// -----------------------------------------------------------------------------
// Name: EventDockWidget::OnShowDetail
// Created: ABR 2013-05-31
// -----------------------------------------------------------------------------
void EventDockWidget::OnShowDetail()
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
// Name: EventDockWidget::OnDiscard
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
void EventDockWidget::OnDiscard()
{
    if( editing_ )
        Fill();
    else
        currentWidget_->Purge();
}

// -----------------------------------------------------------------------------
// Name: EventDockWidget::OnSave
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
void EventDockWidget::OnSave()
{
    timeline::Event event;
    Commit( event );
    assert( event_.get() );
    bool create = !editing_ || event_->GetEvent().done;
    event.uuid = ( create ) ? boost::lexical_cast< std::string >( boost::uuids::random_generator()() ) : event_->GetEvent().uuid;
    event_->Update( event );
    if( create )
        emit CreateEvent( event );
    else
        emit EditEvent( event );

    if( !editing_ )
        SetEditing( true );
}

// -----------------------------------------------------------------------------
// Name: EventDockWidget::OnEditClicked
// Created: ABR 2013-07-02
// -----------------------------------------------------------------------------
void EventDockWidget::OnEditClicked()
{
    if( selected_ )
        StartEdition( *selected_ );
    selected_ = 0;
}

// -----------------------------------------------------------------------------
// Name: EventDockWidget::OnDeleteClicked
// Created: ABR 2013-07-02
// -----------------------------------------------------------------------------
void EventDockWidget::OnDeleteClicked()
{
    if( selected_ )
        emit DeleteEvent( selected_->GetEvent().uuid );
    selected_ = 0;
}

// -----------------------------------------------------------------------------
// Name: EventDockWidget::Draw
// Created: ABR 2013-06-11
// -----------------------------------------------------------------------------
void EventDockWidget::Draw( gui::Viewport_ABC& viewport )
{
    if( currentWidget_ && isVisible() )
        currentWidget_->Draw( viewport );
}

// -----------------------------------------------------------------------------
// Name: EventDockWidget::NotifyActivated
// Created: ABR 2013-07-02
// -----------------------------------------------------------------------------
void EventDockWidget::NotifyActivated( const Event& event )
{
    StartEdition( event );
    ShowWidget( this );
}

// -----------------------------------------------------------------------------
// Name: EventDockWidget::NotifySelected
// Created: BAX 2013-09-17
// -----------------------------------------------------------------------------
void EventDockWidget::NotifySelected( const Event* event )
{
    if( event )
        StartEdition( *event );
}

// -----------------------------------------------------------------------------
// Name: EventDockWidget::NotifyContextMenu
// Created: ABR 2013-07-02
// -----------------------------------------------------------------------------
void EventDockWidget::NotifyContextMenu( const Event& event, kernel::ContextMenu& menu )
{
    selected_ = &event;
    menu.InsertItem( "Command", tr( "Edit" ), this, SLOT( OnEditClicked() ) );
    menu.InsertItem( "Command", tr( "Delete" ), this, SLOT( OnDeleteClicked() ) );
}

// -----------------------------------------------------------------------------
// Name: EventDockWidget::NotifyDeleted
// Created: ABR 2013-06-18
// -----------------------------------------------------------------------------
void EventDockWidget::NotifyDeleted( const Event& event )
{
    if( event_.get() && event_->GetEvent().uuid == event.GetEvent().uuid )
        Purge();
}

// -----------------------------------------------------------------------------
// Name: EventDockWidget::NotifyUpdated
// Created: ABR 2013-06-18
// -----------------------------------------------------------------------------
void EventDockWidget::NotifyUpdated( const Event& event )
{
    if( event_.get() && event_->GetEvent().uuid == event.GetEvent().uuid )
        StartEdition( event );
}

// -----------------------------------------------------------------------------
// Name: EventDockWidget::SetEditing
// Created: ABR 2013-07-04
// -----------------------------------------------------------------------------
void EventDockWidget::SetEditing( bool editing )
{
    editing_ = editing;
    emit EditingChanged( editing );
}

// -----------------------------------------------------------------------------
// Name: EventDockWidget::SetEditing
// Created: LGY 2013-08-21
// -----------------------------------------------------------------------------
void EventDockWidget::NotifyModeChanged( E_Modes newMode, bool useDefault, bool firstChangeToSavedMode )
{
    if( newMode == eModes_Default )
        Purge();
    gui::RichDockWidget::NotifyModeChanged( newMode, useDefault, firstChangeToSavedMode );
}
