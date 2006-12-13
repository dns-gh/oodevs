// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Mission_Wizard_h_
#define __ADN_Mission_Wizard_h_

#include "ADN_Missions_Data.h"
#include "ADN_Wizard_ABC.h"

// =============================================================================
/** @class  ADN_Mission_Wizard
    @brief  ADN_Mission_Wizard
*/
// Created: SBO 2006-12-04
// =============================================================================
class ADN_Mission_Wizard : public ADN_Wizard_ABC< ADN_Missions_Data::Mission >
{

public:
    //! @name Constructors/Destructor
    //@{
             ADN_Mission_Wizard( ADN_Missions_Data::T_Mission_Vector& missions, QWidget* pParent = 0, const char* szName = 0 );
    virtual ~ADN_Mission_Wizard();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ADN_Mission_Wizard( const ADN_Mission_Wizard& );            //!< Copy constructor
    ADN_Mission_Wizard& operator=( const ADN_Mission_Wizard& ); //!< Assignement operator
    //@}
};

#endif // __ADN_Mission_Wizard_h_
