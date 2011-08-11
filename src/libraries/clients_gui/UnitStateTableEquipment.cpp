// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

/* TRANSLATOR gui::UnitStateTableEquipment */

#include "clients_gui_pch.h"
#include "UnitStateTableEquipment.h"
#include "moc_UnitStateTableEquipment.cpp"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: UnitStateTableEquipment constructor
// Created: ABR 2011-02-24
// -----------------------------------------------------------------------------
UnitStateTableEquipment::UnitStateTableEquipment( QWidget* parent, const char* name /*= 0*/ )
    : UnitStateTable_ABC( 0, 3, parent, name )
{
    horizontalHeader()->setLabel( eName,      tr( "Equipment" ) );
    horizontalHeader()->setLabel( eState,     tr( "State" ) );
    horizontalHeader()->setLabel( eBreakdown, tr( "Breakdown" ) );
    setColumnReadOnly( eName, true );
    setColumnStretchable( eName,      true );
    setColumnStretchable( eBreakdown, true );

    Populate( eEquipmentState_InMaintenance, selectionableStates_ );
    readOnlyStates_ << tools::ToString( eEquipmentState_InMaintenance );
    readOnlyStates_ << tools::ToString( eEquipmentState_Prisonner );

    connect( this, SIGNAL( valueChanged( int, int ) ), SLOT( OnValueChanged( int, int ) ) );
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableEquipment destructor
// Created: ABR 2011-02-24
// -----------------------------------------------------------------------------
UnitStateTableEquipment::~UnitStateTableEquipment()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableEquipment::onValueChanged
// Created: ABR 2011-02-25
// -----------------------------------------------------------------------------
void UnitStateTableEquipment::OnValueChanged( int row, int col )
{
    if( col == eState )
        item( row, eBreakdown )->setEnabled( GetComboValue< E_EquipmentState >( row, eState ) == eEquipmentState_RepairableWithEvacuation );
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableEquipment::AddLines
// Created: ABR 2011-07-08
// -----------------------------------------------------------------------------
void UnitStateTableEquipment::AddLines( const QString& name, int size, E_EquipmentState state, const QStringList& breakdowns, const std::vector< unsigned int > currentBreakdowns /*= std::vector< unsigned int >()*/ )
{
    if( !currentBreakdowns.empty() && ( state != eEquipmentState_RepairableWithEvacuation || static_cast< int >( currentBreakdowns.size() ) != size ) )
        throw std::runtime_error( __FUNCTION__ " Bad amount of breakdowns." );
    for( int i = 0; i < size; ++i )
    {
        unsigned int nRow = numRows();
        insertRows( nRow );

        setText( nRow, eName, name );
        bool isReadOnly = state == eEquipmentState_InMaintenance || state == eEquipmentState_Prisonner;
        AddCombo< E_EquipmentState >( nRow, eState, state, isReadOnly ? readOnlyStates_ : selectionableStates_, !isReadOnly );
        AddCombo< unsigned int >( nRow, eBreakdown, currentBreakdowns.empty() ? 0 : currentBreakdowns[ i ], breakdowns, !isReadOnly );
        OnValueChanged( nRow, eState );
    }
}
