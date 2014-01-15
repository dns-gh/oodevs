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

#include "ResourcesListView_ABC.h"
#include "clients_kernel/MaintenanceStates_ABC.h"

// =============================================================================
/** @class  MaintenanceRepairersListView
    @brief  MaintenanceRepairersListView
*/
// Created: SBO 2007-02-19
// =============================================================================
class MaintenanceRepairersListView : public ResourcesListView_ABC< kernel::MaintenanceStates_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             MaintenanceRepairersListView( QWidget* parent, kernel::Controllers& controllers );
    virtual ~MaintenanceRepairersListView();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const kernel::MaintenanceStates_ABC& a );
    virtual void NotifySelected( const kernel::Entity_ABC* entity );
    virtual void UpdateSelected( const kernel::Entity_ABC* entity );
    virtual const std::vector< kernel::Availability >* GetAvailabilities( const kernel::MaintenanceStates_ABC& states ) const;
    //@}
};

#endif // __MaintenanceRepairersListView_h_
