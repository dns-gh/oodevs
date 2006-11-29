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
    : InfoPanel_ABC( parent, panel, tr( "Resources" ) )
    , controllers_( controllers )
    , selected_( controllers )
{
    pEquipment_ = new T_ListView( this, *this, factory );
    pEquipment_->AddColumn( tr( "Equipment" ) )
                .AddColumn( tr( "Available" ) )
                .AddColumn( tr( "Unavailable" ) )
                .AddColumn( tr( "Repairable" ) )
                .AddColumn( tr( "In maintenance" ) )
                .AddColumn( tr( "Prisoner" ) ) ;

    pResources_ = new T_ListView( this, *this, factory );
    pResources_->AddColumn( tr( "Resource" ) )
                .AddColumn( tr( "Quantity" ) );

    pTroops_ = new T_ListView( this, *this, factory );
    pTroops_->AddColumn( tr( "Rank" ) )
             .AddColumn( tr( "Officer" ) )
             .AddColumn( tr( "Warrant-officer" ) )
             .AddColumn( tr( "Private" ) );

    pLendings_ = new T_ListView( this, *this, factory );
    pLendings_->AddColumn( tr( "Borrower" ) )
               .AddColumn( tr( "Equipment" ) )
               .AddColumn( tr( "Quantity" ) );

    pBorrowings_ = new T_ListView( this, *this, factory );
    pBorrowings_->AddColumn( tr( "Lender" ) )
                 .AddColumn( tr( "Equipment" ) )
                 .AddColumn( tr( "Quantity" ) );

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
             .Display( tr( "Quantity" ), dotation.quantity_ );
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
    displayer.Display( tr( "Equipment" ), equipment.type_  )
             .Display( tr( "Available" ), equipment.available_ )
             .Display( tr( "Unavailable" ), equipment.unavailable_ )
             .Display( tr( "Repairable" ), equipment.repairable_ )
             .Display( tr( "In maintenance" ), equipment.inMaintenance_ )
             .Display( tr( "Prisoner" ), equipment.prisonners_ );
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
             .Display( tr( "Equipment" ), loan.type_ )
             .Display( tr( "Quantity" ), loan.quantity_ );
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
    displayer.Display( tr( "Rank" ), humans.state_ )
             .Display( tr( "Officer" ), humans.officers_ )
             .Display( tr( "Warrant-officer" ), humans.subOfficers_ )
             .Display( tr( "Private" ), humans.troopers_ );
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
