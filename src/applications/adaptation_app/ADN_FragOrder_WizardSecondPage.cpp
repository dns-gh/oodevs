// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_FragOrder_WizardSecondPage.h"
#include "ADN_CheckBox.h"   
#include "ADN_Models_Data.h"

// -----------------------------------------------------------------------------
// Name: ADN_FragOrder_WizardSecondPage constructor
// Created: HBD 2010-09-01
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// Name: ADN_FragOrder_WizardSecondPage constructor
// Created: HBD 2010-09-02
// -----------------------------------------------------------------------------
ADN_FragOrder_WizardSecondPage::ADN_FragOrder_WizardSecondPage( QWizard* pParent, const char* szName /*= 0*/, const char* szImageName /*= "adn_wizard.bmp"*/ )
  : ADN_WizardPage_ABC( pParent, szName, szImageName )
{
    QVBox* groupbox = new QVBox( pRightSide_ );
    QHBox* box1 = new QHBox( groupbox );
    addForAllUnits_ = new QCheckBox( box1 );
    QLabel* label1 = new QLabel( box1 );
    label1->setText( tr( "Add for all Units" ) );
    QHBox* box2 = new QHBox( groupbox );
    addForAllAutomata_ = new QCheckBox( box2 );
    QLabel* label2 = new QLabel( box2,  tr("Add for all Automata") );
    label2->setText( tr( "Add for all Automata" ) );
    QHBox* box3 = new QHBox( groupbox );
    addForAllPops_ = new QCheckBox( box3 );
    QLabel* label3 = new QLabel( box3,  tr("Add for all Populations") );
    label3->setText( tr( "Add for all Populations" ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_FragOrder_WizardSecondPage destructor
// Created: HBD 2010-09-01
// -----------------------------------------------------------------------------
ADN_FragOrder_WizardSecondPage::~ADN_FragOrder_WizardSecondPage()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_FragOrder_WizardSecondPage::AddFragOrderForAll
// Created: HBD 2010-09-01
// -----------------------------------------------------------------------------
void ADN_FragOrder_WizardSecondPage::AddFragOrderForAll( const std::string& name )
{
    if ( addForAllUnits_->isChecked() )
    {
        ADN_Missions_Data::FragOrder* fragorder = ADN_Workspace::GetWorkspace().GetMissions().GetData().FindFragOrder( name );
        ADN_Models_Data::T_ModelInfos_Vector& units = ADN_Workspace::GetWorkspace().GetModels().GetData().GetUnitModelsInfos();
        for( ADN_Models_Data::IT_ModelInfos_Vector it1 = units.begin(); it1 != units.end(); ++it1 )
            (*it1)->AddFragOrder( fragorder, name );                  
    }
    if ( addForAllAutomata_->isChecked() )
    {
        ADN_Missions_Data::FragOrder* fragorder = ADN_Workspace::GetWorkspace().GetMissions().GetData().FindFragOrder( name );
        ADN_Models_Data::T_ModelInfos_Vector& automata = ADN_Workspace::GetWorkspace().GetModels().GetData().GetAutomataModelsInfos();
        for( ADN_Models_Data::IT_ModelInfos_Vector it1 = automata.begin(); it1 != automata.end(); ++it1 )
            (*it1)->AddFragOrder( fragorder, name );    
    }
     if ( addForAllPops_->isChecked() )
    {
        ADN_Missions_Data::FragOrder* fragorder = ADN_Workspace::GetWorkspace().GetMissions().GetData().FindFragOrder( name );
        ADN_Models_Data::T_ModelInfos_Vector& pops = ADN_Workspace::GetWorkspace().GetModels().GetData().GetPopulationModelsInfos();
        for( ADN_Models_Data::IT_ModelInfos_Vector it1 = pops.begin(); it1 != pops.end(); ++it1 )
            (*it1)->AddFragOrder( fragorder, name );
    }
}
