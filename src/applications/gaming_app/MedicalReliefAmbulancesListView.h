// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __MedicalReliefAmbulancesListView_h_
#define __MedicalReliefAmbulancesListView_h_

#include "ResourcesListView_ABC.h"
#include "gaming/MedicalStates.h"

// =============================================================================
/** @class  MedicalReliefAmbulancesListView
    @brief  MedicalReliefAmbulancesListView
*/
// Created: SBO 2007-02-20
// =============================================================================
class MedicalReliefAmbulancesListView : public ResourcesListView_ABC< MedicalStates >
{
public:
    //! @name Constructors/Destructor
    //@{
             MedicalReliefAmbulancesListView( QWidget* parent, kernel::Controllers& controllers );
    virtual ~MedicalReliefAmbulancesListView();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const MedicalStates& a );
    //@}
};

#endif // __MedicalReliefAmbulancesListView_h_
