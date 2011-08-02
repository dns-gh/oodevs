// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Wizard_LastPage_h_
#define __ADN_Wizard_LastPage_h_

#include "ADN_WizardPage_ABC.h"

// =============================================================================
/** @class  ADN_Wizard_LastPage
    @brief  ADN_Wizard_LastPage
    @par    Using example
    @code
    ADN_Wizard_LastPage;
    @endcode
*/
// Created: APE 2005-01-11
// =============================================================================
class ADN_Wizard_LastPage : public ADN_WizardPage_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ADN_Wizard_LastPage( Q3Wizard* pParent, const QString& strTitle, const QString& strText, const char* szName = 0, const char* szImageName = "adn_wizard.bmp" );
    virtual ~ADN_Wizard_LastPage();
    //@}
};

#endif // __ADN_Wizard_LastPage_h_
