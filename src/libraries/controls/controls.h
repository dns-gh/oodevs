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
#include <map>
#include <vector>

#ifdef _MSC_VER
#pragma warning( push, 0 )
#endif
#include <internal/cef_ptr.h>
#ifdef _MSC_VER
#pragma warning( pop )
#endif

class CefProcessMessage;

namespace timeline
{
namespace controls
{
    // Name for client to server messages
    const std::string& GetClientToServerMessage();
    // Name for server to client messages
    const std::string& GetServerToClientMessage();

    typedef std::function< void( const std::string& msg ) > T_Logger;
    typedef CefRefPtr< CefProcessMessage > T_Msg;

    // Server commands
    T_Msg UpdateQuery    ( const T_Logger& log, const std::map< std::string, std::string >& query );
    T_Msg CenterClient   ( const T_Logger& log );
    T_Msg CreateEvents   ( const T_Logger& log, const Events& events );
    T_Msg SelectEvent    ( const T_Logger& log, const std::string& uuid );
    T_Msg ReadEvents     ( const T_Logger& log );
    T_Msg ReadEvent      ( const T_Logger& log, const std::string& uuid );
    T_Msg UpdateEvent    ( const T_Logger& log, const Event& event );
    T_Msg DeleteEvents   ( const T_Logger& log, const std::vector< std::string >& uuids );
    T_Msg LoadEvents     ( const T_Logger& log, const std::string& events );
    T_Msg SaveEvents     ( const T_Logger& log );

    // ClientHandler_ABC lists commands that every client must implement
    struct ClientHandler_ABC : public boost::noncopyable
    {
                 ClientHandler_ABC() {}
        virtual ~ClientHandler_ABC() {}

        virtual void OnUpdateQuery  ( const std::map< std::string, std::string >& query ) = 0;
        virtual void OnCenterClient () = 0;
        virtual void OnCreateEvents ( const std::vector< Event >& events ) = 0;
        virtual void OnSelectEvent  ( const std::string& uuid ) = 0;
        virtual void OnReadEvents   () = 0;
        virtual void OnReadEvent    ( const std::string& uuid ) = 0;
        virtual void OnUpdateEvent  ( const Event& event ) = 0;
        virtual void OnDeleteEvents ( const std::vector< std::string >& uuids ) = 0;
        virtual void OnLoadEvents   ( const std::string& events ) = 0;
        virtual void OnSaveEvents   () = 0;
    };

    // Parse a server to client message and potentially call relevant ClientHandler_ABC method
    void ParseClient( ClientHandler_ABC& client, const T_Msg& msg, const T_Logger& log );

    // Client events
    T_Msg ReadyServer           ( const T_Logger& log );
    T_Msg CreatedEvents         ( const T_Logger& log, const Events& events, const Error& error );
    T_Msg ReadEvents            ( const T_Logger& log, const Events& events, const Error& error );
    T_Msg ReadEvent             ( const T_Logger& log, const Event& event, const Error& error );
    T_Msg UpdatedEvent          ( const T_Logger& log, const Event& event, const Error& error );
    T_Msg DeletedEvents         ( const T_Logger& log, const std::vector< std::string >& uuids, const Error& error );
    T_Msg LoadedEvents          ( const T_Logger& log, const Error& error );
    T_Msg SavedEvents           ( const T_Logger& log, const std::string& events, const Error& error );
    T_Msg SelectedEvent         ( const T_Logger& log,  const Event& event );
    T_Msg DeselectedEvent       ( const T_Logger& log );
    T_Msg ActivatedEvent        ( const T_Logger& log, const Event& event );
    T_Msg ContextMenuEvent      ( const T_Logger& log, const Event& event );
    T_Msg ContextMenuBackground ( const T_Logger& log, const std::string& time );
    T_Msg KeyDown               ( const T_Logger& log, int key );
    T_Msg KeyPress              ( const T_Logger& log, int key );
    T_Msg KeyUp                 ( const T_Logger& log, int key );

    // ServerHandler_ABC lists events that every server must implement
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

    // Parse a client to server message and potentially call relevant ServerHandler_ABC method
    void ParseServer( ServerHandler_ABC& server, const T_Msg& msg, const T_Logger& log );
}
}

#endif//CONTROLS_H__
