// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MedicalReliefAmbulancesListView_h_
#define __MedicalReliefAmbulancesListView_h_

#include "LogisticAvailabilitiesListView_ABC.h"

// =============================================================================
/** @class  MedicalReliefAmbulancesListView
    @brief  MedicalReliefAmbulancesListView
*/
// Created: SBO 2007-02-20
// =============================================================================
class MedicalReliefAmbulancesListView : public MedicalAvailabilitiesListView_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             MedicalReliefAmbulancesListView( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory );
    virtual ~MedicalReliefAmbulancesListView();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const MedicalStates& a );
    //@}
};

#endif // __MedicalReliefAmbulancesListView_h_
