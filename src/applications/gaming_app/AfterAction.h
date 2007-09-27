// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AfterAction_h_
#define __AfterAction_h_

namespace kernel
{
    class Controllers;
}
namespace gui
{
    class ItemFactory_ABC;
}
class AfterActionFunction;
class AfterActionCanvas;
class AfterActionModel;
class Publisher_ABC;

// =============================================================================
/** @class  AfterAction
    @brief  AfterAction
*/
// Created: AGE 2007-09-17
// =============================================================================
class AfterAction : public QObject
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             AfterAction( QMainWindow* window, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory, AfterActionModel& model, Publisher_ABC& publisher );
    virtual ~AfterAction();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnEditFunction( const AfterActionFunction* function );
    void OnNewFunction();
    void OnCreateRequest( const AfterActionFunction* function );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AfterAction( const AfterAction& );            //!< Copy constructor
    AfterAction& operator=( const AfterAction& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void CreateEditionDock    ( QMainWindow* window, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory );
    void CreateAfterActionDock( QMainWindow* window, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory, Publisher_ABC& publisher );
    //@}

private:
    //! @name Member data
    //@{
    AfterActionModel& model_;
    QDockWindow* editionDock_;
    AfterActionCanvas* canvas_;
    QTabWidget* functionsTab_;
    //@}
};

#endif // __AfterAction_h_
