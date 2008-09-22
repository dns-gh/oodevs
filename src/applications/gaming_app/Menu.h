// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Menu_h_
#define __Menu_h_

#include "clients_kernel/ElementObserver_ABC.h"
#include <qmenubar.h>

class QMainWindow;
class QDialog;

namespace kernel
{
    class Controllers;
    class Logger_ABC;
}

namespace gui
{
    class ItemFactory_ABC;
    class HelpSystem;
    class LinkInterpreter_ABC;
}

class UserProfileDialog;
class Profile;
class Network;

// =============================================================================
/** @class  Menu
    @brief  Menu
    // $$$$ AGE 2007-12-17: Passer dans clients_gui
*/
// Created: SBO 2006-04-28
// =============================================================================
class Menu : public QMenuBar
           , public kernel::Observer_ABC
           , public kernel::ElementObserver_ABC< Profile >
{

public:
    //! @name Constructors/Destructor
    //@{
             Menu( QMainWindow* pParent, kernel::Controllers& controllers, QDialog& prefDialog
                 , UserProfileDialog& profileDialog, gui::ItemFactory_ABC& factory, const QString& license
                 , const gui::HelpSystem& help, gui::LinkInterpreter_ABC& interpreter, Network& network, kernel::Logger_ABC& logger );
    virtual ~Menu();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Menu( const Menu& );
    Menu& operator=( const Menu& );
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const Profile& profile );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    UserProfileDialog& profileDialog_;
    int profileMenu_;
    //@}
};

#endif // __Menu_h_
