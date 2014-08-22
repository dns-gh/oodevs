// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef SELFTRAINING_LOG_PROGRESS_PAGE_H
#define SELFTRAINING_LOG_PROGRESS_PAGE_H

#include "ContentPage.h"
#include <functional>

namespace frontend
{
    class Process_ABC;
}

class Application;

// LogProgressPage displays a console like output, filled by repeated calls
// to Log(), usually made from running process output streams. The caller is
// responsible of calling NotifyDone with or without error when finished.
// When the user clicks on Back button, the caller "backCallback" is invoked
// progress page closes.
class LogProgressPage : public ContentPage
{
    Q_OBJECT;

public:
             LogProgressPage( Application& app, QStackedWidget* pages, Page_ABC& previous );
    virtual ~LogProgressPage();

    void Start( const std::function< void() >& backCallback );
    virtual void Log( const std::string& line );
    virtual void NotifyDone( const std::string& error );

private slots:
    void OnNotifyDone( const QString& message );
    void OnLog( const QString& line );

private:
    virtual void OnBack();
    virtual void OnCancel();

private:
    Application& app_;
    QListWidget* log_;
    QProgressBar* progressBar_;
    std::function< void() > backCallback_;
};

#endif // SELFTRAINING_LOG_PROGRESS_PAGE_H
