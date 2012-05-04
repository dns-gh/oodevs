// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __ADN_ArmorCategory_Wizard_h_
#define __ADN_ArmorCategory_Wizard_h_

#include "ADN_ArmorInfos.h"
#include "ADN_Wizard_ABC.h"

typedef ADN_Wizard_ABC< helpers::ArmorInfos > ADN_Wizard_ABC_ArmorInfos;

// =============================================================================
/** @class  ADN_ArmorCategory_Wizard
    @brief  ADN_ArmorCategory_Wizard
*/
// Created: ABR 2012-04-24
// =============================================================================
class ADN_ArmorCategory_Wizard : public ADN_Wizard_ABC_ArmorInfos
{

public:
    //! @name Constructors/Destructor
    //@{
             ADN_ArmorCategory_Wizard( QWidget* pParent = 0, const char* szName = 0 );
    virtual ~ADN_ArmorCategory_Wizard();
    //@}
};

#endif // __ADN_ArmorCategory_Wizard_h_
