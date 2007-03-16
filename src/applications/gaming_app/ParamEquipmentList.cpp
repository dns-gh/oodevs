// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ParamEquipmentList.h"
#include "moc_ParamEquipmentList.cpp"
#include "clients_kernel/EquipmentType.h"
#include "clients_gui/ExclusiveComboTableItem.h"
#include "game_asn/Asn.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: ParamEquipmentList constructor
// Created: SBO 2005-09-27
// -----------------------------------------------------------------------------
ParamEquipmentList::ParamEquipmentList( QObject* parent, const QString& name, const Resolver< EquipmentType >& resolver )
    : QObject( parent )
    , Param_ABC( name )
    , resolver_( resolver )
    , table_( 0 )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: ParamEquipmentList destructor
// Created: SBO 2005-09-27
// -----------------------------------------------------------------------------
ParamEquipmentList::~ParamEquipmentList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamEquipmentList::BuildInterface
// Created: SBO 2007-03-13
// -----------------------------------------------------------------------------
void ParamEquipmentList::BuildInterface( QWidget* parent )
{
    table_ = new QTable( 0, 1, parent );
    table_->horizontalHeader()->setLabel( 0, GetName() );
    table_->setColumnWidth( 0, 200 );
    table_->setLeftMargin( 0 );
    table_->setShowGrid( false );
    table_->setSorting( false );

    equipmentList_.append( "" );
    Iterator< const EquipmentType& > it = resolver_.CreateIterator();
    while( it.HasMoreElements() )
    {
        const QString name = it.NextElement().GetName();
        equipmentList_.append( name );
        equipmentTypes_[ name ] = &it.NextElement();
    }
    
    table_->setNumRows( 0 );
    table_->insertRows( 0, 1 );
    table_->setItem( 0, 0, new ExclusiveComboTableItem( table_, equipmentList_ ) );
    table_->setMinimumHeight( table_->rowHeight( 0 ) * 8 );

    connect( table_, SIGNAL( valueChanged( int, int ) ), SLOT( OnEquipmentChanged( int, int ) ) );
}

// -----------------------------------------------------------------------------
// Name: ParamEquipmentList::CommitTo
// Created: SBO 2007-03-14
// -----------------------------------------------------------------------------
void ParamEquipmentList::CommitTo( ASN1T_MissionParameter& asn ) const
{
    if( !table_ )
        InterfaceNotInitialized();
    asn.value.t = T_MissionParameter_value_maintenancePriorites;
    ASN1T_MaintenancePriorites*& list = asn.value.u.maintenancePriorites = new ASN1T_MaintenancePriorites();
    list->n = 0;
    list->elem = 0;
    asn.null_value = table_->numRows() <= 1 ? 1 : 0;
    if( asn.null_value )
        return;
    list->n = table_->numRows() - 1;
    list->elem = new ASN1T_TypeEquipement[ list->n ];
    for( unsigned int i = 0; i < list->n; ++i )
    {
        ExclusiveComboTableItem* comboItem  = static_cast< ExclusiveComboTableItem* >( table_->item( i, 0 ) );
        if( comboItem && !comboItem->currentText().isEmpty() )
        {
            CIT_EquipmentTypes it = equipmentTypes_.find( comboItem->currentText() );
            list->elem[i] = it->second->GetId();
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ParamEquipmentList::Clean
// Created: SBO 2007-03-14
// -----------------------------------------------------------------------------
void ParamEquipmentList::Clean( ASN1T_MissionParameter& asn ) const
{
    if( asn.value.u.maintenancePriorites )
        delete[] asn.value.u.maintenancePriorites->elem;
    delete asn.value.u.maintenancePriorites;
}

// -----------------------------------------------------------------------------
// Name: ParamEquipmentList::OnEquipmentChanged
// Created: SBO 2005-09-27
// -----------------------------------------------------------------------------
void ParamEquipmentList::OnEquipmentChanged( int row, int /*col*/ )
{
    ExclusiveComboTableItem* pComboTableItem = static_cast< ExclusiveComboTableItem* >( table_->item( row, 0 ) );
    if( !pComboTableItem )
        return;

    if( pComboTableItem->currentItem() == 0 )
    {
        // if not last row, delete empty row
        if( row != table_->numRows() - 1 )
        {
            table_->removeRow( row );
            // select last row quantity field
            table_->setCurrentCell( table_->numRows() - 1, 0 );
        }
    }
    else
    {
        // if last row is set, add a new row to table
        if( row == table_->numRows() - 1 )
        {
            // need to save combo box selected element before to insert a line
            const int current = pComboTableItem->currentItem();
            table_->insertRows( row + 1, 1 );
            table_->setItem( row + 1, 0, new ExclusiveComboTableItem( table_, equipmentList_ ) );
            pComboTableItem->setCurrentItem( current );
        }
        table_->setCurrentCell( row, 0 );
    }
}
