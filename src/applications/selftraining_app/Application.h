// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __Application_h_
#define __Application_h_

#include "clients_gui/Application_ABC.h"

namespace frontend
{
    class LauncherClient;
}

namespace gui
{
    class ApplicationMonitor;
}

namespace kernel
{
    class Controllers;
}

namespace tools
{
    class Loader_ABC;
    class RealFileLoaderObserver_ABC;
}

class Config;
class Launcher;
class QMainWindow;

// =============================================================================
/** @class  Application
    @brief  Application
*/
// Created: SBO 2008-02-21
// =============================================================================
class Application : public gui::Application_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             Application( gui::ApplicationMonitor& monitor, int argc, char** argv );
    virtual ~Application();
    //@}

    //! @name gui::Application_ABC operations
    //@{
    virtual int Run();
    virtual void CreateTranslators();
    //@}

    //! @name Accessors
    //@{
    Launcher& GetLauncher() const;
    QWidget* GetMainWindow();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnTimer();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void InitializeStyle();
    virtual bool eventFilter( QObject* emitter, QEvent* event );
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< Config >                             config_;
    std::auto_ptr< tools::RealFileLoaderObserver_ABC >  fileLoaderObserver_;
    std::auto_ptr< tools::Loader_ABC >                  fileLoader_;
    std::auto_ptr< kernel::Controllers >                controllers_;
    std::auto_ptr< Launcher >                           launcher_;
    std::auto_ptr< frontend::LauncherClient >           launcherClient_;
    std::auto_ptr< QTimer >                             timer_;
    QMainWindow*                                        mainWindow_;
    //@}
};

#endif // __Application_h_
