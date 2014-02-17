// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "UnitStateTableCrew.h"
#include "moc_UnitStateTableCrew.cpp"
#include "clients_kernel/Tools.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/ContextMenu.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: UnitStateTableCrew constructor
// Created: ABR 2011-02-24
// -----------------------------------------------------------------------------
UnitStateTableCrew::UnitStateTableCrew( QWidget* parent, kernel::Controllers& controllers )
    : UnitStateTable_ABC( "UnitStateTableCrew", parent, 7, controllers )
    , updating_( false )
{
    horizontalHeaders_ << tr( "Rank" )
                       << tr( "State" )
                       << tr( "Injury seriousness" ) // $$$$ ABR 2011-08-11: should become Injuries with story 660
                       << tr( "Location" )
                       << tr( "Mentally injured" )
                       << tr( "Contaminated" )
                       << tr( "Quantity" );
    setAlternatingRowColors( false );
    connect( &dataModel_, SIGNAL( itemChanged( QStandardItem* ) ), SLOT( OnItemChanged( QStandardItem* ) ) );
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
// Name: UnitStateTableCrew::IsReadOnlyForType
// Created: JSR 2012-03-22
// -----------------------------------------------------------------------------
bool UnitStateTableCrew::IsReadOnlyForType( const std::string& typeName ) const
{
    return typeName != kernel::Agent_ABC::typeName_;
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableCrew::Purge
// Created: ABR 2011-08-12
// -----------------------------------------------------------------------------
void UnitStateTableCrew::Purge()
{
    UnitStateTable_ABC::Purge();
    delegate_.Purge();
    delegate_.AddComboBoxOnColumn( eRank, eNbrHumanRank );
    delegate_.AddComboBoxOnColumn( eState, eNbrHumanState );
    delegate_.AddComboBoxOnColumn( eInjuries, eNbrInjuriesSeriousness );
    for( int rank = 0; rank < eNbrHumanRank; ++rank )
    {
        nbrOfficersTotal_[ static_cast< E_HumanRank >( rank ) ] = 0;
        nbrOfficersHealthy_[ static_cast< E_HumanRank >( rank ) ] = 0;
    }
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableCrew::OnClearItems
// Created: ABR 2011-02-28
// -----------------------------------------------------------------------------
void UnitStateTableCrew::OnClearItems()
{
    dataModel_.removeRows( eNbrHumanRank, dataModel_.rowCount() - eNbrHumanRank );
    ComputeAllValues();
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableCrew::OnRemoveCurrentItem
// Created: ABR 2011-02-28
// -----------------------------------------------------------------------------
void UnitStateTableCrew::OnRemoveCurrentItem()
{
    QModelIndex index = proxyModel_.mapToSource( currentIndex() );
    if( index.isValid() && index.row() >= eNbrHumanRank && index.row() <= dataModel_.rowCount() )
    {
        dataModel_.removeRow( index.row() );
        ComputeAllValues();
    }
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableCrew::contextMenuEvent
// Created: ABR 2011-10-11
// -----------------------------------------------------------------------------
void UnitStateTableCrew::contextMenuEvent( QContextMenuEvent* e )
{
    if( IsReadOnly() )
        return;
    kernel::ContextMenu popupMenu;
    popupMenu.insertItem( tr( "Add" ), this, SLOT( AddLine() ) );
    if( dataModel_.rowCount() > eNbrHumanRank )
    {
        popupMenu.insertSeparator();
        QModelIndex index = indexAt( e->pos() );
        if( index.isValid() && index.row() >= eNbrHumanRank && index.row() <= dataModel_.rowCount() )
        {
            setCurrentIndex( index );
            popupMenu.insertItem( tr( "Delete" ), this, SLOT( OnRemoveCurrentItem() ) );
        }
        popupMenu.insertItem( tr( "Clear" ), this, SLOT( OnClearItems() ) );
    }
    popupMenu.exec( e->globalPos() );
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableCrew::keyPressEvent
// Created: ABR 2011-02-28
// -----------------------------------------------------------------------------
void UnitStateTableCrew::keyPressEvent( QKeyEvent * e )
{
    if( !IsReadOnly() && e->key() == Qt::Key_Delete && currentIndex().isValid() )
        OnRemoveCurrentItem();
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableCrew::OnItemChanged
// Created: ABR 2011-02-28
// -----------------------------------------------------------------------------
void UnitStateTableCrew::OnItemChanged( QStandardItem* item )
{
    if( !item || updating_ )
        return;
    if( item->column() == eRank )
    {
        SetData( item->row(), eNumber, "0", 0 );
        ComputeAllValues();
    }
    else if( item->column() == eNumber )
        ComputeValues( GetEnumData< E_HumanRank >( item->row(), eRank ) );
    else if( item->column() == eState )
    {
        E_HumanState state = GetEnumData< E_HumanState >( item->row(), eState );
        SetEnabled( item->row(), eInjuries, state == eHumanState_Injured );
        SetEnabled( item->row(), ePsy, state != eHumanState_Dead );
        SetEnabled( item->row(), eContaminated, state != eHumanState_Dead );
        if( state == eHumanState_Dead )
        {
            SetCheckedState( item->row(), ePsy, false );
            SetCheckedState( item->row(), eContaminated, false );
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
    if( rank >= dataModel_.rowCount() )
        return;
    // Count for each rank the number of officer
    unsigned int others = 0;
    for( int row = eNbrHumanRank; row < dataModel_.rowCount(); ++row )
        if( rank == GetEnumData< E_HumanRank >( row, eRank ) )
            others += GetUserData( row, eNumber ).toUInt();
    // Update left officers count
    nbrOfficersHealthy_[ rank ] = nbrOfficersTotal_[ rank ] - others;
    // Update max value for each spinbox
    for( int row = eNbrHumanRank; row < dataModel_.rowCount(); ++row )
        if( rank == GetEnumData< E_HumanRank >( row, eRank ) )
            delegate_.SetSpinBoxMinMax( row, eNumber, 0, static_cast< int >( nbrOfficersHealthy_[ rank ] + GetUserData( row, eNumber ).toUInt() ) );
    // Update first line left value
    SetData( rank, eNumber, locale().toString( nbrOfficersHealthy_[ rank ] ), nbrOfficersHealthy_[ rank ] );
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableCrew::AddLine
// Created: ABR 2011-02-25
// -----------------------------------------------------------------------------
void UnitStateTableCrew::AddLine( int number /* = 0*/,
                                  E_HumanRank rank /* = eHumanRank_Officier*/,
                                  E_HumanState state /* = eHumanState_Healhty*/,
                                  E_InjuriesSeriousness seriousness /* = eInjuriesSeriousness_U1*/,
                                  bool psy /* = false*/,
                                  bool contaminated /* = false*/,
                                  E_HumanLocation location /* = eHumanLocation_Battlefield*/ )
{
    updating_ = true;
    unsigned int row = dataModel_.rowCount();
    bool isHealthy = row < eNbrHumanRank;
    Qt::ItemFlags flags = isHealthy ? 0 : Qt::ItemIsEditable;
    Qt::ItemFlags checkFlags = isHealthy ? 0 : Qt::ItemIsEditable | Qt::ItemIsUserCheckable;
    AddItem( row, eRank, tools::ToString( rank ), static_cast< int >( rank ), flags );
    AddItem( row, eState, tools::ToString( state ), static_cast< int >( state ), flags );
    AddItem( row, eInjuries, tools::ToString( seriousness ), static_cast< int >( seriousness ), flags );
    if( !isColumnHidden( eLocation ) )
        AddItem( row, eLocation, tools::ToString( location ), static_cast< int >( location ) );
    AddItem( row, ePsy, QString(), psy, checkFlags );
    SetCheckedState( row, ePsy, psy );
    AddItem( row, eContaminated, QString(), contaminated, checkFlags );
    SetCheckedState( row, eContaminated, contaminated );
    AddItem( row, eNumber, locale().toString( number ), number, flags );

    if( !isHealthy )
        delegate_.AddSpinBox( row, row, eNumber, eNumber, 0, nbrOfficersTotal_[ rank ] );
    else
    {
        SetEnabled( row, ePsy, false );
        SetEnabled( row, eContaminated, false );
        SetCheckedState( row, ePsy, false );
        SetCheckedState( row, eContaminated, false );
        for( int i = 0; i <= static_cast< int >( eNumber ); ++i )
            if( !isColumnHidden( i ) )
                SetColor( row, i, Qt::gray, 0 );
    }
    updating_ = false;
    OnItemChanged( dataModel_.item( row, eState ) );
    ComputeAllValues();
}

// -----------------------------------------------------------------------------
// Name: UnitStateTableCrew::MergeLine
// Created: ABR 2011-07-06
// -----------------------------------------------------------------------------
void UnitStateTableCrew::MergeLine( E_HumanRank rank, E_HumanState state, E_InjuriesSeriousness seriousness, bool psyop, bool contaminated, int number, E_HumanLocation location /* = eHumanLocation_Battlefield*/ )
{
    nbrOfficersTotal_[ rank ] += number;
    if( state == eHumanState_Healthy && location == eHumanLocation_Battlefield && !contaminated && !psyop && seriousness == eInjuriesSeriousness_U1 )
        nbrOfficersHealthy_[ rank ] += number;
    int row = 0;
    for( ; row < dataModel_.rowCount(); ++row )
    {
        if( rank         == GetEnumData< E_HumanRank >( row, eRank ) &&
            state        == GetEnumData< E_HumanState >( row, eState ) &&
            ( isColumnHidden( eLocation ) || location == GetEnumData< E_HumanLocation >( row, eLocation ) ) &&
            psyop        == GetCheckedState( row, ePsy) &&
            contaminated == GetCheckedState( row, eContaminated ) &&
            ( state != eHumanState_Injured || seriousness == GetEnumData< E_InjuriesSeriousness >( row, eInjuries ) ) )
        {
            unsigned int newValue = GetUserData( row, eNumber ).toUInt() + number;
            SetData( row, eNumber, locale().toString( newValue ), newValue );
            break;
        }
    }
    if( row >= dataModel_.rowCount() )
        AddLine( number, rank, state, seriousness, psyop, contaminated, location );
}
