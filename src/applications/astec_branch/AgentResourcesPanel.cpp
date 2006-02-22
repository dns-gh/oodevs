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
#include "Equipment.h"
#include "EquipmentType.h"
#include "Lend.h"

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
    pResources_->DeleteTail( 
        pResources_->Display( a.CreateIterator(), pResources_, (ValuedListItem*)( pResources_->firstChild() ) )
    );
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
    pEquipment_->DeleteTail( 
        pEquipment_->Display( a.CreateIterator(), pEquipment_, (ValuedListItem*)( pEquipment_->firstChild() ) )
    );
}

// -----------------------------------------------------------------------------
// Name: AgentResourcesPanel::Display
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
void AgentResourcesPanel::Display( const Equipment& equipment, ValuedListItem* item )
{
    item->SetValue( &equipment );
    item->setText( 0, equipment.type_.GetName().c_str() );
    item->setText( 1, QString::number( equipment.available_ ) );
    item->setText( 2, QString::number( equipment.unavailable_ ) );
    item->setText( 3, QString::number( equipment.repairable_ ) );
    item->setText( 4, QString::number( equipment.inMaintenance_ ) );
}

// -----------------------------------------------------------------------------
// Name: AgentResourcesPanel::NotifyUpdated
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
void AgentResourcesPanel::NotifyUpdated( const Lends& a )
{
    if( ! ShouldUpdate( a ) )
        return;
    pEquipment_->DeleteTail( 
        pEquipment_->Display( a.lends_.begin(), a.lends_.end(), pEquipment_, (ValuedListItem*)( pEquipment_->firstChild() ) )
    );
}

// -----------------------------------------------------------------------------
// Name: AgentResourcesPanel::Display
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
void AgentResourcesPanel::Display( const Lend& lend, ValuedListItem* item )
{
    item->SetValue( lend );
    item->setText( 0, QString::number( lend.borrower_->GetId() ) ); // $$$$ AGE 2006-02-21: more than id
    item->setText( 1, lend.type_->GetName().c_str() );
    item->setText( 1, QString::number( lend.quantity_ ) );
}

// -----------------------------------------------------------------------------
// Name: AgentResourcesPanel::NotifyUpdated
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
void AgentResourcesPanel::NotifyUpdated( const Troops& a )
{
    if( ! ShouldUpdate( a ) )
        return;

    pTroops_->Display( a.humans_, a.humans_+eTroopHealthStateNbrStates, pTroops_, (ValuedListItem*)( pTroops_->firstChild() ) );
}

// -----------------------------------------------------------------------------
// Name: AgentResourcesPanel::Display
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
void AgentResourcesPanel::Display( const Troops::Humans& humans, ValuedListItem* item )
{
    item->SetValue( humans );
    item->setText( 0, Tools::ToString( (E_TroopHealthState)humans.state_ ) );
    item->setText( 1, QString::number( humans.officers_ ) );
    item->setText( 2, QString::number( humans.subOfficers_ ) );
    item->setText( 3, QString::number( humans.troopers_ ) );
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
