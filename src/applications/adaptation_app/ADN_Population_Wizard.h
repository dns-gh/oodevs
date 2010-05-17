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
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Population_Wizard.h $
// $Author: Ape $
// $Modtime: 14/02/05 17:23 $
// $Revision: 1 $
// $Workfile: ADN_Population_Wizard.h $
//
// *****************************************************************************

#ifndef __ADN_Population_Wizard_h_
#define __ADN_Population_Wizard_h_

#include "ADN_Population_Data.h"
#include "ADN_Wizard_ABC.h"

typedef ADN_Wizard_ABC<ADN_Population_Data::PopulationInfos> ADN_Wizard_ABC_ADN_Population_Data_PopulationInfos; 
// =============================================================================
/** @class  ADN_Population_Wizard
    @brief  ADN_Population_Wizard
*/
// Created: APE 2005-02-14
// =============================================================================
class ADN_Population_Wizard : public ADN_Wizard_ABC_ADN_Population_Data_PopulationInfos
{
    MT_COPYNOTALLOWED( ADN_Population_Wizard )

public:
    //! @name Constructors/Destructor
    //@{
    ADN_Population_Wizard( QWidget* pParent = 0, const char* szName = 0 );
    virtual ~ADN_Population_Wizard();
    //@}
};


#endif // __ADN_Population_Wizard_h_
