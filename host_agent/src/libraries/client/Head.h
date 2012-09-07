// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef HEAD_H_
#define HEAD_H_

#include "ItemModel.h"
#include "QAsync.h"
#include "runtime/Async.h"
#include "ui_head.h"

#include <boost/filesystem/path.hpp>
#include <boost/shared_ptr.hpp>
#include <QFileInfo>
#include <QLabel>
#include <QMainWindow>
#include <QMutex>
#include <QProgressBar>
#include <QSortFilterProxyModel>

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
    typedef boost::filesystem::path Path;
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

class Head : public QMainWindow
{
    Q_OBJECT

public:
     Head( const runtime::Runtime_ABC& runtime, const runtime::FileSystem_ABC& fs, runtime::Pool_ABC& pool );
    ~Head();

    bool ProcessCommand();

private:
    void LoadSettings();
    void SaveSettings();
    void ParseArguments();

    void Register();
    void Unregister();
    void ParseRoot();

signals:
    void ProgressVisible( bool visible );

private slots:
    void OnProgressVisible( bool visible );
    void OnModifiedItems();
    void OnRemove();

private:
    const runtime::Runtime_ABC& runtime_;
    const runtime::FileSystem_ABC& fs_;
    runtime::Pool_ABC& pool_;
    Ui::Head ui_;
    Path root_;
    Command cmd_;
    QString url_;
    boost::shared_ptr< host::Package_ABC > install_;
    ItemModel items_;
    QSortFilterProxyModel proxy_;
    QProgressBar progress_;
    QLabel count_;
    QAsync async_;
    QMutex access_;
    runtime::Async io_async_;
};
}

#endif // HEAD_H_