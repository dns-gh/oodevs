// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Application_h_
#define __Application_h_

#include "clients_gui/Application_ABC.h"

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
class ExerciseContainer;
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
    QWidget* GetMainWindow();
    //@}

    //! @name Operations
    //@{
    void SetWaitingCursor();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnWaitCursorTimeout();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void InitializeStyle();
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< Config >                             config_;
    std::auto_ptr< tools::RealFileLoaderObserver_ABC >  fileLoaderObserver_;
    std::auto_ptr< tools::Loader_ABC >                  fileLoader_;
    std::auto_ptr< kernel::Controllers >                controllers_;
    std::auto_ptr< ExerciseContainer>                   exercises_;
    QMainWindow*                                        mainWindow_;

    std::auto_ptr< QTimer > cursorTimer_;
    bool waitCursor_;

    //@}
};

#endif // __Application_h_
