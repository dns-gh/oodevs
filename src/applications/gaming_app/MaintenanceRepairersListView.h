// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MaintenanceRepairersListView_h_
#define __MaintenanceRepairersListView_h_

#include "LogisticAvailabilitiesListView_ABC.h"

// =============================================================================
/** @class  MaintenanceRepairersListView
    @brief  MaintenanceRepairersListView
*/
// Created: SBO 2007-02-19
// =============================================================================
class MaintenanceRepairersListView : public MaintenanceAvailabilitiesListView_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             MaintenanceRepairersListView( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory );
    virtual ~MaintenanceRepairersListView();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const MaintenanceStates& a );
    //@}
};

#endif // __MaintenanceRepairersListView_h_
