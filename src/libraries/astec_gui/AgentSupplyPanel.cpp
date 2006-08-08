// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_gui_pch.h"
#include "AgentSupplyPanel.h"
#include "astec_gaming/LogSupplyConsign.h"
#include "astec_kernel/Agent_ABC.h"
#include "astec_gaming/SupplyStates.h"
#include "astec_gaming/LogisticConsigns.h"
#include "astec_gaming/DotationRequest.h"
#include "astec_kernel/Controllers.h"
#include "astec_kernel/Units.h"
#include "ListDisplayer.h"
#include "DisplayBuilder.h"
#include "SubItemDisplayer.h"
#include "ItemFactory_ABC.h"

// -----------------------------------------------------------------------------
// Name: AgentSupplyPanel constructor
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
AgentSupplyPanel::AgentSupplyPanel( InfoPanels* pParent, Controllers& controllers, ItemFactory_ABC& factory )
    : LogisticPanel< AgentSupplyPanel, LogSupplyConsign >( pParent, controllers, factory, tr( "Chaine ravitaillement" ) )
    , controllers_( controllers )
    , factory_( factory )
{
    AddConsignColumn( "Pion fournissant les moyens :" );
    AddConsignColumn( "Pion convoyant :" );
    AddConsignColumn( "Etat :" );

    display_ = new DisplayBuilder( this, factory );
    display_->AddGroup( "Etat chaine rav." )
                .AddLabel( "Etat chaine" );

    pStocks_ = new ListDisplayer< AgentSupplyPanel >( this, *this, factory );
    pStocks_->AddColumn( "Stock" )
             .AddColumn( "Quantité" );

    pQuotas_ = new ListDisplayer< AgentSupplyPanel >( this, *this, factory );
    pQuotas_->AddColumn( "Type" )
             .AddColumn( "Quota" );

    
    pDispoTransporters_ = new ListDisplayer< AgentSupplyPanel >( this, *this, factory );
    pDispoTransporters_->AddColumn( "Transporteurs pour convois" )
                        .AddColumn( "Total" )
                        .AddColumn( "Disponible" )
                        .AddColumn( "Au travail" )
                        .AddColumn( "Au repos" );

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
void AgentSupplyPanel::NotifySelected( const Agent_ABC& agent )
{
    display_->Hide();
    pStocks_->hide();
    pQuotas_->hide();
    pDispoTransporters_->hide();

    const SupplyStates* states = agent.Retrieve< SupplyStates >();
    const LogisticConsigns* consigns = agent.Retrieve< LogisticConsigns >();
    if( ( consigns && ! consigns->requestedSupplies_.empty() ) 
       || states )
        Show();
    else
        Hide();

    if( consigns )
        Parent::NotifyUpdated( *consigns );
    if( states )
        NotifyUpdated( *states );
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
    while( child && child->text( 0 ) != tr( "Dotations demandées/accordées/convoyées" ) )
    {
        last = child;
        child = child->nextSibling();
    }
    if( ! child )
    {
        child = factory_.CreateItem( item, last );
        child->setText( 0, tr( "Dotations demandées/accordées/convoyées" ) );
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

    pQuotas_->DeleteTail( 
        pQuotas_->DisplayList( consigns.quotas_.begin(), consigns.quotas_.end() )
        );

    pStocks_->DeleteTail( 
        pStocks_->DisplayList( consigns.CreateIterator() )
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

