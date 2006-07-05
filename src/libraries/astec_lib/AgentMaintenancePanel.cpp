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
AgentMaintenancePanel::AgentMaintenancePanel( InfoPanels* pParent, Controllers& controllers, ItemFactory_ABC& factory )
    : LogisticPanel< AgentMaintenancePanel, LogMaintenanceConsign >( pParent, controllers, factory, tr( "Ch. maint." ) )
    , controllers_( controllers )
{
    AddConsignColumn( "Type d'équipement :" );
    AddConsignColumn( "Type de panne :" );
    AddConsignColumn( "Etat :" );

    display_ = new DisplayBuilder( this, factory );
    display_->AddGroup( "Etat chaine maintenance" )
                .AddLabel( "Etat chaine" )
                .AddLabel( "Régime de travail" )
                .AddLabel( "Priorités" )
                .AddLabel( "Priorités tactiques" );

    dispoHaulers_ = new ListDisplayer< AgentMaintenancePanel >( this, *this, factory );
    dispoHaulers_->AddColumn( "Remorqueur" )
                  .AddColumn( "Total" )
                  .AddColumn( "Disponible" )
                  .AddColumn( "Au travail" )
                  .AddColumn( "Au repos" );
    dispoRepairers_ = new ListDisplayer< AgentMaintenancePanel >( this, *this, factory );
    dispoRepairers_->AddColumn( "Réparateur" )
                    .AddColumn( "Total" )
                    .AddColumn( "Disponible" )
                    .AddColumn( "Au travail" )
                    .AddColumn( "Au repos" );

    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: AgentMaintenancePanel destructor
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
AgentMaintenancePanel::~AgentMaintenancePanel()
{
    controllers_.Remove( *this );
    delete display_;
}

// -----------------------------------------------------------------------------
// Name: AgentMaintenancePanel::NotifySelected
// Created: AGE 2006-07-04
// -----------------------------------------------------------------------------
void AgentMaintenancePanel::NotifySelected( const Agent& agent )
{
    display_->Hide();
    dispoHaulers_->hide();
    dispoRepairers_->hide();

    Show();
    Parent::NotifyUpdated( agent.Get< LogisticConsigns >() );
    
    if( const MaintenanceStates* states = agent.Retrieve< MaintenanceStates >() )
        NotifyUpdated( *states );
}

// -----------------------------------------------------------------------------
// Name: AgentMaintenancePanel::DisplayRequested
// Created: AGE 2006-07-04
// -----------------------------------------------------------------------------
void AgentMaintenancePanel::DisplayRequested( const LogisticConsigns& consigns, ListDisplayer< AgentMaintenancePanel >* list )
{
    list->DeleteTail( 
        list->DisplayList( consigns.requestedMaintenances_.begin(), consigns.requestedMaintenances_.end() )
        );
}

// -----------------------------------------------------------------------------
// Name: AgentMaintenancePanel::DisplayHandled
// Created: AGE 2006-07-04
// -----------------------------------------------------------------------------
void AgentMaintenancePanel::DisplayHandled( const LogisticConsigns& consigns, ListDisplayer< AgentMaintenancePanel >* list )
{
    list->DeleteTail( 
        list->DisplayList( consigns.handledMaintenances_.begin(), consigns.handledMaintenances_.end() )
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
        consign->Display( GetDisplayer( item ) );
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
    availability.Display( displayer );
}

