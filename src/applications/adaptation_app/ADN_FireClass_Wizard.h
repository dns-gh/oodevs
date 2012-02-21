// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ADN_FireClass_Wizard_h_
#define __ADN_FireClass_Wizard_h_

#include "ADN_FireClass_Data.h"
#include "ADN_Wizard_ABC.h"

// =============================================================================
/** @class  ADN_FireClass_Wizard
    @brief  ADN_FireClass_Wizard
*/
// Created: JSR 2010-09-14
// =============================================================================
class ADN_FireClass_Wizard : public ADN_Wizard_ABC< ADN_FireClass_Data::FireClassInfos >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_FireClass_Wizard( QWidget* pParent = 0, const char* szName = 0 );
    virtual ~ADN_FireClass_Wizard();
    //@}
};

#endif // __ADN_FireClass_Wizard_h_
