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
    size_t ReadEvents  ( void* dst, size_t size );
    size_t ReadEvent   ( void* dst, size_t size, const std::string& uuid );
    size_t UpdateEvent ( void* dst, size_t size, const Event& event );
    size_t DeleteEvent ( void* dst, size_t size, const std::string& uuid );
    size_t LoadEvents  ( void* dst, size_t size, const std::string& events );
    size_t SaveEvents  ( void* dst, size_t size );

    struct ClientHandler_ABC : public boost::noncopyable
    {
                 ClientHandler_ABC() {}
        virtual ~ClientHandler_ABC() {}

        virtual void OnResizeClient() = 0;
        virtual void OnQuitClient()   = 0;
        virtual void OnReloadClient() = 0;
        virtual void OnLoadClient( const std::string& url ) = 0;
        virtual void OnCreateEvent( const Event& event ) = 0;
        virtual void OnReadEvents() = 0;
        virtual void OnReadEvent( const std::string& uuid ) = 0;
        virtual void OnUpdateEvent( const Event& event ) = 0;
        virtual void OnDeleteEvent( const std::string& uuid ) = 0;
        virtual void OnLoadEvents( const std::string& events ) = 0;
        virtual void OnSaveEvents() = 0;
    };

    void ParseClient( ClientHandler_ABC& client, const void* src, size_t size );

    size_t ReadyServer          ( void* data, size_t size );
    size_t CreatedEvent         ( void* data, size_t size, const Event& event, const Error& error );
    size_t ReadEvents           ( void* data, size_t size, const Events& events, const Error& error );
    size_t ReadEvent            ( void* data, size_t size, const Event& event, const Error& error );
    size_t UpdatedEvent         ( void* data, size_t size, const Event& event, const Error& error );
    size_t DeletedEvent         ( void* data, size_t size, const std::string& uuid, const Error& error );
    size_t LoadedEvents         ( void* data, size_t size, const Error& error );
    size_t SavedEvents          ( void* data, size_t size, const std::string& events, const Error& error );

    size_t SelectedEvent        ( void* data, size_t size, const Event& event );
    size_t DeselectedEvent      ( void* data, size_t size );
    size_t ActivatedEvent       ( void* data, size_t size, const Event& event );
    size_t ContextMenuEvent     ( void* data, size_t size, const Event& event );
    size_t ContextMenuBackground( void* data, size_t size, const std::string& time );
    size_t KeyDown              ( void* data, size_t size, int key );
    size_t KeyPress             ( void* data, size_t size, int key );
    size_t KeyUp                ( void* data, size_t size, int key );

    struct ServerHandler_ABC : public boost::noncopyable
    {
                 ServerHandler_ABC() {}
        virtual ~ServerHandler_ABC() {}

        virtual void OnReadyServer          () = 0;
        virtual void OnCreatedEvent         ( const Event& event, const Error& error ) = 0;
        virtual void OnReadEvents           ( const Events& events, const Error& error ) = 0;
        virtual void OnReadEvent            ( const Event& event, const Error& error ) = 0;
        virtual void OnUpdatedEvent         ( const Event& event, const Error& error ) = 0;
        virtual void OnDeletedEvent         ( const std::string& uuid, const Error& error ) = 0;
        virtual void OnLoadedEvents         ( const Error& error ) = 0;
        virtual void OnSavedEvents          ( const std::string& events, const Error& error ) = 0;
        virtual void OnSelectedEvent        ( const Event& event ) = 0;
        virtual void OnDeselectedEvent      () = 0;
        virtual void OnActivatedEvent       ( const Event& event ) = 0;
        virtual void OnContextMenuEvent     ( const Event& event ) = 0;
        virtual void OnContextMenuBackground( const std::string& time ) = 0;
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
