// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_ActiveProtections_Wizard.h"
#include "ADN_Workspace.h"
#include "ADN_ActiveProtections_Data.h"
#include "ADN_Wizard_FirstPage_Default.h"
#include "ADN_Wizard_LastPage.h"

typedef ADN_ActiveProtections_Data::ActiveProtectionsInfos ActiveProtectionsInfos;

// -----------------------------------------------------------------------------
// Name: ADN_ActiveProtections_Wizard constructor
// Created: LGY 2010-08-18
// -----------------------------------------------------------------------------
ADN_ActiveProtections_Wizard::ADN_ActiveProtections_Wizard( QWidget* pParent, const char* szName )
    : ADN_Wizard_ABC< ActiveProtectionsInfos >( pParent, szName )
{
    setCaption( qApp->translate( "ADN_ActiveProtections_Wizard", "New active protection creation" ) );
    ADN_ActiveProtections_Data::T_ActiveProtectionsInfosVector& automata = ADN_Workspace::GetWorkspace().GetActiveProtections().GetData().GetActiveProtectionsInfos();
    ADN_Wizard_FirstPage_Default< ActiveProtectionsInfos >* pFirstPage = new ADN_Wizard_FirstPage_Default< ActiveProtectionsInfos >( automata, this );
    pFirstPage->SetCaptions( qApp->translate( "ADN_ActiveProtections_Wizard", "New active protection creation" ), qApp->translate( "ADN_ActiveProtections_Wizard", "Active protection" ) );
    pFirstPage_ = pFirstPage;
    new ADN_Wizard_LastPage( this, qApp->translate( "ADN_ActiveProtections_Wizard",  "Creation finished" ),
        qApp->translate( "ADN_ActiveProtections_Wizard", "Click \"Done\" to create the new active protection." ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_ActiveProtections_Wizard destructor
// Created: LGY 2010-08-18
// -----------------------------------------------------------------------------
ADN_ActiveProtections_Wizard::~ADN_ActiveProtections_Wizard()
{
    // NOTHING
}
