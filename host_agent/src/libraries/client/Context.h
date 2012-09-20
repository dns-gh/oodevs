// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef CONTEXT_H_
#define CONTEXT_H_

#include "runtime/Async.h"

#include <boost/filesystem/path.hpp>
#include <boost/scoped_ptr.hpp>

#include <QMutex>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>

namespace host
{
    struct Package_ABC;
}

namespace runtime
{
    struct FileSystem_ABC;
    struct Pool_ABC;
    struct Runtime_ABC;
}

namespace gui
{
    struct QAsync;
    typedef boost::filesystem::path Path;
    class ItemModel;
}

namespace gui
{
enum Command
{
    CMD_REGISTER,
    CMD_UNREGISTER,
    CMD_RUN,
    CMD_DISPLAY,
    CMD_COUNT,
};

enum HttpCommand
{
    HTTP_CMD_GET_SESSION,
    HTTP_CMD_COUNT,
};

class Context : public QObject
{
    Q_OBJECT

public:
     Context( const runtime::Runtime_ABC& runtime,
              const runtime::FileSystem_ABC& fs,
              runtime::Pool_ABC& pool,
              QAsync& async,
              ItemModel& items );
    ~Context();

public:
    void Start();
    void SetNetworkReply( HttpCommand cmd, QNetworkReply* rpy );

signals:
    void Exit();
    void StatusMessage( const QString& );
    void Progress( bool visible, int min, int max );
    void NetworkRequest( HttpCommand cmd, const QNetworkRequest& req );

public slots:
    void OnRemove();
    void OnGetSession();
    void ParseSession( QNetworkReply* rpy );

private:
    void ParseArguments();
    void ProcessCommand();
    void Register();
    void Unregister();
    void ParseRoot();
    void GetSession();

private:
    const runtime::Runtime_ABC& runtime_;
    const runtime::FileSystem_ABC& fs_;
    runtime::Pool_ABC& pool_;
    QAsync& async_;
    runtime::Async io_;
    ItemModel& items_;
    Command cmd_;
    Path root_;
    QUrl url_;
    QMutex access_;
    boost::scoped_ptr< host::Package_ABC > install_;
};
}

#endif // CONTEXT_H_