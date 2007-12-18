// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __EquipmentsListView_h_
#define __EquipmentsListView_h_

#include "gaming/Equipment.h"
#include "gaming/Equipments.h"
#include "ResourcesListView_ABC.h"

namespace kernel
{
    class Displayer_ABC;
}

namespace gui
{
    class ValuedListItem;
}

// =============================================================================
/** @class  EquipmentsListView
    @brief  EquipmentsListView
*/
// Created: SBO 2007-02-16
// =============================================================================
class EquipmentsListView : public ResourcesListView_ABC< EquipmentsListView, Equipments >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             EquipmentsListView( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory );
    virtual ~EquipmentsListView();
    //@}

    //! @name Operations
    //@{
    void Display( const Equipment& equipment, kernel::Displayer_ABC& displayer, gui::ValuedListItem* );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const Equipments& a );
    //@}
};

#endif // __EquipmentsListView_h_
