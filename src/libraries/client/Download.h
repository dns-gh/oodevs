// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef DOWNLOAD_H_
#define DOWNLOAD_H_

#include "runtime/Io.h"

#include <boost/filesystem/path.hpp>
#include <boost/shared_ptr.hpp>

namespace runtime
{
    struct FileSystem_ABC;
    struct Pool_ABC;
}

namespace gui
{
    typedef boost::filesystem::path Path;
}

namespace gui
{
struct Download_ABC : public QObject, public io::Reader_ABC
{
    Q_OBJECT
public:
             Download_ABC() {}
    virtual ~Download_ABC() {}

    virtual int Read( void* data, size_t size ) = 0;

public slots:
    virtual void OnReadyRead() = 0;
    virtual void OnFinished() = 0;
    virtual void OnAbort() = 0;

signals:
    void ReadyWrite();
    void Abort();
    void End( size_t id, bool valid );
    void Error( size_t id, const QString& error );
    void Progress( size_t id, int64_t current, int progress );
};

size_t GetDownloadId( const QNetworkReply* rpy );

boost::shared_ptr< Download_ABC > MakeDownload( size_t id, QNetworkReply* rpy,
                                                const runtime::FileSystem_ABC& fs,
                                                runtime::Pool_ABC& pool,
                                                const Path& root );
}

#endif // DOWNLOAD_H_
