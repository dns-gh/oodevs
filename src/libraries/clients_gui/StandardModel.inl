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
    return AddChildItem( 0, row, col, flags );
}

// -----------------------------------------------------------------------------
// Name: StandardModel::AddChildItem
// Created: ABR 2012-08-16
// -----------------------------------------------------------------------------
inline
QStandardItem* StandardModel::AddChildItem( QStandardItem* root, int row, int col, Qt::ItemFlags flags /*= 0*/ )
{
    if( !root )
        root = invisibleRootItem();
    QStandardItem* item = new QStandardItem();
    item->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable | flags );
    item->setData( QVariant( showValue_ ), Roles::FilterRole );
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
    return AddChildTextItem( 0, row, col, text, tooltip, flags );
}

// -----------------------------------------------------------------------------
// Name: StandardModel::AddChildTextItem
// Created: ABR 2012-08-16
// -----------------------------------------------------------------------------
inline
QStandardItem* StandardModel::AddChildTextItem( QStandardItem* root, int row, int col, const QString& text, const QString& tooltip,  Qt::ItemFlags flags /*= 0*/ )
{
    QStandardItem* item = AddChildItem( root, row, col, flags );
    item->setData( QVariant( text ), Qt::DisplayRole );
    item->setData( QVariant( tooltip ), Qt::ToolTipRole );
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
    return AddChildDataItem( invisibleRootItem(), row, col, text, tooltip, value, flags );
}

// -----------------------------------------------------------------------------
// Name: StandardModel::AddChildDataItem
// Created: ABR 2012-09-19
// -----------------------------------------------------------------------------
template< typename T >
inline
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
inline
QStandardItem* StandardModel::AddRootSafeItem( int row, int col, const QString& text, const QString& tooltip, const T& value, Qt::ItemFlags flags /*= 0*/ )
{
    return AddChildSafeItem( 0, row, col, text, tooltip, value, flags );
}

// -----------------------------------------------------------------------------
// Name: StandardModel::AddChildSafeItem
// Created: ABR 2012-08-16
// -----------------------------------------------------------------------------
template< typename T >
inline
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

// -----------------------------------------------------------------------------
// Name: StandardModel::AddRootIconItem
// Created: ABR 2012-08-16
// -----------------------------------------------------------------------------
inline
QStandardItem* StandardModel::AddRootIconItem( int row, int col, const QPixmap& pixmap , Qt::ItemFlags flags /*= 0*/ )
{
    return AddChildIconItem( 0, row, col, pixmap, flags );
}

// -----------------------------------------------------------------------------
// Name: StandardModel::AddChildIconItem
// Created: ABR 2012-08-16
// -----------------------------------------------------------------------------
inline
QStandardItem* StandardModel::AddChildIconItem( QStandardItem* root, int row, int col, const QPixmap& pixmap , Qt::ItemFlags flags /*= 0*/ )
{
    QStandardItem* item = AddChildItem( root, row, col, flags );
    item->setData( pixmap, Qt::DecorationRole );
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
    inline
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
inline
T* StandardModel::FindData( const T& value, QStandardItem* root /*= 0*/ ) const
{
    QStandardItem* item = FindDataItem( value, root );
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
QStandardItem* StandardModel::FindDataItem( const T& value, QStandardItem* root /*= 0*/ ) const
{
    return RecFindDataItem( *this, root ? root : invisibleRootItem(), value );
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
// Name: StandardModel::DeleteTree
// Created: ABR 2012-09-20
// -----------------------------------------------------------------------------
inline
void StandardModel::DeleteTree( QStandardItem* item )
{
    if( !item )
        return;
    PurgeChildren( *item );
    DeleteItemRow( item );
}

// -----------------------------------------------------------------------------
// Name: StandardModel::DeleteItemRow
// Created: ABR 2012-09-20
// -----------------------------------------------------------------------------
inline
void StandardModel::DeleteItemRow( QStandardItem* item )
{
    QStandardItem* parentItem = item->parent();
    if( !parentItem )
        parentItem = invisibleRootItem();
    // $$$$ ABR 2012-09-20: Delete safe pointer if needed
    if( controllers_ && item->data( Roles::SafeRole ).isValid() && item->data( Roles::SafeRole ).toBool() &&
        item->data( Roles::DataRole ).isValid() )
    {
        controllers_->Unregister( *const_cast< tools::Observer_ABC* >( static_cast< const tools::Observer_ABC* >( item->data( Roles::DataRole ).value< kernel::VariantPointer >().ptr_ ) ) );
        delete item->data( Roles::DataRole ).value< kernel::VariantPointer >().ptr_;
    }
    // $$$$ ABR 2012-09-20: Delete row
    QList< QStandardItem* > rowItems = parentItem->takeRow( item->row() );
    for( QList< QStandardItem* >::iterator it = rowItems.begin(); it != rowItems.end(); ++it )
        delete *it;
}

// -----------------------------------------------------------------------------
// Name: StandardModel::PurgeChildren
// Created: ABR 2012-09-20
// -----------------------------------------------------------------------------
inline
void StandardModel::PurgeChildren( QStandardItem& item )
{
    while( item.rowCount() )
    {
        QStandardItem* childItem = item.child( 0, 0 );
        assert( childItem );
        DeleteTree( childItem );
    }
}

// -----------------------------------------------------------------------------
// Name: StandardModel::MoveItem
// Created: ABR 2012-09-20
// -----------------------------------------------------------------------------
inline
void StandardModel::MoveItem( QStandardItem& item, QStandardItem& newParent )
{
    QStandardItem* parentItem = item.parent();
    if( parentItem )
        newParent.appendRow( parentItem->takeRow( item.row() ) );
}

// -----------------------------------------------------------------------------
// Name: StandardModel::GetDataFromItem
// Created: ABR 2012-08-17
// -----------------------------------------------------------------------------
template< typename T >
inline
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
// Name: StandardModel::GetItemFromIndex
// Created: ABR 2012-09-21
// -----------------------------------------------------------------------------
inline
QStandardItem* StandardModel::GetItemFromIndex( const QModelIndex& index ) const
{
    if( !index.isValid() )
        return 0;
    return itemFromIndex( index.model() == this ? index : proxy_.mapToSource( index ) );
}

// -----------------------------------------------------------------------------
// Name: StandardModel::GetDataFromIndex
// Created: ABR 2012-08-17
// -----------------------------------------------------------------------------
template< typename T >
inline
T* StandardModel::GetDataFromIndex( const QModelIndex& index ) const
{
    QStandardItem* item = GetItemFromIndex( index );
    if( !item )
        return 0;
    return GetDataFromItem< T >( *item );
}
