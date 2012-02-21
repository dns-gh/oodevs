// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_People_Wizard_h_
#define __ADN_People_Wizard_h_

#include "ADN_People_Data.h"
#include "ADN_Wizard_ABC.h"

typedef ADN_Wizard_ABC<ADN_People_Data::PeopleInfos> ADN_Wizard_ABC_ADN_People_Data_PeopleInfos;

// =============================================================================
/** @class  ADN_People_Wizard
    @brief  ADN_People_Wizard
*/
// Created: SLG 2010-11-23
// =============================================================================
class ADN_People_Wizard : public ADN_Wizard_ABC_ADN_People_Data_PeopleInfos
{
public:
    //! @name Constructors/Destructor
    //@{
             ADN_People_Wizard( QWidget* pParent = 0, const char* szName = 0 );
    virtual ~ADN_People_Wizard();
    //@}
};

#endif // __ADN_People_Wizard_h_
