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
#include "TimelineToolBar.h"
#include "ENT/ENT_Tr.h"
#include "gaming/EventsModel.h"
#include "gaming/Model.h"
#include "gaming/Simulation.h"
#include "MT_Tools/MT_Logger.h"
#include "protocol/Protocol.h"
#include "timeline/api.h"
#include "tools/Base64Converters.h"

// -----------------------------------------------------------------------------
// Name: TimelineFilteredViewWidget constructor
// Created: ABR 2013-05-28
// -----------------------------------------------------------------------------
TimelineFilteredViewWidget::TimelineFilteredViewWidget( QWidget* parent, const Simulation& simulation, Model& model, timeline::Configuration& cfg, int viewNumber, const QStringList& filters )
    : QWidget( parent )
    , toolBar_( 0 )
    , timelineWidget_( 0 )
    , server_( 0 )
    , simulation_( simulation )
    , model_( model )
    , cfg_( new timeline::Configuration( cfg ) )
    , viewNumber_( viewNumber )
{
    setObjectName( QString( "timeline-filteredview-widget-%1" ).arg( viewNumber ) );
    mainLayout_ = new QVBoxLayout( this );
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
    connect( this, SIGNAL( DeleteEventSignal( const std::string& ) ), server_.get(), SLOT( DeleteEvent( const std::string& ) ) );

    connect( server_.get(), SIGNAL( CreatedEvent( const timeline::Event&, const timeline::Error& ) ), this, SLOT( OnCreatedEvent( const timeline::Event&, const timeline::Error& ) ) );
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
        disconnect( this, SIGNAL( DeleteEventSignal( const std::string& ) ), server_.get(), SLOT( DeleteEvent( const std::string& ) ) );

        disconnect( server_.get(), SIGNAL( CreatedEvent( const timeline::Event&, const timeline::Error& ) ), this, SLOT( OnCreatedEvent( const timeline::Event&, const timeline::Error& ) ) );
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
// Name: TimelineFilteredViewWidget::CreateEvent
// Created: ABR 2013-05-17
// -----------------------------------------------------------------------------
void TimelineFilteredViewWidget::CreateEvent( const timeline::Event& event )
{
    creationRequestedEvents_.push_back( event.uuid );
    emit CreateEventSignal( event );
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
    model_.events_.Create( event );
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
    model_.events_.Destroy( uuid );
}

// -----------------------------------------------------------------------------
// Name: TimelineFilteredViewWidget::OnSelectedEvent
// Created: ABR 2013-05-17
// -----------------------------------------------------------------------------
void TimelineFilteredViewWidget::OnSelectedEvent( boost::shared_ptr< timeline::Event > event )
{
    selected_ = event;
    if( selected_.get() )
    {
        Event* gamingEvent = model_.events_.Find( event->uuid );
        if( !gamingEvent )
            gamingEvent = model_.events_.Create( *event );
        //if( gamingEvent->GetType() == eEventTypes_Order )
        //    static_cast< EventOrder* >( gamingEvent )->action_.Select( controllers_.actions_ );
    }
}

// -----------------------------------------------------------------------------
// Name: TimelineFilteredViewWidget::OnActivatedEvent
// Created: ABR 2013-05-24
// -----------------------------------------------------------------------------
void TimelineFilteredViewWidget::OnActivatedEvent( const timeline::Event& /*event*/ )
{
    // Display event dialog filled with this event
    //Event& gamingEvent = model_.events_.Get( event.uuid );
    //eventDialog_.Edit( gamingEvent );
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
        const QString dummyMission = "Create Dummy Mission";
        QMenu createMenu;
        createMenu.setTitle( tr( "Create an event" ) );
        for( int i = 0; i < eNbrEventTypes; ++i )
            createMenu.addAction( QString::fromStdString( ENT_Tr::ConvertFromEventType( static_cast< E_EventTypes >( i ) ) ) );
        menu.addMenu( &createMenu );
        menu.addAction( dummyMission );
        if( QAction* resultingAction = menu.exec( QCursor::pos() ) )
        {
            if( resultingAction->text() == dummyMission )
                CreateDummyMission();
            else
            {
                //E_EventTypes type = ENT_Tr::ConvertToEventType( resultingAction->text() );
                //eventDialog_.Create( type );
            }
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
void TimelineFilteredViewWidget::CreateDummyMission()
{
    // Get a time
    QDateTime dateTime = simulation_.GetDateTime();
    dateTime = dateTime.addSecs( 60 );

    // Create dummy protobuf message
    sword::ClientToSim msg;
    sword::UnitOrder* unitOrder = msg.mutable_message()->mutable_unit_order();

    unitOrder->mutable_tasker()->set_id( 79 );
    unitOrder->mutable_type()->set_id( 44582 ); // Move To
    unitOrder->set_label( "dummy mission" );
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

    unitOrder->mutable_start_time()->set_data( dateTime.toTimeSpec( Qt::UTC ).toString( "yyyyMMddTHHmmss" ) );

    // Action
    timeline::Action action;
    action.target = "sword://sim";
    action.apply = true;
    action.payload = tools::ProtoToBase64( msg );

    // Event
    timeline::Event event;
    //event.uuid; // $$$$ ABR 2013-05-24: Serv will generate it
    event.name = "DummyMission";
    event.info = "DummyMissionInfo";
    event.begin = dateTime.toTimeSpec( Qt::UTC ).toString( "yyyy-MM-ddTHH:mm:ssZ" ).toStdString();
    event.done = false;
    event.action = action;

    CreateEvent( event );
}
