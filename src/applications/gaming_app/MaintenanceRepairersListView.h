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

#include "LogisticResourcesListView_ABC.h"

namespace kernel
{
    class MaintenanceStates_ABC;
}

// =============================================================================
/** @class  MaintenanceRepairersListView
    @brief  MaintenanceRepairersListView
*/
// Created: SBO 2007-02-19
// =============================================================================
class MaintenanceRepairersListView : public LogisticResourcesListView_ABC< kernel::MaintenanceStates_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             MaintenanceRepairersListView( QWidget* parent,
                                           kernel::Controllers& controllers,
                                           bool listenSelectionChanged = true );
    virtual ~MaintenanceRepairersListView();
    //@}

private:
    //! @name Helpers
    //@{
    virtual const std::vector< kernel::Availability >* GetAvailabilities( const kernel::MaintenanceStates_ABC& states ) const;
    //@}
};

#endif // __MaintenanceRepairersListView_h_
