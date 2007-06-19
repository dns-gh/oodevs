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
#include "clients_kernel/OrderParameter.h"
#include "game_asn/Asn.h"
#include "gaming/tools.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: ParamHumanWoundList constructor
// Created: SBO 2005-09-27
// -----------------------------------------------------------------------------
ParamHumanWoundList::ParamHumanWoundList( QObject* parent, const kernel::OrderParameter& parameter )
    : QObject( parent )
    , Param_ABC( parameter.GetName() )
    , parameter_( parameter )
    , table_( 0 )
{
    // NOTHING
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
// Name: ParamHumanWoundList::BuildInterface
// Created: SBO 2007-03-13
// -----------------------------------------------------------------------------
void ParamHumanWoundList::BuildInterface( QWidget* parent )
{
    table_ = new QTable( 0, 1, parent );
    table_->horizontalHeader()->setLabel( 0, GetName() );
    table_->setColumnWidth( 0, 200 );
    table_->setLeftMargin( 0 );
    table_->setShowGrid( false );
    table_->setSorting( false );

    humanWoundsList_.append( "" );
    for( unsigned int i = eHumanWound_BlesseUrgence1; i < eNbrHumanWound; ++i )
        humanWoundsList_.append( tools::ToString( ( E_HumanWound )i ) );

    table_->setNumRows( 0 );
    table_->insertRows( 0, 1 );
    table_->setItem( 0, 0, new ExclusiveComboTableItem( table_, humanWoundsList_ ) );
    table_->setMinimumHeight( table_->rowHeight( 0 ) * 8 );

    connect( table_, SIGNAL( valueChanged( int, int ) ), SLOT( OnHumanWoundChanged( int, int ) ) );
}

// -----------------------------------------------------------------------------
// Name: ParamHumanWoundList::CommitTo
// Created: SBO 2007-03-14
// -----------------------------------------------------------------------------
void ParamHumanWoundList::CommitTo( ASN1T_MissionParameter& asn ) const
{
    if( !table_ )
        InterfaceNotInitialized();
    asn.value.t = T_MissionParameter_value_logMedicalPriorities;
    ASN1T_LogMedicalPriorities*& list = asn.value.u.logMedicalPriorities = new ASN1T_LogMedicalPriorities();
    list->n = 0;
    list->elem = 0;
    asn.null_value = table_->numRows() <= 1 ? 1 : 0;
    if( asn.null_value )
        return;
    list->n = table_->numRows() - 1;
    list->elem = new ASN1T_EnumHumanWound[ list->n ];
    for( unsigned int i = 0; i < list->n; ++i )
    {
        QComboTableItem* comboItem  = static_cast< QComboTableItem* >( table_->item( i, 0 ) );
        if( comboItem )
            list->elem[i] = ASN1T_EnumHumanWound( comboItem->currentItem() - 1 + int( eHumanWound_BlesseUrgence1 ) ); // $$$$ SBO 2007-05-03: 
    }
}

// -----------------------------------------------------------------------------
// Name: ParamHumanWoundList::Clean
// Created: SBO 2007-03-14
// -----------------------------------------------------------------------------
void ParamHumanWoundList::Clean( ASN1T_MissionParameter& asn ) const
{
    if( asn.value.u.logMedicalPriorities )
        delete[] asn.value.u.logMedicalPriorities->elem;
    delete asn.value.u.logMedicalPriorities;
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