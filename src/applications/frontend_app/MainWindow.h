// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MainWindow_h_
#define __MainWindow_h_

#include "clients_kernel/ElementObserver_ABC.h"
#include <qmainwindow.h>

namespace kernel
{
    class Controllers;
}

namespace frontend
{
    class Profile;
    class Networker;
}

class LoginDialog;

// =============================================================================
/** @class  MainWindow
    @brief  MainWindow
*/
// Created: SBO 2007-01-26
// =============================================================================
class MainWindow : public QMainWindow
                 , public kernel::Observer_ABC
                 , public kernel::ElementObserver_ABC< frontend::Profile >
                 , public kernel::ElementObserver_ABC< frontend::Networker >
{

public:
    //! @name Constructors/Destructor
    //@{
             MainWindow( kernel::Controllers& controllers, frontend::Networker& networker, const frontend::Profile& profile );
    virtual ~MainWindow();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MainWindow( const MainWindow& );            //!< Copy constructor
    MainWindow& operator=( const MainWindow& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const frontend::Networker& networker ); 
    virtual void NotifyUpdated( const frontend::Profile& profile ); 
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers&     controllers_;
    frontend::Networker&     networker_;
    const frontend::Profile& profile_;
    LoginDialog*             loginDialog_;
    //@}
};

#endif // __MainWindow_h_
