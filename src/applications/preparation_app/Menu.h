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

namespace kernel
{
    class Controllers;
}

namespace gui
{
    class ItemFactory_ABC;
    class HelpSystem;
}

// =============================================================================
/** @class  Menu
    @brief  Menu
*/
// Created: SBO 2006-04-28
// =============================================================================
class Menu : public QMenuBar
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             Menu( QMainWindow* pParent, kernel::Controllers& controllers
                 , QDialog& prefDialog, QDialog& profileDialog, QDialog& profileWizardDialog, QDialog& importDialog, QDialog& scoreDialog, QDialog& successFactorDialog
                 , gui::ItemFactory_ABC& factory, const QString& license, const gui::HelpSystem& help );
    virtual ~Menu();
    //@}
};

#endif // __Menu_h_
