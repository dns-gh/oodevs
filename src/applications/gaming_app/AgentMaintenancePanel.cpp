// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "AgentMaintenancePanel.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Units.h"
#include "clients_gui/DisplayBuilder.h"
#include "clients_gui/ListDisplayer.h"
#include "clients_gui/SubItemDisplayer.h"
#include "gaming/LogisticConsigns.h"
#include "gaming/LogMaintenanceConsign.h"
#include "gaming/MaintenanceStates.h"
#include "gaming/Availability.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: AgentMaintenancePanel constructor
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
AgentMaintenancePanel::AgentMaintenancePanel( QWidget* parent, PanelStack_ABC& panel, Controllers& controllers, ItemFactory_ABC& factory )
    : LogisticPanel< AgentMaintenancePanel, LogMaintenanceConsign >( parent, panel, controllers, factory, tr( "Chaine maintenance" ) )
    , controllers_( controllers )
{
    AddConsignColumn( tr( "Type d'équipement :" ) );
    AddConsignColumn( tr( "Type de panne :" ) );
    AddConsignColumn( tr( "Etat :" ) );

    display_ = new DisplayBuilder( this, factory );
    display_->AddGroup( tr( "Etat chaine maintenance" ) )
                .AddLabel( tr( "Etat chaine" ) )
                .AddLabel( tr( "Régime de travail" ) )
                .AddLabel( tr( "Priorités" ) )
                .AddLabel( tr( "Priorités tactiques" ) );

    dispoHaulers_ = new ListDisplayer< AgentMaintenancePanel >( this, *this, factory );
    dispoHaulers_->AddColumn( tr( "Remorqueur" ) )
                  .AddColumn( tr( "Total" ) )
                  .AddColumn( tr( "Disponible" ) )
                  .AddColumn( tr( "Au travail" ) )
                  .AddColumn( tr( "Au repos" ) );
    dispoRepairers_ = new ListDisplayer< AgentMaintenancePanel >( this, *this, factory );
    dispoRepairers_->AddColumn( tr( "Réparateur" ) )
                    .AddColumn( tr( "Total" ) )
                    .AddColumn( tr( "Disponible" ) )
                    .AddColumn( tr( "Au travail" ) )
                    .AddColumn( tr( "Au repos" ) );

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
void AgentMaintenancePanel::NotifySelected( const Entity_ABC& agent )
{
    display_->Hide();
    dispoHaulers_->hide();
    dispoRepairers_->hide();

    Show();
    if( const LogisticConsigns* consigns = agent.Retrieve< LogisticConsigns >() )
        Parent::NotifyUpdated( *consigns );
    
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

