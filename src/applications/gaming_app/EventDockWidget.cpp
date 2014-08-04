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
#include "EventMagicWidget.h"
#include "EventTopWidget.h"
#include "EventTaskWidget.h"

#include "clients_gui/Event.h"
#include "clients_gui/EventFactory.h"
#include "clients_gui/EventPresenter.h"
#include "clients_gui/EventViewState.h"

#include "clients_kernel/ActionController.h"
#include "clients_kernel/Time_ABC.h"
#include "clients_kernel/TimelineHandler_ABC.h"
#include "clients_kernel/Tools.h"

#include "gaming/Model.h"
#include "gaming/StaticModel.h"

#include "actions/Action_ABC.h"
#include "actions/ActionError.h"

#include <timeline/api.h>

#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#pragma warning( push )
#pragma warning( disable : 4724 )
#include <boost/uuid/random_generator.hpp>
#pragma warning( pop )
#include <boost/uuid/uuid_io.hpp>

namespace
{
    void AddDefaultView( std::vector< gui::EventDefaultView_ABC* >& views,
                         QStackedWidget& stack,
                         int index,
                         EventDefaultWidget_ABC* widget )
    {
        views.push_back( widget );
        stack.insertWidget( index, widget );
    }
}

// -----------------------------------------------------------------------------
// Name: EventDockWidget constructor
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
EventDockWidget::EventDockWidget( QWidget* parent,
                                  kernel::Controllers& controllers,
                                  Model& model,
                                  const tools::ExerciseConfig& config,
                                  const kernel::Time_ABC& simulation,
                                  actions::gui::InterfaceBuilder_ABC& interfaceBuilder,
                                  const kernel::Profile_ABC& profile,
                                  gui::GlTools_ABC& tools,
                                  const gui::EntitySymbols& entitySymbols )
    : gui::RichDockWidget( controllers, parent, "event-dock-widget" )
    , presenter_( new gui::EventPresenter( *this, model.eventFactory_ ) )
    , selected_( controllers )
    , stack_( new QStackedWidget() )
{
    setWindowTitle( tr( "Event edition" ) );
    setFloating( true );
    setMinimumSize( 350, 350 );
    setVisible( false );

    // Header / footer
    EventTopWidget* header = new EventTopWidget( *presenter_, simulation, controllers.actions_ );
    views_.push_back( header );
    EventBottomWidget* footer = new EventBottomWidget( *presenter_ );
    views_.push_back( footer );

    // Stack
    QScrollArea* stackArea = new QScrollArea();
    QHBoxLayout* stackAreaLayout = new QHBoxLayout( stackArea );
    stackArea->setFrameStyle( QFrame::Box | QFrame::Sunken );
    stackArea->setWidgetResizable( true );
    stackAreaLayout->addWidget( stack_ );

    // Content
    stack_->insertWidget( eEventTypes_Order, new EventOrderWidget( *presenter_, controllers, model,
                                                                   config, interfaceBuilder, profile,
                                                                   tools, simulation, entitySymbols ) );
    stack_->insertWidget( eEventTypes_Magic , new EventMagicWidget( *presenter_, controllers,
                                                                    model, entitySymbols ) );
    stack_->insertWidget( eEventTypes_Task , new EventTaskWidget( *presenter_ ) );
    AddDefaultView( views_, *stack_, eEventTypes_Report     , new EventReportWidget( *presenter_ ) );
    AddDefaultView( views_, *stack_, eEventTypes_Multimedia , new EventMultimediaWidget( *presenter_ ) );
    AddDefaultView( views_, *stack_, eNbrEventTypes         , new EventDetailWidget( *presenter_ ) );

    // Layout
    QWidget* mainWidget = new QWidget();
    QVBoxLayout* mainLayout = new QVBoxLayout( mainWidget );
    mainLayout->setMargin( 5 );
    mainLayout->setSpacing( 5 );
    mainLayout->addWidget( header );
    mainLayout->addWidget( stackArea, 1 );
    mainLayout->addWidget( footer );

    setWidget( mainWidget );
    SetContentVisible( false );

    // Connections
    controllers_.actions_.Unregister( *this );
    controllers_.eventActions_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: EventDockWidget destructor
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
EventDockWidget::~EventDockWidget()
{
    controllers_.eventActions_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: EventDockWidget::GetPresenter
// Created: ABR 2013-11-22
// -----------------------------------------------------------------------------
gui::EventPresenter& EventDockWidget::GetPresenter() const
{
    return *presenter_;
}

// -----------------------------------------------------------------------------
// Name: EventDockWidget::SetTimelineHandler
// Created: ABR 2014-01-07
// -----------------------------------------------------------------------------
void EventDockWidget::SetTimelineHandler( const boost::shared_ptr< kernel::TimelineHandler_ABC >& timelineHandler )
{
    timelineHandler_ = timelineHandler;
    presenter_->SetTimelineHandler( timelineHandler );
}

// -----------------------------------------------------------------------------
// Name: EventDockWidget::ApplyToViews
// Created: ABR 2013-12-04
// -----------------------------------------------------------------------------
void EventDockWidget::ApplyToViews( const EventDockWidget::T_ViewFunctor& functor )
{
    std::for_each( views_.begin(), views_.end(), functor );
}

// -----------------------------------------------------------------------------
// Name: EventDockWidget::Purge
// Created: ABR 2013-11-21
// -----------------------------------------------------------------------------
void EventDockWidget::Purge()
{
    ApplyToViews( boost::bind( &gui::EventBaseView_ABC::Purge, _1 ) );
}

// -----------------------------------------------------------------------------
// Name: EventDockWidget::Build
// Created: ABR 2013-11-19
// -----------------------------------------------------------------------------
void EventDockWidget::Build( const gui::EventViewState& state )
{
    if( !state.event_ || state.mode_ == eEventDockModes_None )
    {
        SetContentVisible( false );
        return;
    }
    SetContentVisible( true );
    stack_->setCurrentIndex( state.event_ && !state.detail_ ? state.event_->GetType() : eNbrEventTypes );
    ApplyToViews( boost::bind( &gui::EventDefaultView_ABC::Build, _1, boost::cref( state ) ) );
    if( state.raise_ )
    {
        setVisible( true );
        raise();
    }
}

// -----------------------------------------------------------------------------
// Name: EventDockWidget::BlockSignals
// Created: ABR 2013-11-22
// -----------------------------------------------------------------------------
void EventDockWidget::BlockSignals( bool blocked )
{
    ApplyToViews( boost::bind( &gui::EventDefaultView_ABC::BlockSignals, _1, blocked ) );
    stack_->blockSignals( blocked );
    blockSignals( blocked );
}

// -----------------------------------------------------------------------------
// Name: EventDockWidget::SetContentVisible
// Created: ABR 2013-12-04
// -----------------------------------------------------------------------------
void EventDockWidget::SetContentVisible( bool visible )
{
    widget()->setVisible( visible );
}

// -----------------------------------------------------------------------------
// Name: EventDockWidget::Draw
// Created: ABR 2013-06-11
// -----------------------------------------------------------------------------
void EventDockWidget::Draw( gui::Viewport_ABC& viewport )
{
    presenter_->Draw( viewport );
    ApplyToViews( boost::bind( &gui::EventDefaultView_ABC::Draw, _1, boost::ref( viewport ) ) );
}

// -----------------------------------------------------------------------------
// Name: EventDockWidget::closeEvent
// Created: ABR 2013-06-11
// -----------------------------------------------------------------------------
void EventDockWidget::closeEvent( QCloseEvent * event )
{
    presenter_->Purge();
    gui::RichDockWidget::closeEvent( event );
}

// -----------------------------------------------------------------------------
// Name: EventDockWidget::OnEditClicked
// Created: ABR 2013-07-02
// -----------------------------------------------------------------------------
void EventDockWidget::OnEditClicked()
{
    if( selected_ )
        presenter_->StartEdition( *selected_, true );
    selected_ = 0;
}

// -----------------------------------------------------------------------------
// Name: EventDockWidget::OnDeleteClicked
// Created: ABR 2013-07-02
// -----------------------------------------------------------------------------
void EventDockWidget::OnDeleteClicked()
{
    if( selected_ )
        timelineHandler_->DeleteEvent( selected_->GetEvent().uuid );
    selected_ = 0;
}

// -----------------------------------------------------------------------------
// Name: EventDockWidget::NotifyActivated
// Created: ABR 2013-07-02
// -----------------------------------------------------------------------------
void EventDockWidget::NotifyActivated( const gui::Event& event )
{
    presenter_->StartEdition( event, true );
}

// -----------------------------------------------------------------------------
// Name: EventDockWidget::NotifySelected
// Created: BAX 2013-09-17
// -----------------------------------------------------------------------------
void EventDockWidget::NotifySelected( const gui::Event* event )
{
    if( event )
        presenter_->StartEdition( *event, true );
}

// -----------------------------------------------------------------------------
// Name: EventDockWidget::NotifyContextMenu
// Created: ABR 2013-07-02
// -----------------------------------------------------------------------------
void EventDockWidget::NotifyContextMenu( const gui::Event& event, kernel::ContextMenu& menu )
{
    selected_ = &event;
    menu.InsertItem( "Command", event.GetEvent().done ? tr( "Display") : tr( "Edit" ),
                     this, SLOT( OnEditClicked() ) );
    if( !event.GetEvent().done )
        menu.InsertItem( "Command", tr( "Delete" ), this, SLOT( OnDeleteClicked() ) );
}

// -----------------------------------------------------------------------------
// Name: EventDockWidget::NotifyDeleted
// Created: ABR 2013-06-18
// -----------------------------------------------------------------------------
void EventDockWidget::NotifyDeleted( const gui::Event& event )
{
    presenter_->OnEventDeleted( event );
}

// -----------------------------------------------------------------------------
// Name: EventDockWidget::NotifyUpdated
// Created: ABR 2013-06-18
// -----------------------------------------------------------------------------
void EventDockWidget::NotifyUpdated( const gui::Event& event )
{
    presenter_->OnEventUpdated( event );
}

// -----------------------------------------------------------------------------
// Name: EventDockWidget::SetEditing
// Created: LGY 2013-08-21
// -----------------------------------------------------------------------------
void EventDockWidget::NotifyModeChanged( E_Modes newMode, bool useDefault, bool firstChangeToSavedMode )
{
    if( newMode == eModes_Default )
        presenter_->Purge();
    gui::RichDockWidget::NotifyModeChanged( newMode, useDefault, firstChangeToSavedMode );
    setVisible( false );
}
