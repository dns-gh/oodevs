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
const QString StandardModel::mimeTypeStr_ = "sword/varianttype";

// -----------------------------------------------------------------------------
// Name: StandardModel constructor
// Created: ABR 2012-08-13
// -----------------------------------------------------------------------------
StandardModel::StandardModel( kernel::Controllers& controllers, QSortFilterProxyModel& proxy, QObject* parent /*= 0*/ )
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
            return *p;
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
    // $$$$ ABR 2012-08-14: TODO
}

namespace
{
    bool HasAnyChildVisible( QStandardItem* root, boost::function< bool ( QStandardItem* ) > func )
    {
        // $$$$ ABR 2012-08-16: TODO: Not tested yet, need to test it on gaming
        if( !root )
            return false;

        bool isVisible = func( root );
        for( int row = 0; row < root->rowCount(); ++row )
        {
            QStandardItem* childItem = root->child( row, 0 );
            if( childItem )
            {
                bool childVisible = HasAnyChildVisible( childItem, func );
                childItem->setData( childVisible ? StandardModel::showValue_ : StandardModel::hideValue_, StandardModel::FilterRole );
                isVisible = isVisible || childVisible;
            }
        }
        return isVisible;
    }
}

// -----------------------------------------------------------------------------
// Name: StandardModel::function< bool
// Created: ABR 2012-08-17
// -----------------------------------------------------------------------------
void StandardModel::ApplyFilter( boost::function< bool ( QStandardItem* ) > func )
{
    for( int row = 0; row < rowCount(); ++row )
    {
        QStandardItem* childItem = item( row, 0 );
        if( childItem )
            childItem->setData( ( ::HasAnyChildVisible( childItem, func ) ) ? StandardModel::showValue_ : StandardModel::hideValue_, StandardModel::FilterRole );
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
     // $$$$ JSR 2012-09-07: TODO g�rer diff�rents mimeTypes? (entity, agenttype, automattype)
    QStringList mimeTypes;
    if( dragAndDropObserver_ && !dndLocked_ )
        mimeTypes << mimeTypeStr_ << dragAndDropObserver_->AdditionalMimeTypes();
    return mimeTypes;
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
    foreach( QModelIndex index, indexes )
    {
        if( index.isValid() )
        {
            QStandardItem* item = itemFromIndex( index.model() == this ? index : proxy_.mapToSource( index ) );
            if( item)
                stream << reinterpret_cast< unsigned int >( item->data( DataRole ).value< kernel::VariantPointer >().ptr_ );
        }
    }
    mimeData->setData( mimeTypeStr_, encodedData );
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
    // TODO encapsuler tout �a dans une classe interm�diaire, ou bien faire des helpers
    QStringList additionalMimeTypes = dragAndDropObserver_->AdditionalMimeTypes();
    QStringList formats = data->formats();
    foreach( QString format, formats )
    {
        if( format == mimeTypeStr_ )
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
        else if( additionalMimeTypes.contains( format ) )
        {
            // old dnd version, to be removed when porting to QT4 is finished
            QByteArray encodedData = data->data( format );
            dragAndDropObserver_->Drop( format, reinterpret_cast< void* >( encodedData.data() ), *item );
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
    return Qt::CopyAction;
}
