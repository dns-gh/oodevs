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

#include <qapplication.h>

namespace frontend
{
    class LauncherClient;
}

namespace kernel
{
    class Controllers;
}

class Config;
class Launcher;
class SessionTray;

// =============================================================================
/** @class  Application
    @brief  Application
*/
// Created: SBO 2008-02-21
// =============================================================================
class Application : public QApplication
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             Application( int argc, char** argv );
    virtual ~Application();
    //@}

    //! @name Operations
    //@{
    void Initialize();
    void CreateTranslators();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnTimer();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Application( const Application& );            //!< Copy constructor
    Application& operator=( const Application& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void AddTranslator( const char* t, const QString& locale );
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< Config > config_;
    std::auto_ptr< kernel::Controllers > controllers_;
    std::auto_ptr< Launcher > launcher_;
    std::auto_ptr< frontend::LauncherClient > launcherClient_;
    std::auto_ptr< SessionTray > sessionTray_;
    QMainWindow* mainWindow_;
    QTimer* timer_;
    //@}

};

#endif // __Application_h_
