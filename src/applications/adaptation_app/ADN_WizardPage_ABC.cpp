// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGN 2003-11-18 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_WizardPage_ABC.cpp $
// $Author: Ape $
// $Modtime: 11/01/05 11:25 $
// $Revision: 2 $
// $Workfile: ADN_WizardPage_ABC.cpp $
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_WizardPage_ABC.h"
#include "ADN_ImageViewer.h"


// -----------------------------------------------------------------------------
// Name: ADN_WizardPage_ABC constructor
// Created: AGN 2003-11-18
// -----------------------------------------------------------------------------
ADN_WizardPage_ABC::ADN_WizardPage_ABC( Q3Wizard* pParent, const char* name, const char* imageName )
    : Q3HBox  ( pParent, name )
    , pWizard_( pParent )
{
    setSpacing( 30 );

    pImage_ = new ADN_ImageViewer( imageName, true, this );
    pImage_->setFixedSize( 131, 324 );

    pRightSide_ = new Q3VBox( this );
}


// -----------------------------------------------------------------------------
// Name: ADN_WizardPage_ABC destructor
// Created: AGN 2003-11-18
// -----------------------------------------------------------------------------
ADN_WizardPage_ABC::~ADN_WizardPage_ABC()
{
}
