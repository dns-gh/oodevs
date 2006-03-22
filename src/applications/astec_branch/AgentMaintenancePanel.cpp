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
// $Archive: /MVW_v10/Build/SDK/Light2/src/AgentMaintenancePanel.cpp $
// $Author: Nld $
// $Modtime: 27/04/05 18:28 $
// $Revision: 3 $
// $Workfile: AgentMaintenancePanel.cpp $
//
// *****************************************************************************

#include "astec_pch.h"
#include "AgentMaintenancePanel.h"
#include "DisplayBuilder.h"
#include "ListDisplayer.h"
#include "Controllers.h"
#include "LogisticConsigns.h"
#include "Agent.h"
#include "LogMaintenanceConsign.h"
#include "MaintenanceStates.h"
#include "Availability.h"
#include "Units.h"
#include "SubItemDisplayer.h"

// -----------------------------------------------------------------------------
// Name: AgentMaintenancePanel constructor
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
AgentMaintenancePanel::AgentMaintenancePanel( InfoPanels* pParent, Controllers& controllers )
    : InfoPanel_ABC( pParent, tr( "Ch. maint." ) )
    , controllers_ ( controllers )
    , selected_    ( 0 )
{
    pConsignListView_        = new ListDisplayer< AgentMaintenancePanel >( this, *this );
    pConsignListView_->AddColumn( "Demandes logistiques" );
    
    pConsignHandledListView_ = new ListDisplayer< AgentMaintenancePanel >( this, *this );
    pConsignHandledListView_->AddColumn( "Consignes en traitement" );

    logDisplay_ = new SubItemDisplayer( "Consigne :" );
    logDisplay_->AddChild( "Pion demandeur :" )
                .AddChild( "Pion traitant :" )
                .AddChild( "Type d'équipement :" )
                .AddChild( "Type de panne :" )
                .AddChild( "Etat :" );

    display_ = new DisplayBuilder( this );
    display_->AddGroup( "Etat chaine maintenance" )
                .AddLabel( "Etat chaine" )
                .AddLabel( "Temps de bordée" )
                .AddLabel( "Priorités" )
                .AddLabel( "Priorités tactiques" );

    dispoHaulers_ = new ListDisplayer< AgentMaintenancePanel >( this, *this );
    dispoHaulers_->AddColumn( "Remorqueur" )
                  .AddColumn( "Disponibles" );
    dispoRepairers_ = new ListDisplayer< AgentMaintenancePanel >( this, *this );
    dispoRepairers_->AddColumn( "Réparateur" )
                    .AddColumn( "Disponibles" );

    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: AgentMaintenancePanel destructor
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
AgentMaintenancePanel::~AgentMaintenancePanel()
{
    controllers_.Remove( *this );
    delete logDisplay_;
    delete display_;
}

// -----------------------------------------------------------------------------
// Name: AgentMaintenancePanel::showEvent
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
void AgentMaintenancePanel::showEvent( QShowEvent* )
{
    const Agent* selected = selected_;
    selected_ = 0;
    NotifySelected( selected );
}

// -----------------------------------------------------------------------------
// Name: AgentMaintenancePanel::ShouldUpdate
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
template< typename Extension >
bool AgentMaintenancePanel::ShouldUpdate( const Extension& e )
{
    return IsVisible() && selected_ && selected_->Retrieve< Extension >() == &e ;
}

// -----------------------------------------------------------------------------
// Name: AgentMaintenancePanel::NotifySelected
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
void AgentMaintenancePanel::NotifySelected( const Agent* agent )
{
    if( ! agent || agent != selected_ )
    {
        selected_ = agent;
        if( selected_ )
        {
            pConsignListView_->hide();
            pConsignHandledListView_->hide();
            display_->Hide();
            dispoHaulers_->hide();
            dispoRepairers_->hide();

            Show();
            NotifyUpdated( selected_->Get< LogisticConsigns >() );
            if( selected_->Retrieve< MaintenanceStates >() )
                NotifyUpdated( selected_->Get< MaintenanceStates >() );
        }
        else
            Hide();
    }
}

// -----------------------------------------------------------------------------
// Name: AgentMaintenancePanel::NotifyUpdated
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
void AgentMaintenancePanel::NotifyUpdated( const LogisticConsigns& consigns )
{
    if( ! ShouldUpdate( consigns ) )
        return;

    pConsignListView_->DeleteTail( 
        pConsignListView_->DisplayList( consigns.requestedMaintenances_.begin(), consigns.requestedMaintenances_.end() )
        );
        
    pConsignHandledListView_->DeleteTail( 
        pConsignHandledListView_->DisplayList( consigns.handledMaintenances_.begin(), consigns.handledMaintenances_.end() )
        );
}

// -----------------------------------------------------------------------------
// Name: AgentMaintenancePanel::Display
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
void AgentMaintenancePanel::Display( const LogMaintenanceConsign* consign, Displayer_ABC&, ValuedListItem* item )
{
    if( consign )
    {
        item->SetValue( consign );
        consign->Display( (*logDisplay_)( item ) );
    }
}

// -----------------------------------------------------------------------------
// Name: AgentMaintenancePanel::NotifyUpdated
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
void AgentMaintenancePanel::NotifyUpdated( const MaintenanceStates& consigns )
{
    if( ! ShouldUpdate( consigns ) )
        return;

    consigns.Display( *display_ );

    dispoHaulers_->DeleteTail( 
        dispoHaulers_->DisplayList( consigns.dispoHaulers_.begin(), consigns.dispoHaulers_.end() )
        );

    dispoRepairers_->DeleteTail( 
        dispoRepairers_->DisplayList( consigns.dispoRepairers_.begin(), consigns.dispoRepairers_.end() )
        );
}

// -----------------------------------------------------------------------------
// Name: AgentMaintenancePanel::Display
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
void AgentMaintenancePanel::Display( const Availability& availability, Displayer_ABC& displayer, ValuedListItem* )
{
    displayer.Display( 0, availability.type_ )
             .Display( 0, availability.available_ * Units::percentage );
}

// -----------------------------------------------------------------------------
// Name: AgentMaintenancePanel::NotifyUpdated
// Created: AGE 2006-03-01
// -----------------------------------------------------------------------------
void AgentMaintenancePanel::NotifyUpdated( const LogMaintenanceConsign& consign )
{
    ValuedListItem* item = FindItem( &consign, pConsignListView_->firstChild() );
    if( ! item )
        item = FindItem( &consign, pConsignHandledListView_->firstChild() );
    if( item )
        consign.Display( (*logDisplay_)( item ) );
}
