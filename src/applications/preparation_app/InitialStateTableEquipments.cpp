// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "InitialStateTableEquipments.h"
#include "moc_InitialStateTableEquipments.cpp"
#include "clients_kernel/tools.h"
#include "preparation/InitialState.h"
#include "preparation/InitialStateEquipment.h"
#include <assert.h>

// -----------------------------------------------------------------------------
// Name: InitialStateTableEquipments constructor
// Created: ABR 2011-02-24
// -----------------------------------------------------------------------------
InitialStateTableEquipments::InitialStateTableEquipments( QWidget* parent, const char* name /*= 0*/ )
    : InitialStateTable_ABC( 0, 3, parent, name )
{
    horizontalHeader()->setLabel( eName, tools::translate( "InitialStateTableEquipments", "Equipments" ) );
    horizontalHeader()->setLabel( eState, tools::translate( "InitialStateTableEquipments", "State" ) );
    horizontalHeader()->setLabel( eBreakdown, tools::translate( "InitialStateTableEquipments", "Breakdowns" ) );
    setColumnReadOnly( eName, true );
    setColumnStretchable( eName, true );
    setColumnStretchable( eBreakdown, true );

    states_ << tools::translate( "InitialStateTableEquipments", "Available" );
    states_ << tools::translate( "InitialStateTableEquipments", "Destroyed" );
    states_ << tools::translate( "InitialStateTableEquipments", "Broken down" );

    connect( this, SIGNAL( valueChanged( int, int ) ), SLOT( OnValueChanged( int, int ) ) );
}

// -----------------------------------------------------------------------------
// Name: InitialStateTableEquipments destructor
// Created: ABR 2011-02-24
// -----------------------------------------------------------------------------
InitialStateTableEquipments::~InitialStateTableEquipments()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InitialStateTableEquipments::onValueChanged
// Created: ABR 2011-02-25
// -----------------------------------------------------------------------------
void InitialStateTableEquipments::OnValueChanged( int row, int col )
{
    if( col == eState )
        item( row, eBreakdown )->setEnabled( static_cast< Q3ComboTableItem* >( item( row, eState ) )->currentItem() == InitialStateEquipment::eBroken );
}

// -----------------------------------------------------------------------------
// Name: InitialStateTableEquipments::ReadExtension
// Created: ABR 2011-03-01
// -----------------------------------------------------------------------------
void InitialStateTableEquipments::ReadExtension( const InitialState& extension )
{
    while( numRows() )
        removeRow( 0 );
    for( InitialState::CIT_Equipments it = extension.equipments_.begin(); it != extension.equipments_.end(); ++it )
    {
        unsigned int nRow = numRows();
        insertRows( nRow );
        Q3ComboTableItem* state = new Q3ComboTableItem( this, states_ );
        state->setCurrentItem( it->state_ );
        Q3ComboTableItem* breakdown = new Q3ComboTableItem( this, it->breakdowns_ );
        breakdown->setCurrentItem( it->currentBreakdown_ );
        breakdown->setEnabled( it->state_ == InitialStateEquipment::eBroken );
        setText( nRow, eName, it->name_ );
        setItem( nRow, eState, state );
        setItem( nRow, eBreakdown, breakdown );
    }
}

// -----------------------------------------------------------------------------
// Name: InitialStateTableEquipments::WriteExtension
// Created: ABR 2011-03-01
// -----------------------------------------------------------------------------
void InitialStateTableEquipments::WriteExtension( InitialState& extension ) const
{
    int nRow = 0;
    assert( extension.equipments_.size() == static_cast< unsigned int >( numRows() ) );
    for( InitialState::IT_Equipments it = extension.equipments_.begin(); it != extension.equipments_.end(); ++it, ++nRow )
    {
        it->state_ = static_cast< InitialStateEquipment::E_EquipmentState >( static_cast< Q3ComboTableItem* >( item( nRow, eState ) )->currentItem() );
        it->currentBreakdown_ = static_cast< Q3ComboTableItem* >( item( nRow, eBreakdown ) )->currentItem();
    }
}
