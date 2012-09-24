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

#include <QNetworkReply>
#include <QPointer>

namespace runtime
{
    struct FileSystem_ABC;
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
    void End( QNetworkReply* );
};

boost::shared_ptr< Download_ABC > MakeDownload( QNetworkReply* rpy, const runtime::FileSystem_ABC& fs, const Path& root );
}

#endif // DOWNLOAD_H_