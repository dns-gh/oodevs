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

#include "ResourcesListView_ABC.h"

namespace kernel
{
    class Equipments_ABC;
}

// =============================================================================
/** @class  EquipmentsListView
    @brief  EquipmentsListView
*/
// Created: SBO 2007-02-16
// =============================================================================
class EquipmentsListView : public ResourcesListView_ABC< kernel::Equipments_ABC >
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
    virtual void NotifyUpdated( const kernel::Equipments_ABC& equipments );
    //@}
};

#endif // __EquipmentsListView_h_
