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
#include "MOS_ObjectPanel.h"
#include "MOS_ObjectReportPanel.h"
#include "MOS_ObjectKnowledgePanel.h"
#include "MOS_PopulationPanel.h"
#include "MOS_PopulationKnowledgePanel.h"
#include "MOS_AgentMaintenancePanel.h"
#include "MOS_AgentMedicalPanel.h"
#include "MOS_AgentSupplyPanel.h"
#include "MOS_SelectedElement.h"
#include "MOS_AgentKnowledge.h"
#include "MOS_ObjectKnowledge.h"
#include "MOS_PopulationKnowledge.h"
#include "MOS_App.h"
#include "MOS_Gtia.h"
#include "MOS_Agent.h"

// -----------------------------------------------------------------------------
// Name: MOS_InfoPanel constructor
// Created: APE 2004-03-10
// -----------------------------------------------------------------------------
MOS_InfoPanel::MOS_InfoPanel( QWidget* pParent )
    : QWidgetStack                 ( pParent )
    , bAgentVisible_               ( false )
    , bAgentKnowledgeVisible_      ( false )
    , bObjectKnowledgeVisible_     ( false )
    , bPopulationKnowledgeVisible_ ( false )
    , bObjectVisible_              ( false )
    , bLogisiticVisible_           ( false )
	, bPopulationVisible_		   ( false )
{
    this->setMinimumSize( 1, 1 );

    pStatePanel_               = new MOS_AgentStatePanel( this );
    pResourcesPanel_           = new MOS_AgentResourcesPanel( this );
    pAgentKnowledgePanel_      = new MOS_AgentKnowledgePanel( this );
    pObjectPanel_              = new MOS_ObjectPanel( this );
    pObjectReportPanel_        = new MOS_ObjectReportPanel( this );
    pObjectKnowledgePanel_     = new MOS_ObjectKnowledgePanel( this );
    pAgentMaintenancePanel_    = new MOS_AgentMaintenancePanel( this );
    pAgentMedicalPanel_        = new MOS_AgentMedicalPanel( this );
    pAgentSupplyPanel_         = new MOS_AgentSupplyPanel( this );
    pReportPanel_              = new MOS_ReportPanel( this );
	pPopulationPanel_		   = new MOS_PopulationPanel( this );
    pPopulationReportPanel_    = new MOS_ReportPanel( this );
    pPopulationKnowledgePanel_ = new MOS_PopulationKnowledgePanel( this );

    pTabWidget_ = new QTabWidget( this );

    ShowAgentPanel              ( true );
    ShowAgentKnowledgePanel     ( true );
    ShowObjectKnowledgePanel    ( true );
    ShowPopulationKnowledgePanel( true );

    addWidget  ( pTabWidget_ );
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
// Created: APE 2004-03-10
// -----------------------------------------------------------------------------
void MOS_InfoPanel::SetSelectedElement( MOS_SelectedElement& selectedElement )
{
    disconnect( &MOS_MainWindow::GetMainWindow(), SIGNAL( ElementSelected( MOS_SelectedElement& ) ), this, SLOT( SetSelectedElement( MOS_SelectedElement& ) ) );

    QWidget* pPreviousPage = pTabWidget_->currentPage();

    if( selectedElement.pAgent_ != 0 )
    {
        ShowAgentPanel              ( true  );
        ShowAgentKnowledgePanel     ( true  );
        ShowPopulationPanel         ( false );
        ShowPopulationKnowledgePanel( true  );
        ShowObjectPanel             ( false );
        ShowObjectKnowledgePanel    ( true  );
        ShowLogisticPanels          ( true  );
    }

    else if( selectedElement.pGtia_ != 0 )
    {
        ShowAgentPanel              ( false );
        ShowAgentKnowledgePanel     ( true  );
        ShowPopulationPanel         ( false );
        ShowPopulationKnowledgePanel( true  );
        ShowObjectPanel             ( false );
        ShowObjectKnowledgePanel    ( true  );
        ShowLogisticPanels          ( false );
    }

    else if( selectedElement.pTeam_ != 0 && selectedElement.pPopulation_ == 0 )
    {
        ShowAgentPanel              ( false );
        ShowAgentKnowledgePanel     ( false );
        ShowPopulationPanel         ( false );
        ShowPopulationKnowledgePanel( false );
        ShowObjectPanel             ( false );
        ShowObjectKnowledgePanel    ( true  );
        ShowLogisticPanels          ( false );
    }

	else if( selectedElement.pPopulation_ != 0 )
    {
        ShowAgentPanel              ( false );
        ShowAgentKnowledgePanel     ( false );
        ShowPopulationPanel         ( true  );
        ShowPopulationKnowledgePanel( false );
        ShowObjectPanel             ( false );
        ShowObjectKnowledgePanel    ( false );
        ShowLogisticPanels          ( false );
    }

    else if( selectedElement.pPopulationKnowledge_ != 0 )
    {
        ShowAgentPanel              ( false );
        ShowAgentKnowledgePanel     ( false );
        ShowPopulationPanel         ( false );
        ShowPopulationKnowledgePanel( true  );
        ShowObjectPanel             ( false );
        ShowObjectKnowledgePanel    ( false );
        ShowLogisticPanels          ( false );
    }

    else if( selectedElement.pAgentKnowledge_ != 0 )
    {
        ShowAgentPanel              ( false );
        ShowAgentKnowledgePanel     ( true  );
        ShowPopulationPanel         ( false );
        ShowPopulationKnowledgePanel( false );
        ShowObjectPanel             ( false );
        ShowObjectKnowledgePanel    ( false );
        ShowLogisticPanels          ( false );
    }

    else if( selectedElement.pObjectKnowledge_ != 0 )
    {
        ShowAgentPanel              ( false );
        ShowAgentKnowledgePanel     ( false );
        ShowPopulationPanel         ( false );
        ShowPopulationKnowledgePanel( false );
        ShowObjectPanel             ( false );
        ShowObjectKnowledgePanel    ( true  );
        ShowLogisticPanels          ( false );
    }

    else if( selectedElement.pObject_ != 0 )
    {
        ShowAgentPanel              ( false );
        ShowAgentKnowledgePanel     ( false );
        ShowPopulationPanel         ( false );
        ShowPopulationKnowledgePanel( false );
        ShowObjectPanel             ( true  );
        ShowObjectKnowledgePanel    ( false );
        ShowLogisticPanels          ( false );
    }

    if( pTabWidget_->count() > 0 )
    {
        int nPreviousPage = pTabWidget_->indexOf( pPreviousPage );
        if( nPreviousPage == -1 )
            pTabWidget_->setCurrentPage( 0 );
        else
            pTabWidget_->setCurrentPage( nPreviousPage );
    }

    pStatePanel_              ->SetSelection( selectedElement );
    pReportPanel_             ->SetSelection( selectedElement );
    pResourcesPanel_          ->SetSelection( selectedElement );
    pAgentKnowledgePanel_     ->SetSelection( selectedElement );

    pPopulationPanel_	      ->SetSelection( selectedElement );
    pPopulationReportPanel_   ->SetSelection( selectedElement );
    pPopulationKnowledgePanel_->SetSelection( selectedElement );

    pObjectPanel_             ->SetSelection( selectedElement );
    pObjectReportPanel_       ->SetSelection( selectedElement );
    pObjectKnowledgePanel_    ->SetSelection( selectedElement );

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
    MOS_SelectedElement       nullSelection;
    pStatePanel_              ->SetSelection( nullSelection );
    pReportPanel_             ->SetSelection( nullSelection );
    pResourcesPanel_          ->SetSelection( nullSelection );
    pAgentKnowledgePanel_     ->SetSelection( nullSelection );

    pPopulationPanel_         ->SetSelection( nullSelection );
    pPopulationReportPanel_   ->SetSelection( nullSelection );
    pPopulationKnowledgePanel_->SetSelection( nullSelection );

    pObjectPanel_             ->SetSelection( nullSelection );
    pObjectReportPanel_       ->SetSelection( nullSelection );
    pObjectKnowledgePanel_    ->SetSelection( nullSelection );
    
    pAgentMaintenancePanel_   ->SetSelection( nullSelection );
    pAgentMedicalPanel_       ->SetSelection( nullSelection );
    pAgentSupplyPanel_        ->SetSelection( nullSelection );
    //this->raiseWidget( pTabWidget_ );
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
        pTabWidget_->insertTab( pReportPanel_, tr( "Rapports" ), 1 );
        pTabWidget_->insertTab( pResourcesPanel_, tr( "Res." ), 2 );
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
// Name: MOS_InfoPanel::ShowPopulationPanel
// Created: HME 2005-10-03
// -----------------------------------------------------------------------------
void MOS_InfoPanel::ShowPopulationPanel( bool bShow )
{
    if( bShow && ! bPopulationVisible_ )
    {
        bPopulationVisible_ = true;
        pTabWidget_->insertTab( pPopulationPanel_, tr( "Etat Population" ), 0 );
        pTabWidget_->insertTab( pPopulationReportPanel_, tr( "Rapports" ), 1 );
    }

    if( !bShow && bPopulationVisible_ )
    {
        bPopulationVisible_ = false;
        pTabWidget_->removePage( pPopulationPanel_ );
        pTabWidget_->removePage( pPopulationReportPanel_ );
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
    if( bShow && ! bObjectKnowledgeVisible_ )
    {
        bObjectKnowledgeVisible_ = true;
        pTabWidget_->insertTab( pObjectKnowledgePanel_, tr( "C. objet" ) );
    }

    if( ! bShow && bObjectKnowledgeVisible_ )
    {
        bObjectKnowledgeVisible_ = false;
        pTabWidget_->removePage( pObjectKnowledgePanel_ );
    }
}


// -----------------------------------------------------------------------------
// Name: MOS_InfoPanel::ShowObjectPanel
// Created: APE 2004-08-03
// -----------------------------------------------------------------------------
void MOS_InfoPanel::ShowObjectPanel( bool bShow )
{
    if( bShow && ! bObjectVisible_ )
    {
        bObjectVisible_ = true;
        pTabWidget_->insertTab( pObjectPanel_, tr( "Objet" ), 0 );
        pTabWidget_->insertTab( pObjectReportPanel_, tr( "Rapports" ), 1 );
    }

    if( ! bShow && bObjectVisible_ )
    {
        bObjectVisible_ = false;
        pTabWidget_->removePage( pObjectPanel_ );
        pTabWidget_->removePage( pObjectReportPanel_ );
    }
}

// -----------------------------------------------------------------------------
// Name: MOS_InfoPanel::ShowPopulationKnowledgePanel
// Created: SBO 2005-10-19
// -----------------------------------------------------------------------------
void MOS_InfoPanel::ShowPopulationKnowledgePanel( bool bShow )
{
    if( bShow && ! bPopulationKnowledgeVisible_ )
    {
        bPopulationKnowledgeVisible_ = true;
        pTabWidget_->insertTab( pPopulationKnowledgePanel_, tr( "C. popu" ) );
    }

    if( ! bShow && bPopulationKnowledgeVisible_ )
    {
        bPopulationKnowledgeVisible_ = false;
        pTabWidget_->removePage( pPopulationKnowledgePanel_ );
    }
}