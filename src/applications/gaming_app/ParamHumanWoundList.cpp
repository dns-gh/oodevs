// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ParamHumanWoundList.h"
#include "moc_ParamHumanWoundList.cpp"
#include "clients_gui/ExclusiveComboTableItem.h"

#include "ENT/ENT_Tr.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: ParamHumanWoundList constructor
// Created: SBO 2005-09-27
// -----------------------------------------------------------------------------
ParamHumanWoundList::ParamHumanWoundList( QWidget* parent, ASN1T_SantePriorites*& asnListHumanWound, const QString& /*strLabel*/ )
    : QTable( 0, 1, parent )
    , pAsnHumanWoundList_( new ASN1T_SantePriorites() )
{
    asnListHumanWound = pAsnHumanWoundList_;
    horizontalHeader()->setLabel( 0, tr( "Injuries" ) );
    setColumnWidth( 0, 200 );
    setLeftMargin( 0 );
    setShowGrid( false );
    setSorting( false );

    humanWoundsList_.append( "" );
    for( unsigned int i = eHumanWound_BlesseUrgence1; i < eNbrHumanWound; ++i )
        humanWoundsList_.append( ENT_Tr::ConvertFromHumanWound( ( E_HumanWound )i ).c_str() );

    setNumRows( 0 );
    insertRows( 0, 1 );
    setItem( 0, 0, new ExclusiveComboTableItem( this, humanWoundsList_ ) );
    setMinimumHeight( rowHeight( 0 ) * 8 );

    connect( this, SIGNAL( valueChanged( int, int ) ), SLOT( OnHumanWoundChanged( int, int ) ) );
}
    
// -----------------------------------------------------------------------------
// Name: ParamHumanWoundList destructor
// Created: SBO 2005-09-27
// -----------------------------------------------------------------------------
ParamHumanWoundList::~ParamHumanWoundList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamHumanWoundList::Commit
// Created: SBO 2006-06-27
// -----------------------------------------------------------------------------
void ParamHumanWoundList::Commit()
{
    if( !pAsnHumanWoundList_ || numRows() <= 1 )
        return;
    pAsnHumanWoundList_->n = numRows() - 1;

    ASN1T_EnumHumanWound* pAsnHumanWound = new ASN1T_EnumHumanWound[ pAsnHumanWoundList_->n ]; //$$$ RAM
    for( unsigned int i = 0; i < pAsnHumanWoundList_->n; ++i )
    {
        QComboTableItem* comboItem  = static_cast< QComboTableItem* >( item( i, 0 ) );
        if( comboItem )
            pAsnHumanWound[i] = ( ASN1T_EnumHumanWound )ENT_Tr::ConvertToHumanWound( comboItem->currentText().ascii() );
    }

    pAsnHumanWoundList_->elem = pAsnHumanWound;
}

// -----------------------------------------------------------------------------
// Name: ParamHumanWoundList::OnHumanWoundChanged
// Created: SBO 2005-09-27
// -----------------------------------------------------------------------------
void ParamHumanWoundList::OnHumanWoundChanged( int row, int /*col*/ )
{
    QComboTableItem* pComboTableItem = static_cast< QComboTableItem* >( item( row, 0 ) );
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
            setItem( row + 1, 0, new ExclusiveComboTableItem( this, humanWoundsList_ ) );
            pComboTableItem->setCurrentItem( current );
        }
        setCurrentCell( row, 0 );
    }
}