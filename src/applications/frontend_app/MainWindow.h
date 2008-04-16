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

#include <qmainwindow.h>
#include "ActionLinker.h"
#include "ActionsContext.h"

class QAction;
class QWidgetStack;

class MainMenu;
class ActionList;

namespace frontend
{
    class Config;
}

// =============================================================================
/** @class  MainWindow
    @brief  MainWindow
*/
// Created: SBO 2007-01-26
// =============================================================================
class MainWindow : public QMainWindow
{

public:
    //! @name Constructors/Destructor
    //@{
             MainWindow();
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
    void CenterWindow();
    template< typename Page >
    void AddAction( const QString& category, QAction& action );
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< frontend::Config > config_;
    ActionsContext context_;
    ActionLinker linker_;
    ActionList* list_;
    MainMenu*   menu_;
    QWidgetStack* pages_;
    //@}
};

#endif // __MainWindow_h_
