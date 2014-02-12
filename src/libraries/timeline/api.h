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
#include <QtCore/QObject>
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
        : sizeof_   ( sizeof( Event ) )
        , error_code( 0 )
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
        : sizeof_   ( sizeof( Event ) )
        , uuid      ( uuid )
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
    bool IsValid() const
    {
        return sizeof_ == sizeof *this;
    }
    std::string uuid;
    std::string name;
    std::string info;
    std::string begin;
    std::string end;
    std::string error_text;
    int         error_code;
    bool        read_only;
    bool        done;
    Action      action;
private:
    size_t      sizeof_;
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

class Server_ABC : public QObject
                 , public boost::noncopyable
{
    Q_OBJECT
public:
             Server_ABC() {}
    virtual ~Server_ABC() {}

    virtual void Start() = 0;

    // Public slots
public slots:
    virtual void Reload() = 0;
    virtual void Load( const std::string& url ) = 0;
    virtual void Center() = 0;
    virtual void UpdateQuery( const std::map< std::string, std::string >& parameters ) = 0;
    virtual bool CreateEvents( const Events& events ) = 0;
    virtual bool SelectEvent( const std::string& uuid ) = 0;
    virtual bool ReadEvents() = 0;
    virtual bool ReadEvent( const std::string& uuid ) = 0;
    virtual bool UpdateEvent( const timeline::Event& event ) = 0;
    virtual bool DeleteEvents( const std::vector< std::string >& uuids ) = 0;
    virtual void LoadEvents( const std::string& events ) = 0;
    virtual void SaveEvents() const = 0;

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
    void ContextMenuEvent( boost::shared_ptr< timeline::Event > event, const std::string& time );
    void KeyDown( int key );
    void KeyPress( int key );
    void KeyUp( int key );
};

struct Configuration
{
    Configuration()
        : sizeof_   ( sizeof( Configuration ) )
        , widget    ( 0 )
        , external  ( true )
        , debug_port( 0 )
    {
        // NOTHING
    }
    bool IsValid() const
    {
        return sizeof_ == sizeof *this;
    }
    tools::Path rundir;
    tools::Path binary;
    tools::Path server_log;
    tools::Path client_log;
    QWidget*    widget;
    std::string url;
    bool        external;   ///< use external process
    int         debug_port; ///< optional remove debug port
private:
    size_t      sizeof_;
};

bool SpawnServer();
std::auto_ptr< Server_ABC > MakeServer( const Configuration& cfg );
}

#endif//TIMELINE_API_H__
