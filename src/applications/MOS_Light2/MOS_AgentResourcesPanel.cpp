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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_AgentResourcesPanel.cpp $
// $Author: Age $
// $Modtime: 6/04/05 11:59 $
// $Revision: 9 $
// $Workfile: MOS_AgentResourcesPanel.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "MOS_Light2_pch.h"
#include "MOS_AgentResourcesPanel.h"

#include "MOS_AgentComposition.h"
#include "MOS_Agent.h"
#include "MOS_App.h"
#include "MOS_Tools.h"

// -----------------------------------------------------------------------------
// Name: MOS_AgentResourcesPanel constructor
/** @param  pParent 
    @param  bCargo 
*/
// Created: APE 2004-03-10
// -----------------------------------------------------------------------------
MOS_AgentResourcesPanel::MOS_AgentResourcesPanel( QWidget* pParent )
    : MOS_InfoPanel_ABC( pParent )
{
    pEquipment_ = new QListView( this );
    pEquipment_->setSorting( -1 );
    pEquipment_->addColumn( tr( "Equipement" ) );
    pEquipment_->addColumn( tr( "Disponible" ) );
    pEquipment_->addColumn( tr( "Indisponible" ) );
    pEquipment_->addColumn( tr( "Réparable" ) );
    pEquipment_->addColumn( tr( "En maintenance" ) );
    pEquipment_->setResizeMode( QListView::LastColumn );
    pEquipment_->setAllColumnsShowFocus ( true );

    pResources_ = new QListView( this );
    pResources_->setSorting( -1 );
    pResources_->addColumn( tr( "Resource" ) );
    pResources_->addColumn( tr( "Quantité" ) );
    pResources_->setResizeMode( QListView::LastColumn );
    pResources_->setAllColumnsShowFocus ( true );

    pTroops_ = new QListView( this );
    pTroops_->setSorting( -1 );
    pTroops_->addColumn( tr( "Catégorie" ) );
    pTroops_->addColumn( tr( "Officiers" ) );
    pTroops_->addColumn( tr( "Sous officiers" ) );
    pTroops_->addColumn( tr( "Mdr" ) );
    pTroops_->setResizeMode( QListView::LastColumn );
    pTroops_->setAllColumnsShowFocus ( true );

    pLends_ = new QListView( this );
    pLends_->setSorting( -1 );
    pLends_->addColumn( tr( "Emprunteur" ) );
    pLends_->addColumn( tr( "Equipement prêté" ) );
    pLends_->addColumn( tr( "Quantité" ) );
    pLends_->setResizeMode( QListView::LastColumn );
    pLends_->setAllColumnsShowFocus ( true );
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentResourcesPanel destructor
// Created: APE 2004-03-10
// -----------------------------------------------------------------------------
MOS_AgentResourcesPanel::~MOS_AgentResourcesPanel()
{
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentResourcesPanel::OnUpdate
// Created: AGE 2005-04-05
// -----------------------------------------------------------------------------
void MOS_AgentResourcesPanel::OnUpdate()
{
    if( selectedItem_.pAgent_ != 0 )
        OnAgentUpdated( *selectedItem_.pAgent_ );
    else
        OnClearSelection();
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentResourcesPanel::OnAgentUpdated
/** @param  agent 
*/
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
void MOS_AgentResourcesPanel::OnAgentUpdated( MOS_Agent& agent )
{
    if( ! ShouldDisplay( agent ) )
        return;

    const MOS_AgentComposition& composition = agent.GetComposition();
    Suit( equipement_, composition.equipment_.size(), pEquipment_ );
    Suit( resources_, composition.resources_.size(), pResources_ );
    Suit( troops_, (int)eTroopHealthStateNbrStates, pTroops_ );
    Suit( lends_, composition.lends_.size(), pLends_ );

    int n1 = 0;
    for( CIT_EquipmentQty_Map it1 = composition.equipment_.begin(); it1 != composition.equipment_.end(); ++it1 )
    {
        QListViewItem* pItem = equipement_[ n1++ ];
        pItem->setText( 0, MOS_App::GetApp().GetEquipmentName( (*it1).first ).c_str() );
        pItem->setText( 1, QString::number( (*it1).second.nNbrAvailable_ ) );
        pItem->setText( 2, QString::number( (*it1).second.nNbrUnavailable_ ) );
        pItem->setText( 3, QString::number( (*it1).second.nNbrReparable_ ) );
        pItem->setText( 4, QString::number( (*it1).second.nNbrInMaintenance_ ) );
    }

    int n2 = 0;
    for( CIT_ResourceQty_Map it2 = composition.resources_.begin(); it2 != composition.resources_.end(); ++it2 )
    {
        QListViewItem* pItem = resources_[ n2++ ];
        pItem->setText( 0, MOS_App::GetApp().GetResourceName( (*it2).first ).c_str() );
        pItem->setText( 1, QString::number( (*it2).second ) );
    }

    for( int n3 = 0; n3 < (int)eTroopHealthStateNbrStates; ++n3 )
    {
        QListViewItem* pItem = troops_[ n3 ];
        pItem->setText( 0, MOS_Tools::ToString( (E_TroopHealthState)n3 ) );
        pItem->setText( 1, QString::number( composition.officiers_[n3] ) );
        pItem->setText( 2, QString::number( composition.sousOfficiers_[n3] ) );
        pItem->setText( 3, QString::number( composition.mdr_[n3] ) );
    }

    for( uint n4 = 0; n4 < composition.lends_.size(); ++n4 )
    {
        QListViewItem* pItem = lends_[ n4 ];
        const MOS_AgentComposition::T_Lend& lend = composition.lends_[ n4 ];
        pItem->setText( 0, QString::number( lend.nBorrowerId_ ) );
        pItem->setText( 1, MOS_App::GetApp().GetEquipmentName( lend.nEquipment_ ).c_str() );
        pItem->setText( 2, QString::number( lend.nQuantity_ ) );
    }
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentResourcesPanel::OnClearSelection
// Created: SBO 2005-09-22
// -----------------------------------------------------------------------------
void MOS_AgentResourcesPanel::OnClearSelection()
{
    pEquipment_->hide();
    pResources_->hide();
    pTroops_   ->hide();
    pLends_    ->hide();
}
