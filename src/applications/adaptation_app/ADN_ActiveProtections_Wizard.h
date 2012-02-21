// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_ActiveProtections_Wizard_h_
#define __ADN_ActiveProtections_Wizard_h_

#include "ADN_ActiveProtections_Data.h"
#include "ADN_Wizard_ABC.h"

typedef ADN_Wizard_ABC< ADN_ActiveProtections_Data::ActiveProtectionsInfos > ADN_Wizard_ABC_ADN_ActiveProtections_Data_ActiveProtectionsInfos;

// =============================================================================
/** @class  ADN_ActiveProtections_Wizard
    @brief  ADN_ActiveProtections_Wizard
*/
// Created: LGY 2010-08-18
// =============================================================================
class ADN_ActiveProtections_Wizard : public ADN_Wizard_ABC_ADN_ActiveProtections_Data_ActiveProtectionsInfos
{

public:
    //! @name Constructors/Destructor
    //@{
             ADN_ActiveProtections_Wizard( QWidget* pParent = 0, const char* szName = 0 );
    virtual ~ADN_ActiveProtections_Wizard();
    //@}
};

#endif // __ADN_ActiveProtections_Wizard_h_
