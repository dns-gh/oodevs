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

namespace timeline
{
namespace controls
{
    size_t ResizeClient( void* dst, size_t size );
    size_t QuitClient  ( void* dst, size_t size );
    size_t ReloadClient( void* dst, size_t size );
    size_t CreateEvent ( void* dst, size_t size, const Event& event );

    struct ClientHandler_ABC : public boost::noncopyable
    {
                 ClientHandler_ABC() {}
        virtual ~ClientHandler_ABC() {}

        virtual void OnResizeClient() = 0;
        virtual void OnQuitClient()   = 0;
        virtual void OnReloadClient() = 0;
        virtual void OnCreateEvent( const Event& event ) = 0;
    };

    void ParseClient( ClientHandler_ABC& client, const void* src, size_t size );

    size_t CreatedEvent   ( void* dst, size_t size, const Event& event, const Error& error );
    size_t SelectedEvent  ( void* dst, size_t size, const Event& event );
    size_t DeselectedEvent( void* dst, size_t size );

    struct ServerHandler_ABC : public boost::noncopyable
    {
                 ServerHandler_ABC() {}
        virtual ~ServerHandler_ABC() {}

        virtual void OnCreatedEvent   ( const Event& event, const Error& error ) = 0;
        virtual void OnSelectedEvent  ( const Event& event ) = 0;
        virtual void OnDeselectedEvent() = 0;
    };

    void ParseServer( ServerHandler_ABC& server, const void* src, size_t size );

}
}

#endif//CONTROLS_H__
