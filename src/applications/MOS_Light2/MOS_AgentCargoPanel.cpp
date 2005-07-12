// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-04-29 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_AgentCargoPanel.cpp $
// $Author: Age $
// $Modtime: 5/04/05 17:15 $
// $Revision: 8 $
// $Workfile: MOS_AgentCargoPanel.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "MOS_Light2_Pch.h"
#include "MOS_AgentCargoPanel.h"

#include "MOS_AgentComposition.h"
#include "MOS_Agent.h"
#include "MOS_App.h"
#include "MOS_Tools.h"

#ifndef MOS_USE_INLINE
#   include "MOS_AgentCargoPanel.inl"
#endif


// -----------------------------------------------------------------------------
// Name: MOS_AgentCargoPanel constructor
/** @param  pParent 
*/
// Created: APE 2004-04-29
// -----------------------------------------------------------------------------
MOS_AgentCargoPanel::MOS_AgentCargoPanel( QWidget* pParent )
    : MOS_InfoPanel_ABC( pParent )
{
    this->setMargin( 5 );
    this->setSpacing( 5 );
    QFontMetrics fontMetrics = this->fontMetrics();

    pTransports_ = new QListView( this );
    pTransports_->setSorting( -1 );
    pTransports_->addColumn( tr( "Type dotation" ) );
    pTransports_->addColumn( tr( "% transportable" ) );
    pTransports_->setResizeMode( QListView::LastColumn );
    pTransports_->setAllColumnsShowFocus ( true );

    pEquipment_ = new QListView( this );
    pEquipment_->setSorting( -1 );
    pEquipment_->addColumn( tr( "Equipement" ) );
    pEquipment_->addColumn( tr( "Panne" ) );
    pEquipment_->addColumn( tr( "NTI" ) );
    pEquipment_->addColumn( tr( "Nombre" ) );
    pEquipment_->setResizeMode( QListView::LastColumn );
    pEquipment_->setAllColumnsShowFocus ( true );

    pResources_ = new QListView( this );
    pResources_->setSorting( -1 );
    pResources_->addColumn( tr( "Resource" ) );
    pResources_->addColumn( tr( "Disponible" ) );
    pResources_->addColumn( tr( "En transit" ) );
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
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentCargoPanel destructor
// Created: APE 2004-04-29
// -----------------------------------------------------------------------------
MOS_AgentCargoPanel::~MOS_AgentCargoPanel()
{
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentCargoPanel::OnUpdate
// Created: AGE 2005-04-05
// -----------------------------------------------------------------------------
void MOS_AgentCargoPanel::OnUpdate()
{
    OnAgentUpdated( *selectedItem_.pAgent_ );
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentCargoPanel::OnAgentUpdated
/** @param  agent 
*/
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
void MOS_AgentCargoPanel::OnAgentUpdated( MOS_Agent& agent )
{
    if( ! ShouldDisplay( agent ) )
        return;

    const MOS_AgentComposition& composition = selectedItem_.pAgent_->GetComposition();
    Suit( transports_, composition.transportCapacities_.size(), pTransports_ );
    Suit( equipement_, composition.transportedEquipment_.size(), pEquipment_ );
    Suit( resources_, composition.transportedResources_.size(), pResources_ );
    Suit( troops_, (int)eTroopHealthStateNbrStates, pTroops_ );

    int n = 0;
    for( CIT_ResourceQty_Map it = composition.transportCapacities_.begin(); it != composition.transportCapacities_.end(); ++it )
    {
        QListViewItem* pItem = transports_[ n++ ];
        pItem->setText( 0, MOS_App::GetApp().GetTransportDotationName( (*it).first ).c_str() );
        pItem->setText( 1, QString::number( (*it).second ) );
    }

    int n1 = 0;
    for( CIT_TransportedEquipment_Map it1 = composition.transportedEquipment_.begin(); it1 != composition.transportedEquipment_.end(); ++it1 )
    {
        QListViewItem* pItem = equipement_[ n1++ ];
        pItem->setText( 0, MOS_App::GetApp().GetEquipmentName( (*it1).first.nRessourceID_ ).c_str() );
        
        if( (*it1).first.nBreakdownID_ == (uint)-1 )
            pItem->setText( 1, tr( "N/A" ) );
        else
            pItem->setText( 1, MOS_App::GetApp().GetBreakDownName( (*it1).first.nBreakdownID_ ).c_str() );
        
        if( (*it1).first.nNTI_ == (uint)-1 )
            pItem->setText( 2, tr( "N/A" ) );
        else
            pItem->setText( 2, QString::number( (*it1).first.nNTI_ ) );
        
        pItem->setText( 3, QString::number( (*it1).second ) );
    }

    int n2 = 0;
    for( CIT_ResourceQtyQty_Map it2 = composition.transportedResources_.begin(); it2 != composition.transportedResources_.end(); ++it2 )
    {
        QListViewItem* pItem = resources_[ n2++ ];
        pItem->setText( 0, MOS_App::GetApp().GetResourceName( (*it2).first ).c_str() );
        pItem->setText( 1, QString::number( (*it2).second.first ) );
        pItem->setText( 2, QString::number( (*it2).second.second ) );
    }

    for( int n3 = 0; n3 < (int)eTroopHealthStateNbrStates; ++n3 )
    {
        QListViewItem* pItem = troops_[ n3 ];
        pItem->setText( 0, MOS_Tools::ToString( (E_TroopHealthState)n3 ) );
        pItem->setText( 1, QString::number( composition.transportedTroops_[0][n3] ) );
        pItem->setText( 2, QString::number( composition.transportedTroops_[1][n3] ) );
        pItem->setText( 3, QString::number( composition.transportedTroops_[2][n3] ) );
    }
}
