// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************
//
// $Created: AGN 2003-11-03 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Breakdown_Wizard.h $
// $Author: Ape $
// $Modtime: 18/03/05 17:25 $
// $Revision: 1 $
// $Workfile: ADN_Breakdown_Wizard.h $
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
class ADN_Breakdown_Wizard
    : public ADN_Wizard_ABC_ADN_Breakdowns_Data_BreakdownInfo_
{
    MT_COPYNOTALLOWED( ADN_Breakdown_Wizard )

public:
    //! @name Constructors/Destructor
    //@{
    ADN_Breakdown_Wizard( QWidget* pParent = 0, const char* szName = 0 );
    virtual ~ADN_Breakdown_Wizard();
    //@}
};


#endif // __ADN_Breakdown_Wizard_h_
