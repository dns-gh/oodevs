// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "AgentResourcesPanel.h"

#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/DotationType.h"
#include "clients_gui/ListDisplayer.h"
#include "clients_gui/Tools.h"
#include "preparation/Dotations.h"
#include "preparation/Dotation.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: AgentResourcesPanel constructor
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
AgentResourcesPanel::AgentResourcesPanel( QWidget* parent, PanelStack_ABC& panel, Controllers& controllers, ItemFactory_ABC& factory )
    : InfoPanel_ABC( parent, panel, tr( "Ressources" ) )
    , controllers_( controllers )
    , selected_( controllers )
{
    pDotations_ = new T_ListView( this, *this, factory );
    pDotations_->AddColumn( tr( "Dotation" ) )
                .AddColumn( tr( "Quantity" ) );

    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: AgentResourcesPanel destructor
// Created: APE 2004-03-10
// -----------------------------------------------------------------------------
AgentResourcesPanel::~AgentResourcesPanel()
{
    controllers_.Remove( *this );
}

// -----------------------------------------------------------------------------
// Name: AgentResourcesPanel::showEvent
// Created: AGE 2006-02-27
// -----------------------------------------------------------------------------
void AgentResourcesPanel::showEvent( QShowEvent* )
{
    const Agent_ABC* selected = selected_;
    selected_ = 0;
    NotifySelected( selected );
}

// -----------------------------------------------------------------------------
// Name: AgentResourcesPanel::ShouldUpdate
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
template< typename T >
bool AgentResourcesPanel::ShouldUpdate( const T& a )
{
    return IsVisible()
        && selected_ 
        && ( selected_->Retrieve< T >() == & a );
}

// -----------------------------------------------------------------------------
// Name: AgentResourcesPanel::NotifyUpdated
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
void AgentResourcesPanel::NotifyUpdated( const Dotations& a )
{
    if( ! ShouldUpdate( a ) )
        return;
    pDotations_->DeleteTail( 
        pDotations_->DisplayList( a.CreateIterator() )
    );
}

// -----------------------------------------------------------------------------
// Name: AgentResourcesPanel::Display
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
void AgentResourcesPanel::Display( const Dotation& dotation, Displayer_ABC& displayer, ValuedListItem* )
{
    displayer.Display( tr( "Dotation" ), dotation.type_ )
             .Display( tr( "Quantity" ), dotation.quantity_ );
}

// -----------------------------------------------------------------------------
// Name: AgentResourcesPanel::UpdateExtension
// Created: AGE 2006-08-03
// -----------------------------------------------------------------------------
template< typename T >
void AgentResourcesPanel::UpdateExtension( const Agent_ABC& agent )
{
    const T* extension = agent.Retrieve< T >();
    if( extension )
        NotifyUpdated( *extension );
}

// -----------------------------------------------------------------------------
// Name: AgentResourcesPanel::NotifySelected
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
void AgentResourcesPanel::NotifySelected( const Agent_ABC* agent )
{
    if( selected_ != agent || ! agent )
    {
        selected_ = agent;
        if( selected_ )
        {
            // $$$$ AGE 2006-02-17: refactor that
            Show();
            UpdateExtension< Dotations > ( *selected_ );
        }
        else
            Hide();
    }
}
