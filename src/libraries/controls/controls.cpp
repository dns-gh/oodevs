// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************
#include "controls.h"

#ifdef _MSC_VER
#pragma warning( push, 0 )
#endif
#include "proto/controls.pb.h"
#include <cef_process_message.h>
#ifdef _MSC_VER
#pragma warning( pop )
#endif

#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/interprocess/streams/bufferstream.hpp>
#include <boost/interprocess/streams/vectorstream.hpp>

using namespace timeline;
using namespace timeline::controls;
namespace bip = boost::interprocess;
using sdk::ClientCommand;
using sdk::ServerCommand;

namespace
{
    const std::string ClientToServerMessage = "TIMELINE_CLIENT_TO_SERVER_MESSAGE";
    const std::string ServerToClientMessage = "TIMELINE_SERVER_TO_CLIENT_MESSAGE";
}

const std::string& controls::GetClientToServerMessage()
{
    return ClientToServerMessage;
}

const std::string& controls::GetServerToClientMessage()
{
    return ServerToClientMessage;
}

namespace
{
    template< typename T >
    void PackWith( CefRefPtr< CefListValue > dst, const T_Logger& log, const T& src )
    {
        if( log )
            log( src.ShortDebugString() );
        bip::basic_vectorstream< std::vector< char> > stream;
        src.SerializeToOstream( &stream );
        const auto& buf = stream.vector();
        dst->SetBinary( 0, CefBinaryValue::Create( &buf[0], buf.size() ) );
    }

    template< typename CMD >
    CefRefPtr< CefProcessMessage > Pack( const T_Logger& log, const CMD& cmd, const std::string& name )
    {
        auto msg = CefProcessMessage::Create( name );
        PackWith( msg->GetArgumentList(), log, cmd );
        return msg;
    }

    CefRefPtr< CefProcessMessage > Pack( const T_Logger& log, const ClientCommand& cmd )
    {
        return Pack( log, cmd, controls::GetServerToClientMessage() );
    }

    CefRefPtr< CefProcessMessage > Pack( const T_Logger& log, const ServerCommand& cmd )
    {
        return Pack( log, cmd, controls::GetClientToServerMessage() );
    }

    template< typename CMD, typename TYPE >
    CefRefPtr< CefProcessMessage > PackType( const T_Logger& log, TYPE type )
    {
        CMD cmd;
        cmd.set_type( type );
        return Pack( log, cmd );
    }

    template< typename T >
    void Unpack( T& dst, CefRefPtr< CefProcessMessage > msg, const T_Logger& log )
    {
        auto binary = msg->GetArgumentList()->GetBinary( 0 );
        std::vector< char > buffer( binary->GetSize() );
        binary->GetData( &buffer[0], buffer.size(), 0 );
        bip::ibufferstream stream( &buffer[0], buffer.size() );
        dst.ParseFromIstream( &stream );
        if( log )
            log( dst.ShortDebugString() );
    }
}

T_Msg controls::UpdateQuery( const T_Logger& log, const std::map< std::string, std::string >& parameters )
{
    ClientCommand cmd;
    cmd.set_type( sdk::CLIENT_QUERY_UPDATE );
    for( auto it = parameters.begin(); it != parameters.end(); ++it )
    {
        auto parameter = cmd.add_query();
        parameter->set_key( it->first );
        parameter->set_value( it->second );
    }
    return Pack( log, cmd );
}

T_Msg controls::CenterClient( const T_Logger& log )
{
    return PackType< ClientCommand >( log, sdk::CLIENT_CENTER );
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
        dst.set_parent( event.parent );
        if( !event.action.target.empty() )
            SetAction( *dst.mutable_action(), event.action );
        dst.set_done( event.done );
        dst.set_read_only( event.read_only );
        dst.set_error_code( event.error_code );
        dst.set_error_text( event.error_text );
        dst.set_metadata( event.metadata );
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
        dst.parent = src.parent();
        if( src.has_action() )
            dst.action = GetAction( src.action() );
        dst.done = src.done();
        dst.read_only = src.read_only();
        dst.error_code = src.error_code();
        dst.error_text = src.error_text();
        dst.metadata = src.metadata();
        return dst;
    }

    Event GetEvent( const google::protobuf::RepeatedPtrField< sdk::Event >& src )
    {
        if( !src.size() )
            return GetEvent( sdk::Event() );
        return GetEvent( *src.begin() );
    }

    Events GetEvents( const google::protobuf::RepeatedPtrField< sdk::Event >& events )
    {
        Events rpy;
        rpy.reserve( events.size() );
        for( auto it = events.begin(); it != events.end(); ++it )
            rpy.push_back( GetEvent( *it ) );
        return rpy;
    }

    std::vector< std::string > GetEventUuids( const google::protobuf::RepeatedPtrField< sdk::Event >& events )
    {
        std::vector< std::string > rpy;
        rpy.reserve( events.size() );
        for( auto it = events.begin(); it != events.end(); ++it )
            rpy.push_back( it->uuid() );
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

T_Msg controls::CreateEvents( const T_Logger& log, const std::vector< Event >& events )
{
    ClientCommand cmd;
    cmd.set_type( sdk::CLIENT_EVENT_CREATE );
    for( auto it = events.begin(); it != events.end(); ++it )
        SetEvent( *cmd.add_events(), *it );
    return Pack( log, cmd );
}

T_Msg controls::SelectEvent( const T_Logger& log, const std::string& uuid )
{
    ClientCommand cmd;
    cmd.set_type( sdk::CLIENT_EVENT_SELECT );
    cmd.add_events()->set_uuid( uuid );
    return Pack( log, cmd );
}

T_Msg controls::ReadEvents( const T_Logger& log )
{
    return PackType< ClientCommand >( log, sdk::CLIENT_EVENT_READ_ALL );
}

T_Msg controls::ReadEvent( const T_Logger& log, const std::string& uuid )
{
    ClientCommand cmd;
    cmd.set_type( sdk::CLIENT_EVENT_READ_ONE );
    cmd.add_events()->set_uuid( uuid );
    return Pack( log, cmd );
}

T_Msg controls::UpdateEvent( const T_Logger& log, const Event& event )
{
    ClientCommand cmd;
    cmd.set_type( sdk::CLIENT_EVENT_UPDATE );
    SetEvent( *cmd.add_events(), event );
    return Pack( log, cmd );
}

T_Msg controls::DeleteEvents( const T_Logger& log, const std::vector< std::string >& uuids )
{
    ClientCommand cmd;
    cmd.set_type( sdk::CLIENT_EVENT_DELETE );
    for( auto it = uuids.begin(); it != uuids.end(); ++it )
        cmd.add_events()->set_uuid( *it );
    return Pack( log, cmd );
}

T_Msg controls::LoadEvents( const T_Logger& log, const std::string& events )
{
    ClientCommand cmd;
    cmd.set_type( sdk::CLIENT_EVENTS_LOAD );
    cmd.set_data( events );
    return Pack( log, cmd );
}

T_Msg controls::SaveEvents( const T_Logger& log )
{
    return PackType< ClientCommand >( log, sdk::CLIENT_EVENTS_SAVE );
}

void controls::ParseClient( ClientHandler_ABC& handler,
                            const T_Msg& msg,
                            const T_Logger& log )
{
    ClientCommand cmd;
    Unpack( cmd, msg, log );
    switch( cmd.type() )
    {
        case sdk::CLIENT_QUERY_UPDATE:          return handler.OnUpdateQuery( GetQuery( cmd.query() ) );
        case sdk::CLIENT_CENTER:                return handler.OnCenterClient();
        case sdk::CLIENT_EVENT_CREATE:          return handler.OnCreateEvents( GetEvents( cmd.events() ) );
        case sdk::CLIENT_EVENT_SELECT:          return handler.OnSelectEvent( GetEvent( cmd.events() ).uuid );
        case sdk::CLIENT_EVENT_READ_ALL:        return handler.OnReadEvents();
        case sdk::CLIENT_EVENT_READ_ONE:        return handler.OnReadEvent( GetEvent( cmd.events() ).uuid );
        case sdk::CLIENT_EVENT_UPDATE:          return handler.OnUpdateEvent( GetEvent( cmd.events() ) );
        case sdk::CLIENT_EVENT_DELETE:          return handler.OnDeleteEvents( GetEventUuids( cmd.events() ) );
        case sdk::CLIENT_EVENTS_LOAD:           return handler.OnLoadEvents( cmd.data() );
        case sdk::CLIENT_EVENTS_SAVE:           return handler.OnSaveEvents();
    }
}

T_Msg controls::ReadyServer( const T_Logger& log )
{
    return PackType< ServerCommand >( log, sdk::SERVER_READY );
}

T_Msg controls::CreatedEvents( const T_Logger& log, const Events& events, const Error& error )
{
    ServerCommand cmd;
    cmd.set_type( sdk::SERVER_EVENT_CREATED );
    for( auto it = events.begin(); it != events.end(); ++it )
        SetEvent( *cmd.add_events(), *it );
    SetError( *cmd.mutable_error(), error );
    return Pack( log, cmd );
}

T_Msg controls::ReadEvents( const T_Logger& log, const Events& events, const Error& error )
{
    ServerCommand cmd;
    cmd.set_type( sdk::SERVER_EVENT_READ_ALL );
    for( auto it = events.begin(); it != events.end(); ++it )
        SetEvent( *cmd.add_events(), *it );
    SetError( *cmd.mutable_error(), error );
    return Pack( log, cmd );
}

T_Msg controls::ReadEvent( const T_Logger& log, const Event& event, const Error& error )
{
    ServerCommand cmd;
    cmd.set_type( sdk::SERVER_EVENT_READ_ONE );
    SetEvent( *cmd.add_events(), event );
    SetError( *cmd.mutable_error(), error );
    return Pack( log, cmd );
}

T_Msg controls::UpdatedEvent( const T_Logger& log, const Event& event, const Error& error )
{
    ServerCommand cmd;
    cmd.set_type( sdk::SERVER_EVENT_UPDATED );
    SetEvent( *cmd.add_events(), event );
    SetError( *cmd.mutable_error(), error );
    return Pack( log, cmd );
}

T_Msg controls::DeletedEvents( const T_Logger& log, const std::vector< std::string >& uuids, const Error& error )
{
    ServerCommand cmd;
    cmd.set_type( sdk::SERVER_EVENT_DELETED );
    for( auto it = uuids.begin(); it != uuids.end(); ++it )
        cmd.add_events()->set_uuid( *it );
    SetError( *cmd.mutable_error(), error );
    return Pack( log, cmd );
}

T_Msg controls::LoadedEvents( const T_Logger& log, const Error& error )
{
    ServerCommand cmd;
    cmd.set_type( sdk::SERVER_EVENTS_LOADED );
    SetError( *cmd.mutable_error(), error );
    return Pack( log, cmd );
}

T_Msg controls::SavedEvents( const T_Logger& log, const std::string& events, const Error& error )
{
    ServerCommand cmd;
    cmd.set_type( sdk::SERVER_EVENTS_SAVED );
    cmd.set_data( events );
    SetError( *cmd.mutable_error(), error );
    return Pack( log, cmd );
}

T_Msg controls::SelectedEvent( const T_Logger& log, const Event& event )
{
    ServerCommand cmd;
    cmd.set_type( sdk::SERVER_EVENT_SELECTED );
    SetEvent( *cmd.add_events(), event );
    return Pack( log, cmd );
}

T_Msg controls::DeselectedEvent( const T_Logger& log )
{
    return PackType< ServerCommand >( log, sdk::SERVER_EVENT_DESELECTED );
}

T_Msg controls::ActivatedEvent( const T_Logger& log, const Event& event )
{
    ServerCommand cmd;
    cmd.set_type( sdk::SERVER_EVENT_ACTIVATED );
    SetEvent( *cmd.add_events(), event );
    return Pack( log, cmd );
}

T_Msg controls::ContextMenuEvent( const T_Logger& log, const Event& event )
{
    ServerCommand cmd;
    cmd.set_type( sdk::SERVER_EVENT_CONTEXTMENU );
    SetEvent( *cmd.add_events(), event );
    return Pack( log, cmd );
}

T_Msg controls::ContextMenuBackground( const T_Logger& log, const std::string& time )
{
    ServerCommand cmd;
    cmd.set_type( sdk::SERVER_EVENT_CONTEXTMENUBACKGROUND );
    cmd.set_time( time );
    return Pack( log, cmd );
}

T_Msg controls::KeyDown( const T_Logger& log, int key )
{
    ServerCommand cmd;
    cmd.set_type( sdk::SERVER_KEYBOARD_KEYDOWN );
    cmd.mutable_keyboardevent()->set_keydown( key );
    return Pack( log, cmd );
}

T_Msg controls::KeyPress( const T_Logger& log, int key )
{
    ServerCommand cmd;
    cmd.set_type( sdk::SERVER_KEYBOARD_KEYPRESS );
    cmd.mutable_keyboardevent()->set_keypress( key );
    return Pack( log, cmd );
}

T_Msg controls::KeyUp( const T_Logger& log, int key )
{
    ServerCommand cmd;
    cmd.set_type( sdk::SERVER_KEYBOARD_KEYUP );
    cmd.mutable_keyboardevent()->set_keyup( key );
    return Pack( log, cmd );
}

void controls::ParseServer( ServerHandler_ABC& handler,
                            const T_Msg& msg,
                            const T_Logger& log )
{
    ServerCommand cmd;
    Unpack( cmd, msg, log );
    switch( cmd.type() )
    {
        case sdk::SERVER_READY:                       return handler.OnReadyServer();
        case sdk::SERVER_EVENT_CREATED:               return handler.OnCreatedEvents( GetEvents( cmd.events() ), GetError( cmd.error() ) );
        case sdk::SERVER_EVENT_READ_ALL:              return handler.OnReadEvents( GetEvents( cmd.events() ), GetError( cmd.error() ) );
        case sdk::SERVER_EVENT_READ_ONE:              return handler.OnReadEvent( GetEvent( cmd.events() ), GetError( cmd.error() ) );
        case sdk::SERVER_EVENT_UPDATED:               return handler.OnUpdatedEvent( GetEvent( cmd.events() ), GetError( cmd.error() ) );
        case sdk::SERVER_EVENT_DELETED:               return handler.OnDeletedEvents( GetEventUuids( cmd.events() ), GetError( cmd.error() ) );
        case sdk::SERVER_EVENTS_LOADED:               return handler.OnLoadedEvents( GetError( cmd.error() ) );
        case sdk::SERVER_EVENTS_SAVED:                return handler.OnSavedEvents( cmd.data(), GetError( cmd.error() ) );
        case sdk::SERVER_EVENT_SELECTED:              return handler.OnSelectedEvent( GetEvent( cmd.events() ) );
        case sdk::SERVER_EVENT_DESELECTED:            return handler.OnDeselectedEvent();
        case sdk::SERVER_EVENT_ACTIVATED:             return handler.OnActivatedEvent( GetEvent( cmd.events() ) );
        case sdk::SERVER_EVENT_CONTEXTMENU:           return handler.OnContextMenuEvent( GetEvent( cmd.events() ) );
        case sdk::SERVER_EVENT_CONTEXTMENUBACKGROUND: return handler.OnContextMenuBackground( cmd.time() );
        case sdk::SERVER_KEYBOARD_KEYDOWN:            return handler.OnKeyDown( cmd.keyboardevent().keydown() );
        case sdk::SERVER_KEYBOARD_KEYPRESS:           return handler.OnKeyPress( cmd.keyboardevent().keypress() );
        case sdk::SERVER_KEYBOARD_KEYUP:              return handler.OnKeyUp( cmd.keyboardevent().keyup() );
    }
}
