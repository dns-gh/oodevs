// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __gaming_Application_h_
#define __gaming_Application_h_

#include "clients_gui/Application_ABC.h"

namespace gui
{
    class ApplicationMonitor;
}

namespace kernel
{
    class Controllers;
    class Workers;
}

class Network;
class MainWindow;
class StaticModel;
class Model;
class Simulation;
class SimulationController;
class Profile;
class GamingConfig;
class LoggerProxy;
class Services;
class RcEntityResolver_ABC;
class ProfileFilter;

// =============================================================================
/** @class  Application
    @brief  Application
*/
// Created: SBO 2006-07-05
// =============================================================================
class Application : public gui::Application_ABC
{
    Q_OBJECT

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

private slots:
    //! @name Slots
    //@{
    void UpdateData();
    //@}

private:
    //! @name Member data
    //@{
    std::unique_ptr< GamingConfig >                      config_;
    std::unique_ptr< kernel::Controllers >               controllers_;
    std::unique_ptr< RcEntityResolver_ABC >              rcResolver_;
    std::unique_ptr< StaticModel >                       staticModel_;
    std::unique_ptr< Model >                             model_;
    std::unique_ptr< Services >                          services_;
    std::unique_ptr< Simulation >                        simulation_;
    std::unique_ptr< SimulationController >              simulationController_;
    std::unique_ptr< Profile >                           profile_;
    std::unique_ptr< ProfileFilter >                     filter_;
    std::unique_ptr< kernel::Workers >                   workers_;
    std::unique_ptr< LoggerProxy >                       logger_;
    std::unique_ptr< Network >                           network_;
    std::unique_ptr< QTimer >                            networkTimer_;
    std::unique_ptr< MainWindow >                        mainWindow_;
    //@}
};

#endif // __gaming_Application_h_
