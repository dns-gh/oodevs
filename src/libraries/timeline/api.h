// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************
#ifndef TIMELINE_API_H__
#define TIMELINE_API_H__

#include <string>
#include <map>
#include <memory>
#include <vector>
#include <boost/noncopyable.hpp>
#include <tools/Path.h>
#pragma warning( push, 0 )
#include <QtCore/QObject>
#pragma warning( pop )
#include <boost/shared_ptr.hpp>

class QWidget;

namespace timeline
{
struct Action
{
    Action()
        : apply( true )
    {
        // NOTHING
    }
    Action( const std::string& target,
            bool apply,
            const std::string& payload )
        : target ( target )
        , apply  ( apply )
        , payload( payload )
    {
        // NOTHING
    }
    std::string target;
    bool        apply;
    std::string payload;
};

struct Event
{
    Event()
        : error_code( 0 )
        , read_only ( false )
        , done      ( false )
    {
        // NOTHING
    }
    Event( const std::string& uuid,
           const std::string& name,
           const std::string& info,
           const std::string& begin,
           const std::string& end,
           bool done,
           const Action& action )
        : uuid      ( uuid )
        , name      ( name )
        , info      ( info )
        , begin     ( begin )
        , end       ( end )
        , error_code( 0 )
        , read_only ( false )
        , done      ( done )
        , action    ( action )
    {
        // NOTHING
    }
    std::string uuid;
    std::string name;
    std::string info;
    std::string begin;
    std::string end;
    std::string parent;
    std::string error_text;
    std::string metadata;
    int         error_code;
    bool        read_only;
    bool        done;
    Action      action;
};

typedef std::vector< Event > Events;

enum ErrorCode
{
    EC_OK                    = 200,
    EC_INTERNAL_SERVER_ERROR = 505,
};

struct Error
{
    Error()
        : code( EC_OK )
    {
        // NOTHING
    }
    Error( int code,
           const std::string& text )
        : code( code )
        , text( text )
    {
        // NOTHING
    }
    int         code;
    std::string text;
};

struct CloseEvent
{
    CloseEvent( const std::string& uuid )
        : uuid( uuid )
        , done( true )
        , lock( false )
    {
        // NOTHING
    }
    std::string uuid;
    bool        done;
    Error       error;
    bool        lock;
};

class Server_ABC : public QObject
                 , public boost::noncopyable
{
    Q_OBJECT
public:
             Server_ABC() {}
    virtual ~Server_ABC() {}

    // Public slots
public slots:
    virtual void Start() = 0;
    virtual void Reload() = 0;
    virtual void Load( const std::string& url ) = 0;
    virtual void Center() = 0;
    virtual void UpdateQuery( const std::map< std::string, std::string >& parameters ) = 0;
    virtual void CreateEvents( const Events& events ) = 0;
    virtual void SelectEvent( const std::string& uuid ) = 0;
    virtual void ReadEvents() = 0;
    virtual void ReadEvent( const std::string& uuid ) = 0;
    virtual void UpdateEvent( const timeline::Event& event ) = 0;
    virtual void DeleteEvents( const std::vector< std::string >& uuids ) = 0;
    virtual void LoadEvents( const std::string& events ) = 0;
    virtual void SaveEvents() const = 0;
    virtual void CloseEvent( const timeline::CloseEvent& msg ) = 0;

    // Public signals
signals:
    void Ready();
    void CreatedEvents( const timeline::Events& events, const timeline::Error& error );
    void GetEvents( const timeline::Events& events, const timeline::Error& error );
    void GetEvent( const timeline::Event& event, const timeline::Error& error );
    void UpdatedEvent( const timeline::Event& event, const timeline::Error& error );
    void DeletedEvents( const std::vector< std::string >& uuids, const timeline::Error& error );
    void LoadedEvents( const timeline::Error& error );
    void SavedEvents( const std::string& events, const timeline::Error& error );
    void SelectedEvent( boost::shared_ptr< timeline::Event > event );
    void ActivatedEvent( const timeline::Event& event );
    void TriggeredEvents( const timeline::Events& events );
    void ClosedEvent( const timeline::Event& event, const timeline::Error& error );
    void ContextMenuEvent( boost::shared_ptr< timeline::Event > event );
    void ContextMenuBackground( const std::string& time );
    void ContextMenuReplay( boost::shared_ptr< timeline::Event > event, const std::string& time );
    void KeyDown( int key );
    void KeyPress( int key );
    void KeyUp( int key );
    void Done();
};

struct LibraryConfiguration
{
    LibraryConfiguration()
        : debug_port( 0 )
    {
        // NOTHING
    }
    tools::Path client_log;
    tools::Path cef_log;
    int         debug_port;
};

// Initializes timeline library. Only one call per process is supported
struct Library;
std::shared_ptr< Library > Initialize( const LibraryConfiguration& cfg );

struct Configuration
{
    Configuration()
        : widget( nullptr )
    {
        // NOTHING
    }
    QWidget*    widget;
    std::string url;
    tools::Path server_log;
};

// Starts timeline rendering inside the widget given in the configuration
std::auto_ptr< Server_ABC > MakeServer( const Configuration& cfg );

// Returns false if this process is not a chrome sub-process
bool RunClient( int argc, const char* argv[] );
}

#endif//TIMELINE_API_H__
