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

#include "astec_pch.h"
#include "AgentResourcesPanel.h"

#include "AgentComposition.h"
#include "Agent.h"
#include "App.h"
#include "Tools.h"
#include "Controller.h"
#include "ActionController.h"
#include "Dotations.h"
#include "Dotation.h"
#include "DotationType.h"
#include "Equipments.h"
#include "Lends.h"
#include "Troops.h"

// -----------------------------------------------------------------------------
// Name: AgentResourcesPanel constructor
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
AgentResourcesPanel::AgentResourcesPanel( InfoPanel* pParent, Controller& controller, ActionController& actionController )
    : InfoPanel_ABC( pParent, tr( "Res." ) )
{
    pEquipment_ = new T_ListView( this, *this );
    pEquipment_->setSorting( -1 );
    pEquipment_->addColumn( tr( "Equipement" ) );
    pEquipment_->addColumn( tr( "Disponible" ) );
    pEquipment_->addColumn( tr( "Indisponible" ) );
    pEquipment_->addColumn( tr( "Réparable" ) );
    pEquipment_->addColumn( tr( "En maintenance" ) );
    pEquipment_->setResizeMode( QListView::LastColumn );
    pEquipment_->setAllColumnsShowFocus ( true );

    pResources_ = new T_ListView( this, *this );
    pResources_->setSorting( -1 );
    pResources_->addColumn( tr( "Resource" ) );
    pResources_->addColumn( tr( "Quantité" ) );
    pResources_->setResizeMode( QListView::LastColumn );
    pResources_->setAllColumnsShowFocus ( true );

    pTroops_ = new T_ListView( this, *this );
    pTroops_->setSorting( -1 );
    pTroops_->addColumn( tr( "Catégorie" ) );
    pTroops_->addColumn( tr( "Officiers" ) );
    pTroops_->addColumn( tr( "Sous officiers" ) );
    pTroops_->addColumn( tr( "Mdr" ) );
    pTroops_->setResizeMode( QListView::LastColumn );
    pTroops_->setAllColumnsShowFocus ( true );

    pLends_ = new T_ListView( this, *this );
    pLends_->setSorting( -1 );
    pLends_->addColumn( tr( "Emprunteur" ) );
    pLends_->addColumn( tr( "Equipement prêté" ) );
    pLends_->addColumn( tr( "Quantité" ) );
    pLends_->setResizeMode( QListView::LastColumn );
    pLends_->setAllColumnsShowFocus ( true );

    controller.Register( *this );
    actionController.Register( *this );
}


// -----------------------------------------------------------------------------
// Name: AgentResourcesPanel destructor
// Created: APE 2004-03-10
// -----------------------------------------------------------------------------
AgentResourcesPanel::~AgentResourcesPanel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentResourcesPanel::OnUpdate
// Created: AGE 2005-04-05
// -----------------------------------------------------------------------------
//void AgentResourcesPanel::OnUpdate()
//{
//    if( selectedItem_.pAgent_ != 0 )
//        OnAgentUpdated( *selectedItem_.pAgent_ );
//    else
//        OnClearSelection();
//}

// -----------------------------------------------------------------------------
// Name: AgentResourcesPanel::OnAgentUpdated
/** @param  agent 
*/
// Created: APE 2004-05-10
// -----------------------------------------------------------------------------
//void AgentResourcesPanel::OnAgentUpdated( Agent& agent )
//{
//    if( ! ShouldDisplay( agent ) )
//        return;
//
//    const AgentComposition& composition = agent.GetComposition();
//    Suit( equipement_, composition.equipment_.size(), pEquipment_ );
//    Suit( resources_, composition.resources_.size(), pResources_ );
//    Suit( troops_, (int)eTroopHealthStateNbrStates, pTroops_ );
//    Suit( lends_, composition.lends_.size(), pLends_ );
//
//    int n1 = 0;
//    for( CIT_EquipmentQty_Map it1 = composition.equipment_.begin(); it1 != composition.equipment_.end(); ++it1 )
//    {
//        QListViewItem* pItem = equipement_[ n1++ ];
//        pItem->setText( 0, App::GetApp().GetEquipmentName( (*it1).first ).c_str() );
//        pItem->setText( 1, QString::number( (*it1).second.nNbrAvailable_ ) );
//        pItem->setText( 2, QString::number( (*it1).second.nNbrUnavailable_ ) );
//        pItem->setText( 3, QString::number( (*it1).second.nNbrReparable_ ) );
//        pItem->setText( 4, QString::number( (*it1).second.nNbrInMaintenance_ ) );
//    }
//
//    int n2 = 0;
//    for( CIT_ResourceQty_Map it2 = composition.resources_.begin(); it2 != composition.resources_.end(); ++it2 )
//    {
//        QListViewItem* pItem = resources_[ n2++ ];
//        pItem->setText( 0, App::GetApp().GetResourceName( (*it2).first ).c_str() );
//        pItem->setText( 1, QString::number( (*it2).second ) );
//    }
//
//    for( int n3 = 0; n3 < (int)eTroopHealthStateNbrStates; ++n3 )
//    {
//        QListViewItem* pItem = troops_[ n3 ];
//        pItem->setText( 0, Tools::ToString( (E_TroopHealthState)n3 ) );
//        pItem->setText( 1, QString::number( composition.officiers_[n3] ) );
//        pItem->setText( 2, QString::number( composition.sousOfficiers_[n3] ) );
//        pItem->setText( 3, QString::number( composition.mdr_[n3] ) );
//    }
//
//    for( uint n4 = 0; n4 < composition.lends_.size(); ++n4 )
//    {
//        QListViewItem* pItem = lends_[ n4 ];
//        const AgentComposition::T_Lend& lend = composition.lends_[ n4 ];
//        pItem->setText( 0, QString::number( lend.nBorrowerId_ ) );
//        pItem->setText( 1, App::GetApp().GetEquipmentName( lend.nEquipment_ ).c_str() );
//        pItem->setText( 2, QString::number( lend.nQuantity_ ) );
//    }
//}

// -----------------------------------------------------------------------------
// Name: AgentResourcesPanel::OnClearSelection
// Created: SBO 2005-09-22
// -----------------------------------------------------------------------------
//void AgentResourcesPanel::OnClearSelection()
//{
//    pEquipment_->hide();
//    pResources_->hide();
//    pTroops_   ->hide();
//    pLends_    ->hide();
//}

// -----------------------------------------------------------------------------
// Name: AgentResourcesPanel::ShouldUpdate
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
template< typename T >
bool AgentResourcesPanel::ShouldUpdate( const T& a )
{
    return isVisible()
        && selected_ 
        && ( selected_->Retrieve< T >() == & a );
}

// -----------------------------------------------------------------------------
// Name: AgentResourcesPanel::NotifyUpdated
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
void AgentResourcesPanel::NotifyUpdated( const Dotations& a )
{
    if( ! ShouldUpdate( a ) )
        return;
    pResources_->DeleteTail( pResources_->Display( a.CreateIterator(), pResources_, (ValuedListItem*)( pResources_->firstChild() ) ) );
}


// -----------------------------------------------------------------------------
// Name: AgentResourcesPanel::Display
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
void AgentResourcesPanel::Display( const Dotation& dotation, ValuedListItem* item )
{
    item->SetValue( &dotation );
    item->setText( 0, dotation.type_.GetCategory().c_str() );
    item->setText( 1, QString::number( dotation.quantity_ ) );
}


// -----------------------------------------------------------------------------
// Name: AgentResourcesPanel::NotifyUpdated
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
void AgentResourcesPanel::NotifyUpdated( const Equipments& a )
{
    if( ! ShouldUpdate( a ) )
        return;
}

// -----------------------------------------------------------------------------
// Name: AgentResourcesPanel::NotifyUpdated
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
void AgentResourcesPanel::NotifyUpdated( const Lends& a )
{
    if( ! ShouldUpdate( a ) )
        return;
}

// -----------------------------------------------------------------------------
// Name: AgentResourcesPanel::NotifyUpdated
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
void AgentResourcesPanel::NotifyUpdated( const Troops& a )
{
    if( ! ShouldUpdate( a ) )
        return;
}

// -----------------------------------------------------------------------------
// Name: AgentResourcesPanel::NotifySelected
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
void AgentResourcesPanel::NotifySelected( const Agent* agent )
{
    if( selected_ != agent || ! agent )
    {
        selected_ = agent;
        if( selected_ )
        {
            // $$$$ AGE 2006-02-17: refactor that
            Show();
            NotifyUpdated( selected_->Get< Dotations >() );
            NotifyUpdated( selected_->Get< Equipments >() );
            NotifyUpdated( selected_->Get< Lends >() );
            NotifyUpdated( selected_->Get< Troops >() );
        }
        else
            Hide();
    }
}
