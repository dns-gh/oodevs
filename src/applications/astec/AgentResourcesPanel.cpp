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
// $Archive: /MVW_v10/Build/SDK/Light2/src/AgentResourcesPanel.cpp $
// $Author: Age $
// $Modtime: 6/04/05 11:59 $
// $Revision: 9 $
// $Workfile: AgentResourcesPanel.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "astec_pch.h"
#include "AgentResourcesPanel.h"

#include "AgentComposition.h"
#include "Agent.h"
#include "App.h"
#include "Tools.h"

// -----------------------------------------------------------------------------
// Name: AgentResourcesPanel constructor
/** @param  pParent 
    @param  bCargo 
*/
// Created: APE 2004-03-10
// -----------------------------------------------------------------------------
AgentResourcesPanel::AgentResourcesPanel( QWidget* pParent )
    : InfoPanel_ABC( pParent )
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

    pLendings_ = new QListView( this );
    pLendings_->setSorting( -1 );
    pLendings_->addColumn( tr( "Emprunteur" ) );
    pLendings_->addColumn( tr( "Equipement prêté" ) );
    pLendings_->addColumn( tr( "Quantité" ) );
    pLendings_->setResizeMode( QListView::LastColumn );
    pLendings_->setAllColumnsShowFocus ( true );

    pBorrowings_ = new QListView( this );
    pBorrowings_->setSorting( -1 );
    pBorrowings_->addColumn( tr( "Prêteur" ) );
    pBorrowings_->addColumn( tr( "Equipement emprunté" ) );
    pBorrowings_->addColumn( tr( "Quantité" ) );
    pBorrowings_->setResizeMode( QListView::LastColumn );
    pBorrowings_->setAllColumnsShowFocus ( true );
}


// -----------------------------------------------------------------------------
// Name: AgentResourcesPanel destructor
// Created: APE 2004-03-10
// -----------------------------------------------------------------------------
AgentResourcesPanel::~AgentResourcesPanel()
{
}

// -----------------------------------------------------------------------------
// Name: AgentResourcesPanel::OnUpdate
// Created: AGE 2005-04-05
// -----------------------------------------------------------------------------
void AgentResourcesPanel::OnUpdate()
{
    if( selectedItem_.pAgent_ != 0 )
        OnAgentUpdated( *selectedItem_.pAgent_ );
    else
        OnClearSelection();
}

// -----------------------------------------------------------------------------
// Name: AgentResourcesPanel::OnAgentUpdated
/** @param  agent 
*/
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
void AgentResourcesPanel::OnAgentUpdated( Agent& agent )
{
    if( ! ShouldDisplay( agent ) )
        return;

    const AgentComposition& composition = agent.GetComposition();
    Suit( equipement_, composition.equipment_.size(), pEquipment_ );
    Suit( resources_, composition.resources_.size(), pResources_ );
    Suit( troops_, (int)eTroopHealthStateNbrStates, pTroops_ );
    Suit( lendings_  , composition.lendings_  .size(), pLendings_   );
    Suit( borrowings_, composition.borrowings_.size(), pBorrowings_ );

    int n1 = 0;
    for( CIT_EquipmentQty_Map it1 = composition.equipment_.begin(); it1 != composition.equipment_.end(); ++it1 )
    {
        QListViewItem* pItem = equipement_[ n1++ ];
        pItem->setText( 0, App::GetApp().GetEquipmentName( (*it1).first ).c_str() );
        pItem->setText( 1, QString::number( (*it1).second.nNbrAvailable_ ) );
        pItem->setText( 2, QString::number( (*it1).second.nNbrUnavailable_ ) );
        pItem->setText( 3, QString::number( (*it1).second.nNbrReparable_ ) );
        pItem->setText( 4, QString::number( (*it1).second.nNbrInMaintenance_ ) );
    }

    int n2 = 0;
    for( CIT_ResourceQty_Map it2 = composition.resources_.begin(); it2 != composition.resources_.end(); ++it2 )
    {
        QListViewItem* pItem = resources_[ n2++ ];
        pItem->setText( 0, App::GetApp().GetResourceName( (*it2).first ).c_str() );
        pItem->setText( 1, QString::number( (*it2).second ) );
    }

    for( int n3 = 0; n3 < (int)eTroopHealthStateNbrStates; ++n3 )
    {
        QListViewItem* pItem = troops_[ n3 ];
        pItem->setText( 0, Tools::ToString( (E_TroopHealthState)n3 ) );
        pItem->setText( 1, QString::number( composition.officiers_[n3] ) );
        pItem->setText( 2, QString::number( composition.sousOfficiers_[n3] ) );
        pItem->setText( 3, QString::number( composition.mdr_[n3] ) );
    }

    for( uint n4 = 0; n4 < composition.lendings_.size(); ++n4 )
    {
        QListViewItem* pItem = lendings_[ n4 ];
        const AgentComposition::T_Lending& loan = composition.lendings_[ n4 ];
        pItem->setText( 0, QString::number( loan.nBorrowerId_ ) );
        pItem->setText( 1, App::GetApp().GetEquipmentName( loan.nEquipment_ ).c_str() );
        pItem->setText( 2, QString::number( loan.nQuantity_ ) );
    }

    for( uint n5 = 0; n5 < composition.borrowings_.size(); ++n5 )
    {
        QListViewItem* pItem = borrowings_[ n5 ];
        const AgentComposition::T_Borrowing& loan = composition.borrowings_[ n5 ];
        pItem->setText( 0, QString::number( loan.nLenderId_ ) );
        pItem->setText( 1, App::GetApp().GetEquipmentName( loan.nEquipment_ ).c_str() );
        pItem->setText( 2, QString::number( loan.nQuantity_ ) );
    }
}

// -----------------------------------------------------------------------------
// Name: AgentResourcesPanel::OnClearSelection
// Created: SBO 2005-09-22
// -----------------------------------------------------------------------------
void AgentResourcesPanel::OnClearSelection()
{
    pEquipment_    ->hide();
    pResources_    ->hide();
    pTroops_       ->hide();
    pLendings_   ->hide();
    pBorrowings_->hide();
}
