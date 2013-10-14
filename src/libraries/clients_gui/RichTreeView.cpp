// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "RichTreeView.h"
#include "moc_RichTreeView.cpp"
#include "ObjectNameManager.h"
#include "StandardModelVisitor_ABC.h"
#include "clients_kernel/Entity_ABC.h"

using namespace gui;

namespace
{
    class CustomSortFilterProxyModel : public QSortFilterProxyModel
    {
    public:
        CustomSortFilterProxyModel( RichTreeView& parent )
            : QSortFilterProxyModel( &parent )
            , parent_( parent )
        {
            setSortLocaleAware( true );
        }

    protected:
        virtual bool lessThan( const QModelIndex& left, const QModelIndex& right ) const
        {
            bool valid = false;
            bool ret = parent_.LessThan( left, right, valid );
            if( valid )
                return ret;
            return QSortFilterProxyModel::lessThan( left, right );
        }

    public:
        const RichTreeView& parent_;
    };

    class HeightDelegate : public QItemDelegate
    {
    public:
        explicit HeightDelegate( QObject* parent ) : QItemDelegate( parent ) {}
        virtual QSize sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const
        {
            QSize s = QItemDelegate::sizeHint( option, index );
            if( s.height() < 20 )
                s.setHeight( 20 );
            return s;
        }
    };
}

// -----------------------------------------------------------------------------
// Name: RichTreeView constructor
// Created: ABR 2012-08-13
// -----------------------------------------------------------------------------
RichTreeView::RichTreeView( const QString& objectName, QWidget* parent /*= 0*/, kernel::Controllers* controllers /*= 0*/ )
    : QTreeView( parent )
    , proxyModel_( new CustomSortFilterProxyModel( *this ) )
    , dataModel_( controllers, *proxyModel_, this )
    , dropAction_( Qt::MoveAction )
{
    ObjectNameManager::getInstance()->SetObjectName( this, objectName );
    setSortingEnabled( true );
    setDropIndicatorShown( false );
    setItemDelegate( new HeightDelegate( this ) );

    setModel( proxyModel_ );
    proxyModel_->setFilterRole( Roles::FilterRole );
    proxyModel_->setFilterRegExp( StandardModel::showValue_ );
    proxyModel_->setDynamicSortFilter( true );
    proxyModel_->sort( 0 );

    QPalette p = palette();
    p.setColor( QPalette::Inactive, QPalette::Highlight, Qt::lightGray );
    setPalette( p );
}

// -----------------------------------------------------------------------------
// Name: RichTreeView destructor
// Created: ABR 2012-08-13
// -----------------------------------------------------------------------------
RichTreeView::~RichTreeView()
{
    ObjectNameManager::getInstance()->RemoveRegisteredName( objectName() );
}

// -----------------------------------------------------------------------------
// Name: RichTreeView::Purge
// Created: ABR 2012-08-14
// -----------------------------------------------------------------------------
void RichTreeView::Purge()
{
    dataModel_.Purge();
}

// -----------------------------------------------------------------------------
// Name: RichTreeView::EnableDragAndDrop
// Created: JSR 2012-09-14
// -----------------------------------------------------------------------------
void RichTreeView::EnableDragAndDrop( bool enable )
{
    dataModel_.SetDragAndDropObserver( enable ? this : 0 );
    viewport()->setAcceptDrops( enable );
    setDragEnabled( enable );
    setAcceptDrops( enable );
    setAutoScroll( enable );
    setAutoExpandDelay( 500 );
}

// -----------------------------------------------------------------------------
// Name: RichTreeView::SetCreationBlocked
// Created: ABR 2012-08-14
// -----------------------------------------------------------------------------
void RichTreeView::SetCreationBlocked( bool creationBlocked )
{
    creationBlocked_ = creationBlocked;
}

// -----------------------------------------------------------------------------
// Name: RichTreeView::IsCreationBlocked
// Created: ABR 2012-08-14
// -----------------------------------------------------------------------------
bool RichTreeView::IsCreationBlocked() const
{
    return creationBlocked_;
}

// -----------------------------------------------------------------------------
// Name: RichTreeView::SetContextMenuBlocked
// Created: ABR 2012-08-14
// -----------------------------------------------------------------------------
void RichTreeView::SetContextMenuBlocked( bool contextMenuBlocked )
{
    contextMenuBlocked_ = contextMenuBlocked;
}

// -----------------------------------------------------------------------------
// Name: RichTreeView::IsContextMenuBlocked
// Created: ABR 2012-08-14
// -----------------------------------------------------------------------------
bool RichTreeView::IsContextMenuBlocked() const
{
    return contextMenuBlocked_;
}

// -----------------------------------------------------------------------------
// Name: RichTreeView::CreateFilters
// Created: ABR 2012-08-14
// -----------------------------------------------------------------------------
void RichTreeView::CreateFilters( SearchTreeView_ABC& /*searchTreeView*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RichTreeView::LessThan
// Created: JSR 2012-09-06
// -----------------------------------------------------------------------------
bool RichTreeView::LessThan( const QModelIndex& /*left*/, const QModelIndex& /*right*/, bool& /*valid*/ ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: RichTreeView::ApplyFilter
// Created: JSR 2012-09-18
// -----------------------------------------------------------------------------
void RichTreeView::ApplyFilter( StandardModel::T_FilterFunction func )
{
    dataModel_.ApplyFilter( func );
}

// -----------------------------------------------------------------------------
// Name: RichTreeView::LockDragAndDrop
// Created: JSR 2012-09-10
// -----------------------------------------------------------------------------
void RichTreeView::LockDragAndDrop( bool lock )
{
    dataModel_.LockDragAndDrop( lock );
}

// -----------------------------------------------------------------------------
// Name: RichTreeView::SearchAndSelect
// Created: ABR 2012-08-14
// -----------------------------------------------------------------------------
void RichTreeView::SearchAndSelect( const QString& searchedText, Qt::MatchFlags compareFlag /* = Qt::MatchContains */, Qt::ItemDataRole searchRole /* = Qt::ToolTipRole */ )
{
    // $$$$ ABR 2012-08-14: TODO, may be that should be on standard model
    searchedText_ = searchedText;
    if( searchedText_.isEmpty() )
        return;
    QModelIndexList indexes = selectedIndexes();
    if( !indexes.empty() )
    {
        const QModelIndex& selectedIndex = indexes.front();
        if( selectedIndex.isValid() )
        {
            QVariant v( searchedText_ );
            QModelIndexList list = proxyModel_->match( selectedIndex, searchRole, v, 1, compareFlag );
            if( list.size() > 0 && list.front() == selectedIndex )
                return;
        }
    }
    SearchAndSelectNext( compareFlag, searchRole );
}

namespace
{
    QString GetId( const QModelIndex& index )
    {
        QStringList ids;
        QModelIndex p = index;
        while( p.isValid() )
        {
            ids.prepend( QString::number( p.row() ) );
            p = p.parent();
        }
        ids.prepend( "0" );
        return ids.join( ":" );
    }

    int CompareIndices( const QModelIndex& l, const QModelIndex& r )
    {
        QString lId = GetId( l );
        QString rId = GetId( r );
        QStringList lIds  = lId.split( ":", QString::SkipEmptyParts );
        QStringList rIds = rId.split( ":", QString::SkipEmptyParts );
        for( int i = 0; i < lIds.count() && i < rIds.count(); ++i )
        {
            int left = lIds[ i ].toInt();
            int right = rIds[ i ].toInt();
            if ( left != right )
                return left - right;
        }
        return lIds.count() < rIds.count() ? -1 : ( lIds.count() == rIds.count() ? 0 : 1 );
    }
}

// -----------------------------------------------------------------------------
// Name: RichTreeView::SearchAndSelectNext
// Created: ABR 2012-08-14
// -----------------------------------------------------------------------------
void RichTreeView::SearchAndSelectNext( Qt::MatchFlags compareFlag /* = Qt::MatchContains */, Qt::ItemDataRole searchRole /* = Qt::ToolTipRole */ )
{
    // $$$$ ABR 2012-08-14: TODO, may be that should be on standard model
    if( searchedText_.isEmpty() || proxyModel_->rowCount() == 0 )
        return;
    QVariant v( searchedText_ );
    QModelIndexList list = proxyModel_->match( proxyModel_->index( 0, 0 ), searchRole, v, -1, compareFlag | Qt::MatchWrap | Qt::MatchRecursive );
    int size = list.size();
    QModelIndexList selecteds = selectedIndexes();
    if( !selecteds.empty() )
    {
        const QModelIndex& selected = selecteds.front();
        if( selected.isValid() )
            for( int i = 0; i < size; ++i )
            {
                const QModelIndex& modelIndex = list.at( i );
                if( proxyModel_->flags( modelIndex ) & ( Qt::ItemIsSelectable | Qt::ItemIsEnabled ) && CompareIndices( modelIndex, selected ) > 0 )
                {
                    selectionModel()->select( modelIndex, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows );
                    scrollTo( modelIndex );
                    return;
                }
            }
    }
    for( int i = 0; i < size; ++i )
    {
        const QModelIndex& modelIndex = list.at( i );
        if( proxyModel_->flags( modelIndex ) & ( Qt::ItemIsSelectable | Qt::ItemIsEnabled ) )
        {
            selectionModel()->select( modelIndex, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows );
            scrollTo( modelIndex );
            return;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: RichTreeView::startDrag
// Created: ABR 2012-11-22
// -----------------------------------------------------------------------------
void RichTreeView::startDrag( Qt::DropActions supportedActions )
{
    QModelIndexList indexes = selectedIndexes();
    if( indexes.empty() )
        return;

    QModelIndexList dragableObjects;
    for( int i = 0; i < indexes.size(); ++i )
    {
        QStandardItem* item = dataModel_.GetItemFromIndex( indexes[ i ] );
        if( item && item->flags() & Qt::ItemIsDragEnabled )
            dragableObjects.push_back( indexes[ i ] );
    }

    QMimeData* data = dataModel_.mimeData( dragableObjects );
    if( !data )
        return;

    QDrag* drag = new QDrag( this );
    drag->setMimeData( data );
    if( supportedActions & dropAction_ )
        drag->exec( dropAction_ );
}
