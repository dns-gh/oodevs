// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __gui_PropertiesPanel_h_
#define __gui_PropertiesPanel_h_

#include "tools/ElementObserver_ABC.h"
#include "tools/SelectionObserver_ABC.h"
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

// =============================================================================
/** @class  PropertiesPanel
    @brief  Panel to display Entity properties registered into
            PropertiesDictionary extension
*/
// Created: SBO 2008-04-08
// =============================================================================
class PropertiesPanel : public QScrollArea
                      , public tools::Observer_ABC
                      , public tools::SelectionObserver< kernel::Entity_ABC >
                      , public tools::ElementObserver_ABC< kernel::Entity_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             PropertiesPanel( QWidget* parent, kernel::Controllers& controllers, kernel::EditorFactory_ABC& factory, gui::TableItemDisplayer& displayer );
    virtual ~PropertiesPanel();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PropertiesPanel( const PropertiesPanel& );            //!< Copy constructor
    PropertiesPanel& operator=( const PropertiesPanel& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void showEvent( QShowEvent* );
    virtual void NotifySelected( const kernel::Entity_ABC* element );
    virtual void NotifyDeleted( const kernel::Entity_ABC& element );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    gui::PropertiesWidget* table_;
    kernel::SafePointer< kernel::Entity_ABC > selected_;
    gui::TableItemDisplayer& displayer_;
    //@}
};

}

#endif // __gui_PropertiesPanel_h_
