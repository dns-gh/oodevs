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
#include <memory>
#include <boost/noncopyable.hpp>
#include <tools/Path.h>
#include <QtCore/QObject>
#include <boost/shared_ptr.hpp>

class QWidget;

namespace timeline
{
struct Action
{
    std::string target;
    bool        apply;
    std::string payload;
};

struct Event
{
    std::string uuid;
    std::string name;
    std::string info;
    std::string begin;
    std::string end;
    bool        done;
    Action      action;
};

enum ErrorCode
{
    EC_OK                    = 200,
    EC_INTERNAL_SERVER_ERROR = 505,
};

struct Error
{
    Error() : code( EC_OK ) {}
    Error( int code, const std::string& text ) : code( code ), text( text ) {}
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

    // Public slots
public slots:
    virtual void Reload() = 0;
    virtual bool CreateEvent( const Event& event ) = 0;
    virtual bool DeleteEvent( const std::string& uuid ) = 0;

    // Public signals
signals:
    void CreatedEvent ( const Event& event, const Error& error );
    void SelectedEvent( boost::shared_ptr< Event > event );
    void DeletedEvent ( const std::string& uuid, const Error& error );
};

struct Configuration
{
    tools::Path rundir;
    tools::Path binary;
    QWidget*    widget;
    std::string url;
    bool        external;   ///< use external process
    int         debug_port; ///< optional remove debug port
};

bool SpawnServer();
std::auto_ptr< Server_ABC > MakeServer( const Configuration& cfg );
}

#endif//TIMELINE_API_H__
