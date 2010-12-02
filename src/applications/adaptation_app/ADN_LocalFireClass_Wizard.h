// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ADN_LocalFireClass_Wizard_h_
#define __ADN_LocalFireClass_Wizard_h_

#include "ADN_LocalFireClass_Data.h"
#include "ADN_Wizard_ABC.h"

// =============================================================================
/** @class  ADN_LocalFireClass_Wizard
    @brief  ADN_LocalFireClass_Wizard
*/
// Created: JSR 2010-09-14
// =============================================================================
class ADN_LocalFireClass_Wizard
    : public ADN_Wizard_ABC< ADN_LocalFireClass_Data::LocalFireClassInfos >
{
    MT_COPYNOTALLOWED( ADN_LocalFireClass_Wizard )
public:
    //! @name Constructors/Destructor
    //@{
             ADN_LocalFireClass_Wizard( QWidget* pParent = 0, const char* szName = 0 );
    virtual ~ADN_LocalFireClass_Wizard();
    //@}
};

#endif // __ADN_LocalFireClass_Wizard_h_
