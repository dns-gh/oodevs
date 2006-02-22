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
#include "Displayer.h"
#include "DisplayGroup.h"
#include "DisplayItem.h"
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
#include "InfoPanel.h"

// -----------------------------------------------------------------------------
// Name: AgentStatePanel constructor
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
AgentStatePanel::AgentStatePanel( InfoPanel* info, Controller& controller, ActionController& actionController  )
    : InfoPanel_ABC( info, tr( "Etat" ) )
    , selected_( 0 )
{
    display_ = new Displayer( this );
    display_->AddGroup( "Info" )
                .AddItem( "Nom:", true )
                .AddItem( "Etat Opérationnel:" )
                .AddItem( "Mort:" )
                .AddItem( "Neutralisé:" )
                .AddItem( "Vitesse:" )
                .AddItem( "Direction:" )
                .AddItem( "Altitude:" )
                .AddItem( "Troupes:" )
                .AddItem( "Transporteurs d'hommes disponibles:" );
    display_->AddGroup( "NBC" )
                .AddItem( "Tenue NBC:" )
                .AddItem( "Agents contaminants:" )
                .AddItem( "Contamination:" );
    display_->AddGroup( "Postures" )
                .AddItem( "Ancienne posture:" )
                .AddItem( "Nouvelle posture:" );
    display_->AddGroup( "Communications" )
                .AddItem( "Brouillé:" )
                .AddItem( "Silence radio:" );
    display_->AddGroup( "Facteurs humains" )
                .AddItem( "Experience:" )
                .AddItem( "Moral:" )
                .AddItem( "Fatigue:" );
    display_->AddGroup( "Etat décisionnel" )
                .AddItem( "Etat opérationnel:" )
                .AddItem( "RoE:" )
                .AddItem( "RoE Population:" )
                .AddItem( "Rapport de force:" )
                .AddItem( "Disponibilité au tir indirect:" )
                .AddItem( "Contact combat:" );
    display_->AddGroup( "Renforts" )
                .AddItem( "Renforce:" )
                .AddItem( "Est renforcé par:" );

    display_->AddGroup( "Pions Transportés" )
                .AddItem( "" );

    display_->AddGroup( "Liens logistiques" )
                .AddItem( "TC2:" )
                .AddItem( "Supérieur maintenance:" )
                .AddItem( "Supérieur santé:" )
                .AddItem( "Supérieur ravitaillement:" );

    display_->AddGroup( "Etat martial" )
                .AddItem( "Fait prisonnier:" )
                .AddItem( "Rendu:" )
                .AddItem( "Réfugiés pris en compte:" );

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
                display_->Group( "Liens logistiques" ).hide();
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
    return isVisible()
        && selected_ 
        && ( selected_->Retrieve< Extension >() == & extension );
}

// -----------------------------------------------------------------------------
// Name: AgentStatePanel::NotifyUpdated
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void AgentStatePanel::NotifyUpdated( const Attributes& attributes )
{
    if( ! ShouldUpdate( attributes ) )
        return;

    display_->Group( "Info" ) .Display( "Nom:", selected_->GetName().c_str() );

    attributes.Display( *display_ );
}

// -----------------------------------------------------------------------------
// Name: AgentStatePanel::NotifyUpdated
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void AgentStatePanel::NotifyUpdated( const Contaminations& attributes )
{
    if( ! ShouldUpdate( attributes ) )
        return;

    attributes.Display( *display_ );
}

// -----------------------------------------------------------------------------
// Name: AgentStatePanel::NotifyUpdated
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void AgentStatePanel::NotifyUpdated( const HumanFactors& attributes )
{
    if( ! ShouldUpdate( attributes ) )
        return;

    attributes.Display( *display_ );
}

// -----------------------------------------------------------------------------
// Name: AgentStatePanel::NotifyUpdated
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void AgentStatePanel::NotifyUpdated( const Reinforcements& attributes )
{
    if( ! ShouldUpdate( attributes ) )
        return;

    attributes.Display( *display_ );
}

// -----------------------------------------------------------------------------
// Name: AgentStatePanel::NotifyUpdated
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void AgentStatePanel::NotifyUpdated( const LogisticLinks& attributes )
{
    if( ! ShouldUpdate( attributes ) )
        return;
    
    attributes.Display( *display_ );
}

// -----------------------------------------------------------------------------
// Name: AgentStatePanel::NotifyUpdated
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void AgentStatePanel::NotifyUpdated( const Transports& attributes )
{
    if( ! ShouldUpdate( attributes ) )
        return;

    attributes.Display( *display_ );
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
