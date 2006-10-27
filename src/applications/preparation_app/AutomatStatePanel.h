// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AutomatStatePanel_h_
#define __AutomatStatePanel_h_

#include "clients_gui/InfoPanel_ABC.h"
#include "clients_kernel/ElementObserver_ABC.h"
#include "clients_kernel/SelectionObserver_ABC.h"
#include "clients_kernel/SafePointer.h"

namespace kernel
{
    class Automat_ABC;
    class Controllers;
    class EditorFactory_ABC;
}

namespace gui
{
    class PropertiesWidget;
}

// =============================================================================
/** @class  AutomatStatePanel
    @brief  AutomatStatePanel
*/
// Created: SBO 2006-10-11
// =============================================================================
class AutomatStatePanel : public gui::InfoPanel_ABC
                      , public kernel::Observer_ABC
                      , public kernel::SelectionObserver< kernel::Automat_ABC >
                      , public kernel::ElementObserver_ABC< kernel::Automat_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             AutomatStatePanel( QWidget* parent, gui::PanelStack_ABC& panel, kernel::Controllers& controllers, kernel::EditorFactory_ABC& editorFactory );
    virtual ~AutomatStatePanel();
    //@}

    //! @name Operations
    //@{
    virtual void NotifySelected( const kernel::Automat_ABC* element );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AutomatStatePanel( const AutomatStatePanel& );            //!< Copy constructor
    AutomatStatePanel& operator=( const AutomatStatePanel& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void showEvent( QShowEvent* );
    virtual void NotifyDeleted( const kernel::Automat_ABC& );
    //@}

private:
    //! @name Member data
    //@{
    gui::PropertiesWidget* properties_;
    kernel::Controllers& controllers_;
    kernel::SafePointer< kernel::Automat_ABC > selected_;
    //@}
};

#endif // __AutomatStatePanel_h_
