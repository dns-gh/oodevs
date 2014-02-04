// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************
#include "controls.h"

#include <tools/IpcDevice.h>

#ifdef _MSC_VER
#pragma warning( push, 0 )
#endif
#include "proto/controls.pb.h"
#ifdef _MSC_VER
#pragma warning( pop )
#endif

#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/interprocess/streams/bufferstream.hpp>
#include <boost/interprocess/streams/vectorstream.hpp>

using namespace timeline;
namespace tic = timeline::controls;
namespace bip = boost::interprocess;
using sdk::ClientCommand;
using sdk::ServerCommand;

namespace
{
    const controls::T_Logger none;

    typedef std::function< size_t( const char*, size_t ) > T_Writer;

    template< typename T >
    size_t PackWith( const T_Writer& write, const controls::T_Logger& log, const T& src )
    {
        if( log )
            log( src.ShortDebugString() );
        bip::basic_vectorstream< std::vector< char> > stream;
        src.SerializeToOstream( &stream );
        const auto& buf = stream.vector();
        if( buf.empty() )
            return 0;
        return write( &buf[0], buf.size() );
    }

    template< typename T >
    size_t Pack( tools::ipc::Device& device, const controls::T_Logger& log, const T& src )
    {
        return PackWith( [&]( const char* data, size_t size ) {
            return device.Write( data, size );
        }, log, src );
    }

    template< typename T >
    void Unpack( T& dst, const void* src, size_t size, const controls::T_Logger& log )
    {
        bip::ibufferstream stream( reinterpret_cast< const char* >( src ), size );
        dst.ParseFromIstream( &stream );
        if( log )
            log( dst.ShortDebugString() );
    }

    template< typename CMD, typename TYPE >
    size_t PackType( tools::ipc::Device& device, const controls::T_Logger& log, TYPE type )
    {
        CMD cmd;
        cmd.set_type( type );
        return Pack( device, log, cmd );
    }
}

size_t tic::TryResizeClient( tools::ipc::Device& device )
{
    // this command is called a LOTS of time, so let's avoid recreating
    // a new packet each time
    static const ClientCommand resize = []() -> ClientCommand {
        ClientCommand cmd;
        cmd.set_type( sdk::CLIENT_RESIZE );
        return cmd;
    }();
    return Pack( device, none, resize );
}

size_t tic::QuitClient( tools::ipc::Device& device, int millisecs )
{
    ClientCommand cmd;
    cmd.set_type( sdk::CLIENT_QUIT );
    return PackWith( [&]( const char* data, size_t size ){
        return device.TimedWrite( data, size, boost::posix_time::milliseconds( millisecs ) );
    }, controls::T_Logger(), cmd );
}

size_t tic::ReloadClient( tools::ipc::Device& device )
{
    return PackType< ClientCommand >( device, none, sdk::CLIENT_RELOAD );
}

size_t tic::LoadClient( tools::ipc::Device& device, const std::string& url )
{
    ClientCommand cmd;
    cmd.set_type( sdk::CLIENT_LOAD );
    cmd.set_url( url );
    return Pack( device, none, cmd );
}

size_t controls::UpdateQuery( tools::ipc::Device& device, const std::map< std::string, std::string >& parameters )
{
    ClientCommand cmd;
    cmd.set_type( sdk::CLIENT_QUERY_UPDATE );
    for( auto it = parameters.begin(); it != parameters.end(); ++it )
    {
        auto parameter = cmd.add_query();
        parameter->set_key( it->first );
        parameter->set_value( it->second );
    }
    return Pack( device, none, cmd );
}

size_t tic::CenterClient( tools::ipc::Device& device )
{
    return PackType< ClientCommand >( device, none, sdk::CLIENT_CENTER );
}

namespace
{
    void SetAction( sdk::Action& dst, const Action& action )
    {
        dst.set_target( action.target );
        dst.set_apply( action.apply );
        dst.set_payload( &action.payload[0], action.payload.size() );
    }

    void SetEvent( sdk::Event& dst, const Event& event )
    {
        dst.set_uuid( event.uuid );
        dst.set_name( event.name );
        dst.set_info( event.info );
        dst.set_begin( event.begin );
        dst.set_end( event.end );
        if( !event.action.target.empty() )
            SetAction( *dst.mutable_action(), event.action );
        dst.set_done( event.done );
        dst.set_read_only( event.read_only );
        dst.set_error_code( event.error_code );
        dst.set_error_text( event.error_text );
    }

    Action GetAction( const sdk::Action& src )
    {
        Action dst;
        dst.target = src.target();
        dst.apply = src.apply();
        std::copy( src.payload().begin(), src.payload().end(), std::back_inserter( dst.payload ) );
        return dst;
    }

    Event GetEvent( const sdk::Event& src )
    {
        Event dst;
        dst.uuid = src.uuid();
        dst.name = src.name();
        dst.info = src.info();
        dst.begin = src.begin();
        dst.end = src.end();
        if( src.has_action() )
            dst.action = GetAction( src.action() );
        dst.done = src.done();
        dst.read_only = src.read_only();
        dst.error_code = src.error_code();
        dst.error_text = src.error_text();
        return dst;
    }

    Events GetEvents( const google::protobuf::RepeatedPtrField< sdk::Event >& events )
    {
        Events rpy;
        for( auto it = events.begin(); it != events.end(); ++it )
            rpy.push_back( GetEvent( *it ) );
        return rpy;
    }

    std::map< std::string, std::string > GetQuery( const google::protobuf::RepeatedPtrField< sdk::QueryParameter >& parameters )
    {
        std::map< std::string, std::string > result;
        for( auto it = parameters.begin(); it != parameters.end(); ++it )
            result[ it->key() ] = it->value();
        return result;
    }

    void SetError( sdk::Error& dst, const Error& error )
    {
        dst.set_code( error.code );
        dst.set_text( error.text );
    }

    Error GetError( const sdk::Error& src )
    {
        return Error( src.code(), src.text() );
    }
}

size_t tic::CreateEvent( tools::ipc::Device& device, const Event& event )
{
    ClientCommand cmd;
    cmd.set_type( sdk::CLIENT_EVENT_CREATE );
    SetEvent( *cmd.mutable_event(), event );
    return Pack( device, none, cmd );
}

size_t tic::SelectEvent( tools::ipc::Device& device, const std::string& uuid )
{
    ClientCommand cmd;
    cmd.set_type( sdk::CLIENT_EVENT_SELECT );
    cmd.mutable_event()->set_uuid( uuid );
    return Pack( device, none, cmd );
}

size_t tic::ReadEvents( tools::ipc::Device& device )
{
    return PackType< ClientCommand >( device, none, sdk::CLIENT_EVENT_READ_ALL );
}

size_t tic::ReadEvent( tools::ipc::Device& device, const std::string& uuid )
{
    ClientCommand cmd;
    cmd.set_type( sdk::CLIENT_EVENT_READ_ONE );
    cmd.mutable_event()->set_uuid( uuid );
    return Pack( device, none, cmd );
}

size_t tic::UpdateEvent( tools::ipc::Device& device, const Event& event )
{
    ClientCommand cmd;
    cmd.set_type( sdk::CLIENT_EVENT_UPDATE );
    SetEvent( *cmd.mutable_event(), event );
    return Pack( device, none, cmd );
}

size_t tic::DeleteEvent( tools::ipc::Device& device, const std::string& uuid )
{
    ClientCommand cmd;
    cmd.set_type( sdk::CLIENT_EVENT_DELETE );
    cmd.mutable_event()->set_uuid( uuid );
    return Pack( device, none, cmd );
}

size_t tic::LoadEvents( tools::ipc::Device& device, const std::string& events )
{
    ClientCommand cmd;
    cmd.set_type( sdk::CLIENT_EVENTS_LOAD );
    cmd.set_data( events );
    return Pack( device, none, cmd );
}

size_t tic::SaveEvents( tools::ipc::Device& device )
{
    return PackType< ClientCommand >( device, none, sdk::CLIENT_EVENTS_SAVE );
}

void tic::ParseClient( ClientHandler_ABC& handler, const void* data, size_t size,
                       const T_Logger& log )
{
    ClientCommand cmd;
    Unpack( cmd, data, size, log );
    switch( cmd.type() )
    {
        case sdk::CLIENT_RESIZE:                return handler.OnResizeClient();
        case sdk::CLIENT_QUIT:                  return handler.OnQuitClient();
        case sdk::CLIENT_RELOAD:                return handler.OnReloadClient();
        case sdk::CLIENT_LOAD:                  return handler.OnLoadClient( cmd.url() );
        case sdk::CLIENT_QUERY_UPDATE:          return handler.OnUpdateQuery( GetQuery( cmd.query() ) );
        case sdk::CLIENT_CENTER:                return handler.OnCenterClient();
        case sdk::CLIENT_EVENT_CREATE:          return handler.OnCreateEvent( GetEvent( cmd.event() ) );
        case sdk::CLIENT_EVENT_SELECT:          return handler.OnSelectEvent( GetEvent( cmd.event() ).uuid );
        case sdk::CLIENT_EVENT_READ_ALL:        return handler.OnReadEvents();
        case sdk::CLIENT_EVENT_READ_ONE:        return handler.OnReadEvent( GetEvent( cmd.event() ).uuid );
        case sdk::CLIENT_EVENT_UPDATE:          return handler.OnUpdateEvent( GetEvent( cmd.event() ) );
        case sdk::CLIENT_EVENT_DELETE:          return handler.OnDeleteEvent( GetEvent( cmd.event() ).uuid );
        case sdk::CLIENT_EVENTS_LOAD:           return handler.OnLoadEvents( cmd.data() );
        case sdk::CLIENT_EVENTS_SAVE:           return handler.OnSaveEvents();
    }
}

size_t tic::ReadyServer( tools::ipc::Device& device, const T_Logger& log )
{
    return PackType< ServerCommand >( device, log, sdk::SERVER_READY );
}

size_t tic::CreatedEvent( tools::ipc::Device& device, const T_Logger& log, const Event& event, const Error& error )
{
    ServerCommand cmd;
    cmd.set_type( sdk::SERVER_EVENT_CREATED );
    SetEvent( *cmd.mutable_event(), event );
    SetError( *cmd.mutable_error(), error );
    return Pack( device, log, cmd );
}

size_t tic::ReadEvents( tools::ipc::Device& device, const T_Logger& log, const Events& events, const Error& error )
{
    ServerCommand cmd;
    cmd.set_type( sdk::SERVER_EVENT_READ_ALL );
    for( auto it = events.begin(); it != events.end(); ++it )
        SetEvent( *cmd.mutable_events()->Add(), *it );
    SetError( *cmd.mutable_error(), error );
    return Pack( device, log, cmd );
}

size_t tic::ReadEvent( tools::ipc::Device& device, const T_Logger& log, const Event& event, const Error& error )
{
    ServerCommand cmd;
    cmd.set_type( sdk::SERVER_EVENT_READ_ONE );
    SetEvent( *cmd.mutable_event(), event );
    SetError( *cmd.mutable_error(), error );
    return Pack( device, log, cmd );
}

size_t tic::UpdatedEvent( tools::ipc::Device& device, const T_Logger& log, const Event& event, const Error& error )
{
    ServerCommand cmd;
    cmd.set_type( sdk::SERVER_EVENT_UPDATED );
    SetEvent( *cmd.mutable_event(), event );
    SetError( *cmd.mutable_error(), error );
    return Pack( device, log, cmd );
}

size_t tic::DeletedEvent( tools::ipc::Device& device, const T_Logger& log, const std::string& uuid, const Error& error )
{
    ServerCommand cmd;
    cmd.set_type( sdk::SERVER_EVENT_DELETED );
    cmd.mutable_event()->set_uuid( uuid );
    SetError( *cmd.mutable_error(), error );
    return Pack( device, log, cmd );
}

size_t tic::LoadedEvents( tools::ipc::Device& device, const T_Logger& log, const Error& error )
{
    ServerCommand cmd;
    cmd.set_type( sdk::SERVER_EVENTS_LOADED );
    SetError( *cmd.mutable_error(), error );
    return Pack( device, log, cmd );
}

size_t tic::SavedEvents( tools::ipc::Device& device, const T_Logger& log, const std::string& events, const Error& error )
{
    ServerCommand cmd;
    cmd.set_type( sdk::SERVER_EVENTS_SAVED );
    cmd.set_data( events );
    SetError( *cmd.mutable_error(), error );
    return Pack( device, log, cmd );
}

size_t tic::SelectedEvent( tools::ipc::Device& device, const T_Logger& log, const Event& event )
{
    ServerCommand cmd;
    cmd.set_type( sdk::SERVER_EVENT_SELECTED );
    SetEvent( *cmd.mutable_event(), event );
    return Pack( device, log, cmd );
}

size_t tic::DeselectedEvent( tools::ipc::Device& device, const T_Logger& log )
{
    return PackType< ServerCommand >( device, log, sdk::SERVER_EVENT_DESELECTED );
}

size_t tic::ActivatedEvent( tools::ipc::Device& device, const T_Logger& log, const Event& event )
{
    ServerCommand cmd;
    cmd.set_type( sdk::SERVER_EVENT_ACTIVATED );
    SetEvent( *cmd.mutable_event(), event );
    return Pack( device, log, cmd );
}

size_t tic::ContextMenuEvent( tools::ipc::Device& device, const T_Logger& log, const Event& event )
{
    ServerCommand cmd;
    cmd.set_type( sdk::SERVER_EVENT_CONTEXTMENU );
    SetEvent( *cmd.mutable_event(), event );
    return Pack( device, log, cmd );
}

size_t tic::ContextMenuBackground( tools::ipc::Device& device, const T_Logger& log, const std::string& time )
{
    ServerCommand cmd;
    cmd.set_type( sdk::SERVER_EVENT_CONTEXTMENUBACKGROUND );
    cmd.set_time( time );
    return Pack( device, log, cmd );
}

size_t tic::KeyDown( tools::ipc::Device& device, const T_Logger& log, int key )
{
    ServerCommand cmd;
    cmd.set_type( sdk::SERVER_KEYBOARD_KEYDOWN );
    cmd.mutable_keyboardevent()->set_keydown( key );
    return Pack( device, log, cmd );
}

size_t tic::KeyPress( tools::ipc::Device& device, const T_Logger& log, int key )
{
    ServerCommand cmd;
    cmd.set_type( sdk::SERVER_KEYBOARD_KEYPRESS );
    cmd.mutable_keyboardevent()->set_keypress( key );
    return Pack( device, log, cmd );
}

size_t tic::KeyUp( tools::ipc::Device& device, const T_Logger& log, int key )
{
    ServerCommand cmd;
    cmd.set_type( sdk::SERVER_KEYBOARD_KEYUP );
    cmd.mutable_keyboardevent()->set_keyup( key );
    return Pack( device, log, cmd );
}

void tic::ParseServer( ServerHandler_ABC& handler, const void* data, size_t size,
                       const T_Logger& log )
{
    ServerCommand cmd;
    Unpack( cmd, data, size, log );
    switch( cmd.type() )
    {
        case sdk::SERVER_READY:                       return handler.OnReadyServer();
        case sdk::SERVER_EVENT_CREATED:               return handler.OnCreatedEvent ( GetEvent( cmd.event() ), GetError( cmd.error() ) );
        case sdk::SERVER_EVENT_READ_ALL:              return handler.OnReadEvents( GetEvents( cmd.events() ), GetError( cmd.error() ) );
        case sdk::SERVER_EVENT_READ_ONE:              return handler.OnReadEvent( GetEvent( cmd.event() ), GetError( cmd.error() ) );
        case sdk::SERVER_EVENT_UPDATED:               return handler.OnUpdatedEvent( GetEvent( cmd.event() ), GetError( cmd.error() ) );
        case sdk::SERVER_EVENT_DELETED:               return handler.OnDeletedEvent( GetEvent( cmd.event() ).uuid, GetError( cmd.error() ) );
        case sdk::SERVER_EVENTS_LOADED:               return handler.OnLoadedEvents( GetError( cmd.error() ) );
        case sdk::SERVER_EVENTS_SAVED:                return handler.OnSavedEvents( cmd.data(), GetError( cmd.error() ) );
        case sdk::SERVER_EVENT_SELECTED:              return handler.OnSelectedEvent( GetEvent( cmd.event() ) );
        case sdk::SERVER_EVENT_DESELECTED:            return handler.OnDeselectedEvent();
        case sdk::SERVER_EVENT_ACTIVATED:             return handler.OnActivatedEvent( GetEvent( cmd.event() ) );
        case sdk::SERVER_EVENT_CONTEXTMENU:           return handler.OnContextMenuEvent( GetEvent( cmd.event() ) );
        case sdk::SERVER_EVENT_CONTEXTMENUBACKGROUND: return handler.OnContextMenuBackground( cmd.time() );
        case sdk::SERVER_KEYBOARD_KEYDOWN:            return handler.OnKeyDown( cmd.keyboardevent().keydown() );
        case sdk::SERVER_KEYBOARD_KEYPRESS:           return handler.OnKeyPress( cmd.keyboardevent().keypress() );
        case sdk::SERVER_KEYBOARD_KEYUP:              return handler.OnKeyUp( cmd.keyboardevent().keyup() );
    }
}
