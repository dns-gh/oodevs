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
#include "host/Package_ABC.h"

#include <boost/filesystem/path.hpp>
#include <boost/property_tree/ptree.hpp>

#include <QReadWriteLock>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QSharedMemory>
#include <QSslError>
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
    HTTP_CMD_DOWNLOAD_ITEM,
    HTTP_CMD_COUNT,
};

class Context : public QObject
{
    Q_OBJECT

public:
     Context( const runtime::Runtime_ABC& runtime,
              const runtime::FileSystem_ABC& fs,
              runtime::Pool_ABC& pool,
              runtime::Async& async,
              ItemModel& items );
    ~Context();

public:
    void Start();

signals:
    void Exit();
    void Show();
    void EnableEdition();
    void StatusMessage( const QString& );
    void ClearMessage();
    void ShowProgress( int min, int max );
    void ClearProgress();
    void NetworkRequest( HttpCommand cmd, const QNetworkRequest& req );
    void SingleInstanceError();

public slots:
    void OnRemove();
    void OnNetworkRequest( HttpCommand cmd, const QNetworkRequest& req );
    void OnGetSession();
    void OnCloseDownload( size_t id, bool valid );
    void OnDownloadProgress( size_t id, size_t current, int progress );
    void OnDownloadError( size_t id, const QString& error );
    void ParsePackages();
    void OnSslErrors( QNetworkReply*, const QList< QSslError >& errors );

private:
    typedef boost::shared_ptr< Download_ABC >           T_Download;
    typedef QHash< size_t, T_Download >                 T_Downloads;
    typedef boost::shared_ptr< host::Package_ABC >      T_Package;
    typedef QHash< QString, host::Package_ABC::T_Item > T_Links;

private:
    void ParseArguments();
    void ProcessCommand();
    void Register();
    void Unregister();
    void ParseRoot();
    void GetSession();
    void ParseSession( const QByteArray& body );
    void ApplySession();
    void AddItem( const Tree& src, const std::string& type, size_t& idx );
    void OpenDownload( QNetworkReply* rpy );
    Path GetPath( const QString& type );
    void StartClient();

private:
    const runtime::Runtime_ABC& runtime_;
    const runtime::FileSystem_ABC& fs_;
    runtime::Pool_ABC& pool_;
    runtime::Async& async_;
    runtime::Async io_;
    QNetworkAccessManager net_;
    Command cmd_;
    Path root_;
    QUrl url_;
    QReadWriteLock access_;
    T_Package install_;
    ItemModel& items_;
    Tree session_;
    T_Links links_;
    T_Downloads downloads_;
    std::unique_ptr< QSharedMemory > single_;
};
}

#endif // CONTEXT_H_
