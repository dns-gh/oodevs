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
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Automata_Wizard.cpp $
// $Author: Ape $
// $Modtime: 11/04/05 10:57 $
// $Revision: 6 $
// $Workfile: ADN_Automata_Wizard.cpp $
//
// *****************************************************************************


#include "adaptation_app_pch.h"
#include "ADN_Automata_Wizard.h"

#include "ADN_Workspace.h"
#include "ADN_Automata_Data.h"
#include "ADN_Wizard_FirstPage_Default.h"
#include "ADN_Wizard_LastPage.h"

typedef ADN_Automata_Data::AutomatonInfos AutomatonInfos;


// -----------------------------------------------------------------------------
// Name: ADN_Automata_Wizard constructor
// Created: AGN 2003-11-03
// -----------------------------------------------------------------------------
ADN_Automata_Wizard::ADN_Automata_Wizard( QWidget* pParent, const char* szName )
: ADN_Wizard_ABC<AutomatonInfos>( pParent, szName )
{
    setCaption( tr( "New automaton creation" ) );

    ADN_Automata_Data::T_AutomatonInfosVector& automata = ADN_Workspace::GetWorkspace().GetAutomata().GetData().GetAutomata();
    ADN_Wizard_FirstPage_Default<AutomatonInfos>* pFirstPage = new ADN_Wizard_FirstPage_Default<AutomatonInfos>( automata, this );
    pFirstPage->SetCaptions( tr( "New automaton creation" ), tr( "Automata" ) );
    pFirstPage_ = pFirstPage;

    new ADN_Wizard_LastPage( this, tr( "Creation finished" ), tr( "Click \"Done\" to create the new automaton." ) );
}


// -----------------------------------------------------------------------------
// Name: ADN_Automata_Wizard destructor
// Created: AGN 2003-11-03
// -----------------------------------------------------------------------------
ADN_Automata_Wizard::~ADN_Automata_Wizard()
{
}

