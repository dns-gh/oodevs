// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: StandardModel::AddRootDataItem
// Created: ABR 2012-09-19
// -----------------------------------------------------------------------------
template< typename T >
QStandardItem* StandardModel::AddRootDataItem( int row, int col, const QString& text, const QString& tooltip, const T& value, Qt::ItemFlags flags /*= 0*/ )
{
    return AddChildDataItem( invisibleRootItem(), row, col, text, tooltip, value, flags );
}

// -----------------------------------------------------------------------------
// Name: StandardModel::AddChildDataItem
// Created: ABR 2012-09-19
// -----------------------------------------------------------------------------
template< typename T >
QStandardItem* StandardModel::AddChildDataItem( QStandardItem* root, int row, int col, const QString& text, const QString& tooltip, const T& value, Qt::ItemFlags flags /*= 0*/ )
{
    QStandardItem* item = AddChildTextItem( root, row, col, text, tooltip, flags );

    QVariant variant;
    variant.setValue( kernel::VariantPointer( &value ) );
    item->setData( variant, Roles::DataRole );
    item->setData( QVariant( false ), Roles::SafeRole );
    item->setData( QString( typeid( T ).name() ), Roles::MimeTypeRole );

    return item;
}

// -----------------------------------------------------------------------------
// Name: StandardModel::AddRootSafeItem
// Created: ABR 2012-08-16
// -----------------------------------------------------------------------------
template< typename T >
QStandardItem* StandardModel::AddRootSafeItem( int row, int col, const QString& text, const QString& tooltip, const T& value, Qt::ItemFlags flags /*= 0*/ )
{
    return AddChildSafeItem( 0, row, col, text, tooltip, value, flags );
}

// -----------------------------------------------------------------------------
// Name: StandardModel::AddChildSafeItem
// Created: ABR 2012-08-16
// -----------------------------------------------------------------------------
template< typename T >
QStandardItem* StandardModel::AddChildSafeItem( QStandardItem* root, int row, int col, const QString& text, const QString& tooltip, const T& value, Qt::ItemFlags flags /*= 0*/ )
{
    assert( controllers_ );
    QStandardItem* item = AddChildTextItem( root, row, col, text, tooltip, flags );
    if( controllers_ )
    {
        QVariant variant;
        variant.setValue( kernel::VariantPointer( new kernel::SafePointer< T >( *controllers_, &value ) ) );
        item->setData( variant, Roles::DataRole );
        item->setData( QVariant( true ), Roles::SafeRole );
        item->setData( QString( typeid( T ).name() ), Roles::MimeTypeRole );
    }
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
            assert( childItem );
            if( childItem->text() == text )
                return childItem;
            else
            {
                QStandardItem* result = RecFindTextItem( childItem, text );
                if( result )
                    return result;
            }
        }
        return 0;
    }

    // -----------------------------------------------------------------------------
    // Name: RecFindDataItem
    // Created: ABR 2012-08-16
    // -----------------------------------------------------------------------------
    template< typename T >
        QStandardItem* RecFindDataItem( const StandardModel& model, QStandardItem* root, const T& value )
    {
        if( root == 0 )
            return 0;
        for( int row = 0; row < root->rowCount(); ++row )
        {
            QStandardItem* childItem = root->child( row, 0 );
            assert( childItem );
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
            assert( childItem );
            RecPurgeObsoleteSafeItem< T >( childItem );

            if( childItem->data( Roles::SafeRole ).isValid() && childItem->data( Roles::SafeRole ).toBool() &&
                childItem->data( Roles::DataRole ).isValid() )
            {
                const kernel::SafePointer< T >* childEntity = static_cast< const kernel::SafePointer< T >* >( childItem->data( Roles::DataRole ).value< kernel::VariantPointer >().ptr_ );
                if( childEntity && *childEntity == 0 )
                {
                    delete childEntity;
                    root->removeRow( row );
                }
                else
                    ++row;
            }
            else
                ++row;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: StandardModel::FindSafeData
// Created: ABR 2012-08-16
// -----------------------------------------------------------------------------
template< typename T >
T* StandardModel::FindData( const T& value, QStandardItem* root /*= 0*/ ) const
{
    QStandardItem* item = FindDataItem( value, root );
    if( item )
        return GetDataFromItem( *item );
    return 0;
}

// -----------------------------------------------------------------------------
// Name: StandardModel::FindDataItem
// Created: ABR 2012-08-16
// -----------------------------------------------------------------------------
template< typename T >
QStandardItem* StandardModel::FindDataItem( const T& value, QStandardItem* root /*= 0*/ ) const
{
    return RecFindDataItem( *this, root ? root : invisibleRootItem(), value );
}

// -----------------------------------------------------------------------------
// Name: StandardModel::PurgeObsoleteSafeItem
// Created: ABR 2012-08-16
// -----------------------------------------------------------------------------
template< typename T >
void StandardModel::PurgeObsoleteSafeItem()
{
    RecPurgeObsoleteSafeItem< T >( invisibleRootItem() );
}

// -----------------------------------------------------------------------------
// Name: StandardModel::GetDataFromItem
// Created: ABR 2012-08-17
// -----------------------------------------------------------------------------
template< typename T >
T* StandardModel::GetDataFromItem( const QStandardItem& item ) const
{
    if( item.data( Roles::SafeRole ).isValid() && item.data( Roles::DataRole ).isValid() )
    {
        if( item.data( Roles::SafeRole ).toBool() )
        {
            const kernel::SafePointer< T >* safePtr = static_cast< const kernel::SafePointer< T >* >( item.data( Roles::DataRole ).value< kernel::VariantPointer >().ptr_ );
            if( safePtr && *safePtr )
                return safePtr->ConstCast();
        }
        else
        {
            return const_cast< T* >( static_cast< const T* >( item.data( Roles::DataRole ).value< kernel::VariantPointer >().ptr_ ) );
        }
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: StandardModel::GetDataFromIndex
// Created: ABR 2012-08-17
// -----------------------------------------------------------------------------
template< typename T >
T* StandardModel::GetDataFromIndex( const QModelIndex& index ) const
{
    QStandardItem* item = GetItemFromIndex( index );
    if( !item )
        return 0;
    return GetDataFromItem< T >( *item );
}
