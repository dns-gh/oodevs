// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-04-27 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Breakdown_Wizard_Page0.h $
// $Author: Nld $
// $Modtime: 27/04/05 10:52 $
// $Revision: 1 $
// $Workfile: ADN_Breakdown_Wizard_Page0.h $
//
// *****************************************************************************

#ifndef __ADN_Breakdown_Wizard_Page0_h_
#define __ADN_Breakdown_Wizard_Page0_h_

#include "ADN_Breakdowns_Data.h"
#include "ADN_Wizard_FirstPage_Default.h"


// =============================================================================
/** @class  ADN_Breakdown_Wizard_Page0
    @brief  ADN_Breakdown_Wizard_Page0
*/
// Created: APE 2005-04-27
// =============================================================================
class ADN_Breakdown_Wizard_Page0
    : public ADN_Wizard_FirstPage_Default<ADN_Breakdowns_Data::BreakdownInfo>
{
    MT_COPYNOTALLOWED( ADN_Breakdown_Wizard_Page0 )

public:
    //! @name Constructors/Destructor
    //@{
    ADN_Breakdown_Wizard_Page0( ADN_Breakdowns_Data::NTIInfo& nti, QWizard* pParent = 0, const char* szName = 0 );
    virtual ~ADN_Breakdown_Wizard_Page0();
    //@}

private:
    //! @name Helpers
    //@{
    ADN_Breakdowns_Data::BreakdownInfo* NewT();
    //@}


private:
    //! @name Member data
    //@{
    ADN_Breakdowns_Data::NTIInfo& nti_;
    //@}
};

#endif // __ADN_Breakdown_Wizard_Page0_h_
