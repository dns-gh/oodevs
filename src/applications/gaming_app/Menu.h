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

#include <tools/ElementObserver_ABC.h>

namespace kernel
{
    class ContextMenu;
    class Controllers;
    class Logger_ABC;
    class ObjectTypes;
}

namespace gui
{
    class ItemFactory_ABC;
    class HelpSystem;
    class ProfileDialog;
    class RichAction;
}

class Profile;
class Network;
class StaticModel;

// =============================================================================
/** @class  Menu
    @brief  Menu
    // $$$$ AGE 2007-12-17: Passer dans clients_gui
*/
// Created: SBO 2006-04-28
// =============================================================================
class Menu : public QMenuBar
           , public tools::Observer_ABC
           , public tools::ElementObserver_ABC< Profile >
{
public:
    //! @name Constructors/Destructor
    //@{
             Menu( QMainWindow* pParent, kernel::Controllers& controllers, StaticModel& staticModel, QDialog& prefDialog,
                   gui::ProfileDialog& profileDialog, const QString& license,
                   Network& network, kernel::Logger_ABC& logger );
    virtual ~Menu();
    //@}

private:
    virtual void NotifyUpdated( const Profile& profile );
    void AddModdedAction( QAction* action, int hiddenModes = 0, int visibleModes = 0, bool visibleByDefault = true );

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    gui::ProfileDialog& profileDialog_;
    int profileMenu_;
    gui::RichAction* windowAction_;
    std::vector< gui::RichAction* > moddedActions_;
    //@}
};

#endif // __Menu_h_
