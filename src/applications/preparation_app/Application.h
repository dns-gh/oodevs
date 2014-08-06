// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __preparation_Application_h_
#define __preparation_Application_h_

#include "clients_gui/Application_ABC.h"
#include <boost/shared_ptr.hpp>

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
class PrepaConfig;

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
    //@}

private:
    //! @name Member data
    //@{
    boost::shared_ptr< FileLoaderObserver >  observer_;
    std::unique_ptr< PrepaConfig >              config_;
    std::unique_ptr< kernel::Controllers > controllers_;
    std::unique_ptr< StaticModel >         staticModel_;
    std::unique_ptr< Model >               model_;
    MainWindow*                          mainWindow_;
    //@}
};

#endif // __preparation_Application_h_
