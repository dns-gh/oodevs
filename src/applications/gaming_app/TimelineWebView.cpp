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
#include "clients_kernel/ContextMenu.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Profile_ABC.h"
#include "ENT/ENT_Tr.h"
#include "gaming/AgentsModel.h"
#include "gaming/EventAction.h"
#include "gaming/EventsModel.h"
#include "gaming/Model.h"
#include "gaming/TimelinePublisher.h"
#include "MT_Tools/MT_Logger.h"
#include "protocol/Protocol.h"
#include "tools/ExerciseConfig.h"
#include "tools/Language.h"
#include "tools/Loader_ABC.h"
#include "tools/SchemaWriter.h"

#include <timeline/api.h>
#include <tools/StdFileWrapper.h>

#include <boost/algorithm/string/join.hpp>
#include <boost/assign.hpp>
#include <boost/bind.hpp>

// -----------------------------------------------------------------------------
// Name: TimelineWebView constructor
// Created: ABR 2013-05-28
// -----------------------------------------------------------------------------
TimelineWebView::TimelineWebView( QWidget* parent,
                                  const tools::ExerciseConfig& config,
                                  kernel::Controllers& controllers,
                                  Model& model,
                                  timeline::Configuration& cfg )
    : QWidget( parent )
    , config_( config )
    , controllers_( controllers )
    , model_( model )
    , server_( 0 )
    , cfg_( new timeline::Configuration( cfg ) )
    , creationSignalMapper_( 0 )
    , horizontal_( false )
{
    setObjectName( "timeline-webview" );

    mainLayout_ = new QVBoxLayout( this );
    mainLayout_->setMargin( 0 );
    mainLayout_->setSpacing( 0 );

    controllers_.actions_.Register( *this );
    controllers_.controller_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: TimelineWebView destructor
// Created: ABR 2013-05-28
// -----------------------------------------------------------------------------
TimelineWebView::~TimelineWebView()
{
    controllers_.actions_.Unregister( *this );
    controllers_.controller_.Unregister( *this );
}

namespace
{
    // find a lib which does that
    std::string UrlEncode( const std::string& value )
    {
        std::ostringstream escaped;
        escaped.fill( '0' );
        escaped << std::hex;
        for( auto it = value.begin(); it != value.end(); ++it )
        {
            const auto& c = *it;
            if( std::isalnum( c ) || c == '-' || c == '_' || c == '.' || c == '~')
                escaped << c;
            else if( c == ' ' )
                escaped << '+';
            else
                escaped << '%' << std::setw(2) << static_cast< int >( c ) << std::setw(0);
        }
        return escaped.str();
    }

    std::string MakeQuery( const std::map< std::string, std::string >& parameters )
    {
        std::vector< std::string > tokens;
        for( auto it = parameters.begin(); it != parameters.end(); ++it )
            if( !it->second.empty() )
                tokens.push_back( it->first + "=" + UrlEncode( it->second ) );
        return "?" + boost::algorithm::join( tokens, "&" );
    }
}

// -----------------------------------------------------------------------------
// Name: TimelineWebView::Connect
// Created: ABR 2013-05-15
// -----------------------------------------------------------------------------
void TimelineWebView::Connect()
{
    timelineWidget_.reset( new QWidget() );
    mainLayout_->addWidget( timelineWidget_.get() );
    timeline::Configuration next = *cfg_;
    next.widget = timelineWidget_.get();
    horizontal_ = false;
    auto query = boost::assign::map_list_of
        ( "lang",                 tools::Language::Current() )
        ( "sword_filter",         "" )
        ( "sword_profile",        lastProfile_ )
        ( "sword_filter_engaged", "true" )
        ( "filter_service",       "sword:true,none:true" )
        ( "horizontal",           "false" );
    next.url += MakeQuery( query );
    server_.reset( MakeServer( next ).release() );

    connect( this, SIGNAL( CreateEventSignal( const timeline::Event& ) ), server_.get(), SLOT( CreateEvent( const timeline::Event& ) ) );
    connect( this, SIGNAL( SelectEventSignal( const std::string& ) ), server_.get(), SLOT( SelectEvent( const std::string& ) ) );
    connect( this, SIGNAL( EditEventSignal( const timeline::Event& ) ), server_.get(), SLOT( UpdateEvent( const timeline::Event& ) ) );
    connect( this, SIGNAL( DeleteEventSignal( const std::string& ) ), server_.get(), SLOT( DeleteEvent( const std::string& ) ) );

    connect( server_.get(), SIGNAL( CreatedEvent( const timeline::Event&, const timeline::Error& ) ), this, SLOT( OnCreatedEvent( const timeline::Event&, const timeline::Error& ) ) );
    connect( server_.get(), SIGNAL( UpdatedEvent( const timeline::Event&, const timeline::Error& ) ), this, SLOT( OnEditedEvent( const timeline::Event&, const timeline::Error& ) ) );
    connect( server_.get(), SIGNAL( DeletedEvent( const std::string&, const timeline::Error& ) ), this, SLOT( OnDeletedEvent( const std::string&, const timeline::Error& ) ) );
    connect( server_.get(), SIGNAL( GetEvents( const timeline::Events&, const timeline::Error& ) ), this, SLOT( OnGetEvents( const timeline::Events&, const timeline::Error& ) ) );
    connect( server_.get(), SIGNAL( LoadedEvents( const timeline::Error& ) ), this, SLOT( OnLoadedEvents( const timeline::Error& ) ) );
    connect( server_.get(), SIGNAL( SavedEvents( const std::string&, const timeline::Error& ) ), this, SLOT( OnSavedEvents( const std::string&, const timeline::Error& ) ) );

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
    if( !server_ )
        return;

    disconnect( this, SIGNAL( CreateEventSignal( const timeline::Event& ) ), server_.get(), SLOT( CreateEvent( const timeline::Event& ) ) );
    disconnect( this, SIGNAL( SelectEventSignal( const std::string& ) ), server_.get(), SLOT( SelectEvent( const std::string& ) ) );
    disconnect( this, SIGNAL( EditEventSignal( const timeline::Event& ) ), server_.get(), SLOT( UpdateEvent( const timeline::Event& ) ) );
    disconnect( this, SIGNAL( DeleteEventSignal( const std::string& ) ), server_.get(), SLOT( DeleteEvent( const std::string& ) ) );

    disconnect( server_.get(), SIGNAL( CreatedEvent( const timeline::Event&, const timeline::Error& ) ), this, SLOT( OnCreatedEvent( const timeline::Event&, const timeline::Error& ) ) );
    disconnect( server_.get(), SIGNAL( UpdatedEvent( const timeline::Event&, const timeline::Error& ) ), this, SLOT( OnEditedEvent( const timeline::Event&, const timeline::Error& ) ) );
    disconnect( server_.get(), SIGNAL( DeletedEvent( const std::string&, const timeline::Error& ) ), this, SLOT( OnDeletedEvent( const std::string&, const timeline::Error& ) ) );
    disconnect( server_.get(), SIGNAL( GetEvents( const timeline::Events&, const timeline::Error& ) ), this, SLOT( OnGetEvents( const timeline::Events&, const timeline::Error& ) ) );
    disconnect( server_.get(), SIGNAL( LoadedEvents( const timeline::Error& ) ), this, SLOT( OnLoadedEvents( const timeline::Error& ) ) );
    disconnect( server_.get(), SIGNAL( SavedEvents( const std::string&, const timeline::Error& ) ), this, SLOT( OnSavedEvents( const std::string&, const timeline::Error& ) ) );

    disconnect( server_.get(), SIGNAL( SelectedEvent( boost::shared_ptr< timeline::Event > ) ), this, SLOT( OnSelectedEvent( boost::shared_ptr< timeline::Event > ) ) );
    disconnect( server_.get(), SIGNAL( ActivatedEvent( const timeline::Event& ) ), this, SLOT( OnActivatedEvent( const timeline::Event& ) ) );
    disconnect( server_.get(), SIGNAL( ContextMenuEvent( boost::shared_ptr< timeline::Event >, const std::string& ) ), this, SLOT( OnContextMenuEvent( boost::shared_ptr< timeline::Event >, const std::string& ) ) );
    disconnect( server_.get(), SIGNAL( KeyUp( int ) ), this, SLOT( OnKeyUp( int ) ) );

    server_.reset();
    mainLayout_->removeWidget( timelineWidget_.get() );
    timelineWidget_.reset();
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
// Name: TimelineWebView::OnCenterView
// Created: BAX 2013-08-26
// -----------------------------------------------------------------------------
void TimelineWebView::OnCenterView()
{
    server_->Center();
}

// -----------------------------------------------------------------------------
// Name: TimelineWebView::CreateEvent
// Created: ABR 2013-05-17
// -----------------------------------------------------------------------------
void TimelineWebView::CreateEvent( const timeline::Event& event )
{
    eventCreated_ = event.uuid;
    emit CreateEventSignal( event );
}

// -----------------------------------------------------------------------------
// Name: TimelineWebView::SelectEvent
// Created: LGY 2013-11-12
// -----------------------------------------------------------------------------
void TimelineWebView::SelectEvent( const std::string& uuid )
{
    emit SelectEventSignal( uuid );
}

// -----------------------------------------------------------------------------
// Name: TimelineWebView::EditEvent
// Created: ABR 2013-05-31
// -----------------------------------------------------------------------------
void TimelineWebView::EditEvent( const timeline::Event& event )
{
    emit EditEventSignal( event );
}

// -----------------------------------------------------------------------------
// Name: TimelineWebView::DeleteEvent
// Created: ABR 2013-05-17
// -----------------------------------------------------------------------------
void TimelineWebView::DeleteEvent( const std::string& uuid )
{
    controllers_.eventActions_.DeselectAll();
    emit DeleteEventSignal( uuid );
}

// -----------------------------------------------------------------------------
// Name: TimelineWebView::OnCreatedEvent
// Created: ABR 2013-05-17
// -----------------------------------------------------------------------------
void TimelineWebView::OnCreatedEvent( const timeline::Event& event, const timeline::Error& error )
{
    if( error.code != timeline::EC_OK )
        MT_LOG_ERROR_MSG( tr( "An error occurred during event creation process: %1" ).arg( QString::fromStdString( error.text ) ).toStdString() );
    model_.events_.Create( event );
    if( !eventCreated_.empty() && event.uuid == eventCreated_ )
    {
        emit SelectEventSignal( event.uuid );
        eventCreated_ = "";
    }
}

// -----------------------------------------------------------------------------
// Name: TimelineWebView::OnEditedEvent
// Created: ABR 2013-05-31
// -----------------------------------------------------------------------------
void TimelineWebView::OnEditedEvent( const timeline::Event& event, const timeline::Error& error )
{
    if( error.code != timeline::EC_OK )
        MT_LOG_ERROR_MSG( tr( "An error occurred during event creation process: %1" ).arg( QString::fromStdString( error.text ) ).toStdString() );
    model_.events_.Update( event );
}

// -----------------------------------------------------------------------------
// Name: TimelineWebView::OnDeletedEvent
// Created: ABR 2013-05-17
// -----------------------------------------------------------------------------
void TimelineWebView::OnDeletedEvent( const std::string& uuid, const timeline::Error& error )
{
    if( error.code != timeline::EC_OK )
        MT_LOG_ERROR_MSG( tr( "An error occurred during event deletion process: %1" ).arg( QString::fromStdString( error.text ) ).toStdString() );
    model_.events_.Destroy( uuid );
}

// -----------------------------------------------------------------------------
// Name: TimelineWebView::OnSelectedEvent
// Created: ABR 2013-05-17
// -----------------------------------------------------------------------------
void TimelineWebView::OnSelectedEvent( boost::shared_ptr< timeline::Event > event )
{
    bool hadSelection = selected_;
    selected_ = event;
    if( selected_ )
    {
        Event& gamingEvent = GetOrCreateEvent( *selected_ );
        gamingEvent.Select( controllers_.eventActions_, controllers_.actions_ );
    }
    else if( hadSelection )
    {
        controllers_.actions_.DeselectAll();
        controllers_.eventActions_.DeselectAll();
    }
}

// -----------------------------------------------------------------------------
// Name: TimelineWebView::OnActivatedEvent
// Created: ABR 2013-05-24
// -----------------------------------------------------------------------------
void TimelineWebView::OnActivatedEvent( const timeline::Event& event )
{
    Event& gamingEvent = GetOrCreateEvent( event );
    gamingEvent.Activate( controllers_.eventActions_ );
}

// -----------------------------------------------------------------------------
// Name: TimelineWebView::OnContextMenuEvent
// Created: ABR 2013-05-24
// -----------------------------------------------------------------------------
void TimelineWebView::OnContextMenuEvent( boost::shared_ptr< timeline::Event > event, const std::string& time )
{
    selectedDateTime_ = QDateTime::fromString( QString::fromStdString( time ), Qt::ISODate );
    if( event )
    {
        Event& gamingEvent = GetOrCreateEvent( *event );
        gamingEvent.ContextMenu( controllers_.eventActions_, QCursor::pos() );
    }
    else
        controllers_.actions_.ContextMenu( selectedDateTime_, QCursor::pos() );
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
    creationSignalMapper_.reset( new QSignalMapper( this ) );
    connect( creationSignalMapper_.get(), SIGNAL( mapped( int ) ), this, SLOT( OnCreateClicked( int ) ) );

    kernel::ContextMenu* createMenu = new kernel::ContextMenu( &menu );
    for( int i = 0; i < eNbrEventTypes; ++i )
        if( i == eEventTypes_Order || i == eEventTypes_Task )
            AddToMenu( *createMenu, creationSignalMapper_.get(), QString::fromStdString( ENT_Tr::ConvertFromEventType( static_cast< E_EventTypes >( i ) ) ), i );

    menu.InsertItem( "Command", tr( "Create an event" ), createMenu );
}

// -----------------------------------------------------------------------------
// Name: TimelineWebView::SetProfile
// Created: BAX 2013-11-14
// -----------------------------------------------------------------------------
void TimelineWebView::SetProfile( const QString& profile )
{
    lastProfile_ = profile;
    if( !server_ || serverProfile_ == lastProfile_ )
        return;
    server_->UpdateQuery( boost::assign::map_list_of( "sword_profile", profile.toStdString() ) );
    serverProfile_ = lastProfile_;
}

// -----------------------------------------------------------------------------
// Name: TimelineWebView::UpdateFilters
// Created: LGY 2013-11-19
// -----------------------------------------------------------------------------
void TimelineWebView::UpdateFilters( const std::string& unitFilter,
                                     bool displayEngaged,
                                     const std::string& services,
                                     const std::string& keyword )
{
    if( server_ )
        server_->UpdateQuery( boost::assign::map_list_of
            ( "sword_filter", unitFilter )
            ( "sword_filter_engaged", displayEngaged ? "false" : "true" )
            ( "filter_keyword", keyword )
            ( "filter_service", services ) );
}

// -----------------------------------------------------------------------------
// Name: TimelineWebView::NotifyCreated
// Created: BAX 2013-11-14
// -----------------------------------------------------------------------------
void TimelineWebView::NotifyCreated( const kernel::Profile_ABC& profile )
{
    NotifyUpdated( profile );
}

// -----------------------------------------------------------------------------
// Name: TimelineWebView::NotifyUpdated
// Created: BAX 2013-11-14
// -----------------------------------------------------------------------------
void TimelineWebView::NotifyUpdated( const kernel::Profile_ABC& profile )
{
    SetProfile( profile.GetLogin() );
}

// -----------------------------------------------------------------------------
// Name: TimelineWebView::OnCreateClicked
// Created: ABR 2013-06-19
// -----------------------------------------------------------------------------
void TimelineWebView::OnCreateClicked( int type )
{
    // Deselect current event
    if( selected_ )
        emit SelectEventSignal( "" );

    assert( type >= 0 && type < eNbrEventTypes );
    emit StartCreation( static_cast< E_EventTypes >( type ), selectedDateTime_, true );
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
void TimelineWebView::ReadActions( xml::xisubstream xis )
{
    xis >> xml::start( "actions" )
            >> xml::list( "action", *this, &TimelineWebView::ReadAction );
}

// -----------------------------------------------------------------------------
// Name: TimelineWebView::ReadAction
// Created: ABR 2013-06-19
// -----------------------------------------------------------------------------
void TimelineWebView::ReadAction( xml::xistream& xis )
{
    boost::scoped_ptr< actions::Action_ABC > action;
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

    if( !action )
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
    if( !server_  )
        return;
    currentFile_ = filename;
    server_->ReadEvents();
}

// -----------------------------------------------------------------------------
// Name: TimelineWebView::OnLoadTimelineSessionFileRequested
// Created: ABR 2013-07-03
// -----------------------------------------------------------------------------
void TimelineWebView::OnLoadTimelineSessionFileRequested( const tools::Path& filename )
{
    if( !server_ )
        return;
    tools::Ifstream is( filename );
    if( !is.is_open() )
        return;
    std::string content;
    while( is.good() )
    {
        std::string line;
        std::getline( is, line );
        content += line;
    }
    is.close();
    if( !content.empty() )
        server_->LoadEvents( content );
}

// -----------------------------------------------------------------------------
// Name: TimelineWebView::OnSaveTimelineSessionFileRequested
// Created: ABR 2013-07-03
// -----------------------------------------------------------------------------
void TimelineWebView::OnSaveTimelineSessionFileRequested( const tools::Path& filename )
{
    if( !server_.get() )
        return;
    currentFile_ = filename;
    server_->SaveEvents();
}

// -----------------------------------------------------------------------------
// Name: TimelineWebView::OnGetEvents
// Created: ABR 2013-06-19
// -----------------------------------------------------------------------------
void TimelineWebView::OnGetEvents( const timeline::Events& events, const timeline::Error& error )
{
    if( currentFile_.IsEmpty() || currentFile_.Extension() != ".ord" )
        return;

    if( error.code != timeline::EC_OK )
    {
        MT_LOG_ERROR_MSG( tr( "An error occurred during 'get all events' request: %1" ).arg( QString::fromStdString( error.text ) ).toStdString() );
        return;
    }

    tools::Xofstream xos( currentFile_ );
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
                xos << xml::attribute( "time", event.GetEvent().begin );
                xos << xml::end; //! action
            }
    }
    xos << xml::end; //! actions

    currentFile_.Clear();
}

// -----------------------------------------------------------------------------
// Name: TimelineWebView::OnLoadedEvents
// Created: ABR 2013-07-03
// -----------------------------------------------------------------------------
void TimelineWebView::OnLoadedEvents( const timeline::Error& error )
{
    if( error.code != timeline::EC_OK )
    {
        MT_LOG_ERROR_MSG( tr( "An error occurred during 'LoadEvents' request: %1" ).arg( QString::fromStdString( error.text ) ).toStdString() );
        return;
    }
}

// -----------------------------------------------------------------------------
// Name: TimelineWebView::OnSavedEvents
// Created: ABR 2013-07-03
// -----------------------------------------------------------------------------
void TimelineWebView::OnSavedEvents( const std::string& content, const timeline::Error& error )
{
    if( currentFile_.IsEmpty() || currentFile_.Extension() != ".timeline" || content.empty() )
        return;

    if( error.code != timeline::EC_OK )
    {
        MT_LOG_ERROR_MSG( tr( "An error occurred during 'SaveEvents' request: %1" ).arg( QString::fromStdString( error.text ) ).toStdString() );
        return;
    }

    tools::Ofstream os( currentFile_ );
    if( !os.is_open() )
        return;
    os << content;
    os.close();
}

// -----------------------------------------------------------------------------
// Name: TimelineWebView::OnToggleLayoutOrientation
// Created: ABR 2013-10-25
// -----------------------------------------------------------------------------
void TimelineWebView::OnToggleLayoutOrientation()
{
    if( server_ )
    {
        horizontal_ = !horizontal_;
        server_->UpdateQuery( boost::assign::map_list_of
            ( "horizontal", horizontal_ ? "true" : "false" ) );
    }
}

// -----------------------------------------------------------------------------
// Name: TimelineWebView::OnEngagedFilterToggled
// Created: SLI 2013-11-20
// -----------------------------------------------------------------------------
void TimelineWebView::OnEngagedFilterToggled( bool displayEngaged )
{
    if( server_ )
        server_->UpdateQuery( boost::assign::map_list_of
            ( "sword_filter_engaged", displayEngaged ? "false" : "true" ) );
}

// -----------------------------------------------------------------------------
// Name: TimelineWebView::OnServicesFilterChanged
// Created: SLI 2013-11-21
// -----------------------------------------------------------------------------
void TimelineWebView::OnServicesFilterChanged( const std::string& services )
{
    if( server_ )
        server_->UpdateQuery( boost::assign::map_list_of
            ( "filter_service", services ) );
}

// -----------------------------------------------------------------------------
// Name: TimelineWebView::OnKeywordFilterChanged
// Created: BAX 2013-11-25
// -----------------------------------------------------------------------------
void TimelineWebView::OnKeywordFilterChanged( const std::string& keyword )
{
    if( server_ )
        server_->UpdateQuery( boost::assign::map_list_of
            ( "filter_keyword", keyword ) );
}