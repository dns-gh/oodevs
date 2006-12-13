// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Mission_Wizard.h"
#include "ADN_Wizard_FirstPage_Default.h"
#include "ADN_Wizard_LastPage.h"

typedef ADN_Missions_Data::Mission Mission;

// -----------------------------------------------------------------------------
// Name: ADN_Mission_Wizard constructor
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
ADN_Mission_Wizard::ADN_Mission_Wizard( ADN_Missions_Data::T_Mission_Vector& missions, QWidget* pParent /*= 0*/, const char* szName /*= 0*/ )
    : ADN_Wizard_ABC< Mission >( pParent, szName )
{
    setCaption( tr( "New mission creation" ) );

    ADN_Wizard_FirstPage_Default< Mission >* pFirstPage = new ADN_Wizard_FirstPage_Default< Mission >( missions, this );
    pFirstPage->SetCaptions( tr( "New mission creation" ), tr( "Missions" ) );
    pFirstPage_ = pFirstPage;
    new ADN_Wizard_LastPage( this, tr( "Creation completed" ), tr( "Click \"Done\" to create the new mission." ) );
}
    
// -----------------------------------------------------------------------------
// Name: ADN_Mission_Wizard destructor
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
ADN_Mission_Wizard::~ADN_Mission_Wizard()
{
    // NOTHING
}
