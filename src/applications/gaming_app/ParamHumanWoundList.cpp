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
    : QObject( parent )
    , pAsnHumanWoundList_( new ASN1T_SantePriorites() )
{
    asnListHumanWound = pAsnHumanWoundList_;

    table_ = new QTable( 0, 1, parent );
    table_->horizontalHeader()->setLabel( 0, tr( "Injuries" ) );
    table_->setColumnWidth( 0, 200 );
    table_->setLeftMargin( 0 );
    table_->setShowGrid( false );
    table_->setSorting( false );

    humanWoundsList_.append( "" );
    for( unsigned int i = eHumanWound_BlesseUrgence1; i < eNbrHumanWound; ++i )
        humanWoundsList_.append( ENT_Tr::ConvertFromHumanWound( ( E_HumanWound )i ).c_str() );

    table_->setNumRows( 0 );
    table_->insertRows( 0, 1 );
    table_->setItem( 0, 0, new ExclusiveComboTableItem( table_, humanWoundsList_ ) );
    table_->setMinimumHeight( table_->rowHeight( 0 ) * 8 );

    connect( table_, SIGNAL( valueChanged( int, int ) ), SLOT( OnHumanWoundChanged( int, int ) ) );
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
    if( !pAsnHumanWoundList_ || table_->numRows() <= 1 )
        return;
    pAsnHumanWoundList_->n = table_->numRows() - 1;

    ASN1T_EnumHumanWound* pAsnHumanWound = new ASN1T_EnumHumanWound[ pAsnHumanWoundList_->n ]; //$$$ RAM
    for( unsigned int i = 0; i < pAsnHumanWoundList_->n; ++i )
    {
        QComboTableItem* comboItem  = static_cast< QComboTableItem* >( table_->item( i, 0 ) );
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
    QComboTableItem* pComboTableItem = static_cast< QComboTableItem* >( table_->item( row, 0 ) );
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
            table_->setItem( row + 1, 0, new ExclusiveComboTableItem( table_, humanWoundsList_ ) );
            pComboTableItem->setCurrentItem( current );
        }
        table_->setCurrentCell( row, 0 );
    }
}