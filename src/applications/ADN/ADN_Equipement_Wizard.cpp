// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-01-19 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Equipement_Wizard.cpp $
// $Author: Ape $
// $Modtime: 11/04/05 14:54 $
// $Revision: 4 $
// $Workfile: ADN_Equipement_Wizard.cpp $
//
// *****************************************************************************

#include "ADN_pch.h"
#include "ADN_Equipement_Wizard.h"

#include "ADN_Equipement_Wizard_Page0.h"
#include "ADN_Wizard_LastPage.h"

typedef ADN_Equipement_Data::CategoryInfo CategoryInfos;

// -----------------------------------------------------------------------------
// Name: ADN_Equipement_Wizard constructor
// Created: APE 2005-01-20
// -----------------------------------------------------------------------------
ADN_Equipement_Wizard::ADN_Equipement_Wizard( ADN_Equipement_Data::DotationInfos& parentDotation, QWidget* pParent, const char* szName )
: ADN_Wizard_ABC<CategoryInfos>( pParent, szName )
{
    setCaption( tr( "New equipement creation" ) );

    ADN_Equipement_Wizard_Page0* pFirstPage = new ADN_Equipement_Wizard_Page0( parentDotation, this );
    pFirstPage->SetCaptions( tr( "New equipement creation" ), tr( "Equipements" ) );
    pFirstPage_ = pFirstPage;

    new ADN_Wizard_LastPage( this, tr( "Creation completed" ), tr( "Click \"Done\" to create the new equipement category." ) );
}


// -----------------------------------------------------------------------------
// Name: ADN_Equipement_Wizard destructor
// Created: APE 2005-01-20
// -----------------------------------------------------------------------------
ADN_Equipement_Wizard::~ADN_Equipement_Wizard()
{
}
