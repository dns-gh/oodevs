// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "AgentMedicalPanel.h"
#include "LogMedicalConsign.h"
#include "Agent.h"
#include "MedicalStates.h"
#include "LogisticConsigns.h"
#include "ListDisplayer.h"
#include "DisplayBuilder.h"
#include "Controllers.h"
#include "SubItemDisplayer.h"
#include "Units.h"

// -----------------------------------------------------------------------------
// Name: AgentMedicalPanel constructor
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
AgentMedicalPanel::AgentMedicalPanel( InfoPanels* pParent, Controllers& controllers )
    : InfoPanel_ABC( pParent, tr( "Ch. medicale" ) )
    , controllers_( controllers )
    , selected_( controllers )
{
    pConsignListView_ = new ListDisplayer< AgentMedicalPanel >( this, *this );
    pConsignListView_->AddColumn( "Demandes logistiques" );
    pConsignListView_->AddColumn( "" );

    pConsignHandledListView_ = new ListDisplayer< AgentMedicalPanel >( this, *this );
    pConsignHandledListView_->AddColumn( "Consignes en traitement" );
    pConsignHandledListView_->AddColumn( "" );

    logDisplay_ = new SubItemDisplayer( "Consigne :" );
    logDisplay_->AddChild( "Pion demandeur :" )
                .AddChild( "Pion traitant :" )
                .AddChild( "Blessure :" )
                .AddChild( "Reac. mental :" )
                .AddChild( "Contaminé NBC :" )
                .AddChild( "Etat :" );

    display_ = new DisplayBuilder( this );
    display_->AddGroup( "Etat chaine santé" )
                .AddLabel( "Etat chaine" )
                .AddLabel( "Priorités" )
                .AddLabel( "Priorités tactiques" );

    dispoReleveAmbulances_ = new ListDisplayer< AgentMedicalPanel >( this, *this );
    dispoReleveAmbulances_->AddColumn( "Ambulances relève" )
                           .AddColumn( "Total" )
                           .AddColumn( "Disponible" )
                           .AddColumn( "Au travail" )
                           .AddColumn( "Au repos" );
    dispoDispoRamassageAmbulances_ = new ListDisplayer< AgentMedicalPanel >( this, *this );
    dispoDispoRamassageAmbulances_->AddColumn( "Ambulances ramassage" )
                                   .AddColumn( "Total" )
                                   .AddColumn( "Disponible" )
                                   .AddColumn( "Au travail" )
                                   .AddColumn( "Au repos" );
    dispoDispoDoctors_ = new ListDisplayer< AgentMedicalPanel >( this, *this );
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
    delete logDisplay_; 
    delete display_;
}

// -----------------------------------------------------------------------------
// Name: AgentMedicalPanel::showEvent
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
void AgentMedicalPanel::showEvent( QShowEvent* )
{
    const Agent* selected = selected_;
    selected_ = 0;
    NotifySelected( selected );
}

// -----------------------------------------------------------------------------
// Name: AgentMedicalPanel::ShouldUpdate
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
template< typename Extension >
bool AgentMedicalPanel::ShouldUpdate( const Extension& e )
{
    return IsVisible() && selected_ && selected_->Retrieve< Extension >() == &e ;
}

// -----------------------------------------------------------------------------
// Name: AgentMedicalPanel::NotifySelected
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
void AgentMedicalPanel::NotifySelected( const Agent* agent )
{
    if( ! agent || agent != selected_ )
    {
        selected_ = agent;
        if( selected_ )
        {
            pConsignListView_->hide();
            pConsignHandledListView_->hide();
            display_->Hide();
            dispoReleveAmbulances_->hide();
            dispoDispoRamassageAmbulances_->hide();
            dispoDispoDoctors_->hide();

            Show();
            NotifyUpdated( selected_->Get< LogisticConsigns >() );
            if( selected_->Retrieve< MedicalStates >() )
                NotifyUpdated( selected_->Get< MedicalStates >() );
        }
        else
            Hide();
    }
}

// -----------------------------------------------------------------------------
// Name: AgentMedicalPanel::NotifyUpdated
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
void AgentMedicalPanel::NotifyUpdated( const LogisticConsigns& consigns )
{
    if( ! ShouldUpdate( consigns ) )
        return;

    pConsignListView_->DeleteTail( 
        pConsignListView_->DisplayList( consigns.requestedMedical_.begin(), consigns.requestedMedical_.end() )
        );
        
    pConsignHandledListView_->DeleteTail( 
        pConsignHandledListView_->DisplayList( consigns.handledMedical_.begin(), consigns.handledMedical_.end() )
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
        consign->Display( (*logDisplay_)( item ) );
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

// -----------------------------------------------------------------------------
// Name: AgentMedicalPanel::NotifyUpdated
// Created: AGE 2006-03-01
// -----------------------------------------------------------------------------
void AgentMedicalPanel::NotifyUpdated( const LogMedicalConsign& consign )
{
    ValuedListItem* item = FindItem( &consign, pConsignListView_->firstChild() );
    if( ! item )
        item = FindItem( &consign, pConsignHandledListView_->firstChild() );
    if( item )
        consign.Display( (*logDisplay_)( item ) );
}
