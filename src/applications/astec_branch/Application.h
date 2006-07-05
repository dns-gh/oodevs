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

class Network;
class MainWindow;
class Model;
class Simulation;
class Controllers;
class Workers;
class AgentServerMsgMgr;

namespace xml { class xistream; }

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
    static AgentServerMsgMgr& GetMessageManager(); // $$$$ SBO 2006-03-16: 

public:
    //! @name Constructors/Destructor
    //@{
             Application( int argc, char** argv );
    virtual ~Application();
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
    void Initialize( int nArgc, char** ppArgv );
    //@}

private:
    //! @name Member data
    //@{
    Controllers* controllers_;
    Model* model_;
    Simulation* simulation_;
    Workers* workers_;
    Network* network_;
    MainWindow* mainWindow_;
    QTimer* networkTimer_;
    //@}

private:
    static Application* pInstance_;
};


#endif // __Application_h_
