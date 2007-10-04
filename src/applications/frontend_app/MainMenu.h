// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MainMenu_h_
#define __MainMenu_h_

#include <qobject.h>

class QMainWindow;
class QAction;
class QPopupMenu;

// =============================================================================
/** @class  MainMenu
    @brief  MainMenu
*/
// Created: SBO 2007-10-04
// =============================================================================
class MainMenu : public QObject
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit MainMenu( QMainWindow* mainWindow );
    virtual ~MainMenu();
    //@}

    //! @name Operations
    //@{
    void AddAction( const QString& category, QAction& action );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MainMenu( const MainMenu& );            //!< Copy constructor
    MainMenu& operator=( const MainMenu& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    typedef std::map< QString, QPopupMenu* > T_Menus;
    //@}

private:
    //! @name Member data
    //@{
    QMainWindow* mainWindow_;
    T_Menus menus_;
    //@}
};

#endif // __MainMenu_h_
