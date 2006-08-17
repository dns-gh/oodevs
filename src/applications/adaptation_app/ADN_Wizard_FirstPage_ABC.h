// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-01-28 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Wizard_FirstPage_ABC.h $
// $Author: Ape $
// $Modtime: 28/01/05 15:20 $
// $Revision: 1 $
// $Workfile: ADN_Wizard_FirstPage_ABC.h $
//
// *****************************************************************************

#ifndef __ADN_Wizard_FirstPage_ABC_h_
#define __ADN_Wizard_FirstPage_ABC_h_

#include "ADN_WizardPage_ABC.h"


// =============================================================================
/** @class  ADN_Wizard_FirstPage_ABC
    @brief  ADN_Wizard_FirstPage_ABC
    @par    Using example
    @code
    ADN_Wizard_FirstPage_ABC;
    @endcode
*/
// Created: APE 2005-01-28
// =============================================================================
template< typename T >
class ADN_Wizard_FirstPage_ABC
: public ADN_WizardPage_ABC
{
    MT_COPYNOTALLOWED( ADN_Wizard_FirstPage_ABC )

public:
    //! @name Constructors/Destructor
    //@{
             ADN_Wizard_FirstPage_ABC( QWizard* pParent, const char* szName = 0, const char* szImageName = "adn_wizard.bmp" );
    virtual ~ADN_Wizard_FirstPage_ABC();
    //@}

    //! @name Operations
    //@{
    virtual T* CreateObject() = 0; 
    //@}
};

#include "ADN_Wizard_FirstPage_ABC.inl"

#endif // __ADN_Wizard_FirstPage_ABC_h_
