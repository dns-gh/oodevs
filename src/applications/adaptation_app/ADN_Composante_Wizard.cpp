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
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Composante_Wizard.cpp $
// $Author: Ape $
// $Modtime: 11/04/05 11:00 $
// $Revision: 7 $
// $Workfile: ADN_Composante_Wizard.cpp $
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Composante_Wizard.h"

#include "ADN_Wizard_FirstPage_Default.h"
#include "ADN_Wizard_LastPage.h"

#include "ADN_Workspace.h"

typedef ADN_Composantes_Data::ComposanteInfos ComposanteInfos;


// -----------------------------------------------------------------------------
// Name: ADN_Composante_Wizard constructor
// Created: AGN 2003-11-03
// -----------------------------------------------------------------------------
ADN_Composante_Wizard::ADN_Composante_Wizard( QWidget* pParent, const char* szName )
: ADN_Wizard_ABC<ADN_Composantes_Data::ComposanteInfos>( pParent, szName )
{
    setCaption( tr( "New equipment creation" ) );

    ADN_Composantes_Data::T_ComposanteInfos_Vector& composantes = ADN_Workspace::GetWorkspace().GetComposantes().GetData().GetComposantes();
    ADN_Wizard_FirstPage_Default<ComposanteInfos>* pFirstPage = new ADN_Wizard_FirstPage_Default<ComposanteInfos>( composantes, this );
    pFirstPage->SetCaptions( tr( "New equipment creation" ), tr( "Equipments" ) );
    pFirstPage_ = pFirstPage;

    new ADN_Wizard_LastPage( this, tr( "Creation completed" ), tr( "Click \"Done\" to create the new equipment." ) );
}

    
// -----------------------------------------------------------------------------
// Name: ADN_Composante_Wizard destructor
// Created: AGN 2003-11-03
// -----------------------------------------------------------------------------
ADN_Composante_Wizard::~ADN_Composante_Wizard()
{
}
