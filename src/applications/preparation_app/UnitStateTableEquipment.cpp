// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "UnitStateTableEquipment.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Types.h"
#include "preparation/InitialState.h"
#include "preparation/InitialStateEquipment.h"
#include <assert.h>

// -----------------------------------------------------------------------------
// Name: UnitStateTableEquipment constructor
// Created: ABR 2011-07-05
// -----------------------------------------------------------------------------
UnitStateTableEquipment::UnitStateTableEquipment( QWidget* parent, gui::DisplayExtractor& extractor, kernel::Controllers& controllers )
    : gui::UnitStateTableEquipment( parent, extractor, controllers )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableEquipment destructor
// Created: ABR 2011-07-05
// -----------------------------------------------------------------------------
UnitStateTableEquipment::~UnitStateTableEquipment()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableEquipment::HasChanged
// Created: ABR 2011-07-11
// -----------------------------------------------------------------------------
bool UnitStateTableEquipment::HasChanged( kernel::Entity_ABC& selected ) const
{
    if( selected.GetTypeName() != kernel::Agent_ABC::typeName_ )
        return false;
    InitialState& extension = selected.Get< InitialState >();
    if( extension.equipments_.size() == 0 )
        return false;
    assert( extension.equipments_.size() == static_cast< unsigned int >( dataModel_.rowCount() ) );
    int row = 0;
    for( InitialState::IT_Equipments it = extension.equipments_.begin(); it != extension.equipments_.end() && row < dataModel_.rowCount(); ++it, ++row )
        if( it->name_ != GetDisplayData( row, eName ) ||
            it->state_ != GetEnumData< E_EquipmentState >( row, eState ) ||
            ( dataModel_.item( row, eBreakdown ) && it->currentBreakdown_ != GetUserData( row, eBreakdown ).toUInt() ) )
            return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableEquipment::Load
// Created: ABR 2011-07-05
// -----------------------------------------------------------------------------
void UnitStateTableEquipment::Load( kernel::Entity_ABC& selected )
{
    assert( selected.GetTypeName() == kernel::Agent_ABC::typeName_ );
    InitialState& extension = selected.Get< InitialState >();
    for( auto it = extension.equipments_.begin(); it != extension.equipments_.end(); ++it )
        if( it->state_ == eEquipmentState_RepairableWithEvacuation )
            AddLines( it->name_, selected, 1, it->state_, it->breakdowns_, std::vector< unsigned int >( 1, it->currentBreakdown_ ) );
        else
            AddLines( it->name_, selected, 1, it->state_, it->breakdowns_ );
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableEquipment::Commit
// Created: ABR 2011-07-05
// -----------------------------------------------------------------------------
void UnitStateTableEquipment::Commit( kernel::Entity_ABC& selected ) const
{
    assert( selected.GetTypeName() == kernel::Agent_ABC::typeName_ );
    InitialState& extension = selected.Get< InitialState >();
    assert( extension.equipments_.size() == static_cast< unsigned int >( dataModel_.rowCount() ) );
    int row = 0;
    for( InitialState::IT_Equipments it = extension.equipments_.begin(); it != extension.equipments_.end(); ++it, ++row )
    {
        it->state_ = GetEnumData< E_EquipmentState >( row, eState );
        it->currentBreakdown_ = ( dataModel_.item( row, eBreakdown ) ) ? GetUserData( row, eBreakdown ).toUInt() : 0;
    }
}
