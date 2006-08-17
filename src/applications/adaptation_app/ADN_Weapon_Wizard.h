// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-01-10 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Weapon_Wizard.h $
// $Author: Ape $
// $Modtime: 28/01/05 15:29 $
// $Revision: 2 $
// $Workfile: ADN_Weapon_Wizard.h $
//
// *****************************************************************************

#ifndef __ADN_Weapon_Wizard_h_
#define __ADN_Weapon_Wizard_h_

#include "ADN_Wizard_ABC.h"
#include "ADN_Weapons_Data.h"


// =============================================================================
/** @class  ADN_Weapon_Wizard
    @brief  ADN_Weapon_Wizard
    @par    Using example
    @code
    ADN_Weapon_Wizard;
    @endcode
*/
// Created: APE 2005-01-10
// =============================================================================
class ADN_Weapon_Wizard
: public ADN_Wizard_ABC<ADN_Weapons_Data::WeaponInfos>
{
    MT_COPYNOTALLOWED( ADN_Weapon_Wizard )

public:
    //! @name Constructors/Destructor
    //@{
             ADN_Weapon_Wizard( QWidget* pParent = 0, const char* szName = 0 );
    virtual ~ADN_Weapon_Wizard();
    //@}
};


#endif // __ADN_Weapon_Wizard_h_
