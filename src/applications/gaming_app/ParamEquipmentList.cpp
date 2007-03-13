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

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: ParamEquipmentList constructor
// Created: SBO 2005-09-27
// -----------------------------------------------------------------------------
ParamEquipmentList::ParamEquipmentList( QWidget* parent, ASN1T_MaintenancePriorites*& asnListEquipment, const QString& /*label*/, const Resolver< EquipmentType >& resolver )
    : QObject( parent )
    , pAsnEquipmentList_ ( new ASN1T_MaintenancePriorites() )
{
    table_ = new QTable( 0, 1, parent );
    asnListEquipment = pAsnEquipmentList_;
    table_->horizontalHeader()->setLabel( 0, tr( "Equipment" ) );
    table_->setColumnWidth( 0, 200 );
    table_->setLeftMargin( 0 );
    table_->setShowGrid( false );
    table_->setSorting( false );

    equipmentList_.append( "" );
    Iterator< const EquipmentType& > it = resolver.CreateIterator();
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
// Name: ParamEquipmentList destructor
// Created: SBO 2005-09-27
// -----------------------------------------------------------------------------
ParamEquipmentList::~ParamEquipmentList()
{
    delete pAsnEquipmentList_;
}

// -----------------------------------------------------------------------------
// Name: ParamEquipmentList::Commit
// Created: SBO 2006-06-27
// -----------------------------------------------------------------------------
void ParamEquipmentList::Commit()
{
    if( !pAsnEquipmentList_ || table_->numRows() <= 1 )
        return;
    pAsnEquipmentList_->n = table_->numRows() - 1;

    ASN1T_TypeEquipement* pAsnEquipement = new ASN1T_TypeEquipement[ pAsnEquipmentList_->n ]; //$$$ RAM
    for( uint i = 0; i < pAsnEquipmentList_->n; ++i )
    {
        ExclusiveComboTableItem* comboItem  = static_cast< ExclusiveComboTableItem* >( table_->item( i, 0 ) );
        if( comboItem && !comboItem->currentText().isEmpty() )
            pAsnEquipement[i] = equipmentTypes_[ comboItem->currentText() ]->GetId();
    }
    pAsnEquipmentList_->elem = pAsnEquipement;
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
