// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __EntityPanel_h_
#define __EntityPanel_h_

#include "clients_gui/InfoPanel_ABC.h"
#include "clients_kernel/ElementObserver_ABC.h"
#include "clients_kernel/SelectionObserver_ABC.h"
#include "clients_kernel/SafePointer.h"

namespace kernel
{
    class Entity_ABC;
    class Controllers;
}

namespace gui
{
    class ItemFactory_ABC;
}

// =============================================================================
/** @class  EntityPanel
    @brief  EntityPanel
*/
// Created: SBO 2007-02-20
// =============================================================================
class EntityPanel : public gui::InfoPanel_ABC
                  , public kernel::Observer_ABC
                  , public kernel::SelectionObserver< kernel::Entity_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             EntityPanel( QWidget* parent, gui::PanelStack_ABC& panel, kernel::Controllers& controllers, const QString& title );
    virtual ~EntityPanel();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    EntityPanel( const EntityPanel& );            //!< Copy constructor
    EntityPanel& operator=( const EntityPanel& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void showEvent( QShowEvent* );
    virtual void NotifySelected( const kernel::Entity_ABC* entity );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    kernel::SafePointer< kernel::Entity_ABC > selected_;
    //@}
};

#endif // __EntityPanel_h_
