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

// =============================================================================
/** @class  EquipmentsListView
    @brief  EquipmentsListView
*/
// Created: SBO 2007-02-16
// =============================================================================
class EquipmentsListView : public ResourcesListView_ABC<  Equipments >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             EquipmentsListView( QWidget* parent, kernel::Controllers& controllers );
    virtual ~EquipmentsListView();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const Equipments& a );
    //@}
};

#endif // __EquipmentsListView_h_
