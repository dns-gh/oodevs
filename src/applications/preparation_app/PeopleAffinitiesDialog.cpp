// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "PeopleAffinitiesDialog.h"
#include "clients_kernel/Inhabitant_ABC.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/tools.h"
#include "clients_kernel/Controllers.h"

// -----------------------------------------------------------------------------
// Name: PeopleAffinitiesDialog constructor
// Created: LGY 2011-03-16
// -----------------------------------------------------------------------------
PeopleAffinitiesDialog::PeopleAffinitiesDialog( QWidget* parent, kernel::Controllers& controllers )
    : AffinitiesDialog( "PeopleAffinitiesDialog", parent, controllers, false )
    , controllers_( controllers )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: PeopleAffinitiesDialog destructor
// Created: LGY 2011-03-16
// -----------------------------------------------------------------------------
PeopleAffinitiesDialog::~PeopleAffinitiesDialog()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: PeopleAffinitiesDialog::NotifyContextMenu
// Created: LGY 2011-03-16
// -----------------------------------------------------------------------------
void PeopleAffinitiesDialog::NotifyContextMenu( const kernel::Inhabitant_ABC& entity, kernel::ContextMenu& menu )
{
    selected_ = const_cast< kernel::Inhabitant_ABC* >( &entity );
    menu.InsertItem( "Helpers", tools::translate( "AffinitiesDialog", "Change affinities" ), this, SLOT( Show() ), false, 1 );
}

// -----------------------------------------------------------------------------
// Name: PeopleAffinitiesDialog::NotifyContextMenu
// Created: LGY 2011-03-16
// -----------------------------------------------------------------------------
void PeopleAffinitiesDialog::NotifyContextMenu( const kernel::Population_ABC& entity, kernel::ContextMenu& menu )
{
    selected_ = const_cast< kernel::Population_ABC* >( &entity );
    menu.InsertItem( "Helpers", tools::translate( "AffinitiesDialog", "Change affinities" ), this, SLOT( Show() ), false, 1 );
}
