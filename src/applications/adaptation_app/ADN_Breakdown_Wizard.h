// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Breakdown_Wizard_h_
#define __ADN_Breakdown_Wizard_h_

#include "ADN_Breakdowns_Data.h"
#include "ADN_Wizard_ABC.h"

typedef ADN_Wizard_ABC<ADN_Breakdowns_Data::BreakdownInfo> ADN_Wizard_ABC_ADN_Breakdowns_Data_BreakdownInfo_;

// =============================================================================
/** @class  ADN_Breakdown_Wizard
    @brief  ADN_Breakdown_Wizard
*/
// Created: APE 2005-02-14
// =============================================================================
class ADN_Breakdown_Wizard : public ADN_Wizard_ABC_ADN_Breakdowns_Data_BreakdownInfo_
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_Breakdown_Wizard( QWidget* pParent = 0, const char* szName = 0 );
    virtual ~ADN_Breakdown_Wizard();
    //@}
};

#endif // __ADN_Breakdown_Wizard_h_
