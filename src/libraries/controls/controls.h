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
    typedef std::function< void( const std::string& msg ) > T_Logger;

    size_t TryResizeClient( tools::ipc::Device& device, const T_Logger& log );
    size_t QuitClient     ( tools::ipc::Device& device, const T_Logger& log, int millisecs );
    size_t ReloadClient   ( tools::ipc::Device& device, const T_Logger& log );
    size_t LoadClient     ( tools::ipc::Device& device, const T_Logger& log, const std::string& url );
    size_t UpdateQuery    ( tools::ipc::Device& device, const T_Logger& log, const std::map< std::string, std::string >& query );
    size_t CenterClient   ( tools::ipc::Device& device, const T_Logger& log );
    size_t CreateEvents   ( tools::ipc::Device& device, const T_Logger& log, const Events& events );
    size_t SelectEvent    ( tools::ipc::Device& device, const T_Logger& log, const std::string& uuid );
    size_t ReadEvents     ( tools::ipc::Device& device, const T_Logger& log );
    size_t ReadEvent      ( tools::ipc::Device& device, const T_Logger& log, const std::string& uuid );
    size_t UpdateEvent    ( tools::ipc::Device& device, const T_Logger& log, const Event& event );
    size_t DeleteEvents   ( tools::ipc::Device& device, const T_Logger& log, const std::vector< std::string >& uuids );
    size_t LoadEvents     ( tools::ipc::Device& device, const T_Logger& log, const std::string& events );
    size_t SaveEvents     ( tools::ipc::Device& device, const T_Logger& log );

    struct ClientHandler_ABC : public boost::noncopyable
    {
                 ClientHandler_ABC() {}
        virtual ~ClientHandler_ABC() {}

        virtual void OnResizeClient() = 0;
        virtual void OnQuitClient()   = 0;
        virtual void OnReloadClient() = 0;
        virtual void OnLoadClient( const std::string& url ) = 0;
        virtual void OnUpdateQuery( const std::map< std::string, std::string >& query ) = 0;
        virtual void OnCenterClient() = 0;
        virtual void OnCreateEvents( const std::vector< Event >& events ) = 0;
        virtual void OnSelectEvent( const std::string& uuid ) = 0;
        virtual void OnReadEvents() = 0;
        virtual void OnReadEvent( const std::string& uuid ) = 0;
        virtual void OnUpdateEvent( const Event& event ) = 0;
        virtual void OnDeleteEvents( const std::vector< std::string >& uuids ) = 0;
        virtual void OnLoadEvents( const std::string& events ) = 0;
        virtual void OnSaveEvents() = 0;
    };

    void ParseClient( ClientHandler_ABC& client, const void* src, size_t size, const T_Logger& log );

    size_t ReadyServer          ( tools::ipc::Device& device, const T_Logger& log );
    size_t CreatedEvents        ( tools::ipc::Device& device, const T_Logger& log, const Events& events, const Error& error );
    size_t ReadEvents           ( tools::ipc::Device& device, const T_Logger& log, const Events& events, const Error& error );
    size_t ReadEvent            ( tools::ipc::Device& device, const T_Logger& log, const Event& event, const Error& error );
    size_t UpdatedEvent         ( tools::ipc::Device& device, const T_Logger& log, const Event& event, const Error& error );
    size_t DeletedEvents        ( tools::ipc::Device& device, const T_Logger& log, const std::vector< std::string >& uuids, const Error& error );
    size_t LoadedEvents         ( tools::ipc::Device& device, const T_Logger& log, const Error& error );
    size_t SavedEvents          ( tools::ipc::Device& device, const T_Logger& log, const std::string& events, const Error& error );
    size_t SelectedEvent        ( tools::ipc::Device& device, const T_Logger& log,  const Event& event );
    size_t DeselectedEvent      ( tools::ipc::Device& device, const T_Logger& log );
    size_t ActivatedEvent       ( tools::ipc::Device& device, const T_Logger& log, const Event& event );
    size_t ContextMenuEvent     ( tools::ipc::Device& device, const T_Logger& log, const Event& event );
    size_t ContextMenuBackground( tools::ipc::Device& device, const T_Logger& log, const std::string& time );
    size_t KeyDown              ( tools::ipc::Device& device, const T_Logger& log, int key );
    size_t KeyPress             ( tools::ipc::Device& device, const T_Logger& log, int key );
    size_t KeyUp                ( tools::ipc::Device& device, const T_Logger& log, int key );

    struct ServerHandler_ABC : public boost::noncopyable
    {
                 ServerHandler_ABC() {}
        virtual ~ServerHandler_ABC() {}

        virtual void OnReadyServer          () = 0;
        virtual void OnCreatedEvents        ( const Events& events, const Error& error ) = 0;
        virtual void OnReadEvents           ( const Events& events, const Error& error ) = 0;
        virtual void OnReadEvent            ( const Event& event, const Error& error ) = 0;
        virtual void OnUpdatedEvent         ( const Event& event, const Error& error ) = 0;
        virtual void OnDeletedEvents        ( const std::vector< std::string >& uuids, const Error& error ) = 0;
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

    void ParseServer( ServerHandler_ABC& server, const void* src, size_t size, const T_Logger& log );
}
}

#endif//CONTROLS_H__
