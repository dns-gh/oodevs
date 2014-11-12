// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __Menu_h_
#define __Menu_h_

#include "clients_gui/RichWidget.h"
#include <tools/ElementObserver_ABC.h>

class QMainWindow;
class QDialog;
class FilterDialogs;

namespace kernel
{
    class Controllers;
    class ModelLoaded;
    class ModelUnLoaded;
}

namespace gui
{
    class GLWidgetManager;
    class HelpSystem;
    class ItemFactory_ABC;
    class RichAction;
    class RichMenu;
}

class DialogContainer;

// =============================================================================
/** @class  Menu
    @brief  Menu
*/
// Created: SBO 2006-04-28
// =============================================================================
class Menu : public gui::RichWidget< QMenuBar >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             Menu( const QString& objectName,
                   QMainWindow& mainWindow,
                   kernel::Controllers& controllers,
                   gui::GLWidgetManager& glWidgetManager_,
                   const DialogContainer& dialogs,
                   const QString& license );
    virtual ~Menu();
    //@}

    //! @name Accessors
    //@{
    QAction* GetNewAction() const;
    QAction* GetOpenAction() const;
    QAction* GetSaveAction() const;
    QAction* GetSaveAsAction() const;
    //@}

private slots:
    //! @name Slots
    //@{
    void InsertFileMenuEntry( const QString name, const QObject* receiver, const char* member, const QKeySequence accel );
    void RemoveFileMenuEntry( const QString name );
    //@}

private:
    //! @name Helpers
    //@{
    void AddModdedAction( QAction* action, int hiddenModes = 0, int visibleModes = 0, bool visibleByDefault = true );
    virtual void mousePressEvent( QMouseEvent* event );
    //@}

private:
    //! @name Member data
    //@{
    QMainWindow& mainWindow_;
    kernel::Controllers& controllers_;
    gui::GLWidgetManager& glWidgetManager_;
    QMenu* windowMenu_;
    std::vector< gui::RichAction* > moddedActions_;
    gui::RichMenu* fileMenu_;
    QAction* helpMenuAction_;
    QAction* newAction_;
    QAction* openAction_;
    QAction* saveAction_;
    QAction* saveAsAction_;
    //@}

private:
    //! @name Static member data
    //@{
    static int filtersIndex_;
    //@}
};

#endif // __Menu_h_
