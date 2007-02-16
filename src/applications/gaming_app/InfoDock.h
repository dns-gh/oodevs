// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __InfoDock_h_
#define __InfoDock_h_

#include "clients_kernel/SelectionObserver_ABC.h"

namespace kernel
{
    class Controllers;
    class Entity_ABC;
}

namespace gui
{
    class SymbolIcons;
    class ItemFactory_ABC;
}

// =============================================================================
/** @class  InfoDock
    @brief  InfoDock
*/
// Created: SBO 2007-02-02
// =============================================================================
class InfoDock : public QDockWindow
               , public kernel::Observer_ABC
               , public kernel::SelectionObserver< kernel::Entity_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             InfoDock( QWidget* parent, kernel::Controllers& controllers, gui::SymbolIcons& icons, gui::ItemFactory_ABC& itemFactory );
    virtual ~InfoDock();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignment
    //@{
    InfoDock( const InfoDock& );            //!< Copy constructor
    InfoDock& operator=( const InfoDock& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifySelected( const kernel::Entity_ABC* entity );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    //@}
};

#endif // __InfoDock_h_
