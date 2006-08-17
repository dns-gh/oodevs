// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_gui_pch.h"
#include "AgentStatePanel.h"
#include "astec_kernel/Controller.h"
#include "DisplayBuilder.h"
#include "GroupDisplayer.h"
#include "LabelDisplayer.h"
#include "astec_kernel/Agent_ABC.h"
#include "astec_kernel/Controllers.h"
#include "astec_gaming/Attributes.h"
#include "astec_gaming/Contaminations.h"
#include "astec_gaming/HumanFactors.h"
#include "astec_gaming/Reinforcements.h"
#include "astec_gaming/Experience.h"
#include "astec_gaming/Morale.h"
#include "astec_gaming/Tiredness.h"
#include "astec_gaming/LogisticLinks.h"
#include "astec_gaming/Transports.h"

// -----------------------------------------------------------------------------
// Name: AgentStatePanel constructor
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
AgentStatePanel::AgentStatePanel( QWidget* parent, PanelStack_ABC& panel, Controllers& controllers, ItemFactory_ABC& factory )
    : InfoPanel_ABC( parent, panel, tr( "Etat agent" ) )
    , controllers_( controllers )
    , selected_( controllers )
{
    display_ = new DisplayBuilder( this, factory );
    display_->AddGroup( "Info" )
                .AddLabel( "Nom:", true )
                .AddLabel( "Etat opérationnel:" )
                .AddLabel( "Mort:" )
                .AddLabel( "Neutralisé:" )
                .AddLabel( "Vitesse:" )
                .AddLabel( "Direction:" )
                .AddLabel( "Altitude:" )
                .AddLabel( "Troupes:" )
                .AddLabel( "Transporteurs d'hommes:" );
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

    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: AgentStatePanel destructor
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
AgentStatePanel::~AgentStatePanel()
{
    controllers_.Remove( *this );
}

// -----------------------------------------------------------------------------
// Name: AgentStatePanel::UpdateExtension
// Created: AGE 2006-08-03
// -----------------------------------------------------------------------------
template< typename Extension >
bool AgentStatePanel::UpdateExtension( const Agent_ABC& agent )
{
    const Extension* extension = agent.Retrieve< Extension>();
    if( extension )
        NotifyUpdated( *extension );
    return extension;
}

// -----------------------------------------------------------------------------
// Name: AgentStatePanel::NotifySelected
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void AgentStatePanel::NotifySelected( const Agent_ABC* agent )
{
    if( selected_ != agent || ! agent )
    {
        selected_ = agent;
        if( selected_ )
        {
            Show();
            UpdateExtension< Attributes >( *selected_ );
            UpdateExtension< Contaminations >( *selected_ );
            UpdateExtension< HumanFactors >( *selected_ );
            UpdateExtension< Reinforcements >( *selected_ );
            if( ! UpdateExtension< LogisticLinks >( *selected_ ) )
                display_->Group( "Liens logistiques" ).Hide();
            UpdateExtension< Transports >( *selected_ );
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
// Name: AgentStatePanel::NotifyDeleted
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void AgentStatePanel::NotifyDeleted( const Agent_ABC& agent )
{
    if( selected_ = & agent )
        NotifySelected( 0 );
}
