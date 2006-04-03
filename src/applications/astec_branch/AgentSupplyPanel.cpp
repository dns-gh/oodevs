// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2005-04-01 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/AgentSupplyPanel.cpp $
// $Author: Age $
// $Modtime: 6/04/05 10:41 $
// $Revision: 2 $
// $Workfile: AgentSupplyPanel.cpp $
//
// *****************************************************************************

#include "astec_pch.h"
#include "AgentSupplyPanel.h"
#include "LogSupplyConsign.h"
#include "Agent.h"
#include "SupplyStates.h"
#include "LogisticConsigns.h"
#include "ListDisplayer.h"
#include "DisplayBuilder.h"
#include "Controllers.h"
#include "SubItemDisplayer.h"
#include "Units.h"
#include "DotationRequest.h"

// -----------------------------------------------------------------------------
// Name: AgentSupplyPanel constructor
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
AgentSupplyPanel::AgentSupplyPanel( InfoPanels* pParent, Controllers& controllers )
    : InfoPanel_ABC( pParent, tr( "Ch. rav." ) )
    , controllers_( controllers )
    , selected_( 0 )
{
    pConsignListView_ = new ListDisplayer< AgentSupplyPanel >( this, *this );
    pConsignListView_->AddColumn( "Demandes logistiques" );
    
    pConsignHandledListView_ = new ListDisplayer< AgentSupplyPanel >( this, *this );
    pConsignHandledListView_->AddColumn( "Consignes en traitement" );

    logDisplay_ = new SubItemDisplayer( "Consigne :" );
    logDisplay_->AddChild( "Pion demandeur :" )
                .AddChild( "Pion traitant :" )
                .AddChild( "Pion fournissant les moyens :" )
                .AddChild( "Pion convoyant :" )
                .AddChild( "Etat :" );

    display_ = new DisplayBuilder( this );
    display_->AddGroup( "Etat chaine rav." )
                .AddLabel( "Etat chaine" );

    pStocks_ = new ListDisplayer< AgentSupplyPanel >( this, *this );
    pStocks_->AddColumn( "Stock" )
             .AddColumn( "Quantité" );

    pQuotas_ = new ListDisplayer< AgentSupplyPanel >( this, *this );
    pQuotas_->AddColumn( "Type" )
             .AddColumn( "Quota" );

    
    pDispoTransporters_ = new ListDisplayer< AgentSupplyPanel >( this, *this );
    pDispoTransporters_->AddColumn( "Transporteurs pour convois" )
                        .AddColumn( "Disponibles" );

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
    delete logDisplay_;
}

// -----------------------------------------------------------------------------
// Name: AgentSupplyPanel::showEvent
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
void AgentSupplyPanel::showEvent( QShowEvent* )
{
    const Agent* selected = selected_;
    selected_ = 0;
    NotifySelected( selected );
}

// -----------------------------------------------------------------------------
// Name: AgentSupplyPanel::ShouldUpdate
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
template< typename Extension >
bool AgentSupplyPanel::ShouldUpdate( const Extension& e )
{
    return IsVisible() && selected_ && selected_->Retrieve< Extension >() == &e ;
}

// -----------------------------------------------------------------------------
// Name: AgentSupplyPanel::NotifySelected
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
void AgentSupplyPanel::NotifySelected( const Agent* agent )
{
    if( ! agent || agent != selected_ )
    {
        selected_ = agent;
        if( selected_ )
        {
            pConsignListView_->hide();
            pConsignHandledListView_->hide();
            display_->Hide();
            pStocks_->hide();
            pQuotas_->hide();
            pDispoTransporters_->hide();

            Show();
            NotifyUpdated( selected_->Get< LogisticConsigns >() );
            if( selected_->Retrieve< SupplyStates >() )
                NotifyUpdated( selected_->Get< SupplyStates >() );
        }
        else
            Hide();
    }
}

// -----------------------------------------------------------------------------
// Name: AgentSupplyPanel::NotifyUpdated
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
void AgentSupplyPanel::NotifyUpdated( const LogisticConsigns& consigns )
{
    if( ! ShouldUpdate( consigns ) )
        return;

    pConsignListView_->DeleteTail( 
        pConsignListView_->DisplayList( consigns.requestedSupplies_.begin(), consigns.requestedSupplies_.end() )
        );
        
    pConsignHandledListView_->DeleteTail( 
        pConsignHandledListView_->DisplayList( consigns.handledSupplies_.begin(), consigns.handledSupplies_.end() )
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
    consign->Display( (*logDisplay_)( item ) );

    // $$$$ AGE 2006-02-28: crado
    QListViewItem* last  = item->firstChild();
    QListViewItem* child = last;
    while( child && child->text( 0 ) != tr( "Dotations demandées/accordées/convoyées" ) )
    {
        last = child;
        child = child->nextSibling();
    }
    if( ! child )
        child = new EmptyListItem( item, last );
    pConsignListView_->DisplayList( consign->CreateIterator(), child );
}

// -----------------------------------------------------------------------------
// Name: AgentSupplyPanel::Display
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
void AgentSupplyPanel::Display( const DotationRequest& request, Displayer_ABC& displayer, ValuedListItem* )
{
    displayer.Item( 0 ).Start( request.type_ ).Add( "/" )
                       .Add( request.requested_ ).Add( "/" )
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

// -----------------------------------------------------------------------------
// Name: AgentSupplyPanel::NotifyUpdated
// Created: AGE 2006-03-01
// -----------------------------------------------------------------------------
void AgentSupplyPanel::NotifyUpdated( const LogSupplyConsign& consign )
{
    ValuedListItem* item = FindItem( &consign, pConsignListView_->firstChild() );
    if( ! item )
        item = FindItem( &consign, pConsignHandledListView_->firstChild() );
    if( item )
        consign.Display( (*logDisplay_)( item ) );
}
