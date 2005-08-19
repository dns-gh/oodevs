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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_AgentMaintenancePanel.cpp $
// $Author: Nld $
// $Modtime: 27/04/05 18:28 $
// $Revision: 3 $
// $Workfile: MOS_AgentMaintenancePanel.cpp $
//
// *****************************************************************************

#include "MOS_Light2_pch.h"
#include "MOS_AgentMaintenancePanel.h"
#include "MOS_LogConsignListView.h"
#include "MOS_LogMaintenanceConsign.h"
#include "MOS_LogMaintenanceConsign_ListView_Item.h"
#include "MOS_Agent.h"
#include "MOS_App.h"

// -----------------------------------------------------------------------------
// Name: MOS_AgentMaintenancePanel constructor
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
MOS_AgentMaintenancePanel::MOS_AgentMaintenancePanel( QWidget* pParent )
    : MOS_AgentLogisticPanel_ABC( pParent )
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
    pStateTempsBordee_       = new QListViewItem( pState_, pStateChainEnabled_, tr( "Temps de bordée" ), " - " );
    pStatePriorites_         = new QListViewItem( pState_, pStateTempsBordee_ , tr( "Priorités" ), " - " );  
    pStateTacticalPriorites_ = new QListViewItem( pState_, pStatePriorites_   , tr( "Priorités tactiques" ), " - " );  
    pState_->hide();

    pDispoHaulers_ = new QListView( this, tr( "Disponibilités remorqueurs" ) );
    pDispoHaulers_->addColumn( tr( "Disponibilités remorqueurs" ) );
    pDispoHaulers_->addColumn( "" );
    pDispoHaulers_->setMargin( 5 );
    pDispoHaulers_->setLineWidth( 2 );
    pDispoHaulers_->setFrameStyle( QFrame::Sunken | QFrame::Box );
    pDispoHaulers_->setSorting( -1, FALSE );
    pDispoHaulers_->hide();

    pDispoRepairers_ = new QListView( this, tr( "Disponibilités réparateurs" ) );
    pDispoRepairers_->addColumn( tr( "Disponibilités réparateurs" ) );
    pDispoRepairers_->addColumn( "" );
    pDispoRepairers_->setMargin( 5 );
    pDispoRepairers_->setLineWidth( 2 );
    pDispoRepairers_->setFrameStyle( QFrame::Sunken | QFrame::Box );
    pDispoRepairers_->setSorting( -1, FALSE );
    pDispoRepairers_->hide();

    connect( &MOS_App::GetApp(), SIGNAL( AgentUpdated( MOS_Agent& ) ), this, SLOT( OnAgentUpdated( MOS_Agent& ) ) );
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentMaintenancePanel destructor
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
MOS_AgentMaintenancePanel::~MOS_AgentMaintenancePanel()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentMaintenancePanel::OnUpdate
// Created: AGE 2005-04-05
// -----------------------------------------------------------------------------
void MOS_AgentMaintenancePanel::OnUpdate()
{
    OnAgentUpdated( *selectedItem_.pAgent_ );
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentMaintenancePanel::OnAgentUpdated
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void MOS_AgentMaintenancePanel::OnAgentUpdated( MOS_Agent& agent )
{
    if( ! ShouldDisplay( agent ) )
        return;

    DisplayConsigns( agent.requestedMaintenances_, *pConsignListView_ );
    DisplayConsigns( agent.handledMaintenances_, *pConsignHandledListView_ );

    if( ! agent.pMaintenanceData_ )
    {
        pConsignHandledListView_->hide();
        pState_->hide();
        pDispoHaulers_->hide();
        pDispoRepairers_->hide();
        return;
    }

    MOS_Agent::T_MaintenanceData& data = *agent.pMaintenanceData_;
    pStateChainEnabled_->setText( 1, data.bChainEnabled_ ? tr( "Activée" ) : tr( "Désactivée" ) );
    pStateTempsBordee_ ->setText( 1, ( QString( "%1 " ) + tr( "heures" ) ).arg( data.nTempsBordee_ ) );
    DisplayPriorities( data.priorities_, *pStatePriorites_, EquipmentResolver() );    
    DisplayPriorities( data.tacticalPriorities_, *pStateTacticalPriorites_, AutomateResolver() );

    pState_->show();

    DisplayAvailabilities( data.dispoHaulers_, *pDispoHaulers_, EquipmentResolver(), "%" );
    DisplayAvailabilities( data.dispoRepairers_, *pDispoRepairers_, EquipmentResolver(), "%" );
}
