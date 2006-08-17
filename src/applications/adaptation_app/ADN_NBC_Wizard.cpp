// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
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

#include "ADN_pch.h"
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
    setCaption( tr( "New NBC creation" ) );

    ADN_NBC_Datas::T_NbcAgentInfos_Vector& nbcs = ADN_Workspace::GetWorkspace().GetNbc().GetData().GetNbcAgentVector();
    ADN_Wizard_FirstPage_Default<NbcAgentInfos>* pFirstPage = new ADN_Wizard_FirstPage_Default<NbcAgentInfos>( nbcs, this );
    pFirstPage->SetCaptions( tr( "New NBC creation" ), tr( "NBCs" ) );
    pFirstPage_ = pFirstPage;

    new ADN_Wizard_LastPage( this, tr( "Creation finished" ), tr( "Click \"Done\" to create the new NBC." ) );
}


// -----------------------------------------------------------------------------
// Name: ADN_NBC_Wizard destructor
// Created: AGN 2003-11-03
// -----------------------------------------------------------------------------
ADN_NBC_Wizard::~ADN_NBC_Wizard()
{
}

