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
#include "clients_kernel/Agent_ABC.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: UnitStateTableEquipment constructor
// Created: ABR 2011-02-24
// -----------------------------------------------------------------------------
UnitStateTableEquipment::UnitStateTableEquipment( QWidget* parent, kernel::Controllers& controllers )
    : UnitStateTable_ABC( parent, 3, controllers )
{
    horizontalHeaders_ << tr( "Equipments" )
                       << tr( "State" )
                       << tr( "Breakdown" );
    connect( &dataModel_, SIGNAL( itemChanged( QStandardItem* ) ), SLOT( OnItemChanged( QStandardItem* ) ) );
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
// Name: UnitStateTableEquipment::IsReadOnlyForType
// Created: JSR 2012-03-22
// -----------------------------------------------------------------------------
bool UnitStateTableEquipment::IsReadOnlyForType( const std::string& typeName ) const
{
    return typeName != kernel::Agent_ABC::typeName_;
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableEquipment::Purge
// Created: ABR 2011-10-11
// -----------------------------------------------------------------------------
void UnitStateTableEquipment::Purge()
{
    UnitStateTable_ABC::Purge();
    delegate_.Purge();
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableEquipment::OnItemChanged
// Created: ABR 2011-10-07
// -----------------------------------------------------------------------------
void UnitStateTableEquipment::OnItemChanged( QStandardItem* item )
{
    if( item && item->column() == eState )
        SetEnabled( item->row(), eBreakdown, GetEnumData< E_EquipmentState >( item->row(), eState ) == eEquipmentState_RepairableWithEvacuation );
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableEquipment::AddLines
// Created: ABR 2011-07-08
// -----------------------------------------------------------------------------
void UnitStateTableEquipment::AddLines( const QString& name, int size, E_EquipmentState state, const QStringList& breakdowns, const std::vector< unsigned int > currentBreakdowns /*= std::vector< unsigned int >()*/ )
{
    if( !currentBreakdowns.empty() && ( state != eEquipmentState_RepairableWithEvacuation || static_cast< int >( currentBreakdowns.size() ) != size ) )
        throw std::runtime_error( __FUNCTION__ " Bad amount of breakdowns." );
    if( !size )
        return;
    if( !breakdowns.empty() )
        delegate_.AddComboBox( dataModel_.rowCount(), dataModel_.rowCount() + size - 1, eBreakdown, eBreakdown, breakdowns );
    for( int i = 0; i < size; ++i )
    {
        unsigned int row = dataModel_.rowCount();
        AddItem( row, eName, name, name );

        unsigned int currentIndex = currentBreakdowns.empty() ? 0 : currentBreakdowns[ i ];
        if( currentIndex < static_cast< unsigned int >( breakdowns.size() ) )
            AddItem( row, eBreakdown, breakdowns[ currentIndex ], currentIndex, Qt::ItemIsEditable );

        bool isReadOnly = state == eEquipmentState_InMaintenance || state == eEquipmentState_Prisonner;
        AddItem( row, eState, tools::ToString( state ), static_cast< int >( state ), Qt::ItemIsEditable );
        if( !isReadOnly )
            delegate_.AddComboBox( row, row, eState, eState, ( breakdowns.size() <= 1 ) ? eEquipmentState_RepairableWithEvacuation : eEquipmentState_InMaintenance );
    }
}
