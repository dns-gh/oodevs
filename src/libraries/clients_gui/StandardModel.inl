// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: StandardModel::AddRootItem
// Created: ABR 2012-08-16
// -----------------------------------------------------------------------------
inline
QStandardItem* StandardModel::AddRootItem( int row, int col, Qt::ItemFlags flags /*= 0*/ )
{
    return AddChildItem( invisibleRootItem(), row, col, flags );
}

// -----------------------------------------------------------------------------
// Name: StandardModel::AddChildItem
// Created: ABR 2012-08-16
// -----------------------------------------------------------------------------
inline
QStandardItem* StandardModel::AddChildItem( QStandardItem* root, int row, int col, Qt::ItemFlags flags /*= 0*/ )
{
    QStandardItem* item = new QStandardItem();
    item->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable | flags );
    item->setData( *new QVariant( showValue_ ), FilterRole );
    item->setData( *new QVariant(), SafeRole );
    item->setData( *new QVariant(), DataRole );
    root->setChild( row, col, item );
    return item;
}

// -----------------------------------------------------------------------------
// Name: StandardModel::AddRootTextItem
// Created: ABR 2012-08-16
// -----------------------------------------------------------------------------
inline
QStandardItem* StandardModel::AddRootTextItem( int row, int col, const QString& text, const QString& tooltip, Qt::ItemFlags flags /*= 0*/ )
{
    return AddChildTextItem( invisibleRootItem(), row, col, text, tooltip, flags );
}

// -----------------------------------------------------------------------------
// Name: StandardModel::AddChildTextItem
// Created: ABR 2012-08-16
// -----------------------------------------------------------------------------
inline
QStandardItem* StandardModel::AddChildTextItem( QStandardItem* root, int row, int col, const QString& text, const QString& tooltip,  Qt::ItemFlags flags /*= 0*/ )
{
    QStandardItem* item = AddChildItem( root, row, col, flags );
    item->setData( *new QVariant( text ), Qt::DisplayRole );
    item->setData( *new QVariant( tooltip ), Qt::ToolTipRole );
    return item;
}


// -----------------------------------------------------------------------------
// Name: StandardModel::AddRootDataItem
// Created: ABR 2012-09-19
// -----------------------------------------------------------------------------
template< typename T >
inline
QStandardItem* StandardModel::AddRootDataItem( int row, int col, const QString& text, const QString& tooltip, const T& value, Qt::ItemFlags flags /*= 0*/ )
{
    return AddRootDataItem( invisibleRootItem(), row, col, text, tooltip, value, flags );
}

// -----------------------------------------------------------------------------
// Name: StandardModel::AddChildDataItem
// Created: ABR 2012-09-19
// -----------------------------------------------------------------------------
template< typename T >
inline
QStandardItem* StandardModel::AddChildDataItem( QStandardItem* parent, int row, int col, const QString& text, const QString& tooltip, const T& value, Qt::ItemFlags flags /*= 0*/ )
{
    QStandardItem* item = AddChildTextItem( root, row, col, text, tooltip, flags );

    QVariant* variant = new QVariant();
    variant->setValue( kernel::VariantPointer( &value ) );
    item->setData( *variant, DataRole );
    item->setData( *new QVariant( false ), SafeRole );

    return item;
}

// -----------------------------------------------------------------------------
// Name: StandardModel::AddRootSafeItem
// Created: ABR 2012-08-16
// -----------------------------------------------------------------------------
template< typename T >
inline
QStandardItem* StandardModel::AddRootSafeItem( int row, int col, const QString& text, const QString& tooltip, const T& value, Qt::ItemFlags flags /*= 0*/ )
{
    return AddChildSafeItem( invisibleRootItem(), row, col, text, tooltip, value, flags );
}

// -----------------------------------------------------------------------------
// Name: StandardModel::AddChildSafeItem
// Created: ABR 2012-08-16
// -----------------------------------------------------------------------------
template< typename T >
inline
QStandardItem* StandardModel::AddChildSafeItem( QStandardItem* root, int row, int col, const QString& text, const QString& tooltip, const T& value, Qt::ItemFlags flags /*= 0*/ )
{
    QStandardItem* item = AddChildTextItem( root, row, col, text, tooltip, flags );

    QVariant* variant = new QVariant();
    variant->setValue( kernel::VariantPointer( new kernel::SafePointer< T >( controllers_, &value ) ) );
    item->setData( *variant, DataRole );
    item->setData( *new QVariant( true ), SafeRole );

    return item;
}

// -----------------------------------------------------------------------------
// Name: StandardModel::AddRootIconItem
// Created: ABR 2012-08-16
// -----------------------------------------------------------------------------
inline
QStandardItem* StandardModel::AddRootIconItem( int row, int col, const QPixmap& pixmap , Qt::ItemFlags flags /*= 0*/ )
{
    return AddChildIconItem( invisibleRootItem(), row, col, pixmap, flags );
}

// -----------------------------------------------------------------------------
// Name: StandardModel::AddChildIconItem
// Created: ABR 2012-08-16
// -----------------------------------------------------------------------------
inline
QStandardItem* StandardModel::AddChildIconItem( QStandardItem* root, int row, int col, const QPixmap& pixmap , Qt::ItemFlags flags /*= 0*/ )
{
    QStandardItem* item = AddChildItem( root, row, col, flags );
    item->setData( pixmap, Qt::DecorationRole ); // $$$$ ABR 2012-08-17: To be tested
    return item;
}

namespace
{
    // -----------------------------------------------------------------------------
    // Name: RecFindTextItem
    // Created: ABR 2012-08-16
    // -----------------------------------------------------------------------------
    QStandardItem* RecFindTextItem( QStandardItem* root, const QString& text )
    {
        if( root == 0 )
            return 0;
        for( int row = 0; row < root->rowCount(); ++row )
        {
            QStandardItem* childItem = root->child( row, 0 );
            if( childItem )
            {
                if( childItem->text() == text )
                    return childItem;
                else
                {
                    QStandardItem* result = RecFindTextItem( childItem, text );
                    if( result )
                        return result;
                }
            }
        }
        return 0;
    }

    // -----------------------------------------------------------------------------
    // Name: RecFindDataItem
    // Created: ABR 2012-08-16
    // -----------------------------------------------------------------------------
    template< typename T >
    inline
        QStandardItem* RecFindDataItem( const StandardModel& model, QStandardItem* root, const T& value )
    {
        if( root == 0 )
            return 0;
        for( int row = 0; row < root->rowCount(); ++row )
        {
            QStandardItem* childItem = root->child( row, 0 );
            if( childItem )
            {
                const T* concretChildEntity = model.GetDataFromItem< T >( *childItem );
                if( concretChildEntity == &value )
                    return childItem;
                else
                {
                    QStandardItem* result = RecFindDataItem( model, childItem, value );
                    if( result )
                        return result;
                }
            }
        }
        return 0;
    }

    // -----------------------------------------------------------------------------
    // Name: PurgeObsoleteSafePointer
    // Created: ABR 2012-08-16
    // -----------------------------------------------------------------------------
    template< typename T >
    void RecPurgeObsoleteSafeItem( QStandardItem* root )
    {
        if( root == 0 )
            return;
        for( int row = 0; row < root->rowCount(); )
        {
            QStandardItem* childItem = root->child( row, 0 );
            if( childItem )
            {
                RecPurgeObsoleteSafeItem< T >( childItem );

                if( childItem->data( StandardModel::SafeRole ).isValid() && childItem->data( StandardModel::SafeRole ).toBool() &&
                    childItem->data( StandardModel::DataRole ).isValid() )
                {
                    const kernel::SafePointer< T >* childEntity = static_cast< const kernel::SafePointer< T >* >( childItem->data( StandardModel::DataRole ).value< kernel::VariantPointer >().ptr_ );
                    if( childEntity && *childEntity == 0 )
                    {
                        QList< QStandardItem* > rowItems = root->takeRow( row );
                        for( QList< QStandardItem *>::iterator it = rowItems.begin(); it != rowItems.end(); ++it )
                            delete *it;
                    }
                    else
                        ++row;
                }
                else
                    ++row;
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: StandardModel::FindSafeData
// Created: ABR 2012-08-16
// -----------------------------------------------------------------------------
template< typename T >
inline
T* StandardModel::FindData( const T& value ) const
{
    QStandardItem* item = FindDataItem( value );
    if( item )
        return GetDataFromItem( *item );
    return 0;
}

// -----------------------------------------------------------------------------
// Name: StandardModel::FindTextItem
// Created: ABR 2012-08-16
// -----------------------------------------------------------------------------
inline
QStandardItem* StandardModel::FindTextItem( const QString& text, QStandardItem* root /*= 0*/ ) const
{
    return RecFindTextItem( root ? root : invisibleRootItem(), text );
}

// -----------------------------------------------------------------------------
// Name: StandardModel::FindDataItem
// Created: ABR 2012-08-16
// -----------------------------------------------------------------------------
template< typename T >
inline
QStandardItem* StandardModel::FindDataItem( const T& value ) const
{
    return RecFindDataItem( *this, invisibleRootItem(), value );
}

// -----------------------------------------------------------------------------
// Name: StandardModel::PurgeObsoleteSafeItem
// Created: ABR 2012-08-16
// -----------------------------------------------------------------------------
template< typename T >
inline
void StandardModel::PurgeObsoleteSafeItem()
{
    RecPurgeObsoleteSafeItem< T >( invisibleRootItem() );
}

// -----------------------------------------------------------------------------
// Name: StandardModel::GetDataFromItem
// Created: ABR 2012-08-17
// -----------------------------------------------------------------------------
template< typename T >
inline
T* StandardModel::GetDataFromItem( const QStandardItem& item ) const
{
    if( item.data( SafeRole ).isValid() && item.data( DataRole ).isValid() )
    {
        if( item.data( SafeRole ).toBool() )
        {
            const kernel::SafePointer< T >* safePtr = static_cast< const kernel::SafePointer< T >* >( item.data( DataRole ).value< kernel::VariantPointer >().ptr_ );
            if( safePtr && *safePtr )
                return safePtr->ConstCast();
        }
        else
            return const_cast< T* >( static_cast< const T* >( item.data( DataRole ).value< kernel::VariantPointer >().ptr_ ) );
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: StandardModel::GetDataFromIndex
// Created: ABR 2012-08-17
// -----------------------------------------------------------------------------
template< typename T >
inline
T* StandardModel::GetDataFromIndex( const QModelIndex& index ) const
{
    if( !index.isValid() )
        return 0;
    QStandardItem* item = itemFromIndex( index.model() == this ? index : proxy_.mapToSource( index ) );
    if( !item )
        return 0;
    return GetDataFromItem< T >( *item );
}
