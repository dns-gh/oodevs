// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "AgentResourcesPanel.h"

#include "EquipmentsListView.h"
#include "DotationsListView.h"
#include "LendingsListView.h"
#include "BorrowingsListView.h"
#include "HumansListView.h"
#include "clients_kernel/Controllers.h"

// -----------------------------------------------------------------------------
// Name: AgentResourcesPanel constructor
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
AgentResourcesPanel::AgentResourcesPanel( QWidget* parent, gui::PanelStack_ABC& panel, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory )
    : gui::InfoPanel_ABC( parent, panel, tr( "Resources" ) )
    , controllers_( controllers )
    , selected_( controllers )
{
    new EquipmentsListView( this, controllers_, factory );
    new DotationsListView( this, controllers_, factory );
    new HumansListView( this, controllers_, factory );
    new LendingsListView( this, controllers_, factory );
    new BorrowingsListView( this, controllers_, factory );
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
    const kernel::Agent_ABC* selected = selected_;
    selected_ = 0;
    NotifySelected( selected );
}

// -----------------------------------------------------------------------------
// Name: AgentResourcesPanel::NotifySelected
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
void AgentResourcesPanel::NotifySelected( const kernel::Agent_ABC* agent )
{
    if( selected_ != agent || ! agent )
    {
        selected_ = agent;
        if( selected_ )
            Show();
        else
            Hide();
    }
}
