// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "EntityTreeView_ABC.h"
#include "ModelObserver_ABC.h"
#include "moc_EntityTreeView_ABC.cpp"
#include "LongNameHelper.h"
#include "StandardModelVisitor_ABC.h"
#include "clients_kernel/ActionController.h"
#include "clients_kernel/AgentKnowledge_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include <boost/bind.hpp>

using namespace gui;

// -----------------------------------------------------------------------------
// Name: EntityTreeView_ABC constructor
// Created: ABR 2012-08-16
// -----------------------------------------------------------------------------
EntityTreeView_ABC::EntityTreeView_ABC( const QString& objectName, kernel::Controllers& controllers, const kernel::Profile_ABC& profile, ModelObserver_ABC& modelObserver, QWidget* parent /*= 0*/ )
    : RichTreeView( objectName, parent, &controllers )
    , controllers_( controllers )
    , profile_( profile )
    , modelObserver_( modelObserver )
    , blockSelect_( false )
{
    dataModel_.setColumnCount( 1 );
    setHeaderHidden( true );
    setEditTriggers( SelectedClicked | EditKeyPressed );
    setAllColumnsShowFocus( true );
    setUniformRowHeights( true );
    header()->setStretchLastSection( false );
    header()->setResizeMode( 0, QHeaderView::ResizeToContents );

    connect( this,             SIGNAL( activated       ( const QModelIndex& ) ),                           this, SLOT( OnActivate( const QModelIndex& ) ) );
    connect( selectionModel(), SIGNAL( selectionChanged( const QItemSelection&, const QItemSelection& ) ), this, SLOT( OnSelect  ( const QItemSelection&, const QItemSelection& ) ) );
    connect( &dataModel_, SIGNAL( DataChanged( const QModelIndex&, const QVariant& ) ), SLOT( OnDataChanged ( const QModelIndex&, const QVariant& ) ) );
    connect( horizontalScrollBar(), SIGNAL( valueChanged( int ) ), this, SLOT( ForceRedraw() ) );

    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: EntityTreeView_ABC destructor
// Created: ABR 2012-08-16
// -----------------------------------------------------------------------------
EntityTreeView_ABC::~EntityTreeView_ABC()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: EntityTreeView_ABC::NotifyActivated
// Created: ABR 2012-08-16
// -----------------------------------------------------------------------------
void EntityTreeView_ABC::NotifyActivated( const kernel::Entity_ABC& entity )
{
    if( IsTypeRejected( entity ) )
        return;
    QStandardItem* item = dataModel_.FindDataItem( entity );
    if( item && item->index().isValid() )
        scrollTo( proxyModel_->mapFromSource( item->index() ) );
}

// -----------------------------------------------------------------------------
// Name: EntityTreeView_ABC::OnActivate
// Created: ABR 2012-08-16
// -----------------------------------------------------------------------------
void EntityTreeView_ABC::OnActivate( const QModelIndex& index )
{
    const kernel::Entity_ABC* entity = dataModel_.GetDataFromIndex< kernel::Entity_ABC >( index );
    if( entity )
        entity->Activate( controllers_.actions_ );
}

// -----------------------------------------------------------------------------
// Name: EntityTreeView_ABC::NotifyCreated
// Created: ABR 2012-08-16
// -----------------------------------------------------------------------------
void EntityTreeView_ABC::NotifyCreated( const kernel::Team_ABC& team )
{
    const kernel::Entity_ABC& entityTeam = static_cast< const kernel::Entity_ABC& >( team );
    QStandardItem* teamItem = dataModel_.FindDataItem( entityTeam );
    if( !teamItem )
        teamItem = dataModel_.AddRootSafeItem( dataModel_.rowCount(), 0, team.GetName(), team.GetTooltip(), entityTeam, ItemSpecificFlags( team ) );
    ApplyProfileFilter();
}

// -----------------------------------------------------------------------------
// Name: EntityTreeView_ABC::NotifyDeleted
// Created: ABR 2012-08-16
// -----------------------------------------------------------------------------
void EntityTreeView_ABC::NotifyDeleted( const kernel::Entity_ABC& /* team */ )
{
    QAbstractItemView::SelectionMode mode = selectionMode();
    setSelectionMode( QAbstractItemView::ExtendedSelection );
    dataModel_.PurgeObsoleteSafeItem< kernel::Entity_ABC >();
    setSelectionMode( mode );
}

// -----------------------------------------------------------------------------
// Name: EntityTreeView_ABC::NotifyUpdated
// Created: JSR 2012-08-31
// -----------------------------------------------------------------------------
void EntityTreeView_ABC::NotifyUpdated( const kernel::Entity_ABC& entity )
{
    if( dynamic_cast< const kernel::AgentKnowledge_ABC* >( &entity ) )
        // SWBUG-12850: spamming with agent knowledge updates massively slows
        // down gaming because the FindDataItem() below and possibly the
        // following invalidate() are super slow.
        //
        // Ignoring agent knowledge name changes here is ugly but better than
        // freezing gaming.
        return;

    QStandardItem* item = dataModel_.FindDataItem( entity );
    if( item )
    {
        item->setData( QVariant( entity.GetName() ), Qt::DisplayRole );
        proxyModel_->invalidate();
    }
}

// -----------------------------------------------------------------------------
// Name: EntityTreeView_ABC::ApplyProfileFilter
// Created: ABR 2012-08-17
// -----------------------------------------------------------------------------
bool EntityTreeView_ABC::ApplyProfileFilter( QStandardItem& item ) const
{
    if( item.data( Roles::SafeRole ).isValid() && item.data( Roles::SafeRole ).toBool() )
    {
        const kernel::Entity_ABC* entity = dataModel_.GetDataFromItem< kernel::Entity_ABC >( item );
        if( entity )
            return profile_.IsVisible( *entity );
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: EntityTreeView_ABC::NotifyUpdated
// Created: ABR 2012-08-16
// -----------------------------------------------------------------------------
void EntityTreeView_ABC::NotifyUpdated( const kernel::Profile_ABC& /*profile*/ )
{
    ApplyProfileFilter();
}

namespace
{
    class SelectedVisitor : public StandardModelVisitor_ABC
                          , private boost::noncopyable
    {
    public:
        SelectedVisitor( const kernel::ActionController& actions, const StandardModel& model )
            : actions_( actions )
            , model_( model)
        {
            //NOTHING
        }

        virtual void Visit( QStandardItem& item )
        {
            if( item.data( Roles::SafeRole ).isValid() && item.data( Roles::SafeRole ).toBool() )
            {
                const kernel::Entity_ABC* entity = model_.GetDataFromItem< kernel::Entity_ABC >( item );
                if( entity && actions_.IsSelected( entity ) )
                    selected_.push_back( entity );
            }
        }

        const std::vector< const kernel::Entity_ABC* >& GetSelected() const
        {
            return selected_;
        }

    private:
        const kernel::ActionController& actions_;
        const StandardModel& model_;
        std::vector< const kernel::Entity_ABC* > selected_;
    };
}

// -----------------------------------------------------------------------------
// Name: EntityTreeView_ABC::ApplyProfileFilter
// Created: ABR 2012-08-16
// -----------------------------------------------------------------------------
void EntityTreeView_ABC::ApplyProfileFilter()
{
    dataModel_.ApplyFilter( boost::bind( &EntityTreeView_ABC::ApplyProfileFilter, this, _1 ) );

    SelectedVisitor visitor( controllers_.actions_, dataModel_ );
    dataModel_.Accept( visitor );
    NotifySelectionChanged( visitor.GetSelected() );
}

// -----------------------------------------------------------------------------
// Name: EntityTreeView_ABC::NotifySelectionChanged
// Created: ABR 2012-08-16
// -----------------------------------------------------------------------------
void EntityTreeView_ABC::NotifySelectionChanged( const std::vector< const kernel::Entity_ABC* >& elements )
{
    if( selectionMode() == QAbstractItemView::NoSelection )
        return;

    blockSelect_ = true;
    selectionModel()->clearSelection();
    for( auto it = elements.begin(); it != elements.end(); ++it )
    {
        if( *it && !IsTypeRejected( **it ) )
            if( QStandardItem* item = dataModel_.FindDataItem( **it ) )
                if( item->data( gui::Roles::FilterRole ).toString() == gui::StandardModel::showValue_ )
                {
                    QModelIndex index = proxyModel_->mapFromSource( dataModel_.indexFromItem( item ) );
                    if( index.isValid() )
                    {
                        selectionModel()->select( index, QItemSelectionModel::Select | QItemSelectionModel::Rows );
                        QTimer::singleShot( 0, this, SLOT( OnScrollToSelected() ) );
                    }
                }
    }
    blockSelect_ = false;
}

// -----------------------------------------------------------------------------
// Name: EntityTreeView_ABC::OnSelect
// Created: ABR 2012-08-16
// -----------------------------------------------------------------------------
void EntityTreeView_ABC::OnSelect( const QItemSelection& /*selected*/, const QItemSelection& /* deselected */ )
{
    if( blockSelect_ )
        return;
    if( selectionMode() == QAbstractItemView::NoSelection )
        return;

    QModelIndexList indexes = selectedIndexes();
    kernel::GraphicalEntity_ABC::T_GraphicalEntities list;
    std::vector< const kernel::Entity_ABC* > entities;
    for( QModelIndexList::const_iterator it = indexes.constBegin(); it != indexes.constEnd(); ++it )
        if( auto entity = dataModel_.GetDataFromIndex< kernel::Entity_ABC >( *it ) )
        {
            entities.push_back( entity );
            list.push_back( entity );
        }
    emit SelectionChanged( entities );
    controllers_.actions_.SetMultipleSelection( list );
}

// -----------------------------------------------------------------------------
// Name: EntityTreeView_ABC::OnDataChanged
// Created: JSR 2012-08-31
// -----------------------------------------------------------------------------
void EntityTreeView_ABC::OnDataChanged( const QModelIndex& index, const QVariant& value )
{
    if( value.type() == QVariant::String )
        if( auto* entity = dataModel_.GetDataFromIndex< kernel::Entity_ABC >( index ) )
            entity->Rename( value.toString() );
}

// -----------------------------------------------------------------------------
// Name: EntityTreeView_ABC::Rename
// Created: LGY 2014-05-2013
// -----------------------------------------------------------------------------
void EntityTreeView_ABC::Rename( kernel::Entity_ABC& entity )
{
    if( !isVisible() || !entity.CanBeRenamed() )
        return;
    QStandardItem* item = dataModel_.FindDataItem( entity );
    if( item )
    {
        const auto index = proxyModel_->mapFromSource( item->index() );
        expand( index.parent() );
        edit( index );
    }
}

// -----------------------------------------------------------------------------
// Name: EntityTreeView_ABC::contextMenuEvent
// Created: ABR 2012-08-16
// -----------------------------------------------------------------------------
void EntityTreeView_ABC::contextMenuEvent( QContextMenuEvent* event )
{
    if( IsReadOnly() || !event )
        return;
    QModelIndex index = indexAt( event->pos() );
    if( proxyModel_->flags( index ) & Qt::ItemIsEditable )
        if( const auto* entity = dataModel_.GetDataFromIndex< kernel::Entity_ABC >( dataModel_.GetMainModelIndex( index ) ) )
        {
            entity->ContextMenu( controllers_.actions_, event->globalPos() );
            return;
        }
    ContextMenuRequested( event->globalPos() );
}

// -----------------------------------------------------------------------------
// Name: EntityTreeView_ABC::keyPressEvent
// Created: LGY 2014-05-27
// -----------------------------------------------------------------------------
void EntityTreeView_ABC::keyPressEvent( QKeyEvent* event )
{
    if( event && event->key() == Qt::Key_F2 )
    {
        const auto indexes = selectionModel()->selectedIndexes();
        if( !indexes.empty() )
            edit( indexes.front() );
    }
    else
        RichTreeView::keyPressEvent( event );
}

// -----------------------------------------------------------------------------
// Name: EntityTreeView_ABC::IsTypeRejected
// Created: ABR 2012-08-16
// -----------------------------------------------------------------------------
bool EntityTreeView_ABC::IsTypeRejected( const kernel::Entity_ABC& /*entity*/ ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: EntityTreeView_ABC::ItemSpecificFlags
// Created: JSR 2012-08-31
// -----------------------------------------------------------------------------
Qt::ItemFlags EntityTreeView_ABC::ItemSpecificFlags( const kernel::Entity_ABC& /*entity*/ ) const
{
    return 0;
}

// -----------------------------------------------------------------------------
// Name: EntityTreeView_ABC::ForceRedraw
// Created: MMC 2013-01-30
// -----------------------------------------------------------------------------
void EntityTreeView_ABC::ForceRedraw()
{
    doItemsLayout();
}

// -----------------------------------------------------------------------------
// Name: EntityTreeView_ABC::OnScrollToSelected
// Created: NPT 2013-04-11
// -----------------------------------------------------------------------------
void EntityTreeView_ABC::OnScrollToSelected()
{
    if( !selectedIndexes().empty() )
    {
        QModelIndexList indexList = selectedIndexes();
        QModelIndex index = indexList[ 0 ];
        blockSignals( true );
        scrollTo( index );
        blockSignals( false );
    }
}

namespace
{
    bool ModelIndexToEntityWrapper( const QModelIndex& left,
                                    const QModelIndex& right,
                                    bool& valid,
                                    const StandardModel& model,
                                    EntityTreeView_ABC::T_LessThanEntityFunctor functor )
    {
        const kernel::Entity_ABC* entity1 = model.GetDataFromIndex< kernel::Entity_ABC >( left );
        const kernel::Entity_ABC* entity2 = model.GetDataFromIndex< kernel::Entity_ABC >( right );
        if( !entity1 || !entity2 )
            return false;
        valid = true;
        return functor( *entity1, *entity2 );
    }
}

// -----------------------------------------------------------------------------
// Name: EntityTreeView_ABC::SetLessThanEntityFunctor
// Created: ABR 2013-10-15
// -----------------------------------------------------------------------------
void EntityTreeView_ABC::SetLessThanEntityFunctor( const T_LessThanEntityFunctor& functor )
{
    SetLessThanFunctor( boost::bind( &ModelIndexToEntityWrapper, _1, _2, _3, boost::cref( dataModel_ ), functor ) );
}

// -----------------------------------------------------------------------------
// Name: EntityTreeView_ABC::Exist
// Created: LGY 2014-05-22
// -----------------------------------------------------------------------------
bool EntityTreeView_ABC::Exist( const kernel::Entity_ABC& entity )
{
    return dataModel_.FindDataItem( entity ) ? true : false;
}

// -----------------------------------------------------------------------------
// Name: EntityTreeView_ABC::IsActivated
// Created: ABR 2014-09-03
// -----------------------------------------------------------------------------
bool EntityTreeView_ABC::IsActivated() const
{
    return true;
}
