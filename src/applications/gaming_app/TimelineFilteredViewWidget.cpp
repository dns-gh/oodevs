// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "TimelineFilteredViewWidget.h"
#include "moc_TimelineFilteredViewWidget.cpp"
#include "EventDialog.h"
#include "TimelineToolBar.h"

#include "actions/Action_ABC.h"
#include "clients_kernel/ActionController.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Time_ABC.h"
#include "ENT/ENT_Tr.h"
#include "gaming/AgentsModel.h"
#include "gaming/EventAction.h"
#include "gaming/EventsModel.h"
#include "MT_Tools/MT_Logger.h"
#include "protocol/Protocol.h"
#include "timeline/api.h"
#include "tools/ProtobufSerialization.h"

// -----------------------------------------------------------------------------
// Name: TimelineFilteredViewWidget constructor
// Created: ABR 2013-05-28
// -----------------------------------------------------------------------------
TimelineFilteredViewWidget::TimelineFilteredViewWidget( QWidget* parent, kernel::Controllers& controllers, const kernel::Time_ABC& simulation, EventsModel& eventsModel, EventDialog& eventDialog, timeline::Configuration& cfg, int viewNumber, const QStringList& filters )
    : QWidget( parent )
    , controllers_( controllers )
    , simulation_( simulation )
    , eventsModel_( eventsModel )
    , eventDialog_( eventDialog )
    , toolBar_( 0 )
    , timelineWidget_( 0 )
    , server_( 0 )
    , cfg_( new timeline::Configuration( cfg ) )
    , viewNumber_( viewNumber )
{
    setObjectName( QString( "timeline-filteredview-widget-%1" ).arg( viewNumber ) );
    mainLayout_ = new QVBoxLayout( this );
    mainLayout_->setMargin( 0 );
    mainLayout_->setSpacing( 0 );
    toolBar_ = new TimelineToolBar( 0, viewNumber == 0, filters );
    setObjectName( QString( "timeline-filteredview-widget-%1" ).arg( viewNumber ) );
    connect( toolBar_, SIGNAL( FilterSelectionChanged( const QStringList& ) ), this, SLOT( OnFilterSelectionChanged( const QStringList& ) ) );
    connect( toolBar_, SIGNAL( AddNewFilteredView( const QStringList& ) ), this, SIGNAL( AddNewFilteredView( const QStringList& ) ) );
    connect( toolBar_, SIGNAL( RemoveCurrentFilteredView() ), this, SIGNAL( RemoveCurrentFilteredView() ) );
}

// -----------------------------------------------------------------------------
// Name: TimelineFilteredViewWidget destructor
// Created: ABR 2013-05-28
// -----------------------------------------------------------------------------
TimelineFilteredViewWidget::~TimelineFilteredViewWidget()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TimelineFilteredViewWidget::GetViewNumber
// Created: ABR 2013-05-28
// -----------------------------------------------------------------------------
int TimelineFilteredViewWidget::GetViewNumber() const
{
    return viewNumber_;
}

// -----------------------------------------------------------------------------
// Name: TimelineFilteredViewWidget::Connect
// Created: ABR 2013-05-15
// -----------------------------------------------------------------------------
void TimelineFilteredViewWidget::Connect()
{
    timelineWidget_ = new QWidget( this );
    timelineWidget_->setObjectName( "timeline-widget" );
    mainLayout_->addWidget( toolBar_ );
    mainLayout_->addWidget( timelineWidget_, 1 );

    cfg_->widget = timelineWidget_;

    server_ = MakeServer( *cfg_ );

    connect( this, SIGNAL( CreateEventSignal( const timeline::Event& ) ), server_.get(), SLOT( CreateEvent( const timeline::Event& ) ) );
    //connect( this, SIGNAL( EditEventSignal( const timeline::Event& ) ), server_.get(), SLOT( EditEvent( const timeline::Event& ) ) );
    connect( this, SIGNAL( DeleteEventSignal( const std::string& ) ), server_.get(), SLOT( DeleteEvent( const std::string& ) ) );

    connect( server_.get(), SIGNAL( CreatedEvent( const timeline::Event&, const timeline::Error& ) ), this, SLOT( OnCreatedEvent( const timeline::Event&, const timeline::Error& ) ) );
    //connect( server_.get(), SIGNAL( EditedEvent( const timeline::Event&, const timeline::Error& ) ), this, SLOT( OnEditedEvent( const timeline::Event&, const timeline::Error& ) ) );
    connect( server_.get(), SIGNAL( DeletedEvent( const std::string&, const timeline::Error& ) ), this, SLOT( OnDeletedEvent( const std::string&, const timeline::Error& ) ) );

    connect( server_.get(), SIGNAL( SelectedEvent( boost::shared_ptr< timeline::Event > ) ), this, SLOT( OnSelectedEvent( boost::shared_ptr< timeline::Event > ) ) );
    connect( server_.get(), SIGNAL( ActivatedEvent( const timeline::Event& ) ), this, SLOT( OnActivatedEvent( const timeline::Event& ) ) );
    connect( server_.get(), SIGNAL( ContextMenuEvent( boost::shared_ptr< timeline::Event > ) ), this, SLOT( OnContextMenuEvent( boost::shared_ptr< timeline::Event > ) ) );
    connect( server_.get(), SIGNAL( KeyUp( int ) ), this, SLOT( OnKeyUp( int ) ) );
}

// -----------------------------------------------------------------------------
// Name: TimelineFilteredViewWidget::Disconnect
// Created: ABR 2013-05-15
// -----------------------------------------------------------------------------
void TimelineFilteredViewWidget::Disconnect()
{
    if( server_.get() )
    {
        disconnect( this, SIGNAL( CreateEventSignal( const timeline::Event& ) ), server_.get(), SLOT( CreateEvent( const timeline::Event& ) ) );
        //disconnect( this, SIGNAL( EditEventSignal( const timeline::Event& ) ), server_.get(), SLOT( EditEvent( const timeline::Event& ) ) );
        disconnect( this, SIGNAL( DeleteEventSignal( const std::string& ) ), server_.get(), SLOT( DeleteEvent( const std::string& ) ) );

        disconnect( server_.get(), SIGNAL( CreatedEvent( const timeline::Event&, const timeline::Error& ) ), this, SLOT( OnCreatedEvent( const timeline::Event&, const timeline::Error& ) ) );
        //disconnect( server_.get(), SIGNAL( EditedEvent( const timeline::Event&, const timeline::Error& ) ), this, SLOT( OnEditedEvent( const timeline::Event&, const timeline::Error& ) ) );
        disconnect( server_.get(), SIGNAL( DeletedEvent( const std::string&, const timeline::Error& ) ), this, SLOT( OnDeletedEvent( const std::string&, const timeline::Error& ) ) );

        disconnect( server_.get(), SIGNAL( SelectedEvent( boost::shared_ptr< timeline::Event > ) ), this, SLOT( OnSelectedEvent( boost::shared_ptr< timeline::Event > ) ) );
        disconnect( server_.get(), SIGNAL( ActivatedEvent( const timeline::Event& ) ), this, SLOT( OnActivatedEvent( const timeline::Event& ) ) );
        disconnect( server_.get(), SIGNAL( ContextMenuEvent( boost::shared_ptr< timeline::Event > ) ), this, SLOT( OnContextMenuEvent( boost::shared_ptr< timeline::Event > ) ) );
        disconnect( server_.get(), SIGNAL( KeyUp( int ) ), this, SLOT( OnKeyUp( int ) ) );

        server_.reset();
        mainLayout_->removeWidget( timelineWidget_ );
        mainLayout_->removeWidget( toolBar_ );
        delete timelineWidget_;
        cfg_->widget = 0;
    }
}

// -----------------------------------------------------------------------------
// Name: TimelineFilteredViewWidget::GetOrCreateEvent
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
Event& TimelineFilteredViewWidget::GetOrCreateEvent( const timeline::Event& event )
{
    Event* gamingEvent = eventsModel_.Find( event.uuid );
    if( !gamingEvent )
        gamingEvent = eventsModel_.Create( event );
    else
        gamingEvent->Update( event );
    return *gamingEvent;
}

// -----------------------------------------------------------------------------
// Name: TimelineFilteredViewWidget::CreateEvent
// Created: ABR 2013-05-17
// -----------------------------------------------------------------------------
void TimelineFilteredViewWidget::CreateEvent( const timeline::Event& event )
{
    creationRequestedEvents_.push_back( event.uuid );
    emit CreateEventSignal( event );
}

// -----------------------------------------------------------------------------
// Name: TimelineFilteredViewWidget::EditEvent
// Created: ABR 2013-05-31
// -----------------------------------------------------------------------------
void TimelineFilteredViewWidget::EditEvent( const timeline::Event& event )
{
    editionRequestedEvents_.push_back( event.uuid );
    emit EditEventSignal( event );
}

// -----------------------------------------------------------------------------
// Name: TimelineFilteredViewWidget::DeleteEvent
// Created: ABR 2013-05-17
// -----------------------------------------------------------------------------
void TimelineFilteredViewWidget::DeleteEvent( const std::string& uuid )
{
    deletionRequestedEvents_.push_back( uuid );
    emit DeleteEventSignal( uuid );
}

// -----------------------------------------------------------------------------
// Name: TimelineFilteredViewWidget::OnCreatedEvent
// Created: ABR 2013-05-17
// -----------------------------------------------------------------------------
void TimelineFilteredViewWidget::OnCreatedEvent( const timeline::Event& event, const timeline::Error& error )
{
    auto it = std::find( creationRequestedEvents_.begin(), creationRequestedEvents_.end(), event.uuid );
    if( it != creationRequestedEvents_.end() )
    {
        if( error.code != timeline::EC_OK )
            MT_LOG_ERROR_MSG( tr( "An error occurred during event creation process: %1" ).arg( QString::fromStdString( error.text ) ).toStdString() );
        creationRequestedEvents_.erase( it );
    }
    eventsModel_.Create( event );
}

// -----------------------------------------------------------------------------
// Name: TimelineFilteredViewWidget::OnEditedEvent
// Created: ABR 2013-05-31
// -----------------------------------------------------------------------------
void TimelineFilteredViewWidget::OnEditedEvent( const timeline::Event& event, const timeline::Error& error )
{
    auto it = std::find( editionRequestedEvents_.begin(), editionRequestedEvents_.end(), event.uuid );
    if( it != editionRequestedEvents_.end() )
    {
        if( error.code != timeline::EC_OK )
            MT_LOG_ERROR_MSG( tr( "An error occurred during event creation process: %1" ).arg( QString::fromStdString( error.text ) ).toStdString() );
        editionRequestedEvents_.erase( it );
    }
    eventsModel_.Update( event );
}

// -----------------------------------------------------------------------------
// Name: TimelineFilteredViewWidget::OnDeletedEvent
// Created: ABR 2013-05-17
// -----------------------------------------------------------------------------
void TimelineFilteredViewWidget::OnDeletedEvent( const std::string& uuid, const timeline::Error& error )
{
    auto it = std::find( deletionRequestedEvents_.begin(), deletionRequestedEvents_.end(), uuid );
    if( it != deletionRequestedEvents_.end() )
    {
        if( error.code != timeline::EC_OK )
            MT_LOG_ERROR_MSG( tr( "An error occurred during event deletion process: %1" ).arg( QString::fromStdString( error.text ) ).toStdString() );
        deletionRequestedEvents_.erase( it );
    }
    eventsModel_.Destroy( uuid );
}

// -----------------------------------------------------------------------------
// Name: TimelineFilteredViewWidget::OnSelectedEvent
// Created: ABR 2013-05-17
// -----------------------------------------------------------------------------
void TimelineFilteredViewWidget::OnSelectedEvent( boost::shared_ptr< timeline::Event > event )
{
    bool hadSelection = selected_.get() != 0;
    selected_ = event;
    if( selected_.get() )
    {
        Event& gamingEvent = GetOrCreateEvent( *event );
        if( gamingEvent.GetType() == eEventTypes_Order )
        {
            const actions::Action_ABC* action = static_cast< EventAction& >( gamingEvent ).GetAction();
            if( action )
                action->Select( controllers_.actions_ );
        }
    }
    else if( hadSelection )
        controllers_.actions_.DeselectAll();
}

// -----------------------------------------------------------------------------
// Name: TimelineFilteredViewWidget::OnActivatedEvent
// Created: ABR 2013-05-24
// -----------------------------------------------------------------------------
void TimelineFilteredViewWidget::OnActivatedEvent( const timeline::Event& event )
{
    Event& gamingEvent = GetOrCreateEvent( event );
    eventDialog_.Edit( gamingEvent );
}

// -----------------------------------------------------------------------------
// Name: TimelineFilteredViewWidget::OnContextMenuEvent
// Created: ABR 2013-05-24
// -----------------------------------------------------------------------------
void TimelineFilteredViewWidget::OnContextMenuEvent( boost::shared_ptr< timeline::Event > event )
{
    QMenu menu;
    if( event )
    {
        const QString editText = tr( "Edit" );
        const QString deleteText = tr( "Delete" );
        menu.addAction( editText );
        menu.addAction( deleteText );
        if( QAction* resultingAction = menu.exec( QCursor::pos() ) )
        {
            if( event && resultingAction->text() == editText )
                OnActivatedEvent( *event );
            else if( event && resultingAction->text() == deleteText )
                DeleteEvent( event->uuid );
        }
    }
    else
    {
        QMenu createMenu, dummyMenu;
        createMenu.setTitle( tr( "Create an event" ) );
        dummyMenu.setTitle( "Create dummy event" );
        for( int i = 0; i < eNbrEventTypes; ++i )
        {
            if( i != eEventTypes_Report )
                createMenu.addAction( QString::fromStdString( ENT_Tr::ConvertFromEventType( static_cast< E_EventTypes >( i ) ) ) );
            dummyMenu.addAction( QString( "Dummy " )+ QString::fromStdString( ENT_Tr::ConvertFromEventType( static_cast< E_EventTypes >( i ) ) ) );
        }
        menu.addMenu( &createMenu );
        menu.addMenu( &dummyMenu );
        if( QAction* resultingAction = menu.exec( QCursor::pos() ) )
        {
            QString text = resultingAction->text();
            QBool isDummy = text.contains( "Dummy " );
            if( isDummy )
                text.remove( "Dummy " );

            E_EventTypes type = ENT_Tr::ConvertToEventType( text.toStdString() );
            assert( type >= 0 && type < eNbrEventTypes );
            if( isDummy )
                CreateDummyEvent( type );
            else
                eventDialog_.Create( type );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: TimelineFilteredViewWidget::OnKeyUp
// Created: ABR 2013-05-24
// -----------------------------------------------------------------------------
void TimelineFilteredViewWidget::OnKeyUp( int key )
{
    if( selected_ && key == VK_DELETE )
        DeleteEvent( selected_->uuid );
}

// -----------------------------------------------------------------------------
// Name: TimelineFilteredViewWidget::OnFilterSelectionChanged
// Created: ABR 2013-05-28
// -----------------------------------------------------------------------------
void TimelineFilteredViewWidget::OnFilterSelectionChanged( const QStringList& )
{
    // Send the new filter list to the timeline_server
}


// -----------------------------------------------------------------------------
// Temporary method to test display
// -----------------------------------------------------------------------------
void TimelineFilteredViewWidget::CreateDummyEvent( E_EventTypes type )
{
    // Action
    timeline::Action action;
    switch( type )
    {
    case eEventTypes_Order:
    case eEventTypes_SupervisorAction:
        action.target = "sword://sim";
        break;
    case eEventTypes_Report:
        action.target = "sword://client";
        break;
    case eEventTypes_Multimedia:
        action.target = "sword://multimedia";
        break;
    }

    // Create dummy protobuf message if order or supervisor action
    if( type == eEventTypes_Order )
    {
        sword::ClientToSim msg;
        sword::UnitOrder* unitOrder = msg.mutable_message()->mutable_unit_order();
        unitOrder->mutable_tasker()->set_id( 159 );
        unitOrder->mutable_type()->set_id( 44582 ); // Move To
        unitOrder->set_label( "dummy" + ENT_Tr::ConvertFromEventType( type ) );
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
        action.payload = tools::ProtoToBinary( msg );
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
        action.payload = tools::ProtoToBinary( msg );
    }

    // Event
    timeline::Event event;
    event.name = "Dummy " + ENT_Tr::ConvertFromEventType( type );
    event.info = "Dummy Infos";
    event.begin = simulation_.GetDateTime().toTimeSpec( Qt::UTC ).toString( "yyyy-MM-ddTHH:mm:ssZ" ).toStdString();
    event.done = false;
    event.action = action;
    CreateEvent( event );
}
