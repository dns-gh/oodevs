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
    : InfoPanel_ABC( parent, panel, tr( "Unit state" ) )
    , controllers_( controllers )
    , selected_( controllers )
{
    display_ = new DisplayBuilder( this, factory );
    display_->AddGroup( tr( "Info" ) )
                .AddLabel( tr( "Name:" ), true )
                .AddLabel( tr( "Operational state:" ) )
                .AddLabel( tr( "Dead:" ) )
                .AddLabel( tr( "Neutralized:" ) )
                .AddLabel( tr( "Speed:" ) )
                .AddLabel( tr( "Direction:" ) )
                .AddLabel( tr( "Height:" ) )
                .AddLabel( tr( "Troops:" ) )
                .AddLabel( tr( "Human transportation:" ) );
    display_->AddGroup( tr( "NBC" ) )
                .AddLabel( tr( "NBC suit on:" ) )
                .AddLabel( tr( "Contaminating agents:" ) )
                .AddLabel( tr( "Contamination level:" ) );
    display_->AddGroup( tr( "Stances" ) )
                .AddLabel( tr( "Previous stance:" ) )
                .AddLabel( tr( "New stance:" ) )
                .AddLabel( tr( "Setup state:" ) );
    display_->AddGroup( tr( "Communications" ) )
                .AddLabel( tr( "Jammed:" ) )
                .AddLabel( tr( "Radio silence:" ) );
    display_->AddGroup( tr( "Human factors" ) )
                .AddLabel( tr( "Experience:" ) )
                .AddLabel( tr( "Moral:" ) )
                .AddLabel( tr( "Tiredness:" ) );
    display_->AddGroup( tr( "Decisional state" ) )
                .AddLabel( tr( "Operational state:" ) )
                .AddLabel( tr( "Rules of engagement:" ) )
                .AddLabel( tr( "Population RoE:" ) )
                .AddLabel( tr( "Force ratio:" ) )
                .AddLabel( tr( "Indirect fire availability:" ) )
                .AddLabel( tr( "Intention:" ) );
    display_->AddGroup( tr( "Reinforcements" ) )
                .AddLabel( tr( "Reinforcing:" ) )
                .AddLabel( tr( "Reinforced by:" ) );

    display_->AddGroup( tr( "Towed unit" ) )
                .AddLabel( tr( "" ) );

    display_->AddGroup( tr( "Logistic links" ) )
                .AddLabel( tr( "TC2:" ) )
                .AddLabel( tr( "Maintenance superior:" ) )
                .AddLabel( tr( "Medical superior:" ) )
                .AddLabel( tr( "Supply superior:" ) );

    display_->AddGroup( tr( "Military state" ) )
                .AddLabel( tr( "Prisoner:" ) )
                .AddLabel( tr( "Surrender:" ) )
                .AddLabel( tr( "Refugees picked up:" ) );

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
