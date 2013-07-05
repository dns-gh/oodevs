// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "TimelineWebView.h"

#include "moc_TimelineWebView.cpp"
#include "TimelineToolBar.h"

#include "actions/Action_ABC.h"
#include "actions/ActionFactory_ABC.h"
#include "actions/ActionTiming.h"
#include "clients_kernel/ActionController.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/ContextMenu.h"
#include "clients_kernel/Controllers.h"
#include "ENT/ENT_Tr.h"
#include "gaming/AgentsModel.h"
#include "gaming/EventAction.h"
#include "gaming/EventsModel.h"
#include "gaming/Model.h"
#include "gaming/TimelinePublisher.h"
#include "MT_Tools/MT_Logger.h"
#include "protocol/Protocol.h"
#include "timeline/api.h"
#include "timeline/api.h"
#include "tools/ExerciseConfig.h"
#include "tools/Loader_ABC.h"
#include "tools/SchemaWriter.h"
#include <boost/bind.hpp>

// -----------------------------------------------------------------------------
// Name: TimelineWebView constructor
// Created: ABR 2013-05-28
// -----------------------------------------------------------------------------
TimelineWebView::TimelineWebView( QWidget* parent, const tools::ExerciseConfig& config, kernel::ActionController& actionController,
                                  Model& model, timeline::Configuration& cfg )
    : QWidget( parent )
    , config_( config )
    , actionController_( actionController )
    , model_( model )
    , server_( 0 )
    , cfg_( new timeline::Configuration( cfg ) )
    , creationSignalMapper_( 0 )
    , dummySignalMapper_( 0 )
{
    setObjectName( "timeline-webview" );

    mainLayout_ = new QVBoxLayout( this );
    mainLayout_->setMargin( 0 );
    mainLayout_->setSpacing( 0 );

    actionController_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: TimelineWebView destructor
// Created: ABR 2013-05-28
// -----------------------------------------------------------------------------
TimelineWebView::~TimelineWebView()
{
    actionController_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: TimelineWebView::Connect
// Created: ABR 2013-05-15
// -----------------------------------------------------------------------------
void TimelineWebView::Connect()
{
    timelineWidget_ = new QWidget();
    mainLayout_->addWidget( timelineWidget_ );
    cfg_->widget = timelineWidget_;
    server_ = MakeServer( *cfg_ );

    connect( this, SIGNAL( CreateEventSignal( const timeline::Event& ) ), server_.get(), SLOT( CreateEvent( const timeline::Event& ) ) );
    connect( this, SIGNAL( EditEventSignal( const timeline::Event& ) ), server_.get(), SLOT( UpdateEvent( const timeline::Event& ) ) );
    connect( this, SIGNAL( DeleteEventSignal( const std::string& ) ), server_.get(), SLOT( DeleteEvent( const std::string& ) ) );

    connect( server_.get(), SIGNAL( CreatedEvent( const timeline::Event&, const timeline::Error& ) ), this, SLOT( OnCreatedEvent( const timeline::Event&, const timeline::Error& ) ) );
    connect( server_.get(), SIGNAL( UpdatedEvent( const timeline::Event&, const timeline::Error& ) ), this, SLOT( OnEditedEvent( const timeline::Event&, const timeline::Error& ) ) );
    connect( server_.get(), SIGNAL( DeletedEvent( const std::string&, const timeline::Error& ) ), this, SLOT( OnDeletedEvent( const std::string&, const timeline::Error& ) ) );
    connect( server_.get(), SIGNAL( GetEvents( const timeline::Events&, const timeline::Error& ) ), this, SLOT( OnGetEvents( const timeline::Events&, const timeline::Error& ) ) );

    connect( server_.get(), SIGNAL( SelectedEvent( boost::shared_ptr< timeline::Event > ) ), this, SLOT( OnSelectedEvent( boost::shared_ptr< timeline::Event > ) ) );
    connect( server_.get(), SIGNAL( ActivatedEvent( const timeline::Event& ) ), this, SLOT( OnActivatedEvent( const timeline::Event& ) ) );
    connect( server_.get(), SIGNAL( ContextMenuEvent( boost::shared_ptr< timeline::Event >, const std::string& ) ), this, SLOT( OnContextMenuEvent( boost::shared_ptr< timeline::Event >, const std::string& ) ) );
    connect( server_.get(), SIGNAL( KeyUp( int ) ), this, SLOT( OnKeyUp( int ) ) );
}

// -----------------------------------------------------------------------------
// Name: TimelineWebView::Disconnect
// Created: ABR 2013-05-15
// -----------------------------------------------------------------------------
void TimelineWebView::Disconnect()
{
    if( server_.get() )
    {
        disconnect( this, SIGNAL( CreateEventSignal( const timeline::Event& ) ), server_.get(), SLOT( CreateEvent( const timeline::Event& ) ) );
        disconnect( this, SIGNAL( EditEventSignal( const timeline::Event& ) ), server_.get(), SLOT( UpdateEvent( const timeline::Event& ) ) );
        disconnect( this, SIGNAL( DeleteEventSignal( const std::string& ) ), server_.get(), SLOT( DeleteEvent( const std::string& ) ) );

        disconnect( server_.get(), SIGNAL( CreatedEvent( const timeline::Event&, const timeline::Error& ) ), this, SLOT( OnCreatedEvent( const timeline::Event&, const timeline::Error& ) ) );
        disconnect( server_.get(), SIGNAL( UpdatedEvent( const timeline::Event&, const timeline::Error& ) ), this, SLOT( OnEditedEvent( const timeline::Event&, const timeline::Error& ) ) );
        disconnect( server_.get(), SIGNAL( DeletedEvent( const std::string&, const timeline::Error& ) ), this, SLOT( OnDeletedEvent( const std::string&, const timeline::Error& ) ) );
        disconnect( server_.get(), SIGNAL( GetEvents( const timeline::Events&, const timeline::Error& ) ), this, SLOT( OnGetEvents( const timeline::Events&, const timeline::Error& ) ) );

        disconnect( server_.get(), SIGNAL( SelectedEvent( boost::shared_ptr< timeline::Event > ) ), this, SLOT( OnSelectedEvent( boost::shared_ptr< timeline::Event > ) ) );
        disconnect( server_.get(), SIGNAL( ActivatedEvent( const timeline::Event& ) ), this, SLOT( OnActivatedEvent( const timeline::Event& ) ) );
        disconnect( server_.get(), SIGNAL( ContextMenuEvent( boost::shared_ptr< timeline::Event >, const std::string& ) ), this, SLOT( OnContextMenuEvent( boost::shared_ptr< timeline::Event >, const std::string& ) ) );
        disconnect( server_.get(), SIGNAL( KeyUp( int ) ), this, SLOT( OnKeyUp( int ) ) );

        server_.reset();
        cfg_->widget = 0;
        mainLayout_->removeWidget( timelineWidget_ );
        delete timelineWidget_;
    }
}

// -----------------------------------------------------------------------------
// Name: TimelineWebView::GetOrCreateEvent
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
Event& TimelineWebView::GetOrCreateEvent( const timeline::Event& event )
{
    Event* gamingEvent = model_.events_.Find( event.uuid );
    if( !gamingEvent )
        gamingEvent = model_.events_.Create( event );
    else
        gamingEvent->Update( event );
    return *gamingEvent;
}

// -----------------------------------------------------------------------------
// Name: TimelineWebView::CreateEvent
// Created: ABR 2013-05-17
// -----------------------------------------------------------------------------
void TimelineWebView::CreateEvent( const timeline::Event& event )
{
    creationRequestedEvents_.push_back( event.uuid );
    emit CreateEventSignal( event );
}

// -----------------------------------------------------------------------------
// Name: TimelineWebView::EditEvent
// Created: ABR 2013-05-31
// -----------------------------------------------------------------------------
void TimelineWebView::EditEvent( const timeline::Event& event )
{
    editionRequestedEvents_.push_back( event.uuid );
    emit EditEventSignal( event );
}

// -----------------------------------------------------------------------------
// Name: TimelineWebView::DeleteEvent
// Created: ABR 2013-05-17
// -----------------------------------------------------------------------------
void TimelineWebView::DeleteEvent( const std::string& uuid )
{
    actionController_.DeselectAll();
    deletionRequestedEvents_.push_back( uuid );
    emit DeleteEventSignal( uuid );
}

// -----------------------------------------------------------------------------
// Name: TimelineWebView::OnCreatedEvent
// Created: ABR 2013-05-17
// -----------------------------------------------------------------------------
void TimelineWebView::OnCreatedEvent( const timeline::Event& event, const timeline::Error& error )
{
    auto it = std::find( creationRequestedEvents_.begin(), creationRequestedEvents_.end(), event.uuid );
    if( it != creationRequestedEvents_.end() )
    {
        if( error.code != timeline::EC_OK )
            MT_LOG_ERROR_MSG( tr( "An error occurred during event creation process: %1" ).arg( QString::fromStdString( error.text ) ).toStdString() );
        creationRequestedEvents_.erase( it );
    }
    model_.events_.Create( event );
}

// -----------------------------------------------------------------------------
// Name: TimelineWebView::OnEditedEvent
// Created: ABR 2013-05-31
// -----------------------------------------------------------------------------
void TimelineWebView::OnEditedEvent( const timeline::Event& event, const timeline::Error& error )
{
    auto it = std::find( editionRequestedEvents_.begin(), editionRequestedEvents_.end(), event.uuid );
    if( it != editionRequestedEvents_.end() )
    {
        if( error.code != timeline::EC_OK )
            MT_LOG_ERROR_MSG( tr( "An error occurred during event creation process: %1" ).arg( QString::fromStdString( error.text ) ).toStdString() );
        editionRequestedEvents_.erase( it );
    }
    model_.events_.Update( event );
}

// -----------------------------------------------------------------------------
// Name: TimelineWebView::OnDeletedEvent
// Created: ABR 2013-05-17
// -----------------------------------------------------------------------------
void TimelineWebView::OnDeletedEvent( const std::string& uuid, const timeline::Error& error )
{
    auto it = std::find( deletionRequestedEvents_.begin(), deletionRequestedEvents_.end(), uuid );
    if( it != deletionRequestedEvents_.end() )
    {
        if( error.code != timeline::EC_OK )
            MT_LOG_ERROR_MSG( tr( "An error occurred during event deletion process: %1" ).arg( QString::fromStdString( error.text ) ).toStdString() );
        deletionRequestedEvents_.erase( it );
    }
    model_.events_.Destroy( uuid );
}

// -----------------------------------------------------------------------------
// Name: TimelineWebView::OnSelectedEvent
// Created: ABR 2013-05-17
// -----------------------------------------------------------------------------
void TimelineWebView::OnSelectedEvent( boost::shared_ptr< timeline::Event > event )
{
    bool hadSelection = selected_.get() != 0;
    selected_ = event;
    if( selected_.get() )
    {
        Event& gamingEvent = GetOrCreateEvent( *selected_ );
        gamingEvent.Select( actionController_ );
    }
    else if( hadSelection )
        actionController_.DeselectAll();
}

// -----------------------------------------------------------------------------
// Name: TimelineWebView::OnActivatedEvent
// Created: ABR 2013-05-24
// -----------------------------------------------------------------------------
void TimelineWebView::OnActivatedEvent( const timeline::Event& event )
{
    Event& gamingEvent = GetOrCreateEvent( event );
    gamingEvent.Select( actionController_ );
    gamingEvent.Activate( actionController_ );
}

// -----------------------------------------------------------------------------
// Name: TimelineWebView::OnContextMenuEvent
// Created: ABR 2013-05-24
// -----------------------------------------------------------------------------
void TimelineWebView::OnContextMenuEvent( boost::shared_ptr< timeline::Event > event, const std::string& time )
{
    selectedDateTime_ = QDateTime::fromString( QString::fromStdString( time ), EVENT_DATE_FORMAT );
    //contextMenuEvent_ = event;
    if( event )
    {
        Event& gamingEvent = GetOrCreateEvent( *event );
        gamingEvent.ContextMenu( actionController_, QCursor::pos() );
    }
    else
        actionController_.ContextMenu( selectedDateTime_, QCursor::pos() );
}

namespace
{
    void AddToMenu( kernel::ContextMenu& menu, QSignalMapper* mapper, const QString& label, int mapping )
    {
        QAction* action = new QAction( label, &menu );
        QObject::connect( action, SIGNAL( triggered() ), mapper, SLOT( map() ) );
        mapper->setMapping( action, mapping );
        menu.InsertAction( "Command", action );
    }
}

// -----------------------------------------------------------------------------
// Name: TimelineWebView::NotifyContextMenu
// Created: ABR 2013-06-19
// -----------------------------------------------------------------------------
void TimelineWebView::NotifyContextMenu( const QDateTime& /* dateTime */, kernel::ContextMenu& menu )
{
    delete creationSignalMapper_;
    creationSignalMapper_ = new QSignalMapper( this );
    connect( creationSignalMapper_, SIGNAL( mapped( int ) ), this, SLOT( OnCreateClicked( int ) ) );

    delete dummySignalMapper_;
    dummySignalMapper_ = new QSignalMapper( this );
    connect( dummySignalMapper_, SIGNAL( mapped( int ) ), this, SLOT( OnCreateDummyClicked( int ) ) );

    kernel::ContextMenu* createMenu = new kernel::ContextMenu( &menu );
    kernel::ContextMenu* dummyMenu = new kernel::ContextMenu( &menu );
    for( int i = 0; i < eNbrEventTypes; ++i )
    {
        if( i != eEventTypes_Report )
            AddToMenu( *createMenu, creationSignalMapper_, QString::fromStdString( ENT_Tr::ConvertFromEventType( static_cast< E_EventTypes >( i ) ) ), i );
        AddToMenu( *dummyMenu, dummySignalMapper_, QString( "Dummy " ) + QString::fromStdString( ENT_Tr::ConvertFromEventType( static_cast< E_EventTypes >( i ) ) ), i );
    }

    menu.InsertItem( "Command", tr( "Create an event" ), createMenu );
    menu.InsertItem( "Command", "Create dummy event", dummyMenu );
}

// -----------------------------------------------------------------------------
// Name: TimelineWebView::OnCreateClicked
// Created: ABR 2013-06-19
// -----------------------------------------------------------------------------
void TimelineWebView::OnCreateClicked( int type )
{
    assert( type >= 0 && type < eNbrEventTypes );
    emit StartCreation( static_cast< E_EventTypes >( type ), selectedDateTime_ );
}

// -----------------------------------------------------------------------------
// Name: TimelineWebView::OnCreateDummyClicked
// Created: ABR 2013-06-19
// -----------------------------------------------------------------------------
void TimelineWebView::OnCreateDummyClicked( int type )
{
    assert( type >= 0 && type < eNbrEventTypes );
    CreateDummyEvent( static_cast< E_EventTypes >( type ) );
}

// -----------------------------------------------------------------------------
// Name: TimelineWebView::OnKeyUp
// Created: ABR 2013-05-24
// -----------------------------------------------------------------------------
void TimelineWebView::OnKeyUp( int key )
{
    if( selected_ && key == VK_DELETE )
        DeleteEvent( selected_->uuid );
}

// -----------------------------------------------------------------------------
// Temporary method to test display
// -----------------------------------------------------------------------------
void TimelineWebView::CreateDummyEvent( E_EventTypes type )
{
    // Action
    timeline::Action action;
    switch( type )
    {
    case eEventTypes_Order:
    case eEventTypes_SupervisorAction:
        action.target = CREATE_EVENT_TARGET( EVENT_ORDER_PROTOCOL, EVENT_SIMULATION_SERVICE );
        break;
    case eEventTypes_Report:
        action.target = CREATE_EVENT_TARGET( EVENT_REPORT_PROTOCOL, EVENT_SIMULATION_SERVICE );
        break;
    case eEventTypes_Multimedia:
        action.target = CREATE_EVENT_TARGET( EVENT_MULTIMEDIA_PROTOCOL, EVENT_MULTIMEDIA_SERVICE );
        break;
    }

    // Create dummy protobuf message if order or supervisor action
    if( type == eEventTypes_Order )
    {
        sword::ClientToSim msg;
        sword::UnitOrder* unitOrder = msg.mutable_message()->mutable_unit_order();
        unitOrder->mutable_tasker()->set_id( 159 );
        unitOrder->mutable_type()->set_id( 44582 ); // Move To
        sword::MissionParameters* parameters = unitOrder->mutable_parameters();
        parameters->add_elem()->add_value()->mutable_heading()->set_heading( 360 );
        parameters->add_elem()->set_null_value( true );
        parameters->add_elem()->set_null_value( true );
        parameters->add_elem()->set_null_value( true );
        sword::Location* location = parameters->add_elem()->add_value()->mutable_point()->mutable_location();
        location->set_type( sword::Location_Geometry_point );
        sword::CoordLatLong* latLong = location->mutable_coordinates()->add_elem();
        latLong->set_latitude( 30.632128244641702 );
        latLong->set_longitude( 28.976535107619295 );
        msg.SerializePartialToString( &action.payload );
    }
    else if( type == eEventTypes_SupervisorAction )
    {
        sword::ClientToSim msg;
        sword::UnitMagicAction* unitMagic = msg.mutable_message()->mutable_unit_magic_action();
        unitMagic->mutable_tasker()->mutable_unit()->set_id( 159 );
        unitMagic->set_type( sword::UnitMagicAction_Type_move_to );
        sword::Location* location = unitMagic->mutable_parameters()->add_elem()->add_value()->mutable_point()->mutable_location();
        location->set_type( sword::Location_Geometry_point );
        sword::CoordLatLong* latLong = location->mutable_coordinates()->add_elem();
        latLong->set_latitude( 30.632128244641702 );
        latLong->set_longitude( 28.976535107619295 );
        msg.SerializePartialToString( &action.payload );
    }

    // Event
    timeline::Event event;
    event.name = "Dummy " + ENT_Tr::ConvertFromEventType( type );
    event.info = "Dummy Infos";
    event.begin = selectedDateTime_.toString( EVENT_DATE_FORMAT ).toStdString();
    event.done = false;
    event.action = action;
    CreateEvent( event );
}

// -----------------------------------------------------------------------------
// Name: TimelineWebView::OnLoadOrderFileRequested
// Created: ABR 2013-06-19
// -----------------------------------------------------------------------------
void TimelineWebView::OnLoadOrderFileRequested( const tools::Path& filename )
{
    config_.GetLoader().LoadFile( filename, boost::bind( &TimelineWebView::ReadActions, this, _1 ) ); // $$$$ ABR 2013-06-19: ReadOnly if replay ? may be we can handle that directly with timeline_ui, or with profile
}

// -----------------------------------------------------------------------------
// Name: TimelineWebView::ReadActions
// Created: ABR 2013-06-19
// -----------------------------------------------------------------------------
void TimelineWebView::ReadActions( xml::xistream& xis )
{
    xis >> xml::start( "actions" )
        >> xml::list( "action", *this, &TimelineWebView::ReadAction )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: TimelineWebView::ReadAction
// Created: ABR 2013-06-19
// -----------------------------------------------------------------------------
void TimelineWebView::ReadAction( xml::xistream& xis )
{
    std::auto_ptr< actions::Action_ABC > action;
    // $$$$ ABR 2013-06-19: The dual try catch was in actionsModel, so i keep it for now but it's ugly and should be remove asap
    try
    {
        action.reset( model_.actionFactory_.CreateAction( xis, false ) );
    }
    catch( const std::exception& )
    {
        try
        {
            action.reset( model_.actionFactory_.CreateStubAction( xis ) );
        }
        catch( const std::exception& )
        {
            // NOTHING
        }
    }

    if( action.get() == 0 )
        return;

    action->Publish( model_.timelinePublisher_, 0 );
    timeline::Event event;
    event.action.payload = model_.timelinePublisher_.GetPayload();
    event.action.apply = true;
    event.action.target = CREATE_EVENT_TARGET( EVENT_ORDER_PROTOCOL, EVENT_SIMULATION_SERVICE );
    event.name = action->GetName();

    const actions::ActionTiming& timing = action->Get< actions::ActionTiming >();
    event.begin = timing.GetTime().toString( EVENT_DATE_FORMAT ).toStdString();
    //event.done = false;
    emit CreateEventSignal( event );
}

// -----------------------------------------------------------------------------
// Name: TimelineWebView::OnSaveOrderFileRequested
// Created: ABR 2013-06-19
// -----------------------------------------------------------------------------
void TimelineWebView::OnSaveOrderFileRequested( const tools::Path& filename )
{
    if( server_.get() )
    {
        currentOrderFile_ = filename;
        server_->ReadEvents();
    }
}

// -----------------------------------------------------------------------------
// Name: TimelineWebView::OnGetEvents
// Created: ABR 2013-06-19
// -----------------------------------------------------------------------------
void TimelineWebView::OnGetEvents( const timeline::Events& events, const timeline::Error& error )
{
    if( currentOrderFile_.IsEmpty() )
        return;

    if( error.code != timeline::EC_OK )
    {
        MT_LOG_ERROR_MSG( tr( "An error occurred during 'get all events' request: %1" ).arg( QString::fromStdString( error.text ) ).toStdString() );
        return;
    }

    tools::Xofstream xos( currentOrderFile_ );
    tools::SchemaWriter schemaWriter;
    xos << xml::start( "actions" );
    schemaWriter.WriteExerciseSchema( xos, "orders" );
    for( auto it = events.begin(); it != events.end(); ++it )
    {
        Event& event = GetOrCreateEvent( *it );
        if( event.GetType() == eEventTypes_Order )
            if( const actions::Action_ABC* action = static_cast< EventAction& >( event ).GetAction() )
            {
                xos << xml::start( "action" );
                action->Serialize( xos );
                xos << xml::end; //! action
            }
    }
    xos << xml::end; //! actions

    currentOrderFile_.Clear();
}
