// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_KnowledgeGroups_Wizard.h"
#include "ADN_Workspace.h"
#include "ADN_KnowledgeGroups_Data.h"
#include "ADN_Wizard_FirstPage_Default.h"
#include "ADN_Wizard_LastPage.h"

typedef ADN_KnowledgeGroups_Data::GroupInfo GroupInfo;

// -----------------------------------------------------------------------------
// Name: ADN_KnowledgeGroups_Wizard constructor
// Created: LGY 2010-08-18
// -----------------------------------------------------------------------------
ADN_KnowledgeGroups_Wizard::ADN_KnowledgeGroups_Wizard( QWidget* pParent, const char* szName )
    : ADN_Wizard_ABC< GroupInfo >( pParent, szName )
{
    setCaption( qApp->translate( "ADN_KnowledgeGroups_Wizard", "New knowledge group creation" ) );
    ADN_KnowledgeGroups_Data::T_GroupInfoVector& knowledge = ADN_Workspace::GetWorkspace().GetKnowledgeGroups().GetData().GetGroupInfos();
    ADN_Wizard_FirstPage_Default< GroupInfo >* pFirstPage = new ADN_Wizard_FirstPage_Default< GroupInfo >( knowledge, this );
    pFirstPage->SetCaptions( qApp->translate( "ADN_KnowledgeGroups_Wizard", "New knowledge group creation" ),qApp->translate( "ADN_KnowledgeGroups_Wizard", "Knowledge group" ) );
    pFirstPage_ = pFirstPage;
    new ADN_Wizard_LastPage( this,qApp->translate( "ADN_KnowledgeGroups_Wizard", "Creation finished" ),
        qApp->translate( "ADN_KnowledgeGroups_Wizard", "Click \"Done\" to create the new knowledge group." ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_KnowledgeGroups_Wizard destructor
// Created: LGY 2010-08-18
// -----------------------------------------------------------------------------
ADN_KnowledgeGroups_Wizard::~ADN_KnowledgeGroups_Wizard()
{
    // NOTHING
}
