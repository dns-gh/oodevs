// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-01-11 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Wizard_LastPage.h $
// $Author: Ape $
// $Modtime: 11/01/05 11:23 $
// $Revision: 1 $
// $Workfile: ADN_Wizard_LastPage.h $
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
class ADN_Wizard_LastPage
: public ADN_WizardPage_ABC
{
    MT_COPYNOTALLOWED( ADN_Wizard_LastPage )

public:
    //! @name Constructors/Destructor
    //@{
             ADN_Wizard_LastPage( QWizard* pParent, const QString& strTitle, const QString& strText, const char* szName = 0, const char* szImageName = "adn_wizard.bmp" );
    virtual ~ADN_Wizard_LastPage();
    //@}
};

#endif // __ADN_Wizard_LastPage_h_
