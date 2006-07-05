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

#include <qmenubar.h>

class QMainWindow;
class QDialog;
class Controllers;
class RecorderToolbar;
class ItemFactory_ABC;

// =============================================================================
/** @class  Menu
    @brief  Menu
*/
// Created: SBO 2006-04-28
// =============================================================================
class Menu : public QMenuBar
{

public:
    //! @name Constructors/Destructor
    //@{
             Menu( QMainWindow* pParent, Controllers& controllers, QDialog& prefDialog, RecorderToolbar& recorderToolBar, ItemFactory_ABC& factory );
    virtual ~Menu();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Menu( const Menu& );            //!< Copy constructor
    Menu& operator=( const Menu& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    //@}
};

#endif // __Menu_h_
