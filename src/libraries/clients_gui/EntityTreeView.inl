// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: EntityTreeView constructor
// Created: ABR 2012-08-13
// -----------------------------------------------------------------------------
template< typename Entity >
EntityTreeView< Entity >::EntityTreeView( const QString& objectName,
                                          kernel::Controllers& controllers,
                                          const kernel::Profile_ABC& profile,
                                          ModelObserver_ABC& modelObserver,
                                          bool editable,
                                          QWidget* parent /*= 0*/ )
    : EntityTreeView_ABC( objectName, controllers, profile, modelObserver, parent )
    , editable_( editable )
{
    dataModel_.setColumnCount( 1 );
    controllers_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: EntityTreeView destructor
// Created: ABR 2012-08-13
// -----------------------------------------------------------------------------
template< typename Entity >
EntityTreeView< Entity >::~EntityTreeView()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: EntityTreeView::NotifyCreated
// Created: ABR 2012-08-14
// -----------------------------------------------------------------------------
template< typename Entity >
void EntityTreeView< Entity >::NotifyCreated( const Entity& entity )
{
    // $$$$ ABR 2012-08-14: TODO

    const kernel::Entity_ABC& team = entity.Get< kernel::TacticalHierarchies >().GetTop();

    QStandardItem* teamItem = dataModel_.FindDataItem( team );
    if( !teamItem )
    {
        const int row = dataModel_.rowCount();
        teamItem = dataModel_.AddRootSafeItem( row, 0, team.GetName(), team.GetTooltip(), team, ItemSpecificFlags( team ) );
        for( int col = 1; col < dataModel_.columnCount(); ++col )
            dataModel_.AddRootItem( row, col );
    }

    const int row = teamItem->rowCount();
    QStandardItem* entityItem = dataModel_.AddChildSafeItem( teamItem, row, 0, entity.GetName(), entity.GetTooltip(), entity, ItemSpecificFlags( entity ) );
    for( int col = 1; col < dataModel_.columnCount(); ++col )
        dataModel_.AddChildItem( teamItem, row, col );
    if( entityItem )
        ApplyProfileFilter();
}

// -----------------------------------------------------------------------------
// Name: EntityTreeView::NotifyDeleted
// Created: ABR 2012-08-14
// -----------------------------------------------------------------------------
template< typename Entity >
void EntityTreeView< Entity >::NotifyDeleted( const Entity& /* entity */ )
{
    dataModel_.PurgeObsoleteSafeItem< Entity >();
}

// -----------------------------------------------------------------------------
// Name: EntityTreeView::IsTypeRejected
// Created: ABR 2012-08-14
// -----------------------------------------------------------------------------
template< typename Entity >
bool EntityTreeView< Entity >::IsTypeRejected( const kernel::Entity_ABC& entity ) const
{
    return entity.GetTypeName() != Entity::typeName_;
}

// -----------------------------------------------------------------------------
// Name: EntityTreeView::ItemSpecificFlags
// Created: ABR 2014-09-09
// -----------------------------------------------------------------------------
template< typename Entity >
Qt::ItemFlags EntityTreeView< Entity >::ItemSpecificFlags( const kernel::Entity_ABC& entity ) const
{
    const Qt::ItemFlags parentFlags = EntityTreeView_ABC::ItemSpecificFlags( entity );
    return editable_ && !IsTypeRejected( entity ) ? Qt::ItemIsEditable | parentFlags: parentFlags;
}
