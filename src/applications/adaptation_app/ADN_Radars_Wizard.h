// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Radars_Wizard_h_
#define __ADN_Radars_Wizard_h_

#include "ADN_Radars_Data.h"
#include "ADN_Wizard_ABC.h"

typedef ADN_Wizard_ABC< ADN_Radars_Data::RadarInfos > ADN_Wizard_ABC_ADN_Radars_Data_RadarInfos;

// =============================================================================
/** @class  ADN_Radars_Wizard
    @brief  ADN_Radars_Wizard
*/
// Created: LGY 2010-08-18
// =============================================================================
class ADN_Radars_Wizard : public ADN_Wizard_ABC_ADN_Radars_Data_RadarInfos
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_Radars_Wizard( QWidget* pParent = 0, const char* szName = 0 );
    virtual ~ADN_Radars_Wizard();
    //@}
};

#endif // __ADN_Radars_Wizard_h_
