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
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Launcher_Wizard.cpp $
// $Author: Ape $
// $Modtime: 11/04/05 14:54 $
// $Revision: 9 $
// $Workfile: ADN_Launcher_Wizard.cpp $
//
// *****************************************************************************


#include "adaptation_app_pch.h"
#include "ADN_Launcher_Wizard.h"
#include "ADN_Workspace.h"
#include "ADN_Launchers_Data.h"
#include "ADN_Wizard_FirstPage_Default.h"
#include "ADN_Wizard_LastPage.h"

typedef ADN_Launchers_Data::LauncherInfos LauncherInfos;

// -----------------------------------------------------------------------------
// Name: ADN_Launcher_Wizard constructor
// Created: AGN 2003-11-03
// -----------------------------------------------------------------------------
ADN_Launcher_Wizard::ADN_Launcher_Wizard( QWidget* pParent, const char* szName )
: ADN_Wizard_ABC<ADN_Launchers_Data::LauncherInfos>( pParent, szName )
{
    setCaption( qApp->translate( "ADN_Launcher_Wizard", "New launcher creation" ) );

    ADN_Launchers_Data::T_LauncherInfos_Vector& launchers = ADN_Workspace::GetWorkspace().GetLaunchers().GetData().GetLaunchersInfos();
    ADN_Wizard_FirstPage_Default<LauncherInfos>* pFirstPage = new ADN_Wizard_FirstPage_Default<LauncherInfos>( launchers, this );
    pFirstPage->SetCaptions( qApp->translate( "ADN_Launcher_Wizard", "New launcher creation" ),
        qApp->translate( "ADN_Launcher_Wizard","Launchers" ) );
    pFirstPage_ = pFirstPage;

    new ADN_Wizard_LastPage( this, qApp->translate( "ADN_Launcher_Wizard", "Creation finished" ),
        qApp->translate( "ADN_Launcher_Wizard", "Click \"Done\" to create the new launcher." ) );
}


// -----------------------------------------------------------------------------
// Name: ADN_Launcher_Wizard destructor
// Created: AGN 2003-11-03
// -----------------------------------------------------------------------------
ADN_Launcher_Wizard::~ADN_Launcher_Wizard()
{
}

