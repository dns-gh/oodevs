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
#include <boost/property_tree/ptree_fwd.hpp>
#include <boost/scoped_ptr.hpp>

#include <QReadWriteLock>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QPointer>
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
    typedef boost::property_tree::ptree Tree;
    typedef boost::filesystem::path Path;
    struct  Download;
    class   ItemModel;
    struct  QAsync;
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
    HTTP_CMD_DOWNLOAD_INSTALL,
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
    void ClearMessage();
    void ShowProgress( int min, int max );
    void ClearProgress();
    void NetworkRequest( HttpCommand cmd, const QNetworkRequest& req );
    void CheckAbort( QPointer< QNetworkReply > ptr );

public slots:
    void OnRemove();
    void OnGetSession();
    void ParseSession( QNetworkReply* rpy );
    void OnDownloadInstall();
    void OnDownloadRead();

private:
    typedef boost::shared_ptr< Download > T_Download;
    typedef QHash< int, T_Download >      T_Downloads;

private:
    void ParseArguments();
    void ProcessCommand();
    void Register();
    void Unregister();
    void ParseRoot();
    void GetSession();
    void AddItem( const Tree& src, const std::string& type );
    void Unpack( T_Download down );

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
    QReadWriteLock access_;
    boost::scoped_ptr< host::Package_ABC > install_;
    T_Downloads downloads_;
};
}

#endif // CONTEXT_H_