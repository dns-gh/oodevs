// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-01-19 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Equipement_Wizard.h $
// $Author: Ape $
// $Modtime: 28/01/05 15:29 $
// $Revision: 1 $
// $Workfile: ADN_Equipement_Wizard.h $
//
// *****************************************************************************

#ifndef __ADN_Equipement_Wizard_h_
#define __ADN_Equipement_Wizard_h_

#include "ADN_Wizard_ABC.h"
#include "ADN_Equipement_Data.h"


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
class ADN_Equipement_Wizard
: public ADN_Wizard_ABC<ADN_Equipement_Data::CategoryInfo>
{
    MT_COPYNOTALLOWED( ADN_Equipement_Wizard )

public:
    //! @name Constructors/Destructor
    //@{
             ADN_Equipement_Wizard( ADN_Equipement_Data::DotationInfos& parentDotation, QWidget* pParent = 0, const char* szName = 0 );
    virtual ~ADN_Equipement_Wizard();
    //@}
};

#endif // __ADN_Equipement_Wizard_h_
