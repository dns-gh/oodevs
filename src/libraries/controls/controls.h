// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************
#ifndef CONTROLS_H__
#define CONTROLS_H__

#include <timeline/api.h>
#include <boost/noncopyable.hpp>
#include <cstdint>
#include <vector>

namespace tools
{
namespace ipc
{
    class Device;
}
}

namespace timeline
{
namespace controls
{
    size_t ResizeClient( void* dst, size_t size );
    size_t QuitClient  ( void* dst, size_t size );
    size_t ReloadClient( void* dst, size_t size );
    size_t LoadClient  ( void* dst, size_t size, const std::string& url );
    size_t CreateEvent ( void* dst, size_t size, const Event& event );
    size_t DeleteEvent ( void* dst, size_t size, const std::string& uuid );

    struct ClientHandler_ABC : public boost::noncopyable
    {
                 ClientHandler_ABC() {}
        virtual ~ClientHandler_ABC() {}

        virtual void OnResizeClient() = 0;
        virtual void OnQuitClient()   = 0;
        virtual void OnReloadClient() = 0;
        virtual void OnLoadClient( const std::string& url ) = 0;
        virtual void OnCreateEvent( const Event& event ) = 0;
        virtual void OnDeleteEvent( const std::string& uuid ) = 0;
    };

    void ParseClient( ClientHandler_ABC& client, const void* src, size_t size );

    size_t ReadyServer          ( void* data, size_t size );
    size_t CreatedEvent         ( void* data, size_t size, const Event& event, const Error& error );
    size_t SelectedEvent        ( void* data, size_t size, const Event& event );
    size_t DeselectedEvent      ( void* data, size_t size );
    size_t DeletedEvent         ( void* data, size_t size, const std::string& uuid, const Error& error );
    size_t ActivatedEvent       ( void* data, size_t size, const Event& event );
    size_t ContextMenuEvent     ( void* data, size_t size, const Event& event );
    size_t ContextMenuBackground( void* data, size_t size );
    size_t KeyDown              ( void* data, size_t size, int key );
    size_t KeyPress             ( void* data, size_t size, int key );
    size_t KeyUp                ( void* data, size_t size, int key );

    struct ServerHandler_ABC : public boost::noncopyable
    {
                 ServerHandler_ABC() {}
        virtual ~ServerHandler_ABC() {}

        virtual void OnReadyServer          () = 0;
        virtual void OnCreatedEvent         ( const Event& event, const Error& error ) = 0;
        virtual void OnSelectedEvent        ( const Event& event ) = 0;
        virtual void OnDeselectedEvent      () = 0;
        virtual void OnDeletedEvent         ( const std::string& uuid, const Error& error ) = 0;
        virtual void OnActivatedEvent       ( const Event& event ) = 0;
        virtual void OnContextMenuEvent     ( const Event& event ) = 0;
        virtual void OnContextMenuBackground() = 0;
        virtual void OnKeyDown              ( int key ) = 0;
        virtual void OnKeyPress             ( int key ) = 0;
        virtual void OnKeyUp                ( int key ) = 0;
    };

    void ParseServer( ServerHandler_ABC& server, const void* src, size_t size );
}

    template< typename T >
    bool Write( tools::ipc::Device& device, const T& operand )
    {
        std::vector< uint8_t > buffer( operand( static_cast< void* >( 0 ), 0 ) );
        operand( &buffer[0], buffer.size() );
        return device.Write( &buffer[0], buffer.size() );
    }
}

#endif//CONTROLS_H__
