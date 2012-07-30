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
#include "protocol/SimulationSenders.h"
#include "tools/MessageController.h"
#include "tools/MessageObserver.h"

#ifdef _MSC_VER
#pragma warning( disable : 4244 )
#endif

#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/format.hpp>
#include <boost/function.hpp>
#include <boost/make_shared.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/shared_mutex.hpp>

using namespace plugins::web_control;

// -----------------------------------------------------------------------------
// Name: StateObserver structure
// Created: BAX 2012-02-28
// -----------------------------------------------------------------------------
struct plugins::web_control::StateObserver
{
    typedef boost::function< void( sword::EnumSimulationState ) > Update;
    StateObserver( const Update& update ) : update_( update )
    {
        // NOTHING
    }
    protected:
    Update update_;
};

namespace
{
// -----------------------------------------------------------------------------
// Name: ControlInformationUpdate structure
// Created: BAX 2012-02-28
// -----------------------------------------------------------------------------
struct ControlInformationUpdate : public StateObserver, private tools::MessageObserver< sword::ControlInformation >
{
    ControlInformationUpdate( tools::MessageController< sword::SimToClient_Content >& controller, const Update& update ) : StateObserver( update )
    {
        CONNECT( controller, *this, control_information );
    }
    void Notify( const sword::ControlInformation& message, int /*context*/ )
    {
        if( message.has_status() )
            update_( message.status() );
    }
};

// -----------------------------------------------------------------------------
// Name: ControlAckUpdate structure
// Created: BAX 2012-02-28
// -----------------------------------------------------------------------------
template< typename T, sword::EnumSimulationState Next >
struct ControlAckUpdate : public StateObserver, public tools::MessageObserver< T >
{
    ControlAckUpdate( const Update& update ) : StateObserver( update )
    {
        // NOTHING
    }
    void Notify( const T& message, int /*context*/ )
    {
        if( message.has_error_code() )
            if( message.error_code() == sword::ControlAck_ErrorCode_no_error )
                update_( Next );
    }
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
    StateObserver::Update callback = boost::bind( &WebControl::UpdateState, this, _1 );
    boost::shared_ptr< ControlInformationUpdate > update = boost::make_shared< ControlInformationUpdate >( boost::ref( *controller_ ), callback );
    observers_.push_back( update );

    boost::shared_ptr< ControlPause > pause = boost::make_shared< ControlPause >( callback );
    CONNECT( *controller_, *pause, control_pause_ack );
    observers_.push_back( pause );

    boost::shared_ptr< ControlResume > resume = boost::make_shared< ControlResume >( callback );
    CONNECT( *controller_, *resume, control_resume_ack );
    observers_.push_back( resume );

    boost::shared_ptr< ControlStop > stop = boost::make_shared< ControlStop >( callback );
    CONNECT( *controller_, *stop, control_stop_ack );
    observers_.push_back( stop );
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
// Name: WebControl::UpdateState
// Created: BAX 2012-02-28
// -----------------------------------------------------------------------------
void WebControl::UpdateState( sword::EnumSimulationState state )
{
    boost::lock_guard< boost::shared_mutex > lock( *access_ );
    state_ = state;
}

// -----------------------------------------------------------------------------
// Name: WebControl::Receive
// Created: BAX 2012-02-28
// -----------------------------------------------------------------------------
void WebControl::Receive( const sword::SimToClient& client )
{
    controller_->Dispatch( client.message(), client.has_context() ? client.context() : -1 );
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

BOOST_STATIC_ASSERT( ( sizeof httpCodes / sizeof *httpCodes ) == HttpStatusCode_Count );

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

// -----------------------------------------------------------------------------
// Name: WebControl::Get
// Created: BAX 2012-02-28
// -----------------------------------------------------------------------------
std::string WebControl::Get()
{
    boost::shared_lock< boost::shared_mutex > lock( *access_ );
    const sword::EnumSimulationState state = state_;
    lock.unlock();

    std::string reply = ( boost::format(
        "{\"state\":\"%1%\"}" )
        % convert( state )
    ).str();
    return WriteHttpReply( httpCodes[ Ok ], reply );
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

#define COUNT_OF( array ) ( sizeof (array) / sizeof *(array) )

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
