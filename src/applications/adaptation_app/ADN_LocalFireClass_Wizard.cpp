// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_LocalFireClass_Wizard.h"
#include "ADN_Wizard_FirstPage_Default.h"
#include "ADN_Wizard_LastPage.h"

// -----------------------------------------------------------------------------
// Name: ADN_LocalFireClass_Wizard constructor
// Created: JSR 2010-09-14
// -----------------------------------------------------------------------------
ADN_LocalFireClass_Wizard::ADN_LocalFireClass_Wizard( QWidget* pParent , const char* szName )
    : ADN_Wizard_ABC< ADN_LocalFireClass_Data::LocalFireClassInfos >( pParent, szName )
{
    setCaption( qApp->translate( "ADN_LocalFireClass_Wizard", "New fire creation" ) );

    ADN_LocalFireClass_Data::T_LocalFireClassInfosVector& classes = ADN_Workspace::GetWorkspace().GetLocalFireClasses().GetData().GetLocalFireClassesInfos();
    ADN_Wizard_FirstPage_Default< ADN_LocalFireClass_Data::LocalFireClassInfos >* pFirstPage = new ADN_Wizard_FirstPage_Default< ADN_LocalFireClass_Data::LocalFireClassInfos >( classes, this );
    pFirstPage->SetCaptions( qApp->translate( "ADN_LocalFireClass_Wizard", "New fire creation" ),
        qApp->translate( "ADN_LocalFireClass_Wizard", "Fire" ) );
    pFirstPage_ = pFirstPage;

    new ADN_Wizard_LastPage( this, qApp->translate( "ADN_LocalFireClass_Wizard", "Creation finished" ),
        qApp->translate( "ADN_LocalFireClass_Wizard", "Click \"Done\" to create the new fire." ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_LocalFireClass_Wizard destructor
// Created: JSR 2010-09-14
// -----------------------------------------------------------------------------
ADN_LocalFireClass_Wizard::~ADN_LocalFireClass_Wizard()
{
    // NOTHING
}
