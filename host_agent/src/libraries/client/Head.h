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

#include "QAsync.h"
#include "ItemModel.h"

#include <QLabel>
#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QProgressBar>

#include "ui_head.h"

#include <boost/scoped_ptr.hpp>

namespace runtime
{
    struct FileSystem_ABC;
    struct Pool_ABC;
    struct Runtime_ABC;
}

namespace gui
{
    class Context;
    enum HttpCommand;
}

namespace gui
{
class Head : public QMainWindow
{
    Q_OBJECT

public:
     Head( const runtime::Runtime_ABC& runtime, const runtime::FileSystem_ABC& fs, runtime::Pool_ABC& pool );
    ~Head();

private:
    void LoadSettings();
    void SaveSettings();

private slots:
    void OnModifiedItems();
    void OnProgress( bool visible, int min, int max );
    void OnNetworkRequest( HttpCommand cmd, const QNetworkRequest& req );

private:
    Ui::Head ui_;
    QProgressBar progress_;
    QLabel count_;
    ItemModel items_;
    boost::scoped_ptr< Context > ctx_;
    QAsync async_;
    QNetworkAccessManager net_;
};
}

#endif // HEAD_H_