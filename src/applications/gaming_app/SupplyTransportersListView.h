// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __SupplyTransportersListView_h_
#define __SupplyTransportersListView_h_

#include "LogisticAvailabilitiesListView_ABC.h"

// =============================================================================
/** @class  SupplyTransportersListView
    @brief  SupplyTransportersListView
*/
// Created: SBO 2007-02-20
// =============================================================================
class SupplyTransportersListView : public SupplyAvailabilitiesListView_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             SupplyTransportersListView( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory );
    virtual ~SupplyTransportersListView();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const SupplyStates& a );
    //@}
};

#endif // __SupplyTransportersListView_h_
