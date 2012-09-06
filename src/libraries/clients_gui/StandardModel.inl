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
    root->setChild( row, col, item );
    //item->setData( *new QVariant( showValue_ ), FilterRole ); // $$$$ ABR 2012-08-17: Default filter entry, anything but show will be hide
    return item;
}

// -----------------------------------------------------------------------------
// Name: StandardModel::AddRootTextItem
// Created: ABR 2012-08-16
// -----------------------------------------------------------------------------
inline
QStandardItem* StandardModel::AddRootTextItem( int row, int col, QString text, Qt::ItemFlags flags /*= 0*/ )
{
    return AddChildTextItem( invisibleRootItem(), row, col, text, flags );
}

// -----------------------------------------------------------------------------
// Name: StandardModel::AddChildTextItem
// Created: ABR 2012-08-16
// -----------------------------------------------------------------------------
inline
QStandardItem* StandardModel::AddChildTextItem( QStandardItem* root, int row, int col, QString text, Qt::ItemFlags flags /*= 0*/ )
{
    QStandardItem* item = AddChildItem( root, row, col, flags );
    item->setData( *new QVariant( text ), Qt::DisplayRole );
    return item;
}

// -----------------------------------------------------------------------------
// Name: StandardModel::AddRootSafeItem
// Created: ABR 2012-08-16
// -----------------------------------------------------------------------------
template< typename T >
inline
QStandardItem* StandardModel::AddRootSafeItem( int row, int col, QString text, const T& value, Qt::ItemFlags flags /*= 0*/ )
{
    return AddChildSafeItem( invisibleRootItem(), row, col, text, value, flags );
}

// -----------------------------------------------------------------------------
// Name: StandardModel::AddChildSafeItem
// Created: ABR 2012-08-16
// -----------------------------------------------------------------------------
template< typename T >
inline
QStandardItem* StandardModel::AddChildSafeItem( QStandardItem* root, int row, int col, QString text, const T& value, Qt::ItemFlags flags /*= 0*/ )
{
    QStandardItem* item = AddChildTextItem( root, row, col, text, flags );

    QVariant* variant = new QVariant();
    variant->setValue( kernel::VariantPointer( new kernel::SafePointer< T >( controllers_, &value ) ) );
    item->setData( *variant, DataRole );

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
    item->setData( pixmap, Qt::DecorationRole ); // $$$$ ABR 2012-08-17: TODO: Test ...
    return item;
}


namespace
{
    // -----------------------------------------------------------------------------
    // Name: StandardModel::FindItem
    // Created: ABR 2012-08-16
    // -----------------------------------------------------------------------------
    QStandardItem* RecFindItem( QStandardItem* root, const QString& text )
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
                    QStandardItem* result = RecFindItem( childItem, text );
                    if( result )
                        return result;
                }
            }
        }
        return 0;
    }

    // -----------------------------------------------------------------------------
    // Name: StandardModel::FindSafeItem
    // Created: ABR 2012-08-16
    // -----------------------------------------------------------------------------
    template< typename T >
    QStandardItem* RecFindSafeItem( QStandardItem* root, const T& value )
    {
        if( root == 0 )
            return 0;
        for( int row = 0; row < root->rowCount(); ++row )
        {
            QStandardItem* childItem = root->child( row, 0 );
            if( childItem )
            {
                const kernel::SafePointer< T >* childEntity = static_cast< const kernel::SafePointer< T >* >( childItem->data( StandardModel::DataRole ).value< kernel::VariantPointer >().ptr_ );
                if( childEntity && *childEntity && *childEntity == &value )
                    return childItem;
                else
                {
                    QStandardItem* result = RecFindSafeItem( childItem, value );
                    if( result )
                        return result;
                }
            }
        }
        return 0;
    }

    // -----------------------------------------------------------------------------
    // Name: StandardModel::PurgeObsoleteSafePointer
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
        }
    }
}

// -----------------------------------------------------------------------------
// Name: StandardModel::FindSafeData
// Created: ABR 2012-08-16
// -----------------------------------------------------------------------------
template< typename T >
inline
T* StandardModel::FindSafeData( const T& value )
{
    QStandardItem* item = FindSafeItem( value );
    if( item )
    {
        const kernel::SafePointer< T >* safePtr = static_cast< const kernel::SafePointer< T >* >( item->data( DataRole ).value< kernel::VariantPointer >().ptr_ );
        if( safePtr )
            return safePtr->ConstCast();
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: StandardModel::FindItem
// Created: ABR 2012-08-16
// -----------------------------------------------------------------------------
inline
QStandardItem* StandardModel::FindItem( const QString& text )
{
    return RecFindItem( invisibleRootItem(), text );
}

// -----------------------------------------------------------------------------
// Name: StandardModel::FindSafeItem
// Created: ABR 2012-08-16
// -----------------------------------------------------------------------------
template< typename T >
inline
QStandardItem* StandardModel::FindSafeItem( const T& value )
{
    return RecFindSafeItem( invisibleRootItem(), value );
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
    const kernel::SafePointer< T >* safePtr = static_cast< const kernel::SafePointer< T >* >( item.data( DataRole ).value< kernel::VariantPointer >().ptr_ );
    if( safePtr && *safePtr )
        return safePtr->ConstCast();
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
    QStandardItem* item = itemFromIndex( index.model() == this ? index : proxy_.mapToSource( index ) );
    if( !item )
        return 0;
    return GetDataFromItem< T >( *item );
}
