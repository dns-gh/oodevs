// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "WebControl.h"
#include "dispatcher/SimulationPublisher_ABC.h"
#include "tools/MessageController.h"
#include "tools/MessageObserver.h"
#include <tools/Helpers.h>

#ifdef _MSC_VER
#pragma warning( disable : 4127 4244 )
#endif

#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/format.hpp>
#include <boost/make_shared.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/shared_mutex.hpp>

#ifdef _MSC_VER
#   pragma warning( push )
#   pragma warning( disable : 4100 4512 )
#endif
#include <boost/property_tree/json_parser.hpp>
#ifdef _MSC_VER
#pragma warning( pop )
#endif

using namespace plugins::web_control;
namespace bpt = boost::property_tree;

namespace
{
template< typename T >
struct MessageObserver : public Observer, public tools::MessageObserver< T >
{
    typedef typename std::function< void( const T& ) > Update;
    MessageObserver( const Update& update )
        : update_( update )
    {
        // NOTHING
    }
    virtual ~MessageObserver()
    {
        // NOTHING
    }
    void Notify( const T& message, int /* context */ )
    {
        update_( message );
    }
private:
    const Update update_;
};

// -----------------------------------------------------------------------------
// Name: ControlAckUpdate structure
// Created: BAX 2012-02-28
// -----------------------------------------------------------------------------
template< typename T, sword::EnumSimulationState Next >
struct ControlAckUpdate : public Observer, public tools::MessageObserver< T >
{
    typedef std::function< void( sword::EnumSimulationState ) > Update;
    ControlAckUpdate( const Update& update )
        : update_( update )
    {
        // NOTHING
    }
    void Notify( const T& message, int /*context*/ )
    {
        if( message.has_error_code() )
            if( message.error_code() == sword::ControlAck_ErrorCode_no_error )
                update_( Next );
    }
private:
    const Update update_;
};
}

typedef ControlAckUpdate< sword::ControlPauseAck,  sword::paused  > ControlPause;
typedef ControlAckUpdate< sword::ControlResumeAck, sword::running > ControlResume;
typedef ControlAckUpdate< sword::ControlStopAck,   sword::stopped > ControlStop;

// -----------------------------------------------------------------------------
// Name: WebControl::WebControl
// Created: BAX 2012-02-28
// -----------------------------------------------------------------------------
WebControl::WebControl( dispatcher::SimulationPublisher_ABC& publisher )
    : publisher_ ( publisher )
    , state_     ( sword::stopped )
    , access_    ( boost::make_shared< boost::shared_mutex >() )
    , controller_( boost::make_shared< tools::MessageController< sword::SimToClient_Content > >() )
{
#define REGISTER_OBSERVER( TYPE, NAME, FUNCTOR ) do {\
    const TYPE ##::Update update = boost::bind( &WebControl::## FUNCTOR, this, _1 );\
    boost::shared_ptr< TYPE > ptr = boost::make_shared< TYPE >( update );\
    CONNECT( *controller_, *ptr, NAME );\
    observers_.push_back( ptr );\
} while( 0 )
    REGISTER_OBSERVER( MessageObserver< sword::ControlInformation >, control_information, OnControlInformation );
    REGISTER_OBSERVER( MessageObserver< sword::ControlBeginTick >,   control_begin_tick,  OnControlBeginTick );
    REGISTER_OBSERVER( ControlPause,                                 control_pause_ack,   OnSimulationState );
    REGISTER_OBSERVER( ControlResume,                                control_resume_ack,  OnSimulationState );
    REGISTER_OBSERVER( ControlStop,                                  control_stop_ack,    OnSimulationState );
#undef REGISTER_OBSERVER
}

// -----------------------------------------------------------------------------
// Name: WebControl::~WebControl
// Created: BAX 2012-02-28
// -----------------------------------------------------------------------------
WebControl::~WebControl()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: WebControl::OnSimulationState
// Created: BAX 2012-02-28
// -----------------------------------------------------------------------------
void WebControl::OnSimulationState( sword::EnumSimulationState state )
{
    boost::lock_guard< boost::shared_mutex > lock( *access_ );
    state_ = state;
}

// -----------------------------------------------------------------------------
// Name: WebControl::OnControlInformation
// Created: BAX 2012-07-30
// -----------------------------------------------------------------------------
void WebControl::OnControlInformation( const sword::ControlInformation& control )
{
    boost::lock_guard< boost::shared_mutex > lock( *access_ );
    state_ = control.status();
    start_time_ = control.initial_date_time().data();
    current_time_ = control.date_time().data();
}

// -----------------------------------------------------------------------------
// Name: WebControl::OnControlBeginTick
// Created: BAX 2012-07-31
// -----------------------------------------------------------------------------
void WebControl::OnControlBeginTick( const sword::ControlBeginTick& msg )
{
    boost::lock_guard< boost::shared_mutex > lock( *access_ );
    current_time_ = msg.date_time().data();
}

// -----------------------------------------------------------------------------
// Name: WebControl::Receive
// Created: BAX 2012-02-28
// -----------------------------------------------------------------------------
void WebControl::Receive( const sword::SimToClient& client )
{
    controller_->Dispatch( client.message(), client.has_context() ? client.context() : -1 );
}

// -----------------------------------------------------------------------------
// Name: WebControl::NotifyClientAuthenticated
// Created: BAX 2012-07-30
// -----------------------------------------------------------------------------
void WebControl::NotifyClientAuthenticated( const std::string& link )
{
    boost::lock_guard< boost::shared_mutex > lock( *access_ );
    clients_.insert( link );
}

// -----------------------------------------------------------------------------
// Name: WebControl::NotifyClientLeft
// Created: BAX 2012-07-30
// -----------------------------------------------------------------------------
void WebControl::NotifyClientLeft( const std::string& link )
{
    boost::lock_guard< boost::shared_mutex > lock( *access_ );
    clients_.erase( link );
}

namespace
{
// -----------------------------------------------------------------------------
// Name: HttpCode structure
// Created: BAX 2012-02-28
// -----------------------------------------------------------------------------
struct HttpCode
{
    int code;
    const char* text;
};
static const HttpCode httpCodes[] =
{
    { 200, "OK" },
    { 400, "Bad Request" },
    { 401, "Unauthorized" },
    { 403, "Forbidden" },
    { 404, "Not Found" },
};

// -----------------------------------------------------------------------------
// Name: HttpStatusCode enumeration
// Created: BAX 2012-02-28
// -----------------------------------------------------------------------------
typedef enum HttpStatusCode
{
    Ok,
    BadRequest,
    Unauthorized,
    Forbidden,
    NotFound,
    HttpStatusCode_Count,
};

static_assert( ( sizeof httpCodes / sizeof *httpCodes ) == HttpStatusCode_Count, "missing http codes" );

// -----------------------------------------------------------------------------
// Name: WriteHttpReply
// Created: BAX 2012-02-28
// -----------------------------------------------------------------------------
std::string WriteHttpReply( const HttpCode& status, const std::string& content = std::string() )
{
    return ( boost::format(
        "HTTP/1.1 %1% %2%\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: %3%\r\n"
        "Connection: close\r\n"
        "\r\n"
        "%4%" )
        % status.code
        % status.text
        % content.size()
        % content
    ).str();
}

// -----------------------------------------------------------------------------
// Name: SendMessage
// Created: BAX 2012-02-28
// -----------------------------------------------------------------------------
template< typename T >
std::string SendMessage( dispatcher::SimulationPublisher_ABC& publisher )
{
    T message;
    message.Send( publisher, 0 );
    return WriteHttpReply( httpCodes[ Ok ] );
}

// -----------------------------------------------------------------------------
// Name: convert
// Created: BAX 2012-02-28
// -----------------------------------------------------------------------------
const char* convert( sword::EnumSimulationState state )
{
    switch( state )
    {
    case sword::running:   return "running";
    case sword::paused:    return "paused";
    case sword::stopped:   return "stopped";
    case sword::loading:   return "loading";
    default:               return "unknown";
    }
}
}

// -----------------------------------------------------------------------------
// Name: WebControl::Pause
// Created: BAX 2012-02-28
// -----------------------------------------------------------------------------
std::string WebControl::Pause()
{
    return SendMessage< simulation::ControlPause >( publisher_ );
}

// -----------------------------------------------------------------------------
// Name: WebControl::Play
// Created: BAX 2012-02-28
// -----------------------------------------------------------------------------
std::string WebControl::Play()
{
    return SendMessage< simulation::ControlResume >( publisher_ );
}

// -----------------------------------------------------------------------------
// Name: WebControl::Stop
// Created: BAX 2012-02-28
// -----------------------------------------------------------------------------
std::string WebControl::Stop()
{
    return SendMessage< simulation::ControlStop >( publisher_ );
}

namespace
{
std::string ToJson( const bpt::ptree& tree )
{
    std::ostringstream out;
    try
    {
        bpt::write_json( out, tree, false );
    }
    catch( ... )
    {
        // NOTHING
    }
    return out.str();
}
}

// -----------------------------------------------------------------------------
// Name: WebControl::Get
// Created: BAX 2012-02-28
// -----------------------------------------------------------------------------
std::string WebControl::Get()
{
    boost::shared_lock< boost::shared_mutex > lock( *access_ );
    const sword::EnumSimulationState state = state_;
    const std::string start = start_time_;
    const std::string current = current_time_;
    lock.unlock();

    bpt::ptree rpy;
    rpy.put( "state", convert( state ) );
    rpy.put( "start_time", start );
    rpy.put( "current_time", current );
    bpt::ptree& clients = rpy.put_child( "clients", bpt::ptree() );
    BOOST_FOREACH( const T_Clients::value_type& client, clients_ )
        clients.push_back( std::make_pair( "", client ) );

    return WriteHttpReply( httpCodes[ Ok ], ToJson( rpy ) );
}

namespace
{
// -----------------------------------------------------------------------------
// Name: UriHandler structure
// Created: BAX 2012-02-28
// -----------------------------------------------------------------------------
struct UriHandler
{
    typedef std::string (WebControl::*Handler)();
    const char* uri;
    Handler handler;
    bool operator==( const std::string& rhs ) const
    {
        return rhs == uri;
    }
};
}

// -----------------------------------------------------------------------------
// Name: WebControl::Notify
// Created: BAX 2012-02-28
// -----------------------------------------------------------------------------
std::string WebControl::Notify( const std::string& method, const std::string& uri )
{
    if( method != "GET" )
        return WriteHttpReply( httpCodes[ BadRequest ], "Invalid method type" );

    static const UriHandler handlers[] =
    {
        { "/get",   &WebControl::Get },
        { "/pause", &WebControl::Pause },
        { "/play",  &WebControl::Play },
        { "/stop",  &WebControl::Stop },
    };
    const UriHandler* end = handlers + COUNT_OF( handlers );
    const UriHandler* ptr = std::find( handlers, end, uri );
    if( ptr == end )
        return WriteHttpReply( httpCodes[ NotFound ], "Unknown URI" );

    return ( this->*( ptr->handler ) )();
}
