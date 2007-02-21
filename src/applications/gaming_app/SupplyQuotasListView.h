// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __SupplyQuotasListView_h_
#define __SupplyQuotasListView_h_

#include "LogisticAvailabilitiesListView_ABC.h"

// =============================================================================
/** @class  SupplyQuotasListView
    @brief  SupplyQuotasListView
*/
// Created: SBO 2007-02-20
// =============================================================================
class SupplyQuotasListView : public QuotasAvailabilitiesListView_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             SupplyQuotasListView( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory );
    virtual ~SupplyQuotasListView();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const Quotas& a );
    //@}
};

#endif // __SupplyQuotasListView_h_
