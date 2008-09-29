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

#include <qapplication.h>

namespace tools
{
    class ApplicationMutex ; 
}

namespace kernel
{
    class Controllers; 
}

class QMainWindow;
class SessionTray; 

// =============================================================================
/** @class  Application
    @brief  Application
*/
// Created: SBO 2008-02-21
// =============================================================================
class Application : public QApplication
{

public:
    //! @name Constructors/Destructor
    //@{
             Application( int argc, char** argv, const QString& locale );
    virtual ~Application();
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
    QMainWindow*                             mainWindow_;
    std::auto_ptr< tools::ApplicationMutex > appMutex_ ; 
    std::auto_ptr< QPopupMenu >              trayMenu_ ; 
    std::auto_ptr<SessionTray>               sessionTray_ ; 
    std::auto_ptr<kernel::Controllers>       controllers_ ; 
    //@}

};

#endif // __Application_h_
