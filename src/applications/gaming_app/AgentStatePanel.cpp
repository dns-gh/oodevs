// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "AgentStatePanel.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/AgentExtensions.h"
#include "clients_kernel/Controllers.h"
#include "clients_gui/DisplayBuilder.h"
#include "clients_gui/GroupDisplayer.h"
#include "clients_gui/LabelDisplayer.h"
#include "gaming/Contaminations.h"
#include "gaming/Reinforcements.h"
#include "gaming/Transports.h"

using namespace kernel;
using namespace gui;

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
    display_->AddGroup( tr( "Info" ) )
                .AddLabel( tr( "Nom:" ), true )
                .AddLabel( tr( "Etat opérationnel:" ) )
                .AddLabel( tr( "Mort:" ) )
                .AddLabel( tr( "Neutralisé:" ) )
                .AddLabel( tr( "Vitesse:" ) )
                .AddLabel( tr( "Direction:" ) )
                .AddLabel( tr( "Altitude:" ) )
                .AddLabel( tr( "Troupes:" ) )
                .AddLabel( tr( "Transporteurs d'hommes:" ) );
    display_->AddGroup( tr( "NBC" ) )
                .AddLabel( tr( "Tenue NBC:" ) )
                .AddLabel( tr( "Agents contaminants:" ) )
                .AddLabel( tr( "Contamination:" ) );
    display_->AddGroup( tr( "Postures" ) )
                .AddLabel( tr( "Ancienne posture:" ) )
                .AddLabel( tr( "Nouvelle posture:" ) );
    display_->AddGroup( tr( "Communications" ) )
                .AddLabel( tr( "Brouillé:" ) )
                .AddLabel( tr( "Silence radio:" ) );
    display_->AddGroup( tr( "Facteurs humains" ) )
                .AddLabel( tr( "Experience:" ) )
                .AddLabel( tr( "Moral:" ) )
                .AddLabel( tr( "Fatigue:" ) );
    display_->AddGroup( tr( "Etat décisionnel" ) )
                .AddLabel( tr( "Etat opérationnel:" ) )
                .AddLabel( tr( "RoE:" ) )
                .AddLabel( tr( "RoE Population:" ) )
                .AddLabel( tr( "Rapport de force:" ) )
                .AddLabel( tr( "Disponibilité au tir indirect:" ) )
                .AddLabel( tr( "Contact combat:" ) );
    display_->AddGroup( tr( "Renforts" ) )
                .AddLabel( tr( "Renforce:" ) )
                .AddLabel( tr( "Est renforcé par:" ) );

    display_->AddGroup( tr( "Pions Transportés" ) )
                .AddLabel( tr( "" ) );

    display_->AddGroup( tr( "Liens logistiques" ) )
                .AddLabel( tr( "TC2:" ) )
                .AddLabel( tr( "Supérieur maintenance:" ) )
                .AddLabel( tr( "Supérieur santé:" ) )
                .AddLabel( tr( "Supérieur ravitaillement:" ) );

    display_->AddGroup( tr( "Etat martial" ) )
                .AddLabel( tr( "Fait prisonnier:" ) )
                .AddLabel( tr( "Rendu:" ) )
                .AddLabel( tr( "Réfugiés pris en compte:" ) );

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
// Name: AgentStatePanel::showEvent
// Created: AGE 2006-08-22
// -----------------------------------------------------------------------------
void AgentStatePanel::showEvent( QShowEvent* )
{
    const Agent_ABC* selected = selected_;
    selected_ = 0;
    NotifySelected( selected );
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
            UpdateExtension< Attributes_ABC >( *selected_ );
            UpdateExtension< HumanFactors_ABC >( *selected_ );
            UpdateExtension< Contaminations >( *selected_ );
            UpdateExtension< Reinforcements >( *selected_ );
            if( ! UpdateExtension< LogisticLinks_ABC >( *selected_ ) )
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
void AgentStatePanel::NotifyUpdated( const Attributes_ABC& attributes )
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
void AgentStatePanel::NotifyUpdated( const HumanFactors_ABC& attributes )
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
void AgentStatePanel::NotifyUpdated( const LogisticLinks_ABC& attributes )
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
