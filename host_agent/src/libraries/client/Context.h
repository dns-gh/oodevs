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
#include <QNetworkAccessManager>
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
    typedef boost::property_tree::ptree Tree;
    typedef boost::filesystem::path Path;
    struct  Download_ABC;
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

signals:
    void Exit();
    void Show();
    void StatusMessage( const QString& );
    void ClearMessage();
    void ShowProgress( int min, int max );
    void ClearProgress();
    void NetworkRequest( HttpCommand cmd, const QNetworkRequest& req );

public slots:
    void OnRemove();
    void OnNetworkRequest( HttpCommand cmd, const QNetworkRequest& req );
    void OnGetSession();
    void OnCloseDownload( size_t id );
    void OnDownloadProgress( size_t id, size_t current, int progress );
    void ParsePackages();

private:
    typedef boost::shared_ptr< Download_ABC >      T_Download;
    typedef QHash< size_t, T_Download >            T_Downloads;
    typedef boost::shared_ptr< host::Package_ABC > T_Package;

private:
    void ParseArguments();
    void ProcessCommand();
    void Register();
    void Unregister();
    void ParseRoot();
    void GetSession();
    void ParseSession( QNetworkReply* rpy );
    void AddItem( const Tree& src, const std::string& type, size_t& idx );
    void OpenDownload( QNetworkReply* rpy );

private:
    const runtime::Runtime_ABC& runtime_;
    const runtime::FileSystem_ABC& fs_;
    runtime::Pool_ABC& pool_;
    QAsync& async_;
    runtime::Async io_;
    QNetworkAccessManager net_;
    Command cmd_;
    Path root_;
    QUrl url_;
    QReadWriteLock access_;
    T_Package install_;
    ItemModel& items_;
    T_Downloads downloads_;
};
}

#endif // CONTEXT_H_