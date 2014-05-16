// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: UnitStateTable_ABC::AddItem
// Created: ABR 2011-10-03
// -----------------------------------------------------------------------------
template< typename T >
void UnitStateTable_ABC::AddItem( int row, int col, QString text, T value, Qt::ItemFlags flags /*= 0*/ )
{
    QStandardItem* item = new QStandardItem();
    item->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable | flags );
    item->setData( QVariant( value ), Qt::UserRole );
    item->setData( QVariant( text ), Qt::DisplayRole );
    item->setData( QVariant( StandardModel::showValue_ ), Roles::FilterRole );
    dataModel_.setItem( row, col, item );
}

// -----------------------------------------------------------------------------
// Name: UnitStateTable_ABC::SetData
// Created: ABR 2011-10-04
// -----------------------------------------------------------------------------
template< typename T >
void UnitStateTable_ABC::SetData( int row, int col, QString text, T value )
{
    QStandardItem* item = dataModel_.item( row, col );
    if( item )
    {
        item->setData( QVariant( value ), Qt::UserRole );
        item->setData( QVariant( text ), Qt::DisplayRole );
    }
}

// -----------------------------------------------------------------------------
// Name: UnitStateTable_ABC::SetEnabled
// Created: ABR 2011-10-11
// -----------------------------------------------------------------------------
inline
void UnitStateTable_ABC::SetEnabled( int row, int col, bool enabled )
{
    QStandardItem* item = dataModel_.item( row, col );
    if( item )
        item->setEnabled( enabled );
}

// -----------------------------------------------------------------------------
// Name: UnitStateTable_ABC::SetCheckedState
// Created: ABR 2011-10-11
// -----------------------------------------------------------------------------
inline
void UnitStateTable_ABC::SetCheckedState( int row, int col, bool checked )
{
    QStandardItem* item = dataModel_.item( row, col );
    if( item )
        item->setCheckState( checked ? Qt::Checked : Qt::Unchecked );
}

// -----------------------------------------------------------------------------
// Name: UnitStateTable_ABC::GetUserData
// Created: ABR 2011-10-04
// -----------------------------------------------------------------------------
inline
QVariant UnitStateTable_ABC::GetUserData( int row, int col ) const
{
    QStandardItem* item = dataModel_.item( row, col );
    assert( item );
    return item->data( Qt::UserRole );
}

// -----------------------------------------------------------------------------
// Name: UnitStateTable_ABC::GetDisplayData
// Created: ABR 2011-10-11
// -----------------------------------------------------------------------------
inline
QString UnitStateTable_ABC::GetDisplayData( int row, int col ) const
{
    QStandardItem* item = dataModel_.item( row, col );
    assert( item );
    return item->data( Qt::DisplayRole ).toString();
}

// -----------------------------------------------------------------------------
// Name: UnitStateTable_ABC::GetEnumData
// Created: ABR 2011-10-04
// -----------------------------------------------------------------------------
template< typename T >
T UnitStateTable_ABC::GetEnumData( int row, int col ) const
{
    return static_cast< T >( GetUserData( row, col ).toInt() );
}

// -----------------------------------------------------------------------------
// Name: UnitStateTable_ABC::GetCheckedState
// Created: ABR 2011-10-11
// -----------------------------------------------------------------------------
inline
bool UnitStateTable_ABC::GetCheckedState( int row, int col ) const
{
    QStandardItem* item = dataModel_.item( row, col );
    assert( item );
    return item->checkState() == Qt::Checked;
}

// -----------------------------------------------------------------------------
// Name: UnitStateTable_ABC::SetReadOnly
// Created: ABR 2011-10-11
// -----------------------------------------------------------------------------
inline
void UnitStateTable_ABC::SetReadOnly( bool readOnly )
{
    delegate_.SetReadOnly( readOnly );
}

// -----------------------------------------------------------------------------
// Name: UnitStateTable_ABC::IsReadOnly
// Created: ABR 2011-10-11
// -----------------------------------------------------------------------------
inline
bool UnitStateTable_ABC::IsReadOnly() const
{
    return delegate_.IsReadOnly();
}
