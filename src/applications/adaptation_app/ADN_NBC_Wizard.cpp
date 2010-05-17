// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-02-14 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_NBC_Wizard.cpp $
// $Author: Ape $
// $Modtime: 11/04/05 14:54 $
// $Revision: 4 $
// $Workfile: ADN_NBC_Wizard.cpp $
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_NBC_Wizard.h"


#include "ADN_Workspace.h"
#include "ADN_NBC_Datas.h"
#include "ADN_Wizard_FirstPage_Default.h"
#include "ADN_Wizard_LastPage.h"

typedef ADN_NBC_Datas::NbcAgentInfos NbcAgentInfos;


// -----------------------------------------------------------------------------
// Name: ADN_NBC_Wizard constructor
// Created: AGN 2003-11-03
// -----------------------------------------------------------------------------
ADN_NBC_Wizard::ADN_NBC_Wizard( QWidget* pParent, const char* szName )
: ADN_Wizard_ABC<NbcAgentInfos>( pParent, szName )
{
    setCaption( qApp->translate( "ADN_NBC_Wizard",  "New NBC creation" ) );

    ADN_NBC_Datas::T_NbcAgentInfos_Vector& nbcs = ADN_Workspace::GetWorkspace().GetNbc().GetData().GetNbcAgentVector();
    ADN_Wizard_FirstPage_Default<NbcAgentInfos>* pFirstPage = new ADN_Wizard_FirstPage_Default<NbcAgentInfos>( nbcs, this );
    pFirstPage->SetCaptions( qApp->translate( "ADN_NBC_Wizard", "New NBC creation" ), qApp->translate( "ADN_NBC_Wizard",  "NBCs" ) );
    pFirstPage_ = pFirstPage;

    new ADN_Wizard_LastPage( this, qApp->translate( "ADN_NBC_Wizard", "Creation finished" ),
        qApp->translate( "ADN_NBC_Wizard",  "Click \"Done\" to create the new NBC." ) );
}


// -----------------------------------------------------------------------------
// Name: ADN_NBC_Wizard destructor
// Created: AGN 2003-11-03
// -----------------------------------------------------------------------------
ADN_NBC_Wizard::~ADN_NBC_Wizard()
{
}

