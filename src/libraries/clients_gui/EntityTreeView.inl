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
EntityTreeView< Entity >::EntityTreeView( kernel::Controllers& controllers, const kernel::Profile_ABC& profile, QWidget* parent /*= 0*/ )
    : EntityTreeView_ABC( controllers, profile, parent )
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
inline
void EntityTreeView< Entity >::NotifyCreated( const Entity& entity )
{
    // $$$$ ABR 2012-08-14: TODO

    const kernel::Entity_ABC& team = entity.Get< kernel::TacticalHierarchies >().GetTop();

    QStandardItem* teamItem = dataModel_.FindSafeItem( team );
    if( !teamItem )
    {
        teamItem = dataModel_.AddRootSafeItem( dataModel_.rowCount(), 0, team.GetName(), team );
        //teamItem->SetNamed( team );
    }

    QStandardItem* entityItem = dataModel_.AddChildSafeItem( teamItem, teamItem->rowCount(), 0, entity.GetName(), entity );
    //popItem->SetNamed( static_cast< const Entity_ABC& >( inhabitant ) );
    //popItem->setDragEnabled( true );
    //popItem->SetToolTip( QString( "%1 [%L2]" ).arg( inhabitant.GetName() ).arg( inhabitant.GetId() ) );
}

// -----------------------------------------------------------------------------
// Name: EntityTreeView::NotifyDeleted
// Created: ABR 2012-08-14
// -----------------------------------------------------------------------------
template< typename Entity >
inline
void EntityTreeView< Entity >::NotifyDeleted( const Entity& /* entity */ )
{
    dataModel_.PurgeObsoleteSafeItem< Entity >();
}

// -----------------------------------------------------------------------------
// Name: EntityTreeView::IsTypeRejected
// Created: ABR 2012-08-14
// -----------------------------------------------------------------------------
template< typename Entity >
inline
bool EntityTreeView< Entity >::IsTypeRejected( const kernel::Entity_ABC& entity ) const
{
    return entity.GetTypeName() != Entity::typeName_;
}
