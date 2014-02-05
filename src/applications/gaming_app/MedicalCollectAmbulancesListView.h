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

#include "LogisticResourcesListView_ABC.h"

class MedicalStates;

// =============================================================================
/** @class  MedicalCollectAmbulancesListView
    @brief  MedicalCollectAmbulancesListView
*/
// Created: SBO 2007-02-20
// =============================================================================
class MedicalCollectAmbulancesListView : public LogisticResourcesListView_ABC< MedicalStates >
{
public:
    //! @name Constructors/Destructor
    //@{
             MedicalCollectAmbulancesListView( QWidget* parent, kernel::Controllers& controllers );
    virtual ~MedicalCollectAmbulancesListView();
    //@}

private:
    //! @name Helpers
    //@{
    virtual const std::vector< kernel::Availability >* GetAvailabilities( const MedicalStates& states ) const;
    //@}
};

#endif // __MedicalCollectAmbulancesListView_h_
