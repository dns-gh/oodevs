// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "AgentStatePanel.h"
#include "Controller.h"
#include "DisplayBuilder.h"
#include "GroupDisplayer.h"
#include "LabelDisplayer.h"
#include "Agent.h"
#include "ActionController.h"
#include "Attributes.h"
#include "Contaminations.h"
#include "HumanFactors.h"
#include "Reinforcements.h"
#include "Tools.h"
#include "Experience.h"
#include "Morale.h"
#include "Tiredness.h"
#include "LogisticLinks.h"
#include "Transports.h"

// -----------------------------------------------------------------------------
// Name: AgentStatePanel constructor
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
AgentStatePanel::AgentStatePanel( InfoPanels* info, Controller& controller, ActionController& actionController  )
    : InfoPanel_ABC( info, tr( "Etat" ) )
    , selected_( 0 )
{
    display_ = new DisplayBuilder( this );
    display_->AddGroup( "Info" )
                .AddLabel( "Nom:", true )
                .AddLabel( "Etat Opérationnel:" )
                .AddLabel( "Mort:" )
                .AddLabel( "Neutralisé:" )
                .AddLabel( "Vitesse:" )
                .AddLabel( "Direction:" )
                .AddLabel( "Altitude:" )
                .AddLabel( "Troupes:" )
                .AddLabel( "Transporteurs d'hommes disponibles:" );
    display_->AddGroup( "NBC" )
                .AddLabel( "Tenue NBC:" )
                .AddLabel( "Agents contaminants:" )
                .AddLabel( "Contamination:" );
    display_->AddGroup( "Postures" )
                .AddLabel( "Ancienne posture:" )
                .AddLabel( "Nouvelle posture:" );
    display_->AddGroup( "Communications" )
                .AddLabel( "Brouillé:" )
                .AddLabel( "Silence radio:" );
    display_->AddGroup( "Facteurs humains" )
                .AddLabel( "Experience:" )
                .AddLabel( "Moral:" )
                .AddLabel( "Fatigue:" );
    display_->AddGroup( "Etat décisionnel" )
                .AddLabel( "Etat opérationnel:" )
                .AddLabel( "RoE:" )
                .AddLabel( "RoE Population:" )
                .AddLabel( "Rapport de force:" )
                .AddLabel( "Disponibilité au tir indirect:" )
                .AddLabel( "Contact combat:" );
    display_->AddGroup( "Renforts" )
                .AddLabel( "Renforce:" )
                .AddLabel( "Est renforcé par:" );

    display_->AddGroup( "Pions Transportés" )
                .AddLabel( "" );

    display_->AddGroup( "Liens logistiques" )
                .AddLabel( "TC2:" )
                .AddLabel( "Supérieur maintenance:" )
                .AddLabel( "Supérieur santé:" )
                .AddLabel( "Supérieur ravitaillement:" );

    display_->AddGroup( "Etat martial" )
                .AddLabel( "Fait prisonnier:" )
                .AddLabel( "Rendu:" )
                .AddLabel( "Réfugiés pris en compte:" );

    controller.Register( *this );
    actionController.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: AgentStatePanel destructor
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
AgentStatePanel::~AgentStatePanel()
{
    delete display_;
}

// -----------------------------------------------------------------------------
// Name: AgentStatePanel::NotifySelected
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void AgentStatePanel::NotifySelected( const Agent* agent )
{
    if( selected_ != agent || ! agent )
    {
        selected_ = agent;
        if( selected_ )
        {
            Show();
            NotifyUpdated( selected_->Get< Attributes >() );
            NotifyUpdated( selected_->Get< Contaminations >() );
            NotifyUpdated( selected_->Get< HumanFactors >() );
            NotifyUpdated( selected_->Get< Reinforcements >() );
            if( selected_->Retrieve< LogisticLinks >() )
                NotifyUpdated( selected_->Get< LogisticLinks >() );
            else
                display_->Group( "Liens logistiques" ).Hide();
            NotifyUpdated( selected_->Get< Transports >() );
        }
        else
            Hide();
    };
}

// -----------------------------------------------------------------------------
// Name: AgentStatePanel::ShouldUpdate
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
template< typename Extension >
bool AgentStatePanel::ShouldUpdate( const Extension& extension )
{
    return IsVisible()
        && selected_ 
        && ( selected_->Retrieve< Extension >() == & extension );
}

// -----------------------------------------------------------------------------
// Name: AgentStatePanel::DisplayIfNeeded
// Created: AGE 2006-03-13
// -----------------------------------------------------------------------------
template< typename Extension >
void AgentStatePanel::DisplayIfNeeded( const Extension& extension )
{
    if( ShouldUpdate( extension ) )
        extension.Display( *display_ );
}

// -----------------------------------------------------------------------------
// Name: AgentStatePanel::NotifyUpdated
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void AgentStatePanel::NotifyUpdated( const Attributes& attributes )
{
    if( ! ShouldUpdate( attributes ) )
        return;

    display_->Group( "Info" ) .Display( "Nom:", selected_ );
    attributes.Display( *display_ );
}

// -----------------------------------------------------------------------------
// Name: AgentStatePanel::NotifyUpdated
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void AgentStatePanel::NotifyUpdated( const Contaminations& attributes )
{
    DisplayIfNeeded( attributes );
}

// -----------------------------------------------------------------------------
// Name: AgentStatePanel::NotifyUpdated
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void AgentStatePanel::NotifyUpdated( const HumanFactors& attributes )
{
    DisplayIfNeeded( attributes );
}

// -----------------------------------------------------------------------------
// Name: AgentStatePanel::NotifyUpdated
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void AgentStatePanel::NotifyUpdated( const Reinforcements& attributes )
{
    DisplayIfNeeded( attributes );
}

// -----------------------------------------------------------------------------
// Name: AgentStatePanel::NotifyUpdated
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void AgentStatePanel::NotifyUpdated( const LogisticLinks& attributes )
{
    DisplayIfNeeded( attributes );
}

// -----------------------------------------------------------------------------
// Name: AgentStatePanel::NotifyUpdated
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void AgentStatePanel::NotifyUpdated( const Transports& attributes )
{
    DisplayIfNeeded( attributes );
}

// $$$$ AGE 2006-02-16: Centraliser tout ca : 

// -----------------------------------------------------------------------------
// Name: AgentStatePanel::NotifyUpdated
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void AgentStatePanel::NotifyUpdated( const Agent& )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentStatePanel::NotifyDeleted
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void AgentStatePanel::NotifyDeleted( const Agent& agent )
{
    if( selected_ = & agent )
        NotifySelected( 0 );
}
