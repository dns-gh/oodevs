// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

/* TRANSLATOR gui::UnitStateTableCrew */

#include "clients_gui_pch.h"
#include "UnitStateTableCrew.h"
#include "moc_UnitStateTableCrew.cpp"
#include "SpinTableItem.h"
#include "clients_kernel/Tools.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: UnitStateTableCrew constructor
// Created: ABR 2011-02-24
// -----------------------------------------------------------------------------
UnitStateTableCrew::UnitStateTableCrew( QWidget* parent, const char* name /*= 0*/ )
    : UnitStateTable_ABC( 0, 7, parent, name )
    , currentRow_( 0 )
{
    tr( "" ); // $$$$ ABR 2011-08-11: HACK: neccessaire sinon le premier appel a tr n'est pas traduit !??!?!?!?!?
    horizontalHeader()->setLabel( eRank,         tr( "Rank" ) );
    horizontalHeader()->setLabel( eState,        tr( "State" ) );
    horizontalHeader()->setLabel( eInjuries,     tr( "Injury seriousness" ) ); // $$$$ ABR 2011-08-11: should become Injuries with story 660
    horizontalHeader()->setLabel( eLocation,     tr( "Location" ) );
    horizontalHeader()->setLabel( ePsy,          tr( "Mentally injured" ) );
    horizontalHeader()->setLabel( eContaminated, tr( "Contaminated" ) );
    horizontalHeader()->setLabel( eNumber,       tr( "Quantity" ) );
    setSelectionMode( NoSelection );
    setFocusStyle( FollowStyle );
    setColumnStretchable( eRank, true );
    setColumnStretchable( eState, true );
    setColumnStretchable( eInjuries, true );
    adjustColumn( ePsy );
    adjustColumn( eContaminated );
    adjustColumn( eNumber );
    setRowReadOnly( eHumanRank_Officier, true );
    setRowReadOnly( eHumanRank_SousOfficer, true );
    setRowReadOnly( eHumanRank_Mdr, true );
    setColumnReadOnly( eLocation, true );

    Populate( eNbrHumanRank, humanRanks_ );
    Populate( eNbrHumanState, humanStates_ );
    Populate( eNbrInjuriesSeriousness, injuriesSeriousness_ );
    Populate( eNbrHumanLocation, humanLocations_ );

    connect( this, SIGNAL( contextMenuRequested( int, int, const QPoint& ) ), SLOT( OnRequestContextMenu( int, int, const QPoint& ) ) );
    connect( this, SIGNAL( valueChanged( int, int ) ), SLOT( OnValueChanged( int, int ) ) );
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableCrew destructor
// Created: ABR 2011-02-24
// -----------------------------------------------------------------------------
UnitStateTableCrew::~UnitStateTableCrew()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableCrew::paintCell
// Created: ABR 2011-02-28
// -----------------------------------------------------------------------------
void UnitStateTableCrew::paintCell( QPainter * p, int row, int col, const QRect & cr, bool selected, const QColorGroup & cg )
{
    if( row < eNbrHumanRank )
    {
        QColor backgroundColor;
        backgroundColor.setRgb( 200, 200, 200 );
        QColorGroup newCg( cg );
        newCg.setColor( QColorGroup::Base, backgroundColor );
        Q3Table::paintCell( p, row, col, cr, selected, newCg );
    }
    else
    {
        Q3Table::paintCell( p, row, col, cr, selected, cg );
    }
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableCrew::Purge
// Created: ABR 2011-08-12
// -----------------------------------------------------------------------------
void UnitStateTableCrew::Purge()
{
    for( int rank = 0; rank < eNbrHumanRank; ++rank )
    {
        nbrOfficersTotal_[ static_cast< E_HumanRank >( rank ) ] = 0;
        nbrOfficersHealthy_[ static_cast< E_HumanRank >( rank ) ] = 0;
    }
    UnitStateTable_ABC::Purge();
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableCrew::OnClearItems
// Created: ABR 2011-02-28
// -----------------------------------------------------------------------------
void UnitStateTableCrew::OnClearItems()
{
    while( numRows() > eNbrHumanRank )
        removeRow( eNbrHumanRank );
    ComputeAllValues();
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableCrew::OnRemoveItem
// Created: ABR 2011-02-28
// -----------------------------------------------------------------------------
void UnitStateTableCrew::OnRemoveItem()
{
    if( currentRow_ >= eNbrHumanRank && currentRow_ <= numRows() )
    {
        removeRow( currentRow_ );
        ComputeAllValues();
        currentRow_ = 0;
    }
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableCrew::OnRequestContextMenu
// Created: ABR 2011-02-28
// -----------------------------------------------------------------------------
void UnitStateTableCrew::OnRequestContextMenu( int row, int /*col*/, const QPoint& pos )
{
    if( isReadOnly() )
        return;
    currentRow_ = row;
    Q3PopupMenu popupMenu;
    popupMenu.insertItem( tr( "Add" ), this, SLOT( AddLine() ) );
    if( numRows() > eNbrHumanRank )
    {
        popupMenu.insertSeparator();
        if( row >= eNbrHumanRank && row <= numRows() )
            popupMenu.insertItem( tr( "Delete" ), this, SLOT( OnRemoveItem() ) );
        popupMenu.insertItem( tr( "Clear" ), this, SLOT( OnClearItems() ) );
    }
    popupMenu.exec( pos );
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableCrew::keyPressEvent
// Created: ABR 2011-02-28
// -----------------------------------------------------------------------------
void UnitStateTableCrew::keyPressEvent( QKeyEvent * e )
{
    if( e->key() == Qt::Key_Delete && currentRow() >= eNbrHumanRank )
        OnRemoveItem();
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableCrew::OnValueChanged
// Created: ABR 2011-02-28
// -----------------------------------------------------------------------------
void UnitStateTableCrew::OnValueChanged( int row, int col )
{
    if( col == eRank )
    {
        setText( row, eNumber, "0" );
        ComputeAllValues();
    }
    else if( col == eNumber )
        ComputeValues( GetComboValue< E_HumanRank >( row, eRank ) );
    else if( col == eState )
    {
        E_HumanState state = GetComboValue< E_HumanState >( row, eState );
        static_cast< ComboTableItem* >( item( row, eInjuries ) )->setEnabled( state == eHumanState_Injured );
        static_cast< Q3CheckTableItem* >( item( row, ePsy ) )->setEnabled( state != eHumanState_Dead );
        static_cast< Q3CheckTableItem* >( item( row, eContaminated ) )->setEnabled( state != eHumanState_Dead );
        if( state == eHumanState_Dead )
        {
            static_cast< Q3CheckTableItem* >( item( row, ePsy ) )->setChecked( false );
            static_cast< Q3CheckTableItem* >( item( row, eContaminated ) )->setChecked( false );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableCrew::ComputeAllValues
// Created: ABR 2011-02-28
// -----------------------------------------------------------------------------
void UnitStateTableCrew::ComputeAllValues()
{
    ComputeValues( eHumanRank_Officier );
    ComputeValues( eHumanRank_SousOfficer );
    ComputeValues( eHumanRank_Mdr );
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableCrew::ComputeValues
// Created: ABR 2011-02-28
// -----------------------------------------------------------------------------
void UnitStateTableCrew::ComputeValues( E_HumanRank rank )
{
    // Count for each rank the number of officer
    unsigned int others = 0;
    for( int row = eNbrHumanRank; row < numRows(); ++row )
        if( rank == GetComboValue< E_HumanRank >( row, eRank ) )
            others += item( row, eNumber )->text().toUInt();
    // Update left officers count
    nbrOfficersHealthy_[ rank ] = nbrOfficersTotal_[ rank ] - others;
    // Update max value for each spinbox
    for( int row = eNbrHumanRank; row < numRows(); ++row )
        if( rank == GetComboValue< E_HumanRank >( row, eRank ) )
            static_cast< gui::SpinTableItem< int >* >( item( row, eNumber ) )->SetMinMaxValue( 0, nbrOfficersHealthy_[ rank ] + item( row, eNumber )->text().toUInt() );
    // Update first line left value
    item( rank, eNumber )->setText( QString::number( nbrOfficersHealthy_[ rank ] ) );
    updateCell( rank, eNumber );
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableCrew::AddLine
// Created: ABR 2011-02-25
// -----------------------------------------------------------------------------
void UnitStateTableCrew::AddLine( int number /*= 0*/,
                                  E_HumanRank rank /*= eHumanRank_Officier*/,
                                  E_HumanState state /*= eHumanState_Healhty*/,
                                  E_InjuriesSeriousness seriousness /*= eInjuriesSeriousness_U1*/,
                                  bool psy /*= false*/,
                                  bool contaminated /*= false*/,
                                  E_HumanLocation location /*= eHumanLocation_Battlefield*/ )
{
    unsigned int nRow = numRows();
    insertRows( nRow );
    AddCombo< E_HumanRank >( nRow, eRank, rank, humanRanks_, nRow >= eNbrHumanRank );
    AddCombo< E_HumanState >( nRow, eState, state, humanStates_, nRow >= eNbrHumanRank );
    AddCombo< E_InjuriesSeriousness >( nRow, eInjuries, seriousness, injuriesSeriousness_, nRow >= eNbrHumanRank );
    if( !isColumnHidden( eLocation ) )
        AddCombo< E_HumanLocation>( nRow, eLocation, location, humanLocations_, nRow >= eNbrHumanRank );
    setItem( nRow, ePsy, new Q3CheckTableItem( this, "" ) );
    static_cast< Q3CheckTableItem* >( item( nRow, ePsy ) )->setChecked( psy );
    setItem( nRow, eContaminated, new Q3CheckTableItem( this, "" ) );
    static_cast< Q3CheckTableItem* >( item( nRow, eContaminated ) )->setChecked( contaminated );
    setItem( nRow, eNumber, new gui::SpinTableItem< int >( this, 0, nbrOfficersTotal_[ rank ] ) );
    item( nRow, eNumber )->setText( QString::number( number ) );
    OnValueChanged( nRow, eState );
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableCrew::MergeLine
// Created: ABR 2011-07-06
// -----------------------------------------------------------------------------
void UnitStateTableCrew::MergeLine( E_HumanRank rank, E_HumanState state, E_InjuriesSeriousness seriousness, bool psyop, bool contaminated, int number, E_HumanLocation location /*= eHumanLocation_Battlefield*/ )
{
    nbrOfficersTotal_[ rank ] += number;
    if( state == eHumanState_Healthy && location == eHumanLocation_Battlefield && !contaminated && !psyop && seriousness == eInjuriesSeriousness_U1 )
        nbrOfficersHealthy_[ rank ] += number;
    int row = 0;
    for( ; row < numRows(); ++row )
    {
        if( rank         == GetComboValue< E_HumanRank >( row, eRank ) &&
            state        == GetComboValue< E_HumanState >( row, eState ) &&
            ( isColumnHidden( eLocation ) || location == GetComboValue< E_HumanLocation >( row, eLocation ) ) &&
            psyop        == GetCheckboxValue( row, ePsy) &&
            contaminated == GetCheckboxValue( row, eContaminated ) &&
            ( state != eHumanState_Injured || seriousness == GetComboValue< E_InjuriesSeriousness >( row, eInjuries ) ) )
        {
            setText( row, eNumber, QString::number( item( row, eNumber )->text().toUInt() + number ) );
            break;
        }
    }
    if( row >= numRows() )
        AddLine( number, rank, state, seriousness, psyop, contaminated, location );
}
