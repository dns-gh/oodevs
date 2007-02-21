// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MedicalCollectAmbulancesListView_h_
#define __MedicalCollectAmbulancesListView_h_

#include "LogisticAvailabilitiesListView_ABC.h"

// =============================================================================
/** @class  MedicalCollectAmbulancesListView
    @brief  MedicalCollectAmbulancesListView
*/
// Created: SBO 2007-02-20
// =============================================================================
class MedicalCollectAmbulancesListView : public MedicalAvailabilitiesListView_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             MedicalCollectAmbulancesListView( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory );
    virtual ~MedicalCollectAmbulancesListView();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const MedicalStates& a );
    //@}
};

#endif // __MedicalCollectAmbulancesListView_h_
