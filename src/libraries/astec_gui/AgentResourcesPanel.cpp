// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_gui_pch.h"
#include "AgentResourcesPanel.h"

#include "astec_kernel/Agent_ABC.h"
#include "Tools.h"
#include "astec_kernel/Controllers.h"
#include "astec_gaming/Dotations.h"
#include "astec_gaming/Dotation.h"
#include "astec_kernel/DotationType.h"
#include "astec_gaming/Equipments.h"
#include "astec_gaming/Troops.h"
#include "astec_gaming/Equipment.h"
#include "astec_kernel/EquipmentType.h"
#include "astec_gaming/Loan.h"
#include "astec_gaming/Lendings.h"
#include "astec_gaming/Borrowings.h"
#include "astec_gaming/Humans.h"
#include "ListDisplayer.h"

// -----------------------------------------------------------------------------
// Name: AgentResourcesPanel constructor
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
AgentResourcesPanel::AgentResourcesPanel( InfoPanels* pParent, Controllers& controllers, ItemFactory_ABC& factory )
    : InfoPanel_ABC( pParent, tr( "Ressources" ) )
    , controllers_( controllers )
    , selected_( controllers )
{
    pEquipment_ = new T_ListView( this, *this, factory );
    pEquipment_->AddColumn( "Equipement" )
                .AddColumn( "Disponible" )
                .AddColumn( "Indisponible" )
                .AddColumn( "Réparable" )
                .AddColumn( "En maintenance" );

    pResources_ = new T_ListView( this, *this, factory );
    pResources_->AddColumn( "Resource" )
                .AddColumn( "Quantité" );

    pTroops_ = new T_ListView( this, *this, factory );
    pTroops_->AddColumn( "Catégorie" )
             .AddColumn( "Officiers" )
             .AddColumn( "Sous officiers" )
             .AddColumn( "Mdr" );

    pLendings_ = new T_ListView( this, *this, factory );
    pLendings_->AddColumn( "Emprunteur" )
               .AddColumn( "Equipement" )
               .AddColumn( "Quantité" );

    pBorrowings_ = new T_ListView( this, *this, factory );
    pBorrowings_->AddColumn( "Prêteur" )
                 .AddColumn( "Equipement" )
                 .AddColumn( "Quantité" );

    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: AgentResourcesPanel destructor
// Created: APE 2004-03-10
// -----------------------------------------------------------------------------
AgentResourcesPanel::~AgentResourcesPanel()
{
    controllers_.Remove( *this );
}

// -----------------------------------------------------------------------------
// Name: AgentResourcesPanel::showEvent
// Created: AGE 2006-02-27
// -----------------------------------------------------------------------------
void AgentResourcesPanel::showEvent( QShowEvent* )
{
    const Agent_ABC* selected = selected_;
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
             .Display( "Quantité", dotation.quantity_ );
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
             .Display( "Réparable", equipment.repairable_ )
             .Display( "En maintenance", equipment.inMaintenance_ );
}

// -----------------------------------------------------------------------------
// Name: AgentResourcesPanel::NotifyUpdated
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
void AgentResourcesPanel::NotifyUpdated( const Lendings& a )
{
    if( ! ShouldUpdate( a ) )
        return;
    pLendings_->DeleteTail( 
        pLendings_->DisplayList( a.lendings_.begin(), a.lendings_.end() )
    );
}

// -----------------------------------------------------------------------------
// Name: AgentResourcesPanel::NotifyUpdated
// Created: SBO 2006-07-18
// -----------------------------------------------------------------------------
void AgentResourcesPanel::NotifyUpdated( const Borrowings& a )
{
    if( ! ShouldUpdate( a ) )
        return;
    pBorrowings_->DeleteTail( 
        pBorrowings_->DisplayList( a.borrowings_.begin(), a.borrowings_.end() )
    );    
}

// -----------------------------------------------------------------------------
// Name: AgentResourcesPanel::Display
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
void AgentResourcesPanel::Display( const Loan& loan, Displayer_ABC& displayer, ValuedListItem* )
{
    displayer.Display( 0, loan.agent_ )
             .Display( "Equipement", loan.type_ )
             .Display( "Quantité", loan.quantity_ );
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
void AgentResourcesPanel::Display( const Humans& humans, Displayer_ABC& displayer, ValuedListItem* )
{
    displayer.Display( "Catégorie", humans.state_ )
             .Display( "Officiers", humans.officers_ )
             .Display( "Sous officiers", humans.subOfficers_ )
             .Display( "Mdr", humans.troopers_ );
}

// -----------------------------------------------------------------------------
// Name: AgentResourcesPanel::UpdateExtension
// Created: AGE 2006-08-03
// -----------------------------------------------------------------------------
template< typename T >
void AgentResourcesPanel::UpdateExtension( const Agent_ABC& agent )
{
    const T* extension = agent.Retrieve< T >();
    if( extension )
        NotifyUpdated( *extension );
}

// -----------------------------------------------------------------------------
// Name: AgentResourcesPanel::NotifySelected
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
void AgentResourcesPanel::NotifySelected( const Agent_ABC* agent )
{
    if( selected_ != agent || ! agent )
    {
        selected_ = agent;
        if( selected_ )
        {
            // $$$$ AGE 2006-02-17: refactor that
            Show();
            UpdateExtension< Dotations > ( *selected_ );
            UpdateExtension< Equipments >( *selected_ );
            UpdateExtension< Lendings >  ( *selected_ );
            UpdateExtension< Borrowings >( *selected_ );
            UpdateExtension< Troops >    ( *selected_ );
        }
        else
            Hide();
    }
}
