// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_ArmorCategory_Wizard.h"
#include "ADN_Workspace.h"
#include "ADN_Categories_Data.h"
#include "ADN_Wizard_FirstPage_Default.h"
#include "ADN_Wizard_LastPage.h"

typedef helpers::ArmorInfos ArmorInfos;

// -----------------------------------------------------------------------------
// Name: ADN_ArmorCategory_Wizard constructor
// Created: ABR 2012-04-24
// -----------------------------------------------------------------------------
ADN_ArmorCategory_Wizard::ADN_ArmorCategory_Wizard( QWidget* pParent /*= 0*/, const char* szName /*= 0*/ )
    : ADN_Wizard_ABC< ArmorInfos >( pParent, szName )
{
    setCaption( qApp->translate( "ADN_ArmorCategory_Wizard", "New armor plating creation" ) );
    helpers::T_ArmorInfos_Vector& armors = ADN_Workspace::GetWorkspace().GetCategories().GetData().GetArmorsInfos();
    ADN_Wizard_FirstPage_Default< ArmorInfos >* pFirstPage = new ADN_Wizard_FirstPage_Default< ArmorInfos >( armors, this );
    pFirstPage->SetCaptions( qApp->translate( "ADN_ArmorCategory_Wizard", "New armor plating creation" ), qApp->translate( "ADN_ArmorCategory_Wizard", "Armor plating" ) );
    pFirstPage_ = pFirstPage;
    new ADN_Wizard_LastPage( this, qApp->translate( "ADN_ArmorCategory_Wizard",  "Creation finished" ),
        qApp->translate( "ADN_ArmorCategory_Wizard", "Click \"Done\" to create the new armor plating." ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_ArmorCategory_Wizard destructor
// Created: ABR 2012-04-24
// -----------------------------------------------------------------------------
ADN_ArmorCategory_Wizard::~ADN_ArmorCategory_Wizard()
{
    // NOTHING
}
