// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
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

class Network;
class MainWindow;
class StaticModel;
class Model;
class Simulation;
class Profile;
class Config;
class LoggerProxy;
class Services;

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
    //! @name Copy/Assignement
    //@{
    Application( const Application& );            //!< Copy constructor
    Application& operator=( const Application& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void Initialize( int argc, char** argv );
    //@}

private:
    //! @name Member data
    //@{
    Config* config_;
    kernel::Controllers* controllers_;
    StaticModel* staticModel_;
    Model* model_;
    Services* services_;
    Simulation* simulation_;
    Profile* profile_;
    kernel::Workers* workers_;
    LoggerProxy* logger_;
    Network* network_;
    MainWindow* mainWindow_;
    QTimer* networkTimer_;
    const QString expiration_;
    //@}
};


#endif // __Application_h_
