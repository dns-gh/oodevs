// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-03-10 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_InfoPanel.cpp $
// $Author: Age $
// $Modtime: 21/04/05 16:05 $
// $Revision: 10 $
// $Workfile: MOS_InfoPanel.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "MOS_Light2_pch.h"
#include "MOS_InfoPanel.h"
#include "moc_MOS_InfoPanel.cpp"

#include "MOS_MainWindow.h"
#include "MOS_AgentStatePanel.h"
#include "MOS_ReportPanel.h"
#include "MOS_AgentResourcesPanel.h"
#include "MOS_AgentKnowledgePanel.h"
#include "MOS_DynaObjectPanel.h"
#include "MOS_DynaObjectKnowledgePanel.h"
#include "MOS_AgentMaintenancePanel.h"
#include "MOS_AgentMedicalPanel.h"
#include "MOS_AgentSupplyPanel.h"
#include "MOS_SelectedElement.h"
#include "MOS_AgentKnowledge.h"
#include "MOS_DynaObjectKnowledge.h"
#include "MOS_App.h"
#include "MOS_Gtia.h"
#include "MOS_Agent.h"

#ifndef MOS_USE_INLINE
#   include "MOS_InfoPanel.inl"
#endif


// -----------------------------------------------------------------------------
// Name: MOS_InfoPanel constructor
/** @param  pParent 
*/
// Created: APE 2004-03-10
// -----------------------------------------------------------------------------
MOS_InfoPanel::MOS_InfoPanel( QWidget* pParent )
    : QWidgetStack                  ( pParent )
    , bAgentVisible_                ( false )
    , bAgentKnowledgeVisible_       ( false )
    , bDynaObjectKnowledgeVisible_  ( false )
    , bDynaObjectVisible_           ( false )
    , bLogisiticVisible_            ( false )
{
    this->setMinimumSize( 1, 1 );

    pStatePanel_               = new MOS_AgentStatePanel( this );
    pResourcesPanel_           = new MOS_AgentResourcesPanel( this );
    pAgentKnowledgePanel_      = new MOS_AgentKnowledgePanel( this );
    pDynaObjectPanel_          = new MOS_DynaObjectPanel( this );
    pDynaObjectKnowledgePanel_ = new MOS_DynaObjectKnowledgePanel( this );
    pAgentMaintenancePanel_    = new MOS_AgentMaintenancePanel( this );
    pAgentMedicalPanel_        = new MOS_AgentMedicalPanel( this );
    pAgentSupplyPanel_         = new MOS_AgentSupplyPanel( this );
    pReportPanel_              = new MOS_ReportPanel( this );

    pTabWidget_ = new QTabWidget( this );

    ShowAgentPanel( true );
    ShowAgentKnowledgePanel( true );
    ShowObjectKnowledgePanel( true );

    addWidget( pTabWidget_ );
    raiseWidget( pTabWidget_ );
    pTabWidget_->setCurrentPage( 0 );

    connect( &MOS_MainWindow::GetMainWindow(), SIGNAL( ElementSelected( MOS_SelectedElement& ) ), this, SLOT( SetSelectedElement( MOS_SelectedElement& ) ) );

    // Whatever the connexion status change is, we must clear the panels.
    connect( &MOS_App::GetApp(), SIGNAL( ConnexionStatusChanged( bool ) ), this, SLOT( ClearAll() ) );
}


// -----------------------------------------------------------------------------
// Name: MOS_InfoPanel destructor
// Created: APE 2004-03-10
// -----------------------------------------------------------------------------
MOS_InfoPanel::~MOS_InfoPanel()
{
}


// -----------------------------------------------------------------------------
// Name: MOS_InfoPanel::SetSelectedElement
/** @param  pAgent 
*/
// Created: APE 2004-03-10
// -----------------------------------------------------------------------------
void MOS_InfoPanel::SetSelectedElement( MOS_SelectedElement& selectedElement )
{
    disconnect( &MOS_MainWindow::GetMainWindow(), SIGNAL( ElementSelected( MOS_SelectedElement& ) ), this, SLOT( SetSelectedElement( MOS_SelectedElement& ) ) );

    if( selectedElement.pAgent_ != 0 )
    {
        ShowAgentPanel( true );
        ShowAgentKnowledgePanel( true );
        ShowObjectKnowledgePanel( true );
        ShowLogisticPanels( true );
        ShowObjectPanel( false );

        pTabWidget_->setCurrentPage( 0 );
    }

    else if( selectedElement.pGtia_ != 0 )
    {
        ShowAgentPanel( false );
        ShowAgentKnowledgePanel( true );
        ShowObjectKnowledgePanel( true );
        ShowLogisticPanels( false );
        ShowObjectPanel( false );
    }

    else if( selectedElement.pTeam_ != 0 )
    {
        ShowAgentPanel( false );
        ShowAgentKnowledgePanel( false );
        ShowLogisticPanels( false );
        ShowObjectKnowledgePanel( true );
        ShowObjectPanel( false );
    }

    else if( selectedElement.pAgentKnowledge_ != 0 )
    {
//        if( ! ( bAgentVisible_ 
//             && selectedElement.pAgent_ != 0 
//             && &(selectedElement.pAgent_->GetGtia()) == &(selectedElement.pAgentKnowledge_->GetOwner()) ) )
            ShowAgentPanel( false );
        ShowAgentKnowledgePanel( true );
        ShowObjectKnowledgePanel( true );
        ShowObjectPanel( false );
        ShowLogisticPanels( false );
    }

    else if( selectedElement.pDynaObjectKnowledge_ != 0 )
    {
//        if( ! (bAgentVisible_ 
//            && selectedElement.pAgent_ != 0 && &(pStatePanel_->GetAgent()->GetTeam()) ==  &(selectedElement.pDynaObjectKnowledge_->GetOwner()) ) )
//            ShowAgentPanel( false );
//
//        if( ! (bAgentKnowledgeVisible_ && pAgentKnowledgePanel_->GetGtia() != 0 && &(pAgentKnowledgePanel_->GetGtia()->GetTeam()) ==  &(selectedElement.pDynaObjectKnowledge_->GetOwner()) ) )
//            ShowAgentPanel( false );

        ShowObjectKnowledgePanel( true );
        ShowObjectPanel( false );
        ShowLogisticPanels( false );
    }

    else if( selectedElement.pDynaObject_ != 0 )
    {
        ShowAgentPanel( false );
        ShowAgentKnowledgePanel( false );
        ShowObjectKnowledgePanel( false );
        ShowObjectPanel( true );
        ShowLogisticPanels( false );
        pTabWidget_->setCurrentPage( 0 );
    }

    else
    {
        ShowAgentPanel( true );
        ShowAgentKnowledgePanel( true );
        ShowObjectKnowledgePanel( true );
        ShowObjectPanel( false );
        pTabWidget_->setCurrentPage( 0 );
    }

    pStatePanel_              ->SetSelection( selectedElement );
    pReportPanel_             ->SetSelection( selectedElement );
    pResourcesPanel_          ->SetSelection( selectedElement );
    pAgentKnowledgePanel_     ->SetSelection( selectedElement );
    pDynaObjectPanel_         ->SetSelection( selectedElement );
    pDynaObjectKnowledgePanel_->SetSelection( selectedElement );
    pAgentMaintenancePanel_   ->SetSelection( selectedElement );
    pAgentMedicalPanel_       ->SetSelection( selectedElement );
    pAgentSupplyPanel_        ->SetSelection( selectedElement );

    connect( &MOS_MainWindow::GetMainWindow(), SIGNAL( ElementSelected( MOS_SelectedElement& ) ), this, SLOT( SetSelectedElement( MOS_SelectedElement& ) ) );
}

// -----------------------------------------------------------------------------
// Name: MOS_InfoPanel::ClearAll
// Created: APE 2004-05-26
// -----------------------------------------------------------------------------
void MOS_InfoPanel::ClearAll()
{
    MOS_SelectedElement nullSelection;
    pStatePanel_              ->SetSelection( nullSelection );
    pReportPanel_             ->SetSelection( nullSelection );
    pResourcesPanel_          ->SetSelection( nullSelection );
    pAgentKnowledgePanel_     ->SetSelection( nullSelection );
    pDynaObjectPanel_         ->SetSelection( nullSelection );
    pDynaObjectKnowledgePanel_->SetSelection( nullSelection );
    pAgentMaintenancePanel_   ->SetSelection( nullSelection );
    pAgentMedicalPanel_       ->SetSelection( nullSelection );
    pAgentSupplyPanel_        ->SetSelection( nullSelection );
    this->raiseWidget( pTabWidget_ );
}

// -----------------------------------------------------------------------------
// Name: MOS_InfoPanel::ShowLogisticPanels
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void MOS_InfoPanel::ShowLogisticPanels( bool bShow )
{
    if( bShow && ! bLogisiticVisible_ )
    {
        bLogisiticVisible_ = true;
        pTabWidget_->insertTab( pAgentSupplyPanel_,      tr( "Ch. rav." ) );
        pTabWidget_->insertTab( pAgentMaintenancePanel_, tr( "Ch. maint." ) );
        pTabWidget_->insertTab( pAgentMedicalPanel_,     tr( "Ch. medicale" ) );
    }

    if( !bShow && bLogisiticVisible_ )
    {
        bLogisiticVisible_ = false;
        pTabWidget_->removePage( pAgentSupplyPanel_ );
        pTabWidget_->removePage( pAgentMaintenancePanel_ );
        pTabWidget_->removePage( pAgentMedicalPanel_ );
    }
}


// -----------------------------------------------------------------------------
// Name: MOS_InfoPanel::ShowAgentPanel
// Created: APE 2004-08-03
// -----------------------------------------------------------------------------
void MOS_InfoPanel::ShowAgentPanel( bool bShow )
{
    if( bShow && ! bAgentVisible_ )
    {
        bAgentVisible_ = true;
        pTabWidget_->insertTab( pStatePanel_, tr( "Etat" ), 0 );
        pTabWidget_->insertTab( pReportPanel_, tr( "Rapports" ), 0 );
        pTabWidget_->insertTab( pResourcesPanel_, tr( "Res." ), 1 );
    }

    if( !bShow && bAgentVisible_ )
    {
        bAgentVisible_ = false;
        pTabWidget_->removePage( pStatePanel_ );
        pTabWidget_->removePage( pReportPanel_ );
        pTabWidget_->removePage( pResourcesPanel_ );
    }
}


// -----------------------------------------------------------------------------
// Name: MOS_InfoPanel::ShowAgentKnowledgePanel
// Created: APE 2004-08-03
// -----------------------------------------------------------------------------
void MOS_InfoPanel::ShowAgentKnowledgePanel( bool bShow )
{
    if( bShow && ! bAgentKnowledgeVisible_ )
    {
        bAgentKnowledgeVisible_ = true;
        pTabWidget_->insertTab( pAgentKnowledgePanel_, tr( "C. agent" ) );
    }

    if( !bShow && bAgentKnowledgeVisible_ )
    {
        bAgentKnowledgeVisible_ = false;
        pTabWidget_->removePage( pAgentKnowledgePanel_ );
    }
}


// -----------------------------------------------------------------------------
// Name: MOS_InfoPanel::ShowObjectKnowledgePanel
// Created: APE 2004-08-03
// -----------------------------------------------------------------------------
void MOS_InfoPanel::ShowObjectKnowledgePanel( bool bShow )
{
    if( bShow && ! bDynaObjectKnowledgeVisible_ )
    {
        bDynaObjectKnowledgeVisible_ = true;
        pTabWidget_->insertTab( pDynaObjectKnowledgePanel_, tr( "C. objet" ) );
    }

    if( ! bShow && bDynaObjectKnowledgeVisible_ )
    {
        bDynaObjectKnowledgeVisible_ = false;
        pTabWidget_->removePage( pDynaObjectKnowledgePanel_ );
    }
}


// -----------------------------------------------------------------------------
// Name: MOS_InfoPanel::ShowObjectPanel
// Created: APE 2004-08-03
// -----------------------------------------------------------------------------
void MOS_InfoPanel::ShowObjectPanel( bool bShow )
{
    if( bShow && ! bDynaObjectVisible_ )
    {
        bDynaObjectVisible_ = true;
        pTabWidget_->insertTab( pDynaObjectPanel_, tr( "Objet" ) );
    }

    if( ! bShow && bDynaObjectVisible_ )
    {
        bDynaObjectVisible_ = false;
        pTabWidget_->removePage( pDynaObjectPanel_ );
    }
}
