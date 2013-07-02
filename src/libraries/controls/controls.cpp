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
#ifdef _MSC_VER
#pragma warning( pop )
#endif

#include <boost/interprocess/streams/bufferstream.hpp>
#include <boost/interprocess/streams/vectorstream.hpp>

using namespace timeline;
namespace tic = timeline::controls;
namespace bip = boost::interprocess;
using sdk::ClientCommand;
using sdk::ServerCommand;

namespace
{
    template< typename T >
    size_t Marshall( void* dst, size_t size, const T& src )
    {
        const size_t need = src.ByteSize();
        if( !dst )
            return need;
        if( need > size )
            return SIZE_MAX;
        bip::obufferstream stream( reinterpret_cast< char* >( dst ), size );
        src.SerializeToOstream( &stream );
        return need;
    }

    template< typename T >
    void Unmarshall( T& dst, const void* src, size_t size )
    {
        bip::ibufferstream stream( reinterpret_cast< const char* >( src ), size );
        dst.ParseFromIstream( &stream );
    }

    template< typename CMD, typename TYPE >
    size_t MarshallType( void* data, size_t size, TYPE type )
    {
        CMD cmd;
        cmd.set_type( type );
        return Marshall( data, size, cmd );
    }
}

size_t tic::ResizeClient( void* data, size_t size )
{
    return MarshallType< ClientCommand >( data, size, sdk::CLIENT_RESIZE );
}

size_t tic::QuitClient( void* data, size_t size )
{
    return MarshallType< ClientCommand >( data, size, sdk::CLIENT_QUIT );
}

size_t tic::ReloadClient( void* data, size_t size )
{
    return MarshallType< ClientCommand >( data, size, sdk::CLIENT_RELOAD );
}

size_t tic::LoadClient( void* data, size_t size, const std::string& url )
{
    ClientCommand cmd;
    cmd.set_type( sdk::CLIENT_LOAD );
    cmd.set_url( url );
    return Marshall( data, size, cmd );
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
        return dst;
    }

    Events GetEvents( const google::protobuf::RepeatedPtrField< sdk::Event >& events )
    {
        Events rpy;
        for( auto it = events.begin(); it != events.end(); ++it )
            rpy.push_back( GetEvent( *it ) );
        return rpy;
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

size_t tic::CreateEvent( void* data, size_t size, const Event& event )
{
    ClientCommand cmd;
    cmd.set_type( sdk::CLIENT_EVENT_CREATE );
    SetEvent( *cmd.mutable_event(), event );
    return Marshall( data, size, cmd );
}

size_t tic::ReadEvents( void* data, size_t size )
{
    return MarshallType< ClientCommand >( data, size, sdk::CLIENT_EVENT_READ_ALL );
}

size_t tic::ReadEvent( void* data, size_t size, const std::string& uuid )
{
    ClientCommand cmd;
    cmd.set_type( sdk::CLIENT_EVENT_READ_ONE );
    cmd.mutable_event()->set_uuid( uuid );
    return Marshall( data, size, cmd );
}

size_t tic::UpdateEvent( void* data, size_t size, const Event& event )
{
    ClientCommand cmd;
    cmd.set_type( sdk::CLIENT_EVENT_UPDATE );
    SetEvent( *cmd.mutable_event(), event );
    return Marshall( data, size, cmd );
}

size_t tic::DeleteEvent( void* data, size_t size, const std::string& uuid )
{
    ClientCommand cmd;
    cmd.set_type( sdk::CLIENT_EVENT_DELETE );
    cmd.mutable_event()->set_uuid( uuid );
    return Marshall( data, size, cmd );
}

size_t tic::LoadEvents( void* data, size_t size, const std::string& events )
{
    ClientCommand cmd;
    cmd.set_type( sdk::CLIENT_EVENTS_LOAD );
    cmd.set_data( events );
    return Marshall( data, size, cmd );
}

size_t tic::SaveEvents( void* data, size_t size )
{
    return MarshallType< ClientCommand >( data, size, sdk::CLIENT_EVENTS_SAVE );
}

void tic::ParseClient( ClientHandler_ABC& handler, const void* data, size_t size )
{
    ClientCommand cmd;
    Unmarshall( cmd, data, size );
    switch( cmd.type() )
    {
        case sdk::CLIENT_RESIZE:         return handler.OnResizeClient();
        case sdk::CLIENT_QUIT:           return handler.OnQuitClient();
        case sdk::CLIENT_RELOAD:         return handler.OnReloadClient();
        case sdk::CLIENT_LOAD:           return handler.OnLoadClient( cmd.url() );
        case sdk::CLIENT_EVENT_CREATE:   return handler.OnCreateEvent( GetEvent( cmd.event() ) );
        case sdk::CLIENT_EVENT_READ_ALL: return handler.OnReadEvents();
        case sdk::CLIENT_EVENT_READ_ONE: return handler.OnReadEvent( GetEvent( cmd.event() ).uuid );
        case sdk::CLIENT_EVENT_UPDATE:   return handler.OnUpdateEvent( GetEvent( cmd.event() ) );
        case sdk::CLIENT_EVENT_DELETE:   return handler.OnDeleteEvent( GetEvent( cmd.event() ).uuid );
        case sdk::CLIENT_EVENTS_LOAD:    return handler.OnLoadEvents( cmd.data() );
        case sdk::CLIENT_EVENTS_SAVE:    return handler.OnSaveEvents();
    }
}

size_t tic::ReadyServer( void* data, size_t size )
{
    return MarshallType< ServerCommand >( data, size, sdk::SERVER_READY );
}

size_t tic::CreatedEvent( void* data, size_t size, const Event& event, const Error& error )
{
    ServerCommand cmd;
    cmd.set_type( sdk::SERVER_EVENT_CREATED );
    SetEvent( *cmd.mutable_event(), event );
    SetError( *cmd.mutable_error(), error );
    return Marshall( data, size, cmd );
}

size_t tic::ReadEvents( void* data, size_t size, const Events& events, const Error& error )
{
    ServerCommand cmd;
    cmd.set_type( sdk::SERVER_EVENT_READ_ALL );
    for( auto it = events.begin(); it != events.end(); ++it )
        SetEvent( *cmd.mutable_events()->Add(), *it );
    SetError( *cmd.mutable_error(), error );
    return Marshall( data, size, cmd );
}

size_t tic::ReadEvent( void* data, size_t size, const Event& event, const Error& error )
{
    ServerCommand cmd;
    cmd.set_type( sdk::SERVER_EVENT_READ_ONE );
    SetEvent( *cmd.mutable_event(), event );
    SetError( *cmd.mutable_error(), error );
    return Marshall( data, size, cmd );
}

size_t tic::UpdatedEvent( void* data, size_t size, const Event& event, const Error& error )
{
    ServerCommand cmd;
    cmd.set_type( sdk::SERVER_EVENT_UPDATED );
    SetEvent( *cmd.mutable_event(), event );
    SetError( *cmd.mutable_error(), error );
    return Marshall( data, size, cmd );
}

size_t tic::DeletedEvent( void* data, size_t size, const std::string& uuid, const Error& error )
{
    ServerCommand cmd;
    cmd.set_type( sdk::SERVER_EVENT_DELETED );
    cmd.mutable_event()->set_uuid( uuid );
    SetError( *cmd.mutable_error(), error );
    return Marshall( data, size, cmd );
}

size_t tic::LoadedEvents( void* data, size_t size, const Error& error )
{
    ServerCommand cmd;
    cmd.set_type( sdk::SERVER_EVENTS_LOADED );
    SetError( *cmd.mutable_error(), error );
    return Marshall( data, size, cmd );
}

size_t tic::SavedEvents( void* data, size_t size, const std::string& events, const Error& error )
{
    ServerCommand cmd;
    cmd.set_type( sdk::SERVER_EVENTS_SAVED );
    cmd.set_data( events );
    SetError( *cmd.mutable_error(), error );
    return Marshall( data, size, cmd );
}

size_t tic::SelectedEvent( void* data, size_t size, const Event& event )
{
    ServerCommand cmd;
    cmd.set_type( sdk::SERVER_EVENT_SELECTED );
    SetEvent( *cmd.mutable_event(), event );
    return Marshall( data, size, cmd );
}

size_t tic::DeselectedEvent( void* data, size_t size )
{
    return MarshallType< ServerCommand >( data, size, sdk::SERVER_EVENT_DESELECTED );
}

size_t tic::ActivatedEvent( void* data, size_t size, const Event& event )
{
    ServerCommand cmd;
    cmd.set_type( sdk::SERVER_EVENT_ACTIVATED );
    SetEvent( *cmd.mutable_event(), event );
    return Marshall( data, size, cmd );
}

size_t tic::ContextMenuEvent( void* data, size_t size, const Event& event )
{
    ServerCommand cmd;
    cmd.set_type( sdk::SERVER_EVENT_CONTEXTMENU );
    SetEvent( *cmd.mutable_event(), event );
    return Marshall( data, size, cmd );
}

size_t tic::ContextMenuBackground( void* data, size_t size, const std::string& time )
{
    ServerCommand cmd;
    cmd.set_type( sdk::SERVER_EVENT_CONTEXTMENUBACKGROUND );
    cmd.set_time( time );
    return Marshall( data, size, cmd );
}

size_t tic::KeyDown( void* data, size_t size, int key )
{
    ServerCommand cmd;
    cmd.set_type( sdk::SERVER_KEYBOARD_KEYDOWN );
    cmd.mutable_keyboardevent()->set_keydown( key );
    return Marshall( data, size, cmd );
}

size_t tic::KeyPress( void* data, size_t size, int key )
{
    ServerCommand cmd;
    cmd.set_type( sdk::SERVER_KEYBOARD_KEYPRESS );
    cmd.mutable_keyboardevent()->set_keypress( key );
    return Marshall( data, size, cmd );
}

size_t tic::KeyUp( void* data, size_t size, int key )
{
    ServerCommand cmd;
    cmd.set_type( sdk::SERVER_KEYBOARD_KEYUP );
    cmd.mutable_keyboardevent()->set_keyup( key );
    return Marshall( data, size, cmd );
}

void tic::ParseServer( ServerHandler_ABC& handler, const void* data, size_t size )
{
    ServerCommand cmd;
    Unmarshall( cmd, data, size );
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
