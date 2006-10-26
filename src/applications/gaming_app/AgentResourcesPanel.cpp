// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "AgentResourcesPanel.h"

#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/EquipmentType.h"
#include "clients_gui/ListDisplayer.h"
#include "gaming/Tools.h"
#include "gaming/Dotations.h"
#include "gaming/Dotation.h"
#include "gaming/Equipments.h"
#include "gaming/Troops.h"
#include "gaming/Equipment.h"
#include "gaming/Loan.h"
#include "gaming/Lendings.h"
#include "gaming/Borrowings.h"
#include "gaming/Humans.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: AgentResourcesPanel constructor
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
AgentResourcesPanel::AgentResourcesPanel( QWidget* parent, PanelStack_ABC& panel, Controllers& controllers, ItemFactory_ABC& factory )
    : InfoPanel_ABC( parent, panel, tr( "Ressources" ) )
    , controllers_( controllers )
    , selected_( controllers )
{
    pEquipment_ = new T_ListView( this, *this, factory );
    pEquipment_->AddColumn( tr( "Equipement" ) )
                .AddColumn( tr( "Disponible" ) )
                .AddColumn( tr( "Indisponible" ) )
                .AddColumn( tr( "Réparable" ) )
                .AddColumn( tr( "En maintenance" ) )
                .AddColumn( tr( "Prisonniers" ) ) ;

    pResources_ = new T_ListView( this, *this, factory );
    pResources_->AddColumn( tr( "Resource" ) )
                .AddColumn( tr( "Quantité" ) );

    pTroops_ = new T_ListView( this, *this, factory );
    pTroops_->AddColumn( tr( "Catégorie" ) )
             .AddColumn( tr( "Officiers" ) )
             .AddColumn( tr( "Sous officiers" ) )
             .AddColumn( tr( "Mdr" ) );

    pLendings_ = new T_ListView( this, *this, factory );
    pLendings_->AddColumn( tr( "Emprunteur" ) )
               .AddColumn( tr( "Equipement" ) )
               .AddColumn( tr( "Quantité" ) );

    pBorrowings_ = new T_ListView( this, *this, factory );
    pBorrowings_->AddColumn( tr( "Prêteur" ) )
                 .AddColumn( tr( "Equipement" ) )
                 .AddColumn( tr( "Quantité" ) );

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
    displayer.Display( tr( "Resource" ), dotation.type_ )
             .Display( tr( "Quantité" ), dotation.quantity_ );
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
    displayer.Display( tr( "Equipement" ), equipment.type_  )
             .Display( tr( "Disponible" ), equipment.available_ )
             .Display( tr( "Indisponible" ), equipment.unavailable_ )
             .Display( tr( "Réparable" ), equipment.repairable_ )
             .Display( tr( "En maintenance" ), equipment.inMaintenance_ )
             .Display( tr( "Prisonniers" ), equipment.prisonners_ );
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
    displayer.Display( QString(), loan.agent_ )
             .Display( tr( "Equipement" ), loan.type_ )
             .Display( tr( "Quantité" ), loan.quantity_ );
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
    displayer.Display( tr( "Catégorie" ), humans.state_ )
             .Display( tr( "Officiers" ), humans.officers_ )
             .Display( tr( "Sous officiers" ), humans.subOfficers_ )
             .Display( tr( "Mdr" ), humans.troopers_ );
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
