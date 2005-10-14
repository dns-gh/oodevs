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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_AgentSupplyPanel.cpp $
// $Author: Age $
// $Modtime: 6/04/05 10:41 $
// $Revision: 2 $
// $Workfile: MOS_AgentSupplyPanel.cpp $
//
// *****************************************************************************

#include "MOS_Light2_pch.h"
#include "MOS_AgentSupplyPanel.h"
#include "MOS_LogConsignListView.h"
#include "MOS_LogSupplyConsign.h"
#include "MOS_LogSupplyConsign_ListView_Item.h"
#include "MOS_Agent.h"
#include "MOS_App.h"

// -----------------------------------------------------------------------------
// Name: MOS_AgentSupplyPanel constructor
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
MOS_AgentSupplyPanel::MOS_AgentSupplyPanel( QWidget* pParent )
    : MOS_AgentLogisticPanel_ABC( pParent )
{
    pConsignListView_       = new T_List( this, false );
    pConsignHandledListView_= new T_List( this, true );
    pConsignHandledListView_->hide();

    pState_ = new QListView( this, tr( "Etat chaine rav." ) );
    pState_->addColumn( tr( "Etat" ) );
    pState_->addColumn( "" );
    pState_->setMargin( 5 );
    pState_->setLineWidth( 2 );
    pState_->setFrameStyle( QFrame::Sunken | QFrame::Box );
    pStateChainEnabled_ = new QListViewItem( pState_, tr( "Etat chaine" ), " - " );
    pState_->hide();

    pStocks_ = new QListView( this, tr( "Stocks" ) );
    pStocks_->addColumn( tr( "Stock" ) );
    pStocks_->addColumn( tr( "Quantité" ) );
    pStocks_->setMargin( 5 );
    pStocks_->setLineWidth( 2 );
    pStocks_->setFrameStyle( QFrame::Sunken | QFrame::Box );
    pStocks_->hide();

    pQuotas_ = new QListView( this, tr( "Quotas" ) );
    pQuotas_->addColumn( tr( "Type" ) );
    pQuotas_->addColumn( tr( "Quota" ) );
    pQuotas_->setMargin( 5 );
    pQuotas_->setLineWidth( 2 );
    pQuotas_->setFrameStyle( QFrame::Sunken | QFrame::Box );
    pQuotas_->hide();
    
    pDispoTransporters_ = new QListView( this, tr( "Disponibilités transporteurs pour convois" ) );
    pDispoTransporters_->addColumn( tr( "Disponibilités transporteurs pour convois" ) );
    pDispoTransporters_->addColumn( "" );
    pDispoTransporters_->setMargin( 5 );
    pDispoTransporters_->setLineWidth( 2 );
    pDispoTransporters_->setFrameStyle( QFrame::Sunken | QFrame::Box );
    pDispoTransporters_->setSorting( -1, FALSE );
    pDispoTransporters_->hide();

    pDispoCommanders_ = new QListView( this, tr( "Disponibilités chefs de convois" ) );
    pDispoCommanders_->addColumn( tr( "Disponibilités chefs de convois" ) );
    pDispoCommanders_->addColumn( "" );
    pDispoCommanders_->setMargin( 5 );
    pDispoCommanders_->setLineWidth( 2 );
    pDispoCommanders_->setFrameStyle( QFrame::Sunken | QFrame::Box );
    pDispoCommanders_->setSorting( -1, FALSE );
    pDispoCommanders_->hide();

    connect( &MOS_App::GetApp(), SIGNAL( AgentUpdated( MOS_Agent& ) ), this, SLOT( OnAgentUpdated( MOS_Agent& ) ) );
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentSupplyPanel destructor
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
MOS_AgentSupplyPanel::~MOS_AgentSupplyPanel()
{
    //NOTHING
}

namespace
{
    class ResourceResolver
    {
    public:
        std::string GetName( unsigned int n ) const
        {
            return MOS_App::GetApp().GetResourceName( n );
        }
    };
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentSupplyPanel::OnUpdate
// Created: AGE 2005-04-05
// -----------------------------------------------------------------------------
void MOS_AgentSupplyPanel::OnUpdate()
{
    if( selectedItem_.pAgent_ != 0 )
        OnAgentUpdated( *selectedItem_.pAgent_ );
    else
        OnClearSelection();
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentSupplyPanel::OnClearSelection
// Created: SBO 2005-09-22
// -----------------------------------------------------------------------------
void MOS_AgentSupplyPanel::OnClearSelection()
{
    pConsignListView_       ->hide();
    pConsignHandledListView_->hide();
    pState_                 ->hide();
    pStocks_                ->hide();
    pQuotas_                ->hide();
    pDispoTransporters_     ->hide();
    pDispoCommanders_       ->hide();
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentSupplyPanel::OnAgentUpdated
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void MOS_AgentSupplyPanel::OnAgentUpdated( MOS_Agent& agent )
{
    if( ! ShouldDisplay( agent ) )
        return;
    DisplayConsigns( agent.requestedSupplies_, *pConsignListView_ );
    DisplayConsigns( agent.handledSupplies_,   *pConsignHandledListView_ );

    if( ! agent.pSupplyData_ )
    {
        pState_->hide();
        pStocks_->hide();
        pQuotas_->hide();
        pDispoTransporters_->hide();
        pDispoCommanders_->hide();
        return;
    }

    MOS_Agent::T_SupplyData& data = *agent.pSupplyData_;
    pStateChainEnabled_->setText( 1, data.bChainEnabled_ ? tr( "Activée" ) : tr( "Désactivée" ) );

    DisplayAvailabilities( data.dispoCommanders_, *pDispoCommanders_, EquipmentResolver(), "%");
    DisplayAvailabilities( data.dispoTransporters_, *pDispoTransporters_, EquipmentResolver(), "%" );
    DisplayAvailabilities( data.stocks_, *pStocks_, ResourceResolver(), "" );
    DisplayAvailabilities( data.quotas_, *pQuotas_, ResourceResolver(), "" );

    pState_->show();
}
