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
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Weapon_Wizard.cpp $
// $Author: Ape $
// $Modtime: 28/01/05 16:07 $
// $Revision: 2 $
// $Workfile: ADN_Weapon_Wizard.cpp $
//
// *****************************************************************************

#include "ADN_pch.h"
#include "ADN_Weapon_Wizard.h"

#include "ADN_Weapon_Wizard_Page0.h"
#include "ADN_Wizard_LastPage.h"


// -----------------------------------------------------------------------------
// Name: ADN_Weapon_Wizard constructor
// Created: APE 2005-01-12
// -----------------------------------------------------------------------------
ADN_Weapon_Wizard::ADN_Weapon_Wizard( QWidget* pParent, const char* szName )
: ADN_Wizard_ABC<ADN_Weapons_Data::WeaponInfos>( pParent, szName )
{
    setCaption( tr( "New weapon creation" ) );

    pFirstPage_ = new ADN_Weapon_Wizard_Page0( this );
    new ADN_Wizard_LastPage( this, tr( "Creation completed" ), tr( "Click \"Done\" to create the new weapon." ) );
}


// -----------------------------------------------------------------------------
// Name: ADN_Weapon_Wizard destructor
// Created: APE 2005-01-12
// -----------------------------------------------------------------------------
ADN_Weapon_Wizard::~ADN_Weapon_Wizard()
{
}
