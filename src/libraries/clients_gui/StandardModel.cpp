// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "StandardModel.h"
#include "moc_StandardModel.cpp"
#include "DragAndDropObserver_ABC.h"
#include "ItemDecorationGetter_ABC.h"
#include "StandardModelVisitor_ABC.h"

using namespace gui;

const QString StandardModel::showValue_ = "show";
const QString StandardModel::hideValue_ = "hide";

// -----------------------------------------------------------------------------
// Name: StandardModel constructor
// Created: ABR 2012-08-13
// -----------------------------------------------------------------------------
StandardModel::StandardModel( kernel::Controllers* controllers, QSortFilterProxyModel& proxy, QObject* parent /*= 0*/ )
    : QStandardItemModel( parent )
    , controllers_( controllers )
    , proxy_( proxy )
    , dragAndDropObserver_( 0 )
    , dndLocked_( false )
    , decorationGetter_( 0 )
{
    proxy.setSourceModel( this );
}

// -----------------------------------------------------------------------------
// Name: StandardModel destructor
// Created: ABR 2012-08-13
// -----------------------------------------------------------------------------
StandardModel::~StandardModel()
{
    Purge();
}

// -----------------------------------------------------------------------------
// Name: StandardModel::data
// Created: JSR 2012-09-14
// -----------------------------------------------------------------------------
QVariant StandardModel::data( const QModelIndex &index, int role /*= Qt::DisplayRole*/ ) const
{
    if( role == Qt::DecorationRole && decorationGetter_ )
        if( const QPixmap* p = decorationGetter_->GetDecoration( index ) )
            return QIcon( *p );
    return QStandardItemModel::data( index, role );
}

// -----------------------------------------------------------------------------
// Name: StandardModel::setData
// Created: JSR 2012-08-31
// -----------------------------------------------------------------------------
bool StandardModel::setData( const QModelIndex& index, const QVariant& value, int role /*= Qt::EditRole*/ )
{
    bool ret = QStandardItemModel::setData( index, value, role );
    if( ret && role == Qt::EditRole )
        emit DataChanged( index, value );
    return ret;
}

// -----------------------------------------------------------------------------
// Name: StandardModel::SetDecorationGetter
// Created: JSR 2012-09-14
// -----------------------------------------------------------------------------
void StandardModel::SetDecorationGetter( ItemDecorationGetter_ABC* decorationGetter )
{
    decorationGetter_ = decorationGetter;
}

// -----------------------------------------------------------------------------
// Name: StandardModel::SetDragAndDropObserver
// Created: JSR 2012-09-07
// -----------------------------------------------------------------------------
void StandardModel::SetDragAndDropObserver( DragAndDropObserver_ABC* dragAndDropObserver )
{
    dragAndDropObserver_ = dragAndDropObserver;
}

// -----------------------------------------------------------------------------
// Name: StandardModel::Accept
// Created: JSR 2012-09-14
// -----------------------------------------------------------------------------
void StandardModel::Accept( StandardModelVisitor_ABC& visitor, QStandardItem* root /*= 0*/ )
{
    if( root == 0)
        root = invisibleRootItem();
    for( int row = 0; row < root->rowCount(); ++row )
    {
        QStandardItem* childItem = root->child( row, 0 );
        if( childItem )
        {
            visitor.Visit( *childItem );
            Accept( visitor, childItem );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: StandardModel::Purge
// Created: ABR 2012-08-14
// -----------------------------------------------------------------------------
void StandardModel::Purge()
{
    if( invisibleRootItem() )
        PurgeChildren( *invisibleRootItem() );
}

namespace
{
    void SetRowVisible( const QStandardItem& root, int row, bool visible )
    {
        for( int col = 0; col < root.columnCount(); ++col )
        {
            QStandardItem* curItem = root.child( row, col );
            if( curItem )
                curItem->setData( StandardModel::GetShowValue( visible ), Roles::FilterRole );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: StandardModel::HasAnyChildVisible
// Created: JSR 2012-09-18
// -----------------------------------------------------------------------------
bool StandardModel::HasAnyChildVisible( QStandardItem& root, const T_Filter& filter, int col ) const
{
    bool isVisible = filter( root );
    for( int row = 0; row < root.rowCount(); ++row )
    {
        QStandardItem* childItem = root.child( row, col );
        assert( childItem );
        bool isChildVisible = HasAnyChildVisible( *childItem, filter, col );
        SetRowVisible( root, row, isChildVisible );
        isVisible |= isChildVisible;
    }
    return isVisible;
}

// -----------------------------------------------------------------------------
// Name: StandardModel::ApplyFilter
// Created: ABR 2012-08-17
// -----------------------------------------------------------------------------
void StandardModel::ApplyFilter( const T_Filter& filter, int col /* = 0 */ ) const
{
    for( int row = 0; row < rowCount(); ++row )
    {
        QStandardItem* childItem = item( row, col );
        assert( childItem );
        if( invisibleRootItem() )
            SetRowVisible( *invisibleRootItem(), row, HasAnyChildVisible( *childItem, filter, col ) );
    }
}

// -----------------------------------------------------------------------------
// Name: StandardModel::InternalApplyFilters
// Created: ABR 2014-04-28
// -----------------------------------------------------------------------------
void StandardModel::InternalApplyFilters( int row, int col, const std::vector< std::shared_ptr< Filter_ABC > >& filters, bool& result ) const
{
    QStandardItem* childItem = item( row, col );
    if( !childItem )
        return;
    for( auto filter = filters.begin(); filter != filters.end(); ++filter )
        result &= HasAnyChildVisible( *childItem, [&]( QStandardItem& item ){ return ( *filter )->Apply( item ); }, col );
}

// -----------------------------------------------------------------------------
// Name: StandardModel::HasAnyColumnVisible
// Created: ABR 2014-04-28
// -----------------------------------------------------------------------------
bool StandardModel::HasAnyColumnVisible( int row, const std::vector< std::shared_ptr< Filter_ABC > >& filters ) const
{
    bool result = false;
    for( int col = 0; col < columnCount(); ++col )
    {
        bool filtersResult = true;
        InternalApplyFilters( row, col, filters, filtersResult );
        result |= filtersResult;
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: StandardModel::ApplyFilters
// Created: ABR 2014-04-28
// -----------------------------------------------------------------------------
void StandardModel::ApplyFilters( const std::map< int, std::vector< std::shared_ptr< Filter_ABC > > >& filters ) const
{
    for( int row = 0; row < rowCount(); ++row )
    {
        bool result = true;
        for( auto it = filters.begin(); it != filters.end(); ++it )
            if( it->first == -1 )
                result &= HasAnyColumnVisible( row, it->second );
            else
                InternalApplyFilters( row, it->first, it->second, result );
        SetRowVisible( *invisibleRootItem(), row, result );
    }
}

// -----------------------------------------------------------------------------
// Name: StandardModel::GetMainModelIndex
// Created: JSR 2012-09-12
// -----------------------------------------------------------------------------
QModelIndex StandardModel::GetMainModelIndex( const QModelIndex& index ) const
{
    return index.model()->index( index.row(), 0, index.parent() );
}

// -----------------------------------------------------------------------------
// Name: StandardModel::LockDragAndDrop
// Created: JSR 2012-09-13
// -----------------------------------------------------------------------------
void StandardModel::LockDragAndDrop( bool lock )
{
    dndLocked_ = lock;
}

// -----------------------------------------------------------------------------
// Name: StandardModel::mimeTypes
// Created: JSR 2012-09-06
// -----------------------------------------------------------------------------
QStringList StandardModel::mimeTypes() const
{
    if( dragAndDropObserver_ && !dndLocked_ )
        return dragAndDropObserver_->MimeTypes();
    return QStringList();
}

// -----------------------------------------------------------------------------
// Name: StandardModel::mimeData
// Created: JSR 2012-09-06
// -----------------------------------------------------------------------------
QMimeData* StandardModel::mimeData( const QModelIndexList& indexes ) const
{
    if( !dragAndDropObserver_ || dndLocked_)
        return 0;
    bool overriden = false;
    QMimeData* mimeData = dragAndDropObserver_->MimeData( indexes, overriden );
    if( overriden )
        return mimeData;
    mimeData = new QMimeData();
    QByteArray encodedData;
    QDataStream stream( &encodedData, QIODevice::WriteOnly );
    QString mimeType;
    foreach( QModelIndex index, indexes )
    {
        if( index.isValid() )
        {
            QStandardItem* item = itemFromIndex( index.model() == this ? index : proxy_.mapToSource( index ) );
            if( item && item->data( Roles::MimeTypeRole ).isValid() )
            {
                const QString itemMimeType = item->data( Roles::MimeTypeRole ).value< QString >();
                if( mimeType.isNull() || mimeType == itemMimeType )
                {
                    mimeType = itemMimeType;
                    stream << reinterpret_cast< unsigned int >( item->data( Roles::DataRole ).value< kernel::VariantPointer >().ptr_ );
                }
            }
        }
    }
    mimeData->setData( mimeType, encodedData );
    return mimeData;
}

// -----------------------------------------------------------------------------
// Name: StandardModel::flags
// Created: JSR 2012-09-06
// -----------------------------------------------------------------------------
Qt::ItemFlags StandardModel::flags(const QModelIndex& index) const
{
    const Qt::ItemFlags defaultFlags = QStandardItemModel::flags( index );
    if( !index.isValid() || !dragAndDropObserver_ || dndLocked_)
        return defaultFlags & ~( Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled );
    return defaultFlags;
}

// -----------------------------------------------------------------------------
// Name: StandardModel::dropMimeData
// Created: JSR 2012-09-06
// -----------------------------------------------------------------------------
bool StandardModel::dropMimeData( const QMimeData* data, Qt::DropAction action, int /*row*/, int /*column*/, const QModelIndex& parent )
{
    if( action == Qt::IgnoreAction )
        return true;
    if( !dragAndDropObserver_ || !parent.isValid() || dndLocked_ )
        return false;
    QStandardItem* item = itemFromIndex( parent.model() == this ? parent : proxy_.mapToSource( parent ) );
    if( !item )
        return false;
    // TODO encapsuler tout ça dans une classe intermédiaire, ou bien faire des helpers
    QStringList additionalMimeTypes = dragAndDropObserver_->MimeTypes();
    QStringList formats = data->formats();
    foreach( QString format, formats )
    {
        QByteArray encodedData = data->data( format );
        QDataStream stream( &encodedData, QIODevice::ReadOnly );
        while( !stream.atEnd() )
        {
            unsigned int ptr = 0;
            stream >> ptr;
            if( ptr )
                dragAndDropObserver_->Drop( format, reinterpret_cast< void* >( ptr ), *item );
        }
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: StandardModel::supportedDropActions
// Created: JSR 2012-09-06
// -----------------------------------------------------------------------------
Qt::DropActions StandardModel::supportedDropActions() const
{
    return Qt::CopyAction | Qt::MoveAction;
}

// -----------------------------------------------------------------------------
// Name: StandardModel::AddRootItem
// Created: ABR 2012-08-16
// -----------------------------------------------------------------------------
QStandardItem* StandardModel::AddRootItem( int row, int col, Qt::ItemFlags flags /*= 0*/ )
{
    return AddChildItem( 0, row, col, flags );
}

// -----------------------------------------------------------------------------
// Name: StandardModel::AddChildItem
// Created: ABR 2012-08-16
// -----------------------------------------------------------------------------
QStandardItem* StandardModel::AddChildItem( QStandardItem* root, int row, int col, Qt::ItemFlags flags /*= 0*/ )
{
    if( !root )
        root = invisibleRootItem();
    QStandardItem* item = new QStandardItem();
    item->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable | flags );
    item->setData( QVariant( showValue_ ), Roles::FilterRole );
    item->setData( QVariant(), Roles::DataRole );
    item->setData( QVariant(), Roles::SafeRole );
    item->setData( QVariant(), Roles::MimeTypeRole );
    root->setChild( row, col, item );
    return item;
}

// -----------------------------------------------------------------------------
// Name: StandardModel::AddRootTextItem
// Created: ABR 2012-08-16
// -----------------------------------------------------------------------------
QStandardItem* StandardModel::AddRootTextItem( int row, int col, const QString& text, const QString& tooltip, Qt::ItemFlags flags /*= 0*/ )
{
    return AddChildTextItem( 0, row, col, text, tooltip, flags );
}

// -----------------------------------------------------------------------------
// Name: StandardModel::AddChildTextItem
// Created: ABR 2012-08-16
// -----------------------------------------------------------------------------
QStandardItem* StandardModel::AddChildTextItem( QStandardItem* root, int row, int col, const QString& text, const QString& tooltip,  Qt::ItemFlags flags /*= 0*/ )
{
    QStandardItem* item = AddChildItem( root, row, col, flags );
    item->setData( QVariant( text ), Qt::DisplayRole );
    item->setData( QVariant( tooltip ), Qt::ToolTipRole );
    return item;
}

// -----------------------------------------------------------------------------
// Name: StandardModel::AddRootIconItem
// Created: ABR 2012-08-16
// -----------------------------------------------------------------------------
QStandardItem* StandardModel::AddRootIconItem( int row, int col, const QPixmap& pixmap , Qt::ItemFlags flags /*= 0*/ )
{
    return AddChildIconItem( 0, row, col, pixmap, flags );
}

// -----------------------------------------------------------------------------
// Name: StandardModel::AddChildIconItem
// Created: ABR 2012-08-16
// -----------------------------------------------------------------------------
QStandardItem* StandardModel::AddChildIconItem( QStandardItem* root, int row, int col, const QPixmap& pixmap , Qt::ItemFlags flags /*= 0*/ )
{
    QStandardItem* item = AddChildItem( root, row, col, flags );
    item->setData( pixmap, Qt::DecorationRole );
    return item;
}

// -----------------------------------------------------------------------------
// Name: StandardModel::FindTextItem
// Created: ABR 2012-08-16
// -----------------------------------------------------------------------------
QStandardItem* StandardModel::FindTextItem( const QString& text, QStandardItem* root /*= 0*/ ) const
{
    return RecFindTextItem( root ? root : invisibleRootItem(), text );
}

// -----------------------------------------------------------------------------
// Name: StandardModel::DeleteTree
// Created: ABR 2012-09-20
// -----------------------------------------------------------------------------
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
        item->setData( QVariant(), Roles::DataRole );
    }
    // $$$$ ABR 2012-09-20: Delete row
    QList< QStandardItem* > rowItems = parentItem->takeRow( item->row() );
    qDeleteAll( rowItems );
}

// -----------------------------------------------------------------------------
// Name: StandardModel::PurgeChildren
// Created: ABR 2012-09-20
// -----------------------------------------------------------------------------
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
void StandardModel::MoveItem( QStandardItem& item, QStandardItem& newParent )
{
    QStandardItem* parentItem = item.parent();
    if( parentItem )
        newParent.appendRow( parentItem->takeRow( item.row() ) );
}

// -----------------------------------------------------------------------------
// Name: StandardModel::GetItemFromIndex
// Created: ABR 2012-09-21
// -----------------------------------------------------------------------------
QStandardItem* StandardModel::GetItemFromIndex( const QModelIndex& index ) const
{
    if( !index.isValid() )
        return 0;
    return itemFromIndex( index.model() == this ? index : proxy_.mapToSource( index ) );
}
