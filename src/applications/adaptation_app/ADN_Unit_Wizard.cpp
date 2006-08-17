// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGN 2003-11-03 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Unit_Wizard.cpp $
// $Author: Ape $
// $Modtime: 11/04/05 11:00 $
// $Revision: 7 $
// $Workfile: ADN_Unit_Wizard.cpp $
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Unit_Wizard.h"

#include "ADN_Workspace.h"
#include "ADN_Units_Data.h"
#include "ADN_Wizard_FirstPage_Default.h"
#include "ADN_Wizard_LastPage.h"

typedef ADN_Units_Data::UnitInfos UnitInfos;

// -----------------------------------------------------------------------------
// Name: ADN_Unit_Wizard constructor
// Created: AGN 2003-12-02
// -----------------------------------------------------------------------------
ADN_Unit_Wizard::ADN_Unit_Wizard( QWidget* pParent, const char* szName )
: ADN_Wizard_ABC<ADN_Units_Data::UnitInfos>( pParent, szName )
{
    setCaption( tr( "New unit creation" ) );

    ADN_Units_Data::T_UnitInfos_Vector& units = ADN_Workspace::GetWorkspace().GetUnits().GetData().GetUnitsInfos();
    ADN_Wizard_FirstPage_Default<UnitInfos>* pFirstPage = new ADN_Wizard_FirstPage_Default<UnitInfos>( units, this );
    pFirstPage->SetCaptions( tr( "New unit creation" ), tr( "Units" ) );
    pFirstPage_ = pFirstPage;

    new ADN_Wizard_LastPage( this, tr( "Creation completed" ), tr( "Click \"Done\" to create the new unit." ) );
}


// -----------------------------------------------------------------------------
// Name: ADN_Unit_Wizard destructor
// Created: AGN 2003-12-02
// -----------------------------------------------------------------------------
ADN_Unit_Wizard::~ADN_Unit_Wizard()
{
}
