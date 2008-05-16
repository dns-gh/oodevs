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

#include <qapplication.h>

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

// =============================================================================
/** @class  Application
    @brief  Application
*/
// Created: SBO 2006-07-05
// =============================================================================
class Application : public QApplication
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             Application( int argc, char** argv, const QString& locale, const QString& expiration );
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
    void AddTranslator( const QString& locale, const char* t );
    //@}

private:
    //! @name Member data
    //@{
    Config* config_;
    kernel::Controllers* controllers_;
    StaticModel* staticModel_;
    Model* model_;
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
