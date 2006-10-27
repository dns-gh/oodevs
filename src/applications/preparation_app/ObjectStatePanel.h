// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectStatePanel_h_
#define __ObjectStatePanel_h_

#include "clients_gui/InfoPanel_ABC.h"
#include "clients_kernel/ElementObserver_ABC.h"
#include "clients_kernel/SelectionObserver_ABC.h"
#include "clients_kernel/SafePointer.h"

namespace kernel
{
    class Object_ABC;
    class Controllers;
    class EditorFactory_ABC;
}

namespace gui
{
    class PropertiesWidget;
}

// =============================================================================
/** @class  ObjectStatePanel
    @brief  ObjectStatePanel
*/
// Created: SBO 2006-10-20
// =============================================================================
class ObjectStatePanel : public gui::InfoPanel_ABC
                       , public kernel::Observer_ABC
                       , public kernel::SelectionObserver< kernel::Object_ABC >
                       , public kernel::ElementObserver_ABC< kernel::Object_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             ObjectStatePanel( QWidget* parent, gui::PanelStack_ABC& panel, kernel::Controllers& controllers, kernel::EditorFactory_ABC& editorFactory );
    virtual ~ObjectStatePanel();
    //@}

    //! @name Operations
    //@{
    virtual void NotifySelected( const kernel::Object_ABC* element );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ObjectStatePanel( const ObjectStatePanel& );            //!< Copy constructor
    ObjectStatePanel& operator=( const ObjectStatePanel& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void showEvent( QShowEvent* );
    virtual void NotifyDeleted( const kernel::Object_ABC& );
    //@}

private:
    //! @name Member data
    //@{
    gui::PropertiesWidget* properties_;
    kernel::Controllers& controllers_;
    kernel::SafePointer< kernel::Object_ABC > selected_;
    //@}
};

#endif // __ObjectStatePanel_h_
