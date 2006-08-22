// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "AgentMedicalPanel.h"
#include "gaming/LogMedicalConsign.h"
#include "clients_kernel/Agent_ABC.h"
#include "gaming/MedicalStates.h"
#include "gaming/LogisticConsigns.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Units.h"
#include "SubItemDisplayer.h"
#include "ListDisplayer.h"
#include "DisplayBuilder.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: AgentMedicalPanel constructor
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
AgentMedicalPanel::AgentMedicalPanel( QWidget* parent, PanelStack_ABC& panel, Controllers& controllers, ItemFactory_ABC& factory )
    : LogisticPanel< AgentMedicalPanel, LogMedicalConsign >( parent, panel, controllers, factory, tr( "Chaine santé" ) )
    , controllers_( controllers )
{
    AddConsignColumn( "Blessure :" );
    AddConsignColumn( "Reac. mental :" );
    AddConsignColumn( "Contaminé NBC :" );
    AddConsignColumn( "Etat :" );

    display_ = new DisplayBuilder( this, factory );
    display_->AddGroup( "Etat chaine santé" )
                .AddLabel( "Etat chaine" )
                .AddLabel( "Priorités" )
                .AddLabel( "Priorités tactiques" );

    dispoReleveAmbulances_ = new ListDisplayer< AgentMedicalPanel >( this, *this, factory );
    dispoReleveAmbulances_->AddColumn( "Ambulances relève" )
                           .AddColumn( "Total" )
                           .AddColumn( "Disponible" )
                           .AddColumn( "Au travail" )
                           .AddColumn( "Au repos" );
    dispoDispoRamassageAmbulances_ = new ListDisplayer< AgentMedicalPanel >( this, *this, factory );
    dispoDispoRamassageAmbulances_->AddColumn( "Ambulances ramassage" )
                                   .AddColumn( "Total" )
                                   .AddColumn( "Disponible" )
                                   .AddColumn( "Au travail" )
                                   .AddColumn( "Au repos" );
    dispoDispoDoctors_ = new ListDisplayer< AgentMedicalPanel >( this, *this, factory );
    dispoDispoDoctors_->AddColumn( "Médecins" )
                       .AddColumn( "Total" )
                       .AddColumn( "Disponibles" )
                       .AddColumn( "Au travail" )
                       .AddColumn( "Au repos" );

    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: AgentMedicalPanel destructor
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
AgentMedicalPanel::~AgentMedicalPanel()
{
    controllers_.Remove( *this );
    delete display_;
}


// -----------------------------------------------------------------------------
// Name: AgentMedicalPanel::NotifySelected
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
void AgentMedicalPanel::NotifySelected( const Agent_ABC& agent )
{
    display_->Hide();
    dispoReleveAmbulances_->hide();
    dispoDispoRamassageAmbulances_->hide();
    dispoDispoDoctors_->hide();

    Show();
    if( const LogisticConsigns* consigns = agent.Retrieve< LogisticConsigns >() )
        Parent::NotifyUpdated( *consigns );
    if( const MedicalStates* states = agent.Retrieve< MedicalStates >() )
        NotifyUpdated( *states );
}

// -----------------------------------------------------------------------------
// Name: AgentMedicalPanel::DisplayRequested
// Created: AGE 2006-07-04
// -----------------------------------------------------------------------------
void AgentMedicalPanel::DisplayRequested( const LogisticConsigns& consigns, ListDisplayer< AgentMedicalPanel >* list )
{
    list->DeleteTail( 
        list->DisplayList( consigns.requestedMedical_.begin(), consigns.requestedMedical_.end() )
        );
}

// -----------------------------------------------------------------------------
// Name: AgentMedicalPanel::DisplayHandled
// Created: AGE 2006-07-04
// -----------------------------------------------------------------------------
void AgentMedicalPanel::DisplayHandled( const LogisticConsigns& consigns, ListDisplayer< AgentMedicalPanel >* list )
{
    list->DeleteTail( 
        list->DisplayList( consigns.handledMedical_.begin(), consigns.handledMedical_.end() )
        );
}

// -----------------------------------------------------------------------------
// Name: AgentMedicalPanel::Display
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
void AgentMedicalPanel::Display( const LogMedicalConsign* consign, Displayer_ABC& , ValuedListItem* item )
{
    if( consign )
    {
        item->SetValue( consign );
        consign->Display( GetDisplayer( item ) );
    }
}

// -----------------------------------------------------------------------------
// Name: AgentMedicalPanel::NotifyUpdated
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
void AgentMedicalPanel::NotifyUpdated( const MedicalStates& consigns )
{
    if( ! ShouldUpdate( consigns ) )
        return;

    consigns.Display( *display_ );

    dispoReleveAmbulances_->DeleteTail( 
        dispoReleveAmbulances_->DisplayList( consigns.dispoReleveAmbulances_.begin(), consigns.dispoReleveAmbulances_.end() )
        );

    dispoDispoRamassageAmbulances_->DeleteTail( 
        dispoDispoRamassageAmbulances_->DisplayList( consigns.dispoRamassageAmbulances_.begin(), consigns.dispoRamassageAmbulances_.end() )
        );

    dispoDispoDoctors_->DeleteTail( 
        dispoDispoDoctors_->DisplayList( consigns.dispoDoctors_.begin(), consigns.dispoDoctors_.end() )
        );
}


// -----------------------------------------------------------------------------
// Name: AgentMedicalPanel::Display
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
void AgentMedicalPanel::Display( const Availability& availability, Displayer_ABC& displayer, ValuedListItem* )
{
    availability.Display( displayer );
}

