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

#include "ResourcesListView_ABC.h"
#include "gaming/MedicalStates.h"

// =============================================================================
/** @class  MedicalCollectAmbulancesListView
    @brief  MedicalCollectAmbulancesListView
*/
// Created: SBO 2007-02-20
// =============================================================================
class MedicalCollectAmbulancesListView : public ResourcesListView_ABC< MedicalStates >
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
    virtual void NotifyUpdated( const MedicalStates& a );
    virtual void NotifySelected( const kernel::Entity_ABC* entity );
    virtual void UpdateSelected( const kernel::Entity_ABC* entity );
    virtual const std::vector< kernel::Availability >* GetAvailabilities( const MedicalStates& states ) const;
    //@}
};

#endif // __MedicalCollectAmbulancesListView_h_
