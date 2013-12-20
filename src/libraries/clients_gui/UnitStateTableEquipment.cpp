// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "UnitStateTableEquipment.h"
#include "moc_UnitStateTableEquipment.cpp"
#include "LinkItemDelegate.h"
#include "DisplayExtractor.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Formation_ABC.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: UnitStateTableEquipment constructor
// Created: ABR 2011-02-24
// -----------------------------------------------------------------------------
UnitStateTableEquipment::UnitStateTableEquipment( QWidget* parent, DisplayExtractor& extractor )
    : UnitStateTable_ABC( "UnitStateTableEquipment", parent, 4 )
    , linkItemDelegate_( 0 )
    , extractor_       ( extractor )
{
    horizontalHeaders_ << tr( "Equipments" )
                       << tr( "Unit" )
                       << tr( "State" )
                       << tr( "Breakdown" );
    linkItemDelegate_ = new LinkItemDelegate( this );
    setItemDelegateForColumn( eUnit, linkItemDelegate_ );

    connect( &dataModel_, SIGNAL( itemChanged( QStandardItem* ) ), SLOT( OnItemChanged( QStandardItem* ) ) );
    connect( linkItemDelegate_, SIGNAL( LinkClicked( const QString&, const QModelIndex& ) ), SLOT( OnLinkClicked( const QString&, const QModelIndex& ) ) );
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
// Name: UnitStateTableEquipment::Purge
// Created: ABR 2011-10-11
// -----------------------------------------------------------------------------
void UnitStateTableEquipment::Purge()
{
    UnitStateTable_ABC::Purge();
    horizontalHeader()->setResizeMode( QHeaderView::ResizeToContents );
    horizontalHeader()->setResizeMode( 0, QHeaderView::Stretch );
    horizontalHeader()->setResizeMode( 1, QHeaderView::Stretch );
    delegate_.Purge();
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableEquipment::OnLinkClicked
// Created: LGY 2013-12-19
// -----------------------------------------------------------------------------
void UnitStateTableEquipment::OnLinkClicked( const QString& url, const QModelIndex& /*index*/ )
{
    extractor_.NotifyLinkClicked( url + "#select" );
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
void UnitStateTableEquipment::AddLines( const QString& name, const kernel::Entity_ABC& entity, int size, E_EquipmentState state, const QStringList& breakdowns,
                                        const std::vector< unsigned int > currentBreakdowns /* = std::vector< unsigned int >()*/ )
{
    if( !size )
        return;
    if( !breakdowns.empty() )
        delegate_.AddComboBox( dataModel_.rowCount(), dataModel_.rowCount() + size - 1, eBreakdown, eBreakdown, breakdowns );
    int currentSize = static_cast< int >( currentBreakdowns.size() );
    for( int i = 0; i < size; ++i )
    {
        unsigned int row = dataModel_.rowCount();
        AddItem( row, eName, name, name );
        AddItem( row, eUnit, GetDisplayName( entity ), entity.GetName() );

        unsigned int currentIndex = ( currentSize <= i ) ? 0 : currentBreakdowns[ i ];
        if( currentIndex < static_cast< unsigned int >( breakdowns.size() ) )
            AddItem( row, eBreakdown, breakdowns[ currentIndex ], currentIndex, Qt::ItemIsEditable );

        bool isReadOnly = state == eEquipmentState_InMaintenance || state == eEquipmentState_Prisonner;
        AddItem( row, eState, tools::ToString( state ), static_cast< int >( state ), Qt::ItemIsEditable );
        if( !isReadOnly )
            delegate_.AddComboBox( row, row, eState, eState, ( breakdowns.size() <= 1 ) ? eEquipmentState_RepairableWithEvacuation : eEquipmentState_InMaintenance );
    }
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableEquipment::GetDisplayName
// Created: LGY 2013-12-19
// -----------------------------------------------------------------------------
QString UnitStateTableEquipment::GetDisplayName( const kernel::Entity_ABC& entity ) const
{
    if( const kernel::Agent_ABC* pAgent = dynamic_cast< const kernel::Agent_ABC* >( &entity ) )
        return extractor_.GetDisplayName( *pAgent );
    if( const kernel::Automat_ABC* pAutomat = dynamic_cast< const kernel::Automat_ABC* >( &entity ) )
        return extractor_.GetDisplayName( *pAutomat );
    if( const kernel::Formation_ABC* pFormation = dynamic_cast< const kernel::Formation_ABC* >( &entity ) )
        return extractor_.GetDisplayName( *pFormation );
    return extractor_.GetDisplayName( entity );
}
