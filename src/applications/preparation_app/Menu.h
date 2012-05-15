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

#include "tools/ElementObserver_ABC.h"
#include <boost/noncopyable.hpp>

class QMainWindow;
class QDialog;
class FilterDialogs;

namespace kernel
{
    class ContextMenu;
    class Controllers;
    class ModelLoaded;
    class ModelUnLoaded;
}

namespace gui
{
    class ItemFactory_ABC;
    class HelpSystem;
    class RichAction;
}

// =============================================================================
/** @class  Menu
    @brief  Menu
*/
// Created: SBO 2006-04-28
// =============================================================================
class Menu : public QMenuBar
           , private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
    Menu( QMainWindow* pParent, kernel::Controllers& controllers, QDialog& prefDialog, QDialog& profileDialog,
          QDialog& profileWizardDialog, QDialog& scoreDialog, QDialog& successFactorDialog, QDialog& exerciseDialog,
          QDialog& consistencyDialog, QDialog& performanceDialog,
          gui::ItemFactory_ABC& factory, const QString& license, const gui::HelpSystem& help );
    virtual ~Menu();
    //@}

    //! @name Operations
    //@{
    void InsertFileMenuEntry( const QString& name, const QObject* receiver, const char* member, const QKeySequence& accel = 0, int index = -1 );
    void RemoveFileMenuEntry( int index );
    //@}

    //! @name Accessors
    //@{
    QAction* GetNewAction() const; // $$$$ ABR 2012-05-14: Add and use a QActionFactory
    QAction* GetOpenAction() const;
    QAction* GetSaveAction() const;
    QAction* GetSaveAsAction() const;
    //@}

private:
    //! @name Helpers
    //@{
    void AddModdedAction( QAction* action, int defaultModes = 0, int hiddenModes = 0, int visibleModes = 0 );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers&            controllers_;
    std::vector< gui::RichAction* > moddedActions_;
    kernel::ContextMenu*            fileMenu_;
    QAction*                        newAction_;
    QAction*                        openAction_;
    QAction*                        saveAction_;
    QAction*                        saveAsAction_;
    //@}
};

#endif // __Menu_h_
