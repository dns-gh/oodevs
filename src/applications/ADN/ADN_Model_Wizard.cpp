// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGN 2003-12-03 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Model_Wizard.cpp $
// $Author: Ape $
// $Modtime: 11/04/05 11:00 $
// $Revision: 7 $
// $Workfile: ADN_Model_Wizard.cpp $
//
// *****************************************************************************

#include "ADN_pch.h"
#include "ADN_Model_Wizard.h"

#include "ADN_Workspace.h"
#include "ADN_Wizard_FirstPage_Default.h"
#include "ADN_Wizard_LastPage.h"

typedef ADN_Models_Data::ModelInfos ModelInfos;

// -----------------------------------------------------------------------------
// Name: ADN_Model_Wizard constructor
// Created: AGN 2003-11-03
// -----------------------------------------------------------------------------
ADN_Model_Wizard::ADN_Model_Wizard( bool bForAutomata, QWidget* pParent, const char* szName )
: ADN_Wizard_ABC<ModelInfos>( pParent, szName )
{
    setCaption( tr( "New model creation" ) );

    ADN_Models_Data::T_ModelInfos_Vector* pUnitsList;
    if( bForAutomata )
        pUnitsList = & ADN_Workspace::GetWorkspace().GetModels().GetData().GetAutomataModelsInfos();
    else
        pUnitsList = & ADN_Workspace::GetWorkspace().GetModels().GetData().GetUnitModelsInfos();

    ADN_Wizard_FirstPage_Default<ModelInfos>* pFirstPage = new ADN_Wizard_FirstPage_Default<ModelInfos>( *pUnitsList, this );
    pFirstPage->SetCaptions( tr( "New model creation" ), tr( "Models" ) );
    pFirstPage_ = pFirstPage;

    new ADN_Wizard_LastPage( this, tr( "Creation completed" ), tr( "Click \"Done\" to create the new model." ) );
}


// -----------------------------------------------------------------------------
// Name: ADN_Model_Wizard destructor
// Created: AGN 2003-11-03
// -----------------------------------------------------------------------------
ADN_Model_Wizard::~ADN_Model_Wizard()
{
}
