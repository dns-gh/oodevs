// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************
#ifndef SERVER_H__
#define SERVER_H__

#include "timeline/api.h"
#include "controls/controls.h"

#ifdef _MSC_VER
#pragma warning( push, 0 )
#endif
#include <internal/cef_ptr.h>
#ifdef _MSC_VER
#pragma warning( pop )
#endif

namespace boost
{
    class mutex;
}

namespace tools
{
    class Ofstream;
}

namespace timeline
{
    class Browser;
    class ServerApp;
}

namespace timeline
{
// SignalWaiter allows waiting synchronously on any signal
class SignalWaiter : public QObject
{
    Q_OBJECT
public:
    SignalWaiter() : signaled_( false ) {}
    bool IsSignaled() const { return signaled_; }
public slots:
    void Signal() { signaled_ = true; }
private:
    bool signaled_;
};

// Server implements a timeline view which can be modified, rendered and
// listened to. CEF is roughly separated in two parts:
// * The browser: A browser window, attached to a widget and urls, see
// cef_browser.h. This part runs in the main process
// * The renderer: A renderer, with a javascript vm, see cef_v8.h. This part
// runs in a separate process
// This class maintains two things:
// * ServerApp: Wrapper around CefClient for CEF initialization & customization
// * Browser: Wrapper around CefBrowser. Creating a browser object will also
// create the renderer process in the background
// The rendered process is customized with the Client object, see Client.h
// Server & Client communicate using CEF message pumps
// Deleting a server object also delete its render process
class Server : public Server_ABC
             , public controls::ServerHandler_ABC
{
public:
    typedef std::function< void( const std::string& ) > T_Logger;

    explicit Server( const Configuration& cfg );
    virtual ~Server();

    /// Server_ABC methods
    virtual void Start();
    virtual void Reload();
    virtual void Load( const std::string& url );
    virtual void UpdateQuery( const std::map< std::string, std::string >& parameters );
    virtual void Center();
    virtual void CreateEvents( const Events& events );
    virtual void SelectEvent( const std::string& uuid );
    virtual void ReadEvents();
    virtual void ReadEvent( const std::string& uuid );
    virtual void UpdateEvent( const Event& event );
    virtual void DeleteEvents( const std::vector< std::string >& uuids );
    virtual void LoadEvents( const std::string& events );
    virtual void SaveEvents() const;

    /// controls::Server_ABC methods
    virtual void OnReadyServer();
    virtual void OnCreatedEvents( const Events& events, const Error& error );
    virtual void OnReadEvents( const Events& events, const Error& error );
    virtual void OnReadEvent( const Event& event, const Error& error );
    virtual void OnUpdatedEvent( const Event& event, const Error& error );
    virtual void OnDeletedEvents( const std::vector< std::string >& uuids, const Error& error );
    virtual void OnLoadedEvents( const Error& error );
    virtual void OnSavedEvents( const std::string& uuid, const Error& error );
    virtual void OnSelectedEvent( const Event& event );
    virtual void OnDeselectedEvent();
    virtual void OnActivatedEvent( const Event& event );
    virtual void OnContextMenuEvent( const Event& event );
    virtual void OnContextMenuBackground( const std::string& time );
    virtual void OnKeyDown( int key );
    virtual void OnKeyPress( int key );
    virtual void OnKeyUp( int key );

    /// public methods
    virtual void Resize();
    virtual void Quit();

private:
    void Log( const std::string& msg );

private:
    const Configuration cfg_;
    const T_Logger logger_;
    QWidget* frame_;
    CefRefPtr< ServerApp > app_;
    CefRefPtr< Browser > browser_;
    std::unique_ptr< boost::mutex > lock_;
    std::unique_ptr< tools::Ofstream > log_;
};
}

#endif//SERVER_H__
