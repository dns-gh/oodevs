// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "InhabitantChangeHealthStateDialog.h"
#include "actions/ActionsModel.h"
#include "clients_kernel/Inhabitant_ABC.h"
#include "clients_kernel/tools.h"
//Added by qt3to4:
#include <Qt3Support/q3gridlayout.h>

// -----------------------------------------------------------------------------
// Name: InhabitantChangeHealthStateDialog constructor
// Created: ABR 2011-01-25
// -----------------------------------------------------------------------------
InhabitantChangeHealthStateDialog::InhabitantChangeHealthStateDialog( QWidget* pParent, kernel::Controllers& controllers, actions::ActionsModel& actionsModel, const kernel::Time_ABC& simulation, const kernel::Profile_ABC& profile )
    : ChangeHealthStateDialog_ABC( pParent, controllers, actionsModel, simulation, profile, 3 )
{
    AddLine( 0, tools::translate( "InhabitantChangeHealthStateDialog", "Healthy:" ) );
    AddLine( 1, tools::translate( "InhabitantChangeHealthStateDialog", "Wounded:" ) );
    AddLine( 2, tools::translate( "InhabitantChangeHealthStateDialog", "Dead:" ) );

}

// -----------------------------------------------------------------------------
// Name: InhabitantChangeHealthStateDialog destructor
// Created: JSR 2011-03-15
// -----------------------------------------------------------------------------
InhabitantChangeHealthStateDialog::~InhabitantChangeHealthStateDialog()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InhabitantChangeHealthStateDialog::NotifyContextMenu
// Created: JSR 2011-03-15
// -----------------------------------------------------------------------------
void InhabitantChangeHealthStateDialog::NotifyContextMenu( const kernel::Inhabitant_ABC& entity, kernel::ContextMenu& menu )
{
    DoNotifyContextMenu( entity, menu );
}

// -----------------------------------------------------------------------------
// Name: InhabitantChangeHealthStateDialog::CreateMagicAction
// Created: JSR 2011-03-15
// -----------------------------------------------------------------------------
actions::Action_ABC* InhabitantChangeHealthStateDialog::CreateMagicAction( actions::ActionsModel& actionsModel ) const
{
    return actionsModel.CreateInhabitantChangeHealthStateAction( GetSpinboxValue( 0 ), GetSpinboxValue( 1 ), GetSpinboxValue( 2 ), GetSelected() );
}

// -----------------------------------------------------------------------------
// Name: InhabitantChangeHealthStateDialog::GetValue
// Created: JSR 2011-03-15
// -----------------------------------------------------------------------------
unsigned int InhabitantChangeHealthStateDialog::GetValue( unsigned int index ) const
{
    const kernel::Inhabitant_ABC& selected = static_cast< const kernel::Inhabitant_ABC& >( GetSelected() );
    switch( index )
    {
    case 0:
        return selected.GetHealthy();
    case 1:
        return selected.GetWounded();
    case 2:
        return selected.GetDead();
    default:
        throw std::exception( "InhabitantChangeHealthStateDialog : bad Id" );
    }
}
