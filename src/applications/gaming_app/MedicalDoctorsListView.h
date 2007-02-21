// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MedicalDoctorsListView_h_
#define __MedicalDoctorsListView_h_

#include "LogisticAvailabilitiesListView_ABC.h"

// =============================================================================
/** @class  MedicalDoctorsListView
    @brief  MedicalDoctorsListView
*/
// Created: SBO 2007-02-20
// =============================================================================
class MedicalDoctorsListView : public MedicalAvailabilitiesListView_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             MedicalDoctorsListView( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory );
    virtual ~MedicalDoctorsListView();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const MedicalStates& a );
    //@}
};

#endif // __MedicalDoctorsListView_h_
