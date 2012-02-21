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
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Population_Wizard.cpp $
// $Author: Ape $
// $Modtime: 11/04/05 10:57 $
// $Revision: 6 $
// $Workfile: ADN_Population_Wizard.cpp $
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Population_Wizard.h"

#include "ADN_Workspace.h"
#include "ADN_Population_Data.h"
#include "ADN_Wizard_FirstPage_Default.h"
#include "ADN_Wizard_LastPage.h"

typedef ADN_Population_Data::PopulationInfos PopulationInfos;

// -----------------------------------------------------------------------------
// Name: ADN_Population_Wizard constructor
// Created: AGN 2003-11-03
// -----------------------------------------------------------------------------
ADN_Population_Wizard::ADN_Population_Wizard( QWidget* pParent, const char* szName )
: ADN_Wizard_ABC<PopulationInfos>( pParent, szName )
{
    setCaption( qApp->translate( "ADN_Population_Wizard", "New crowd creation" ) );

    ADN_Population_Data::T_PopulationInfosVector& automata = ADN_Workspace::GetWorkspace().GetPopulation().GetData().GetPopulation();
    ADN_Wizard_FirstPage_Default<PopulationInfos>* pFirstPage = new ADN_Wizard_FirstPage_Default<PopulationInfos>( automata, this );
    pFirstPage->SetCaptions( qApp->translate( "ADN_Population_Wizard", "New crowd creation" ), qApp->translate( "ADN_Population_Wizard", "Crowd" ) );
    pFirstPage_ = pFirstPage;

    new ADN_Wizard_LastPage( this, qApp->translate( "ADN_Population_Wizard",  "Creation finished" ),
        qApp->translate( "ADN_Population_Wizard", "Click \"Done\" to create the new crowd." ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_Population_Wizard destructor
// Created: AGN 2003-11-03
// -----------------------------------------------------------------------------
ADN_Population_Wizard::~ADN_Population_Wizard()
{
}

