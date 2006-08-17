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
// $Archive: /MVW_v10/Build/SDK/ADN2/src/ADN_Breakdown_Wizard.cpp $
// $Author: Nld $
// $Modtime: 27/04/05 10:54 $
// $Revision: 3 $
// $Workfile: ADN_Breakdown_Wizard.cpp $
//
// *****************************************************************************


#include "ADN_pch.h"
#include "ADN_Breakdown_Wizard.h"

#include "ADN_Workspace.h"
#include "ADN_Breakdowns_Data.h"
#include "ADN_Wizard_LastPage.h"
#include "ADN_Wizard_FirstPage_Default.h"

typedef ADN_Breakdowns_Data::BreakdownInfo BreakdownInfo;


// -----------------------------------------------------------------------------
// Name: ADN_Breakdown_Wizard constructor
// Created: AGN 2003-11-03
// -----------------------------------------------------------------------------
ADN_Breakdown_Wizard::ADN_Breakdown_Wizard( QWidget* pParent, const char* szName )
: ADN_Wizard_ABC<BreakdownInfo>( pParent, szName )
{
    setCaption( tr( "New breakdown creation" ) );

    ADN_Breakdowns_Data::T_BreakdownInfoVector& breakdowns = ADN_Workspace::GetWorkspace().GetBreakdowns().GetData().vBreakdowns_;
    ADN_Wizard_FirstPage_Default<BreakdownInfo>* pFirstPage = new ADN_Wizard_FirstPage_Default<BreakdownInfo>( breakdowns, this );
    pFirstPage->SetCaptions( tr( "New breakdown creation" ), tr( "Breakdowns" ) );
    pFirstPage_ = pFirstPage;

    new ADN_Wizard_LastPage( this, tr( "Creation finished" ), tr( "Click \"Done\" to create the new breakdown." ) );
}


// -----------------------------------------------------------------------------
// Name: ADN_Breakdown_Wizard destructor
// Created: AGN 2003-11-03
// -----------------------------------------------------------------------------
ADN_Breakdown_Wizard::~ADN_Breakdown_Wizard()
{
}

