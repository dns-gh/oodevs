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
#include <QProcess>

namespace boost
{
    class thread;
}

namespace tools
{
namespace ipc
{
    class Device;
}
}

namespace timeline
{
    class Embedded_ABC;
}

namespace timeline
{
class Server : public Server_ABC
             , public controls::ServerHandler_ABC
{
    Q_OBJECT
public:
             Server( const Configuration& cfg );
    virtual ~Server();

    /// Server_ABC methods
    virtual void Reload();
    virtual void Load( const std::string& url );
    virtual void UpdateQuery( const std::map< std::string, std::string >& parameters );
    virtual void Center();
    virtual bool CreateEvent( const Event& event );
    virtual bool SelectEvent( const std::string& uuid );
    virtual bool ReadEvents();
    virtual bool ReadEvent( const std::string& uuid );
    virtual bool UpdateEvent( const Event& event );
    virtual bool DeleteEvent( const std::string& uuid );
    virtual void LoadEvents( const std::string& events );
    virtual void SaveEvents() const;

    /// controls::Server_ABC methods
    virtual void OnReadyServer();
    virtual void OnCreatedEvent( const Event& event, const Error& error );
    virtual void OnReadEvents( const Events& events, const Error& error );
    virtual void OnReadEvent( const Event& event, const Error& error );
    virtual void OnUpdatedEvent( const Event& event, const Error& error );
    virtual void OnDeletedEvent( const std::string& uuid, const Error& error );
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

private:
    void Run();
    void StartProcess();

private slots:
    void OnError( QProcess::ProcessError error );

private:
    const Configuration cfg_;
    const std::string uuid_;
    std::auto_ptr< tools::ipc::Device > write_;
    std::auto_ptr< tools::ipc::Device > read_;
    std::auto_ptr< Embedded_ABC > embedded_;
    std::auto_ptr< boost::thread > thread_;
};
}

#endif//SERVER_H__
