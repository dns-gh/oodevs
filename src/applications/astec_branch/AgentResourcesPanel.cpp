// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Math�matiques Appliqu�es SA (MASA)
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
#include "ListDisplayer.h"

// -----------------------------------------------------------------------------
// Name: AgentResourcesPanel constructor
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
AgentResourcesPanel::AgentResourcesPanel( InfoPanel* pParent, Controller& controller, ActionController& actionController )
    : InfoPanel_ABC( pParent, tr( "Res." ) )
{
    pEquipment_ = new T_ListView( this, *this );
    pEquipment_->AddColumn( "Equipement" )
                .AddColumn( "Disponible" )
                .AddColumn( "Indisponible" )
                .AddColumn( "R�parable" )
                .AddColumn( "En maintenance" );

    pResources_ = new T_ListView( this, *this );
    pResources_->AddColumn( "Resource" )
                .AddColumn( "Quantit�" );

    pTroops_ = new T_ListView( this, *this );
    pTroops_->AddColumn( "Cat�gorie" )
             .AddColumn( "Officiers" )
             .AddColumn( "Sous officiers" )
             .AddColumn( "Mdr" );

    pLends_ = new T_ListView( this, *this );
    pLends_->AddColumn( "Emprunteur" )
            .AddColumn( "Equipement pr�t�" )
            .AddColumn( "Quantit�" );

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
// Name: AgentResourcesPanel::showEvent
// Created: AGE 2006-02-27
// -----------------------------------------------------------------------------
void AgentResourcesPanel::showEvent( QShowEvent* )
{
    const Agent* selected = selected_;
    selected_ = 0;
    NotifySelected( selected );
}

// -----------------------------------------------------------------------------
// Name: AgentResourcesPanel::ShouldUpdate
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
template< typename T >
bool AgentResourcesPanel::ShouldUpdate( const T& a )
{
    return IsVisible()
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
        pResources_->DisplayList( a.CreateIterator() )
    );
}

// -----------------------------------------------------------------------------
// Name: AgentResourcesPanel::Display
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
void AgentResourcesPanel::Display( const Dotation& dotation, Displayer_ABC& displayer, ValuedListItem* )
{
    displayer.Display( "Resource", dotation.type_ )
             .Display( "Quantit�", dotation.quantity_ );
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
        pEquipment_->DisplayList( a.CreateIterator() )
    );
}

// -----------------------------------------------------------------------------
// Name: AgentResourcesPanel::Display
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
void AgentResourcesPanel::Display( const Equipment& equipment, Displayer_ABC& displayer, ValuedListItem* )
{
    displayer.Display( "Equipement", equipment.type_  )
             .Display( "Disponible", equipment.available_ )
             .Display( "Indisponible", equipment.unavailable_ )
             .Display( "R�parable", equipment.repairable_ )
             .Display( "En maintenance", equipment.inMaintenance_ );
}

// -----------------------------------------------------------------------------
// Name: AgentResourcesPanel::NotifyUpdated
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
void AgentResourcesPanel::NotifyUpdated( const Lends& a )
{
    if( ! ShouldUpdate( a ) )
        return;
    pLends_->DeleteTail( 
        pLends_->DisplayList( a.lends_.begin(), a.lends_.end() )
    );
}

// -----------------------------------------------------------------------------
// Name: AgentResourcesPanel::Display
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
void AgentResourcesPanel::Display( const Lend& lend, Displayer_ABC& displayer, ValuedListItem* )
{
    displayer.Display( "Emprunteur", lend.borrower_ )
             .Display( "Equipement pr�t�", lend.type_ )
             .Display( "Quantit�", lend.quantity_ );
}

// -----------------------------------------------------------------------------
// Name: AgentResourcesPanel::NotifyUpdated
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
void AgentResourcesPanel::NotifyUpdated( const Troops& a )
{
    if( ! ShouldUpdate( a ) )
        return;

    pTroops_->DeleteTail(
        pTroops_->DisplayList( &*a.humans_, a.humans_+eTroopHealthStateNbrStates )
        );
}

// -----------------------------------------------------------------------------
// Name: AgentResourcesPanel::Display
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
void AgentResourcesPanel::Display( const Troops::Humans& humans, Displayer_ABC& displayer, ValuedListItem* )
{
    displayer.Display( "Cat�gorie", humans.state_ )
             .Display( "Officiers", humans.officers_ )
             .Display( "Sous officiers", humans.subOfficers_ )
             .Display( "Mdr", humans.troopers_ );
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
