// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __MaintenanceHaulersListView_h_
#define __MaintenanceHaulersListView_h_

#include "LogisticAvailabilitiesListView_ABC.h"

// =============================================================================
/** @class  MaintenanceHaulersListView
    @brief  MaintenanceHaulersListView
*/
// Created: SBO 2007-02-19
// =============================================================================
class MaintenanceHaulersListView : public MaintenanceAvailabilitiesListView_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             MaintenanceHaulersListView( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory );
    virtual ~MaintenanceHaulersListView();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const kernel::MaintenanceStates_ABC& a );
    //@}
};

#endif // __MaintenanceHaulersListView_h_
