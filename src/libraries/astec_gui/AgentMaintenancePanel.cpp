// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "astec_gui_pch.h"
#include "AgentMaintenancePanel.h"
#include "DisplayBuilder.h"
#include "ListDisplayer.h"
#include "astec_kernel/Controllers.h"
#include "astec_gaming/LogisticConsigns.h"
#include "astec_kernel/Agent_ABC.h"
#include "astec_gaming/LogMaintenanceConsign.h"
#include "astec_gaming/MaintenanceStates.h"
#include "astec_gaming/Availability.h"
#include "astec_kernel/Units.h"
#include "SubItemDisplayer.h"

// -----------------------------------------------------------------------------
// Name: AgentMaintenancePanel constructor
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
AgentMaintenancePanel::AgentMaintenancePanel( PanelStack_ABC* pParent, Controllers& controllers, ItemFactory_ABC& factory )
    : LogisticPanel< AgentMaintenancePanel, LogMaintenanceConsign >( pParent, controllers, factory, tr( "Chaine maintenance" ) )
    , controllers_( controllers )
{
    AddConsignColumn( "Type d'�quipement :" );
    AddConsignColumn( "Type de panne :" );
    AddConsignColumn( "Etat :" );

    display_ = new DisplayBuilder( this, factory );
    display_->AddGroup( "Etat chaine maintenance" )
                .AddLabel( "Etat chaine" )
                .AddLabel( "R�gime de travail" )
                .AddLabel( "Priorit�s" )
                .AddLabel( "Priorit�s tactiques" );

    dispoHaulers_ = new ListDisplayer< AgentMaintenancePanel >( this, *this, factory );
    dispoHaulers_->AddColumn( "Remorqueur" )
                  .AddColumn( "Total" )
                  .AddColumn( "Disponible" )
                  .AddColumn( "Au travail" )
                  .AddColumn( "Au repos" );
    dispoRepairers_ = new ListDisplayer< AgentMaintenancePanel >( this, *this, factory );
    dispoRepairers_->AddColumn( "R�parateur" )
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
void AgentMaintenancePanel::NotifySelected( const Agent_ABC& agent )
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

