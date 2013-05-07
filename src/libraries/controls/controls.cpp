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
#include "proto/controls.pb.cc"
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
    cmd.set_type( sdk::EVENT_CREATE );
    SetEvent( *cmd.mutable_event(), event );
    return Marshall( data, size, cmd );
}

void tic::ParseClient( ClientHandler_ABC& handler, const void* data, size_t size )
{
    ClientCommand cmd;
    Unmarshall( cmd, data, size );
    switch( cmd.type() )
    {
        case sdk::CLIENT_RESIZE: return handler.OnResizeClient();
        case sdk::CLIENT_QUIT:   return handler.OnQuitClient();
        case sdk::CLIENT_RELOAD: return handler.OnReloadClient();
        case sdk::EVENT_CREATE:  return handler.OnCreateEvent( GetEvent( cmd.event() ) );
    }
}

size_t tic::CreatedEvent( void* data, size_t size, const Event& event, const Error& error )
{
    ServerCommand cmd;
    cmd.set_type( sdk::EVENT_CREATED );
    SetEvent( *cmd.mutable_event(), event );
    SetError( *cmd.mutable_error(), error );
    return Marshall( data, size, cmd );
}

size_t tic::SelectedEvent( void* data, size_t size, const Event& event )
{
    ServerCommand cmd;
    cmd.set_type( sdk::EVENT_SELECTED );
    SetEvent( *cmd.mutable_event(), event );
    return Marshall( data, size, cmd );
}

size_t tic::DeselectedEvent( void* data, size_t size )
{
    return MarshallType< ServerCommand >( data, size, sdk::EVENT_DESELECTED );
}

void tic::ParseServer( ServerHandler_ABC& handler, const void* data, size_t size )
{
    ServerCommand cmd;
    Unmarshall( cmd, data, size );
    switch( cmd.type() )
    {
        case sdk::EVENT_CREATED:    return handler.OnCreatedEvent ( GetEvent( cmd.event() ), GetError( cmd.error() ) );
        case sdk::EVENT_SELECTED:   return handler.OnSelectedEvent( GetEvent( cmd.event() ) );
        case sdk::EVENT_DESELECTED: return handler.OnDeselectedEvent();
    }
}