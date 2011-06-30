// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Application_h_
#define __Application_h_

#include "clients_kernel/Application_ABC.h"

namespace kernel
{
    class Controllers;
    class Workers;
}

namespace tools
{
    class RealFileLoaderObserver_ABC;
}

class Network;
class MainWindow;
class StaticModel;
class Model;
class Simulation;
class Profile;
class Config;
class LoggerProxy;
class Services;
class RcEntityResolver_ABC;

// =============================================================================
/** @class  Application
    @brief  Application
*/
// Created: SBO 2006-07-05
// =============================================================================
class Application : public Application_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             Application( int argc, char** argv, const QString& expiration );
    virtual ~Application();
    //@}

    //! @name
    //@{
    void Initialize();
    //@}

private slots:
    //! @name Slots
    //@{
    void UpdateData();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Application( const Application& );            //!< Copy constructor
    Application& operator=( const Application& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void Initialize( int argc, char** argv );
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< tools::RealFileLoaderObserver_ABC > observer_;
    std::auto_ptr< Config > config_;
    std::auto_ptr< kernel::Controllers > controllers_;
    std::auto_ptr< RcEntityResolver_ABC > rcResolver_;
    std::auto_ptr< StaticModel > staticModel_;
    std::auto_ptr< Model > model_;
    std::auto_ptr< Services > services_;
    std::auto_ptr< Simulation > simulation_;
    std::auto_ptr< Profile > profile_;
    std::auto_ptr< kernel::Workers > workers_;
    std::auto_ptr< LoggerProxy > logger_;
    std::auto_ptr< Network > network_;
    MainWindow* mainWindow_;
    QTimer* networkTimer_;
    const QString expiration_;
    //@}
};

#endif // __Application_h_
