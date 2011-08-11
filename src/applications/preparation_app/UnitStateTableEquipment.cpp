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
UnitStateTableEquipment::UnitStateTableEquipment( QWidget* parent, const char* name /*= 0*/ )
    : gui::UnitStateTableEquipment( parent, name )
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
    assert( extension.equipments_.size() == static_cast< unsigned int >( numRows() ) );
    int nRow = 0;
    for( InitialState::IT_Equipments it = extension.equipments_.begin(); it != extension.equipments_.end() && nRow < numRows(); ++it, ++nRow )
        if( it->name_ != item( nRow, eName )->text() ||
            it->state_ != GetComboValue< E_EquipmentState >( nRow, eState ) ||
            it->currentBreakdown_ != GetComboValue< unsigned int >( nRow, eBreakdown ) )
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
    for( InitialState::CIT_Equipments it = extension.equipments_.begin(); it != extension.equipments_.end(); ++it )
        if( it->state_ == eEquipmentState_RepairableWithEvacuation )
            AddLines( it->name_, 1, it->state_, it->breakdowns_, std::vector< unsigned int >( 1, it->currentBreakdown_ ) );
        else
            AddLines( it->name_, 1, it->state_, it->breakdowns_ );
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableEquipment::Commit
// Created: ABR 2011-07-05
// -----------------------------------------------------------------------------
void UnitStateTableEquipment::Commit( kernel::Entity_ABC& selected ) const
{
    assert( selected.GetTypeName() == kernel::Agent_ABC::typeName_ );
    InitialState& extension = selected.Get< InitialState >();
    assert( extension.equipments_.size() == static_cast< unsigned int >( numRows() ) );
    int nRow = 0;
    for( InitialState::IT_Equipments it = extension.equipments_.begin(); it != extension.equipments_.end(); ++it, ++nRow )
    {
        it->state_ = GetComboValue< E_EquipmentState >( nRow, eState );
        it->currentBreakdown_ = GetComboValue< unsigned int >( nRow, eBreakdown );
    }
}
