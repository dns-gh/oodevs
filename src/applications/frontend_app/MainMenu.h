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

#include <QtCore/qobject.h>
#include <Qt3Support/q3action.h>

class Q3MainWindow;
class QAction;
class Q3PopupMenu;

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
    explicit MainMenu( Q3MainWindow* mainWindow );
    virtual ~MainMenu();
    //@}

    //! @name Operations
    //@{
    void AddAction( const QString& category, Q3Action& action );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MainMenu( const MainMenu& );            //!< Copy constructor
    MainMenu& operator=( const MainMenu& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    typedef std::map< QString, Q3PopupMenu* > T_Menus;
    //@}

private:
    //! @name Member data
    //@{
    Q3MainWindow* mainWindow_;
    T_Menus menus_;
    //@}
};

#endif // __MainMenu_h_
