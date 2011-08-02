// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "InitialStateTableCrew.h"
#include "moc_InitialStateTableCrew.cpp"
#include "clients_gui/SpinTableItem.h"
#include "clients_kernel/tools.h"
#include "preparation/InitialState.h"
#include "preparation/InitialStateCrew.h"
#include <assert.h>

// -----------------------------------------------------------------------------
// Name: InitialStateTableCrew constructor
// Created: ABR 2011-02-24
// -----------------------------------------------------------------------------
InitialStateTableCrew::InitialStateTableCrew( QWidget* parent, const char* name /*= 0*/ )
    : InitialStateTable_ABC( 0, 5, parent, name )
{
    horizontalHeader()->setLabel( eRank,         tools::translate( "InitialStateTableCrew", "Rank" ) );
    horizontalHeader()->setLabel( eState,        tools::translate( "InitialStateTableCrew", "State" ) );
    horizontalHeader()->setLabel( ePsy,          tools::translate( "InitialStateTableCrew", "Psyop" ) );
    horizontalHeader()->setLabel( eContaminated, tools::translate( "InitialStateTableCrew", "Contaminated" ) );
    horizontalHeader()->setLabel( eNumber,       tools::translate( "InitialStateTableCrew", "Number" ) );
    setSelectionMode( NoSelection );
    setFocusStyle( FollowStyle );
    setColumnStretchable( eRank, true );
    setColumnStretchable( eState, true );
    adjustColumn( ePsy );
    adjustColumn( eContaminated );
    adjustColumn( eNumber );
    setRowReadOnly( InitialStateCrew::eOfficer, true );
    setRowReadOnly( InitialStateCrew::eWarrant, true );
    setRowReadOnly( InitialStateCrew::ePrivate, true );

    ranks_ << tools::translate( "InitialStateTableCrew", "Officer" );
    ranks_ << tools::translate( "InitialStateTableCrew", "Warrant officer" );
    ranks_ << tools::translate( "InitialStateTableCrew", "Private" );

    states_ << tools::translate( "InitialStateTableCrew", "Healthy" );
    states_ << tools::translate( "InitialStateTableCrew", "UE Wounded - UE" );
    states_ << tools::translate( "InitialStateTableCrew", "U1 Wounded - U1" );
    states_ << tools::translate( "InitialStateTableCrew", "U2 Wounded - U2" );
    states_ << tools::translate( "InitialStateTableCrew", "U3 Wounded - U3" );
    states_ << tools::translate( "InitialStateTableCrew", "Dead" );

    connect( this, SIGNAL( contextMenuRequested( int, int, const QPoint& ) ), SLOT( OnRequestContextMenu( int, int, const QPoint& ) ) );
    connect( this, SIGNAL( valueChanged( int, int ) ), SLOT( OnValueChanged( int, int ) ) );
}

// -----------------------------------------------------------------------------
// Name: InitialStateTableCrew destructor
// Created: ABR 2011-02-24
// -----------------------------------------------------------------------------
InitialStateTableCrew::~InitialStateTableCrew()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InitialStateTableCrew::paintCell
// Created: ABR 2011-02-28
// -----------------------------------------------------------------------------
void InitialStateTableCrew::paintCell( QPainter * p, int row, int col, const QRect & cr, bool selected, const QColorGroup & cg )
{
    if( row > 2 )
        Q3Table::paintCell( p, row, col, cr, selected, cg );
    else
    {
        QColor backgroundColor;
        backgroundColor.setRgb( 200, 200, 200 );
        QColorGroup newCg( cg );
        newCg.setColor( QColorGroup::Base, backgroundColor );
        Q3Table::paintCell( p, row, col, cr, selected, newCg );
    }
}

// -----------------------------------------------------------------------------
// Name: InitialStateTableCrew::OnAddItem
// Created: ABR 2011-02-28
// -----------------------------------------------------------------------------
void InitialStateTableCrew::OnAddItem()
{
    AddLine();
}

// -----------------------------------------------------------------------------
// Name: InitialStateTableCrew::OnClearItems
// Created: ABR 2011-02-28
// -----------------------------------------------------------------------------
void InitialStateTableCrew::OnClearItems()
{
    while( numRows() - 1 > InitialStateCrew::ePrivate )
        removeRow( numRows() - 1 );
    ComputeAllValues();
}

// -----------------------------------------------------------------------------
// Name: InitialStateTableCrew::OnRemoveItem
// Created: ABR 2011-02-28
// -----------------------------------------------------------------------------
void InitialStateTableCrew::OnRemoveItem()
{
    removeRow( currentRow() );
    ComputeAllValues();
}

// -----------------------------------------------------------------------------
// Name: InitialStateTableCrew::OnRequestContextMenu
// Created: ABR 2011-02-28
// -----------------------------------------------------------------------------
void InitialStateTableCrew::OnRequestContextMenu( int row, int /*col*/, const QPoint& pos )
{
    Q3PopupMenu popupMenu;
    popupMenu.insertItem( tools::translate( "InitialStateTableCrew", "Add" ), this, SLOT( OnAddItem() ) );
    if( numRows() - 1 > InitialStateCrew::ePrivate )
    {
        popupMenu.insertSeparator();
        if( row > InitialStateCrew::ePrivate && row <= numRows() )
            popupMenu.insertItem( tools::translate( "InitialStateTableCrew", "Delete" ), this, SLOT( OnRemoveItem() ) );
        popupMenu.insertItem( tools::translate( "InitialStateTableCrew", "Clear" ), this, SLOT( OnClearItems() ) );
    }
    popupMenu.exec( pos );
}

// -----------------------------------------------------------------------------
// Name: InitialStateTableCrew::keyPressEvent
// Created: ABR 2011-02-28
// -----------------------------------------------------------------------------
void InitialStateTableCrew::keyPressEvent( QKeyEvent * e )
{
    if( e->key() == Qt::Key_Delete && currentRow() > InitialStateCrew::ePrivate )
        OnRemoveItem();
}

// -----------------------------------------------------------------------------
// Name: InitialStateTableCrew::OnValueChanged
// Created: ABR 2011-02-28
// -----------------------------------------------------------------------------
void InitialStateTableCrew::OnValueChanged( int row, int col )
{
    if( col == eRank )
    {
        setText( row, eNumber, "0" );
        ComputeAllValues();
    }
    else if( col == eNumber )
        ComputeValues( static_cast< InitialStateCrew::E_CrewRanks > ( static_cast< Q3ComboTableItem* >( item( row, eRank ) )->currentItem() ) );
}

// -----------------------------------------------------------------------------
// Name: InitialStateTableCrew::ComputeAllValues
// Created: ABR 2011-02-28
// -----------------------------------------------------------------------------
void InitialStateTableCrew::ComputeAllValues()
{
    ComputeValues( InitialStateCrew::eOfficer );
    ComputeValues( InitialStateCrew::eWarrant );
    ComputeValues( InitialStateCrew::ePrivate );
}

// -----------------------------------------------------------------------------
// Name: InitialStateTableCrew::ComputeValues
// Created: ABR 2011-02-28
// -----------------------------------------------------------------------------
void InitialStateTableCrew::ComputeValues( InitialStateCrew::E_CrewRanks rank )
{
    // Count for each rank the number of officer
    unsigned int others = 0;
    for( int i = InitialStateCrew::ePrivate + 1; i < numRows(); ++i )
        if( rank == static_cast< InitialStateCrew::E_CrewRanks > ( static_cast< Q3ComboTableItem* >( item( i, eRank ) )->currentItem() ) )
            others += item( i, eNumber )->text().toUInt();
    // Update left officers count
    nbrOfficersLeft_[ rank ] = nbrOfficers_[ rank ] - others;
    // Update max value for each spinbox
    for( int i = InitialStateCrew::ePrivate + 1; i < numRows(); ++i )
        if( rank == static_cast< InitialStateCrew::E_CrewRanks > ( static_cast< Q3ComboTableItem* >( item( i, eRank ) )->currentItem() ) )
            static_cast< gui::SpinTableItem< int >* >( item( i, eNumber ) )->SetMinMaxValue( 0, nbrOfficersLeft_[ rank ] + item( i, eNumber )->text().toUInt() );
    // Update first line left value
    item( rank, eNumber )->setText( QString::number( nbrOfficersLeft_[ rank ] ) );
    updateCell( rank, eNumber );
}

// -----------------------------------------------------------------------------
// Name: InitialStateTableCrew::AddLine
// Created: ABR 2011-02-25
// -----------------------------------------------------------------------------
void InitialStateTableCrew::AddHeader( InitialStateCrew::E_CrewRanks rank, int number )
{
    unsigned int nRow = numRows();
    insertRows( nRow );
    setText( nRow, eRank, ranks_[ rank ] );
    setText( nRow, eState, *states_.begin() );
    setItem( nRow, ePsy, new Q3CheckTableItem( this, "" ) );
    setItem( nRow, eContaminated, new Q3CheckTableItem( this, "" ) );
    setText( nRow, eNumber, QString::number( number ) );
}

// -----------------------------------------------------------------------------
// Name: InitialStateTableCrew::AddLine
// Created: ABR 2011-02-25
// -----------------------------------------------------------------------------
void InitialStateTableCrew::AddLine( InitialStateCrew::E_CrewRanks rank /*= InitialStateCrew::eOfficer*/, InitialStateCrew::E_CrewStates state /*= InitialStateCrew::eHealhty*/,
                                   bool psy /*= false*/, bool contaminated /*= false*/, int number /*= 0*/ )
{
    unsigned int nRow = numRows();
    insertRows( nRow );
    setItem( nRow, eRank, new Q3ComboTableItem( this, ranks_ ) );
    static_cast< Q3ComboTableItem* >( item( nRow, eRank ) )->setCurrentItem( static_cast< int >( rank ) );
    setItem( nRow, eState, new Q3ComboTableItem( this, states_ ) );
    static_cast< Q3ComboTableItem* >( item( nRow, eState ) )->setCurrentItem( static_cast< int >( state ) );
    setItem( nRow, ePsy, new Q3CheckTableItem( this, "" ) );
    static_cast< Q3CheckTableItem* >( item( nRow, ePsy ) )->setChecked( psy );
    setItem( nRow, eContaminated, new Q3CheckTableItem( this, "" ) );
    static_cast< Q3CheckTableItem* >( item( nRow, eContaminated ) )->setChecked( contaminated );
    setItem( nRow, eNumber, new gui::SpinTableItem< int >( this, 0, nbrOfficers_[ rank ] ) );
    item( nRow, eNumber )->setText( QString::number( number ) );
}

// -----------------------------------------------------------------------------
// Name: InitialStateTableCrew::ReadExtension
// Created: ABR 2011-03-01
// -----------------------------------------------------------------------------
void InitialStateTableCrew::ReadExtension( const InitialState& extension )
{
    while( numRows() )
        removeRow( 0 );
    assert( extension.crews_.size() > 2 );
    for( InitialState::CIT_Crews it = extension.crews_.begin(); it != extension.crews_.end(); ++it )
        if( it - extension.crews_.begin() < 3 )
        {
            AddHeader( it->rank_, it->number_ );
            nbrOfficersLeft_[ it->rank_ ] = it->number_;
            nbrOfficers_[ it->rank_ ] = nbrOfficersLeft_[ it->rank_ ];
        }
        else
        {
            AddLine( it->rank_, it->state_, it->psyop_, it->contaminated_, it->number_ );
            nbrOfficers_[ it->rank_ ] += it->number_;
        }
}

// -----------------------------------------------------------------------------
// Name: InitialStateTableCrew::WriteExtension
// Created: ABR 2011-03-01
// -----------------------------------------------------------------------------
void InitialStateTableCrew::WriteExtension( InitialState& extension ) const
{
    extension.crews_.clear();
    for( int nRow = 0; nRow <= InitialStateCrew::ePrivate; ++nRow )
    {
        InitialStateCrew::E_CrewRanks rank = static_cast< InitialStateCrew::E_CrewRanks >( nRow );
        unsigned int number = item( nRow, eNumber )->text().toUInt();
        extension.crews_.push_back( InitialStateCrew( rank, InitialStateCrew::eHealthy, false, false, number ) );
    }
    for( int nRow = InitialStateCrew::ePrivate + 1; nRow < numRows(); ++nRow )
    {
        unsigned int number = item( nRow, eNumber )->text().toUInt();
        InitialStateCrew::E_CrewRanks rank = static_cast< InitialStateCrew::E_CrewRanks >( static_cast< Q3ComboTableItem* >( item( nRow, eRank ) )->currentItem() );
        InitialStateCrew::E_CrewStates state = static_cast< InitialStateCrew::E_CrewStates >( static_cast< Q3ComboTableItem* >( item( nRow, eState ) )->currentItem() );
        bool psy = static_cast< Q3CheckTableItem* >( item( nRow, ePsy ) )->isChecked();
        bool contaminated = static_cast< Q3CheckTableItem* >( item( nRow, eContaminated ) )->isChecked();
        if( !number )
            continue;
        if( state == InitialStateCrew::eHealthy && !psy && !contaminated )
        {
            extension.crews_[ rank ].number_ += number;
            continue;
        }
        extension.crews_.push_back( InitialStateCrew( rank, state, psy, contaminated, number ) );
    }
}
