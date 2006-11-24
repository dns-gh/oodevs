// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "AgentSupplyPanel.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Units.h"
#include "clients_gui/ListDisplayer.h"
#include "clients_gui/DisplayBuilder.h"
#include "clients_gui/ItemFactory_ABC.h"
#include "gaming/LogSupplyConsign.h"
#include "gaming/SupplyStates.h"
#include "gaming/LogisticConsigns.h"
#include "gaming/DotationRequest.h"
#include "gaming/Quotas.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: AgentSupplyPanel constructor
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
AgentSupplyPanel::AgentSupplyPanel( QWidget* parent, PanelStack_ABC& panel, Controllers& controllers, ItemFactory_ABC& factory )
    : LogisticPanel< AgentSupplyPanel, LogSupplyConsign >( parent, panel, controllers, factory, tr( "Supply system" ) )
    , controllers_( controllers )
    , factory_( factory )
{
    AddConsignColumn( tr( "Supplier:" ) );
    AddConsignColumn( tr( "Convoyer:" ) );
    AddConsignColumn( tr( "State:" ) );

    display_ = new DisplayBuilder( this, factory );
    display_->AddGroup( tr( "Supply system state" ) )
                .AddLabel( tr( "System status" ) );

    pStocks_ = new ListDisplayer< AgentSupplyPanel >( this, *this, factory );
    pStocks_->AddColumn( tr( "Stock" ) )
             .AddColumn( tr( "Quantity" ) );

    pQuotas_ = new ListDisplayer< AgentSupplyPanel >( this, *this, factory );
    pQuotas_->AddColumn( tr( "Type" ) )
             .AddColumn( tr( "Quota" ) );

    
    pDispoTransporters_ = new ListDisplayer< AgentSupplyPanel >( this, *this, factory );
    pDispoTransporters_->AddColumn( tr( "Transporters" ) )
                        .AddColumn( tr( "Total" ) )
                        .AddColumn( tr( "Available" ) )
                        .AddColumn( tr( "Working" ) )
                        .AddColumn( tr( "Resting" ) );

    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: AgentSupplyPanel destructor
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
AgentSupplyPanel::~AgentSupplyPanel()
{
    controllers_.Remove( *this );
    delete display_;
}

// -----------------------------------------------------------------------------
// Name: AgentSupplyPanel::NotifySelected
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
void AgentSupplyPanel::NotifySelected( const Entity_ABC& agent )
{
    display_->Hide();
    pStocks_->hide();
    pQuotas_->hide();
    pDispoTransporters_->hide();

    const SupplyStates*     states = agent.Retrieve< SupplyStates >();
    const Quotas*           quotas = agent.Retrieve< Quotas >();
    const LogisticConsigns* consigns = agent.Retrieve< LogisticConsigns >();

    if( ( consigns && ! consigns->requestedSupplies_.empty() ) 
       || states 
       || quotas )
        Show();
    else
        Hide();

    if( consigns ) Parent::NotifyUpdated( *consigns );
    if( quotas )   NotifyUpdated( *quotas );
    if( states )   NotifyUpdated( *states );
}

// -----------------------------------------------------------------------------
// Name: AgentSupplyPanel::DisplayRequested
// Created: AGE 2006-07-04
// -----------------------------------------------------------------------------
void AgentSupplyPanel::DisplayRequested( const LogisticConsigns& consigns, ListDisplayer< AgentSupplyPanel >* list )
{
    list->DeleteTail( 
        list->DisplayList( consigns.requestedSupplies_.begin(), consigns.requestedSupplies_.end() )
    );
}

// -----------------------------------------------------------------------------
// Name: AgentSupplyPanel::DisplayHandled
// Created: AGE 2006-07-04
// -----------------------------------------------------------------------------
void AgentSupplyPanel::DisplayHandled( const LogisticConsigns& consigns, ListDisplayer< AgentSupplyPanel >* list )
{
    list->DeleteTail( 
        list->DisplayList( consigns.handledSupplies_.begin(), consigns.handledSupplies_.end() )
    );
}

// -----------------------------------------------------------------------------
// Name: AgentSupplyPanel::Display
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
void AgentSupplyPanel::Display( const LogSupplyConsign* consign, Displayer_ABC& , ValuedListItem* item )
{
    if( ! consign )
        return;

    item->SetValue( consign );
    consign->Display( GetDisplayer( item ) );

    // $$$$ AGE 2006-02-28: crado
    QListViewItem* last  = item->firstChild();
    QListViewItem* child = last;
    while( child && child->text( 0 ) != tr( "Dotations requested/granted/convoyed" ) )
    {
        last = child;
        child = child->nextSibling();
    }
    if( ! child )
    {
        child = factory_.CreateItem( item, last );
        child->setText( 0, tr( "Dotations requested/granted/convoyed" ) );
    }

    // $$$$ AGE 2006-07-04: 
    ListDisplayer< AgentSupplyPanel >* list = (ListDisplayer< AgentSupplyPanel >*)( child->listView() );

    list->DeleteTail( 
        list->DisplayList( consign->CreateIterator(), child )
        );
}

// -----------------------------------------------------------------------------
// Name: AgentSupplyPanel::Display
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
void AgentSupplyPanel::Display( const DotationRequest& request, Displayer_ABC& displayer, ValuedListItem* )
{
    displayer.Display( 0, request.type_ )
             .Item( 0 ).Start( request.requested_ ).Add( "/" )
                         .Add( request.granted_ ).Add( "/" )
                         .Add( request.convoyed_ ).End();
}

// -----------------------------------------------------------------------------
// Name: AgentSupplyPanel::NotifyUpdated
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
void AgentSupplyPanel::NotifyUpdated( const SupplyStates& consigns )
{
    if( ! ShouldUpdate( consigns ) )
        return;

    consigns.Display( *display_ );

    pDispoTransporters_->DeleteTail( 
        pDispoTransporters_->DisplayList( consigns.dispoTransporters_.begin(), consigns.dispoTransporters_.end() )
        );

    pStocks_->DeleteTail( 
        pStocks_->DisplayList( consigns.CreateIterator() )
        );
}

// -----------------------------------------------------------------------------
// Name: AgentSupplyPanel::NotifyUpdated
// Created: AGE 2006-10-11
// -----------------------------------------------------------------------------
void AgentSupplyPanel::NotifyUpdated( const Quotas& quotas )
{
    if( ! ShouldUpdate( quotas ) )
        return;

    pQuotas_->DeleteTail( 
        pQuotas_->DisplayList( quotas.quotas_.begin(), quotas.quotas_.end() )
        );
}

// -----------------------------------------------------------------------------
// Name: AgentSupplyPanel::Display
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
void AgentSupplyPanel::Display( const Dotation& quota, Displayer_ABC& displayer, ValuedListItem* )
{
    displayer.Display( 0, quota.type_ )
             .Display( 0, quota.quantity_ );
}

// -----------------------------------------------------------------------------
// Name: AgentSupplyPanel::Display
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
void AgentSupplyPanel::Display( const Availability& availability, Displayer_ABC& displayer, ValuedListItem* )
{
    availability.Display( displayer );
}

