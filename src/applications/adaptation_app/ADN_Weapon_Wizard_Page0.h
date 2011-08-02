// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Weapon_Wizard_Page0_h_
#define __ADN_Weapon_Wizard_Page0_h_

#include "ADN_Wizard_FirstPage_ABC.h"
#include "ADN_Weapons_Data.h"

class QComboBox;
class QRadioButton;
class Q3ListView;

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
class ADN_Weapon_Wizard_Page0 : public ADN_Wizard_FirstPage_ABC<ADN_Weapons_Data::WeaponInfos>
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_Weapon_Wizard_Page0( Q3Wizard* pParent, const char* szName = 0 );
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
    Q3ListView* pWeaponList_;
    //@}
};

#endif // __ADN_Weapon_Wizard_Page0_h_
