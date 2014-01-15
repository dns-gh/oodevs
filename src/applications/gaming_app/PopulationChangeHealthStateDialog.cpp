// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "PopulationChangeHealthStateDialog.h"
#include "actions/ActionsModel.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/tools.h"

// -----------------------------------------------------------------------------
// Name: PopulationChangeHealthStateDialog constructor
// Created: JSR 2011-03-15
// -----------------------------------------------------------------------------
PopulationChangeHealthStateDialog::PopulationChangeHealthStateDialog( QWidget* pParent, kernel::Controllers& controllers, actions::ActionsModel& actionsModel, const kernel::Time_ABC& simulation, const kernel::Profile_ABC& profile )
    : ChangeHealthStateDialog_ABC( pParent, controllers, actionsModel, simulation, profile, 4 )
{
    gui::SubObjectName subObject( "PopulationChangeHealthStateDialog" );
    AddLine( 0, "Healthy", tools::translate( "PopulationChangeHealthStateDialog", "Healthy:" ) );
    AddLine( 1, "Wounded", tools::translate( "PopulationChangeHealthStateDialog", "Wounded:" ) );
    AddLine( 2, "Contaminated", tools::translate( "PopulationChangeHealthStateDialog", "Contaminated:" ) );
    AddLine( 3, "Dead", tools::translate( "PopulationChangeHealthStateDialog", "Dead:" ) );
}

// -----------------------------------------------------------------------------
// Name: PopulationChangeHealthStateDialog destructor
// Created: JSR 2011-03-15
// -----------------------------------------------------------------------------
PopulationChangeHealthStateDialog::~PopulationChangeHealthStateDialog()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationChangeHealthStateDialog::NotifyContextMenu
// Created: JSR 2011-03-15
// -----------------------------------------------------------------------------
void PopulationChangeHealthStateDialog::NotifyContextMenu( const kernel::Population_ABC& entity, kernel::ContextMenu& menu )
{
    DoNotifyContextMenu( entity, menu );
}

// -----------------------------------------------------------------------------
// Name: PopulationChangeHealthStateDialog::CreateMagicAction
// Created: JSR 2011-03-15
// -----------------------------------------------------------------------------
void PopulationChangeHealthStateDialog::CreateMagicAction( actions::ActionsModel& actionsModel ) const
{
    actionsModel.PublishCrowdChangeHealthStateAction( GetSpinboxValue( 0 ), GetSpinboxValue( 1 ), GetSpinboxValue( 2 ), GetSpinboxValue( 3 ), GetSelected() );
}

// -----------------------------------------------------------------------------
// Name: PopulationChangeHealthStateDialog::GetValue
// Created: JSR 2011-03-15
// -----------------------------------------------------------------------------
unsigned int PopulationChangeHealthStateDialog::GetValue( unsigned int index ) const
{
    const kernel::Population_ABC& selected = static_cast< const kernel::Population_ABC& >( GetSelected() );
    switch( index )
    {
    case 0:
        return selected.GetHealthyHumans();
    case 1:
        return selected.GetWoundedHumans();
    case 2:
        return selected.GetContaminatedHumans();
    case 3:
        return selected.GetDeadHumans();
    default:
        throw MASA_EXCEPTION( "PopulationChangeHealthStateDialog : bad Id" );
    }
}
