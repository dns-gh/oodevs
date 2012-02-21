// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_FireClass_Wizard.h"
#include "ADN_Wizard_FirstPage_Default.h"
#include "ADN_Wizard_LastPage.h"

// -----------------------------------------------------------------------------
// Name: ADN_FireClass_Wizard constructor
// Created: JSR 2010-09-14
// -----------------------------------------------------------------------------
ADN_FireClass_Wizard::ADN_FireClass_Wizard( QWidget* pParent , const char* szName )
    : ADN_Wizard_ABC< ADN_FireClass_Data::FireClassInfos >( pParent, szName )
{
    setCaption( qApp->translate( "ADN_FireClass_Wizard", "New fire creation" ) );

    ADN_FireClass_Data::T_FireClassInfosVector& classes = ADN_Workspace::GetWorkspace().GetFireClasses().GetData().GetFireClassesInfos();
    ADN_Wizard_FirstPage_Default< ADN_FireClass_Data::FireClassInfos >* pFirstPage = new ADN_Wizard_FirstPage_Default< ADN_FireClass_Data::FireClassInfos >( classes, this );
    pFirstPage->SetCaptions( qApp->translate( "ADN_FireClass_Wizard", "New fire creation" ),
        qApp->translate( "ADN_FireClass_Wizard", "Fire" ) );
    pFirstPage_ = pFirstPage;

    new ADN_Wizard_LastPage( this, qApp->translate( "ADN_FireClass_Wizard", "Creation finished" ),
        qApp->translate( "ADN_FireClass_Wizard", "Click \"Done\" to create the new fire." ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_FireClass_Wizard destructor
// Created: JSR 2010-09-14
// -----------------------------------------------------------------------------
ADN_FireClass_Wizard::~ADN_FireClass_Wizard()
{
    // NOTHING
}
