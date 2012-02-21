// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Unit_Wizard_h_
#define __ADN_Unit_Wizard_h_

#include "ADN_Wizard_ABC.h"
#include "ADN_Units_Data.h"

typedef ADN_Wizard_ABC<ADN_Units_Data::UnitInfos> ADN_Wizard_ABC_ADN_Units_Data_UnitInfos_;

// =============================================================================
/** @class  ADN_Unit_Wizard
    @brief  ADN_Unit_Wizard
    @par    Using example
    @code
    ADN_Unit_Wizard;
    @endcode
*/
// Created: AGN 2003-11-03
// =============================================================================
class ADN_Unit_Wizard : public ADN_Wizard_ABC_ADN_Units_Data_UnitInfos_
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_Unit_Wizard( QWidget* pParent = 0, const char* szName = 0 );
    virtual ~ADN_Unit_Wizard();
    //@}
};

#endif // __ADN_Unit_Wizard_h_
