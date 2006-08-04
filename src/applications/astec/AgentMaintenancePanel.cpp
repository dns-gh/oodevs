// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2005-04-01 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/AgentMaintenancePanel.cpp $
// $Author: Nld $
// $Modtime: 27/04/05 18:28 $
// $Revision: 3 $
// $Workfile: AgentMaintenancePanel.cpp $
//
// *****************************************************************************

#include "astec_pch.h"
#include "AgentMaintenancePanel.h"
#include "LogConsignListView.h"
#include "LogMaintenanceConsign.h"
#include "LogMaintenanceConsign_ListView_Item.h"
#include "Agent.h"
#include "App.h"

// -----------------------------------------------------------------------------
// Name: AgentMaintenancePanel constructor
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
AgentMaintenancePanel::AgentMaintenancePanel( QWidget* pParent )
    : AgentLogisticPanel_ABC( pParent )
{
    pConsignListView_       = new T_List( this, false );
    pConsignHandledListView_= new T_List( this, true );
    pConsignListView_       ->hide();
    pConsignHandledListView_->hide();

    pState_ = new QListView( this, tr( "Etat chaine maintenance" ) );
    pState_->addColumn( tr( "Etat" ) );
    pState_->addColumn( "" );
    pState_->setMargin( 5 );
    pState_->setLineWidth( 2 );
    pState_->setFrameStyle( QFrame::Sunken | QFrame::Box );
    pStateChainEnabled_      = new QListViewItem( pState_, tr( "Etat chaine" ), " - " );
    pStateWorkRate_          = new QListViewItem( pState_, pStateChainEnabled_, tr( "Régime de travail" ), " - " );
    pStatePriorites_         = new QListViewItem( pState_, pStateWorkRate_    , tr( "Priorités" ), " - " );  
    pStateTacticalPriorites_ = new QListViewItem( pState_, pStatePriorites_   , tr( "Priorités tactiques" ), " - " );  
    pState_->hide();

    pDispoHaulers_ = new QListView( this, tr( "Disponibilités remorqueurs" ) );
    pDispoHaulers_->addColumn( tr( "Disponibilités remorqueurs" ) );
    pDispoHaulers_->addColumn( tr( "Total" ) );
    pDispoHaulers_->addColumn( tr( "Disponibles" ) );
    pDispoHaulers_->addColumn( tr( "Au travail" ) );
    pDispoHaulers_->addColumn( tr( "Au repos" ) );
    pDispoHaulers_->addColumn( tr( "Prêtés" ) );
    pDispoHaulers_->setMargin( 5 );
    pDispoHaulers_->setLineWidth( 2 );
    pDispoHaulers_->setFrameStyle( QFrame::Sunken | QFrame::Box );
    pDispoHaulers_->setSorting( -1, FALSE );
    pDispoHaulers_->hide();

    pDispoRepairers_ = new QListView( this, tr( "Disponibilités réparateurs" ) );
    pDispoRepairers_->addColumn( tr( "Disponibilités réparateurs" ) );
    pDispoRepairers_->addColumn( tr( "Total" ) );
    pDispoRepairers_->addColumn( tr( "Disponibles" ) );
    pDispoRepairers_->addColumn( tr( "Au travail" ) );
    pDispoRepairers_->addColumn( tr( "Au repos" ) );
    pDispoRepairers_->addColumn( tr( "Prêtés" ) );
    pDispoRepairers_->setMargin( 5 );
    pDispoRepairers_->setLineWidth( 2 );
    pDispoRepairers_->setFrameStyle( QFrame::Sunken | QFrame::Box );
    pDispoRepairers_->setSorting( -1, FALSE );
    pDispoRepairers_->hide();

    connect( &App::GetApp(), SIGNAL( AgentUpdated( Agent& ) ), this, SLOT( OnAgentUpdated( Agent& ) ) );
}

// -----------------------------------------------------------------------------
// Name: AgentMaintenancePanel destructor
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
AgentMaintenancePanel::~AgentMaintenancePanel()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentMaintenancePanel::OnUpdate
// Created: AGE 2005-04-05
// -----------------------------------------------------------------------------
void AgentMaintenancePanel::OnUpdate()
{
    if( selectedItem_.pAgent_ != 0 )
        OnAgentUpdated( *selectedItem_.pAgent_ );
    else
        OnClearSelection();
}

// -----------------------------------------------------------------------------
// Name: AgentMaintenancePanel::OnClearSelection
// Created: SBO 2005-09-22
// -----------------------------------------------------------------------------
void AgentMaintenancePanel::OnClearSelection()
{
    pConsignListView_       ->hide();
    pConsignHandledListView_->hide();
    pState_                 ->hide();
    pDispoHaulers_          ->hide();
    pDispoRepairers_        ->hide();
}

// -----------------------------------------------------------------------------
// Name: AgentMaintenancePanel::OnAgentUpdated
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentMaintenancePanel::OnAgentUpdated( Agent& agent )
{
    if( ! ShouldDisplay( agent ) )
        return;

    DisplayConsigns( agent.requestedMaintenances_, *pConsignListView_ );
    DisplayConsigns( agent.handledMaintenances_, *pConsignHandledListView_ );

    if( ! agent.pMaintenanceData_ )
    {
        pState_->hide();
        pDispoHaulers_->hide();
        pDispoRepairers_->hide();
        return;
    }

    Agent::T_MaintenanceData& data = *agent.pMaintenanceData_;
    pStateChainEnabled_->setText( 1, data.bChainEnabled_ ? tr( "Activée" ) : tr( "Désactivée" ) );
    pStateWorkRate_    ->setText( 1, ( QString( "R%1" ) ).arg( data.nWorkRate_ + 1 ) );
    DisplayPriorities( data.priorities_, *pStatePriorites_, EquipmentResolver() );    
    DisplayPriorities( data.tacticalPriorities_, *pStateTacticalPriorites_, AutomateResolver() );

    pState_->show();

    DisplayLogAvailabilities( data.dispoHaulers_, *pDispoHaulers_, EquipmentResolver() );
    DisplayLogAvailabilities( data.dispoRepairers_, *pDispoRepairers_, EquipmentResolver() );
}
