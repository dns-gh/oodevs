// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "AgentAffinitiesDialog.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/tools.h"
#include "clients_kernel/Controllers.h"

// -----------------------------------------------------------------------------
// Name: AgentAffinitiesDialog::AgentAffinitiesDialog
// Created: LGY 2011-03-14
// -----------------------------------------------------------------------------
AgentAffinitiesDialog::AgentAffinitiesDialog( QWidget* parent, kernel::Controllers& controllers )
    : AffinitiesDialog( "AgentAffinitiesDialog", parent, controllers, true )
    , controllers_( controllers )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: AgentAffinitiesDialog::~AgentAffinitiesDialog
// Created: LGY 2011-03-14
// -----------------------------------------------------------------------------
AgentAffinitiesDialog::~AgentAffinitiesDialog()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: AgentAffinitiesDialog::NotifyContextMenu
// Created: LGY 2011-03-16
// -----------------------------------------------------------------------------
void AgentAffinitiesDialog::NotifyContextMenu( const kernel::Agent_ABC& entity, kernel::ContextMenu& menu )
{
    selected_ = const_cast< kernel::Agent_ABC* >( &entity );
    menu.InsertItem( "Helpers", tools::translate( "AffinitiesDialog", "Change affinities" ), this, SLOT( Show() ), false, 1 );
}
