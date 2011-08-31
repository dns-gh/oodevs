// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

/* TRANSLATOR gui::UnitStateTableResource */

#include "clients_gui_pch.h"
#include "UnitStateTableResource.h"
#include "moc_UnitStateTableResource.cpp"
#include "SpinTableItem.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: UnitStateTableResource constructor
// Created: ABR 2011-07-05
// -----------------------------------------------------------------------------
UnitStateTableResource::UnitStateTableResource( QWidget* parent, bool forPreparation /*= false*/, const char* name /*= 0*/ )
    : UnitStateTable_ABC( 0, 6, parent, name )
    , isPreparation_( forPreparation )
{
    tr( "" ); // $$$$ ABR 2011-08-11: HACK: neccessaire sinon le premier appel a tr n'est pas traduit !??!?!?!?!?
    horizontalHeader()->setLabel( eName,       tr( "Resources" ) );
    horizontalHeader()->setLabel( eCategory,   tr( "Category" ) );
    horizontalHeader()->setLabel( eQuantity,   tr( "Quantity" ) );
    horizontalHeader()->setLabel( eMaximum,    ( isPreparation_ ) ? tr( "Default capacity" ) : tr( "Maximal capacity" ) );
    horizontalHeader()->setLabel( ePercentage, tr( "Percentage (%)" ) );
    horizontalHeader()->setLabel( eThreshold,  tr( "Logistic threshold (%)" ) );
    setLeftMargin( 0 );
    setColumnStretchable( eName, true );
    setColumnReadOnly( eName,       true );
    setColumnReadOnly( eCategory,   true );
    setColumnReadOnly( eMaximum,    true );
    setColumnReadOnly( ePercentage, true );
    setSelectionMode( NoSelection );
    setFocusStyle( FollowStyle );

    connect( this, SIGNAL( valueChanged( int, int ) ), SLOT( OnValueChanged( int, int ) ) );
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableResource destructor
// Created: ABR 2011-07-05
// -----------------------------------------------------------------------------
UnitStateTableResource::~UnitStateTableResource()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableResource::MergeLine
// Created: ABR 2011-07-06
// -----------------------------------------------------------------------------
void UnitStateTableResource::MergeLine( const QString& name, const QString& category, unsigned quantity, unsigned maximum, double threshold )
{
    for( int row = 0; row < numRows(); ++row )
        if( item( row, eName )->text() == name )
        {
            setText( row, eQuantity, QString::number( GetNumericValue< unsigned int >( row, eQuantity ) + quantity ) );
            setText( row, eMaximum, QString::number( GetNumericValue< unsigned int >( row, eMaximum ) + maximum ) );
            if( !isPreparation_ )
                static_cast< gui::SpinTableItem< int >* >( item( row, eQuantity ) )->SetMinMaxValue( 0, GetNumericValue< unsigned int >( row, eMaximum ) );
            return;
        }
    AddLine( name, category, quantity, maximum, threshold );
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableResource::AddLine
// Created: ABR 2011-07-05
// -----------------------------------------------------------------------------
void UnitStateTableResource::AddLine( const QString& name, const QString& category, unsigned quantity /*= 0*/, unsigned maximum /*= 0*/, double threshold /*= 0.*/ )
{
    int nRow = numRows();
    insertRows( nRow );
    setText( nRow, eName, name );
    setText( nRow, eCategory, category );
    setItem( nRow, eQuantity, new gui::SpinTableItem< int >( this, 0, std::numeric_limits< int >::max(), 10 ) );
    item( nRow, eQuantity )->setText( QString::number( quantity ) );
    setText( nRow, eMaximum, QString::number( maximum ) );
    setItem( nRow, eThreshold, new gui::SpinTableItem< double >( this, 0, 100, 5 ) );
    item( nRow, eThreshold )->setText( QString::number( threshold, 'f', 2 ) );
    if( !isPreparation_ )
        static_cast< gui::SpinTableItem< int >* >( item( nRow, eQuantity ) )->SetMinMaxValue( 0, GetNumericValue< unsigned int >( nRow, eMaximum ) );
    OnValueChanged( nRow, eQuantity );
    for( int i = eName; i <= eThreshold; ++i )
        adjustColumn( i );
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableResource::OnValueChanged
// Created: ABR 2011-07-05
// -----------------------------------------------------------------------------
void UnitStateTableResource::OnValueChanged( int row, int col )
{
    if( col == eQuantity )
    {
        unsigned int quantity = GetNumericValue< unsigned int >( row, eQuantity );
        unsigned int maximum = GetNumericValue< unsigned int >( row, eMaximum );
        if( quantity == 0 && maximum == 0 )
            setText( row, ePercentage, "0.00" );
        else
            setText( row, ePercentage, QString::number( std::min( quantity * 100. / static_cast< double >( maximum ), 100. ), 'f', 2 ) );
        item( row, eMaximum )->setEnabled( quantity <= maximum );
        item( row, ePercentage )->setEnabled( quantity <= maximum );
    }
}
