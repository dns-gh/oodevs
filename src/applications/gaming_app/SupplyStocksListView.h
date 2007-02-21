// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __SupplyStocksListView_h_
#define __SupplyStocksListView_h_

#include "LogisticAvailabilitiesListView_ABC.h"

// =============================================================================
/** @class  SupplyStocksListView
    @brief  SupplyStocksListView
*/
// Created: SBO 2007-02-20
// =============================================================================
class SupplyStocksListView : public SupplyAvailabilitiesListView_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             SupplyStocksListView( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory );
    virtual ~SupplyStocksListView();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const SupplyStates& a );
    //@}
};

#endif // __SupplyStocksListView_h_
