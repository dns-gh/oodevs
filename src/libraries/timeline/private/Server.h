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
    class mutex;
}

namespace tools
{
    class Ofstream;
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
    virtual void Start();
    virtual void Reload();
    virtual void Load( const std::string& url );
    virtual void UpdateQuery( const std::map< std::string, std::string >& parameters );
    virtual void Center();
    virtual bool CreateEvents( const Events& events );
    virtual bool SelectEvent( const std::string& uuid );
    virtual bool ReadEvents();
    virtual bool ReadEvent( const std::string& uuid );
    virtual bool UpdateEvent( const Event& event );
    virtual bool DeleteEvents( const std::vector< std::string >& uuids );
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

private:
    void Run();
    void StartProcess();
    void Log( const std::string& msg );
    void Log( const std::string& msg, bool read );

private slots:
    void OnError( QProcess::ProcessError error );

private:
    const Configuration cfg_;
    const std::string uuid_;
    const std::function< void( const std::string& msg ) > logger_;
    std::unique_ptr< boost::mutex > lock_;
    std::unique_ptr< tools::Ofstream > log_;
    std::unique_ptr< tools::ipc::Device > write_;
    std::unique_ptr< tools::ipc::Device > read_;
    std::unique_ptr< Embedded_ABC > embedded_;
    std::unique_ptr< boost::thread > thread_;
};
}

#endif//SERVER_H__
