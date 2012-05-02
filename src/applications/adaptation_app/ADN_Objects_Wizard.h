// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ADN_Objects_Wizard_h_
#define __ADN_Objects_Wizard_h_

#include "ADN_Objects_Data.h"
#include "ADN_Wizard_ABC.h"

typedef ADN_Wizard_ABC< ADN_Objects_Data_ObjectInfos > ADN_Wizard_ABC_ADN_Object_Data_ObjectInfos;

// =============================================================================
/** @class  ADN_Objects_Wizard
    @brief  ADN_Objects_Wizard
*/
// Created: LDC 2010-06-21
// =============================================================================
class ADN_Objects_Wizard : public ADN_Wizard_ABC_ADN_Object_Data_ObjectInfos
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_Objects_Wizard( QWidget* pParent = 0, const char* szName = 0 );
    virtual ~ADN_Objects_Wizard();
    //@}

    virtual ADN_Ref_ABC* CreateObject();
};

#endif // __ADN_Objects_Wizard_h_
