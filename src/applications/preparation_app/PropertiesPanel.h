// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PropertiesPanel_h_
#define __PropertiesPanel_h_

#include "clients_gui/InfoPanel_ABC.h"
#include "clients_kernel/ElementObserver_ABC.h"
#include "clients_kernel/SelectionObserver_ABC.h"
#include "clients_kernel/SafePointer.h"

namespace kernel
{
    class Entity_ABC;
    class Controllers;
    class EditorFactory_ABC;
}

namespace gui
{
    class PropertiesWidget;
    class TableItemDisplayer;
}

// =============================================================================
/** @class  PropertiesPanel
    @brief  PropertiesPanel
*/
// Created: SBO 2006-10-27
// =============================================================================
class PropertiesPanel : public gui::InfoPanel_ABC
                      , public kernel::Observer_ABC
                      , public kernel::SelectionObserver< kernel::Entity_ABC >
                      , public kernel::ElementObserver_ABC< kernel::Entity_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             PropertiesPanel( QWidget* parent, gui::PanelStack_ABC& panel, kernel::Controllers& controllers, kernel::EditorFactory_ABC& editorFactory );
    virtual ~PropertiesPanel();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    PropertiesPanel( const PropertiesPanel& );            //!< Copy constructor
    PropertiesPanel& operator=( const PropertiesPanel& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void showEvent( QShowEvent* );
    virtual void NotifySelected( const kernel::Entity_ABC* element );
    virtual void NotifyDeleted( const kernel::Entity_ABC& element );
    //@}

private:
    //! @name Member data
    //@{
    gui::PropertiesWidget* properties_;
    kernel::Controllers& controllers_;
    kernel::SafePointer< kernel::Entity_ABC > selected_;
    gui::TableItemDisplayer& tableItemDisplayer_;
    //@}
};

#endif // __PropertiesPanel_h_
