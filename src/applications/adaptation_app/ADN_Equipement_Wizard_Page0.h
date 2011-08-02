// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Equipement_Wizard_Page0_h_
#define __ADN_Equipement_Wizard_Page0_h_

#include "ADN_Equipement_Data.h"
#include "ADN_Wizard_FirstPage_Default.h"

// =============================================================================
/** @class  ADN_Equipement_Wizard_Page0
    @brief  ADN_Equipement_Wizard_Page0
*/
// Created: APE 2005-01-20
// =============================================================================
class ADN_Equipement_Wizard_Page0 : public ADN_Wizard_FirstPage_Default<ADN_Equipement_Data::CategoryInfo>
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_Equipement_Wizard_Page0( ADN_Equipement_Data::ResourceInfos& parentDotation, Q3Wizard* pParent = 0, const char* szName = 0 );
    virtual ~ADN_Equipement_Wizard_Page0();
    //@}

private:
    //! @name Helpers
    //@{
    ADN_Equipement_Data::CategoryInfo* NewT();
    //@}

private:
    //! @name Member data
    //@{
    ADN_Equipement_Data::ResourceInfos& parentResource_;
    //@}
};

#endif // __ADN_Equipement_Wizard_Page0_h_
