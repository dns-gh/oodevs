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
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Weapon_Wizard_Page0.h $
// $Author: Ape $
// $Modtime: 28/01/05 16:08 $
// $Revision: 2 $
// $Workfile: ADN_Weapon_Wizard_Page0.h $
//
// *****************************************************************************

#ifndef __ADN_Weapon_Wizard_Page0_h_
#define __ADN_Weapon_Wizard_Page0_h_

#include "ADN_Wizard_FirstPage_ABC.h"
#include "ADN_Weapons_Data.h"

class QComboBox;
class QRadioButton;
class QListView;


// =============================================================================
/** @class  ADN_Weapon_Wizard_Page0
    @brief  ADN_Weapon_Wizard_Page0
    @par    Using example
    @code
    ADN_Weapon_Wizard_Page0;
    @endcode
*/
// Created: APE 2005-01-11
// =============================================================================
class ADN_Weapon_Wizard_Page0
: public ADN_Wizard_FirstPage_ABC<ADN_Weapons_Data::WeaponInfos>
{
    MT_COPYNOTALLOWED( ADN_Weapon_Wizard_Page0 )

public:
    //! @name Constructors/Destructor
    //@{
             ADN_Weapon_Wizard_Page0( QWizard* pParent, const char* szName = 0 );
    virtual ~ADN_Weapon_Wizard_Page0();
    //@}

    //! @name Operations
    //@{
    ADN_Weapons_Data::WeaponInfos* CreateObject();
    //@}

private:
    //! @name Member data
    //@{
    QComboBox* pComboLauncher_;
    QComboBox* pComboAmmo_;
    QRadioButton* pCopyRadioButton_;
    QListView* pWeaponList_;
    //@}
};

#endif // __ADN_Weapon_Wizard_Page0_h_
