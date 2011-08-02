// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-01-11 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Wizard_LastPage.cpp $
// $Author: Ape $
// $Modtime: 19/01/05 17:21 $
// $Revision: 2 $
// $Workfile: ADN_Wizard_LastPage.cpp $
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Wizard_LastPage.h"

// -----------------------------------------------------------------------------
// Name: ADN_Wizard_LastPage constructor
// Created: APE 2005-01-11
// -----------------------------------------------------------------------------
ADN_Wizard_LastPage::ADN_Wizard_LastPage( Q3Wizard* pParent, const QString& strTitle, const QString& strText, const char* szName, const char* szImageName )
: ADN_WizardPage_ABC( pParent, szName, szImageName )
{
    new QLabel( strText, pRightSide_ );

    pParent->addPage( this, strTitle );
    pParent->setBackEnabled( this, pParent->indexOf( this ) != 1 ); // Prevent going back to the first page (index 0).
    pParent->setNextEnabled( this, false );
    pParent->setHelpEnabled( this, false );
    pParent->setFinishEnabled( this, true );
}


// -----------------------------------------------------------------------------
// Name: ADN_Wizard_LastPage destructor
// Created: APE 2005-01-11
// -----------------------------------------------------------------------------
ADN_Wizard_LastPage::~ADN_Wizard_LastPage()
{
}
