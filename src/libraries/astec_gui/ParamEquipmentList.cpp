// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_gui_pch.h"
#include "ParamEquipmentList.h"
#include "moc_ParamEquipmentList.cpp"
#include "astec_kernel/EquipmentType.h"
#include "ExclusiveComboTableItem.h"

// -----------------------------------------------------------------------------
// Name: ParamEquipmentList constructor
// Created: SBO 2005-09-27
// -----------------------------------------------------------------------------
ParamEquipmentList::ParamEquipmentList( QWidget* pParent, ASN1T_MaintenancePriorites& asnListEquipment, const std::string& /*label*/, const Resolver< EquipmentType >& resolver )
    : QTable ( 0, 1, pParent )
    , pAsnEquipmentList_ ( &asnListEquipment )
{
    horizontalHeader()->setLabel( 0, tr( "Equipement" ) );
    setColumnWidth( 0, 200 );
    setLeftMargin( 0 );
    setShowGrid( false );
    setSorting( false );

    equipmentList_.append( "" );
    Iterator< const EquipmentType& > it = resolver.CreateIterator();
    while( it.HasMoreElements() )
    {
        const QString name = it.NextElement().GetName().c_str();
        equipmentList_.append( name );
        equipmentTypes_[ name ] = &it.NextElement();
    }
    
    setNumRows( 0 );
    insertRows( 0, 1 );
    setItem( 0, 0, new ExclusiveComboTableItem( this, equipmentList_ ) );
    setMinimumHeight( rowHeight( 0 ) * 8 );

    connect( this, SIGNAL( valueChanged( int, int ) ), SLOT( OnEquipmentChanged( int, int ) ) );
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
    if( !pAsnEquipmentList_ || numRows() <= 1 )
        return;
    pAsnEquipmentList_->n = numRows() - 1;

    ASN1T_TypeEquipement* pAsnEquipement = new ASN1T_TypeEquipement[ pAsnEquipmentList_->n ]; //$$$ RAM
    for( uint i = 0; i < pAsnEquipmentList_->n; ++i )
    {
        ExclusiveComboTableItem* comboItem  = static_cast< ExclusiveComboTableItem* >( item( i, 0 ) );
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
    ExclusiveComboTableItem* pComboTableItem = static_cast< ExclusiveComboTableItem* >( item( row, 0 ) );
    if( !pComboTableItem )
        return;

    if( pComboTableItem->currentItem() == 0 )
    {
        // if not last row, delete empty row
        if( row != numRows() - 1 )
        {
            removeRow( row );
            // select last row quantity field
            setCurrentCell( numRows() - 1, 0 );
        }
    }
    else
    {
        // if last row is set, add a new row to table
        if( row == numRows() - 1 )
        {
            // need to save combo box selected element before to insert a line
            const int current = pComboTableItem->currentItem();
            insertRows( row + 1, 1 );
            setItem( row + 1, 0, new ExclusiveComboTableItem( this, equipmentList_ ) );
            pComboTableItem->setCurrentItem( current );
        }
        setCurrentCell( row, 0 );
    }
}
