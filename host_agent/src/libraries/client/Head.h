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
#include "ui_head.h"

#include <boost/shared_ptr.hpp>
#include <QFileInfo>
#include <QMainWindow>
#include <QProgressBar>

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

private:
    const runtime::Runtime_ABC& runtime_;
    const runtime::FileSystem_ABC& fs_;
    runtime::Pool_ABC& pool_;
    Ui::Head ui_;
    QFileInfo root_;
    Command cmd_;
    QString url_;
    boost::shared_ptr< host::Package_ABC > install_;
    ItemModel items_;
    QProgressBar progress_;
    QAsync async_;
};
}

#endif // HEAD_H_