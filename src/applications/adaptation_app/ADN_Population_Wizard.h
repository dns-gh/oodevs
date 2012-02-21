// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 Mathématiques Appliquées SA (MASA)
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

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_Population_Wizard( QWidget* pParent = 0, const char* szName = 0 );
    virtual ~ADN_Population_Wizard();
    //@}
};

#endif // __ADN_Population_Wizard_h_
