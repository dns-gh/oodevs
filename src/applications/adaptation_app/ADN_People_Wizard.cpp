// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_People_Wizard.h"
#include "ADN_Workspace.h"
#include "ADN_People_Data.h"
#include "ADN_Wizard_FirstPage_Default.h"
#include "ADN_Wizard_LastPage.h"

typedef ADN_People_Data::PeopleInfos PeopleInfos;

// -----------------------------------------------------------------------------
// Name: ADN_People_Wizard constructor
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
ADN_People_Wizard::ADN_People_Wizard( QWidget* pParent, const char* szName )
    : ADN_Wizard_ABC< PeopleInfos >( pParent, szName )
{
    setCaption( qApp->translate( "ADN_People_Wizard", "New population creation" ) );

    ADN_People_Data::T_PeopleInfosVector& automata = ADN_Workspace::GetWorkspace().GetPeople().GetData().GetPeople();
    ADN_Wizard_FirstPage_Default< PeopleInfos >* pFirstPage = new ADN_Wizard_FirstPage_Default< PeopleInfos >( automata, this );
    pFirstPage->SetCaptions( qApp->translate( "ADN_People_Wizard", "New population creation" ), qApp->translate( "ADN_People_Wizard", "Population" ) );
    pFirstPage_ = pFirstPage;

    new ADN_Wizard_LastPage( this, qApp->translate( "ADN_People_Wizard",  "Creation finished" ),
        qApp->translate( "ADN_People_Wizard", "Click \"Done\" to create the new population." ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_People_Wizard destructor
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
ADN_People_Wizard::~ADN_People_Wizard()
{
    // NOTHING
}
