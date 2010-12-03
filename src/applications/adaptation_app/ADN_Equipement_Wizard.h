// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Equipement_Wizard_h_
#define __ADN_Equipement_Wizard_h_

#include "ADN_Wizard_ABC.h"
#include "ADN_Equipement_Data.h"

typedef ADN_Wizard_ABC<ADN_Equipement_Data::CategoryInfo>  ADN_Wizard_ABC_ADN_Equipement_Data_CategoryInfo_;

// =============================================================================
/** @class  ADN_Equipement_Wizard
    @brief  ADN_Equipement_Wizard
    @par    Using example
    @code
    ADN_Equipement_Wizard;
    @endcode
*/
// Created: APE 2005-01-19
// =============================================================================
class ADN_Equipement_Wizard : public ADN_Wizard_ABC_ADN_Equipement_Data_CategoryInfo_
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_Equipement_Wizard( ADN_Equipement_Data::ResourceInfos& parentDotation, QWidget* pParent = 0, const char* szName = 0 );
    virtual ~ADN_Equipement_Wizard();
    //@}
};

#endif // __ADN_Equipement_Wizard_h_
