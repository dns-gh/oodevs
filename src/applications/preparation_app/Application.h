// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __preparation_Application_h_
#define __preparation_Application_h_

#include "clients_gui/Application_ABC.h"

namespace gui
{
    class ApplicationMonitor;
}

namespace kernel
{
    class Controllers;
}

class FileLoaderObserver;
class MainWindow;
class StaticModel;
class Model;
class Config;

// =============================================================================
/** @class  Application
    @brief  Application
*/
// Created: SBO 2006-07-05
// =============================================================================
class Application : public gui::Application_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Application( gui::ApplicationMonitor& monitor, int& argc, char** argv );
    virtual ~Application();
    //@}

    //! @name gui::Application_ABC operations
    //@{
    virtual int Run();
    virtual void CreateTranslators();
    void CheckInterfaceComponentNaming();
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< FileLoaderObserver >  observer_;
    std::auto_ptr< Config >              config_;
    std::auto_ptr< kernel::Controllers > controllers_;
    std::auto_ptr< StaticModel >         staticModel_;
    std::auto_ptr< Model >               model_;
    MainWindow*                          mainWindow_;
    //@}
};

#endif // __preparation_Application_h_
