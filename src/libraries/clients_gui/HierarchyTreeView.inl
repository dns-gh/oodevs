// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: HierarchyTreeView constructor
// Created: ABR 2012-08-13
// -----------------------------------------------------------------------------
template< typename Hierarchy >
HierarchyTreeView< Hierarchy >::HierarchyTreeView( const QString& objectName, kernel::Controllers& controllers, const kernel::Profile_ABC& profile, ModelObserver_ABC& modelObserver, const EntitySymbols& symbols, QWidget* parent /*= 0*/ )
    : HierarchyTreeView_ABC( objectName, controllers, profile, modelObserver, symbols, parent )
    , controllers_( controllers )
{
    // Remove an item from the qtreeview, scrolls automatically to the top of the list
    // Disable auto scroll for avoid this problem
    setAutoScroll( false );

    controllers_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: HierarchyTreeView destructor
// Created: ABR 2012-08-13
// -----------------------------------------------------------------------------
template< typename Hierarchy >
HierarchyTreeView< Hierarchy >::~HierarchyTreeView()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: HierarchyTreeView::NotifyCreated
// Created: ABR 2012-08-17
// -----------------------------------------------------------------------------
template< typename Hierarchy >
inline
void HierarchyTreeView< Hierarchy >::NotifyCreated( const Hierarchy& hierarchy )
{
    InternalNotifyCreated( static_cast< const kernel::Hierarchies& >( hierarchy ) );
}

// -----------------------------------------------------------------------------
// Name: HierarchyTreeView::NotifyUpdated
// Created: ABR 2012-08-17
// -----------------------------------------------------------------------------
template< typename Hierarchy >
inline
void HierarchyTreeView< Hierarchy >::NotifyUpdated( const Hierarchy& hierarchy )
{
    InternalNotifyUpdated( static_cast< const kernel::Hierarchies& >( hierarchy ) );
}

// -----------------------------------------------------------------------------
// Name: HierarchyTreeView::NotifyDeleted
// Created: ABR 2012-08-17
// -----------------------------------------------------------------------------
template< typename Hierarchy >
inline
void HierarchyTreeView< Hierarchy >::NotifyDeleted( const Hierarchy& /*hierarchy*/ )
{
    dataModel_.PurgeObsoleteSafeItem< kernel::Entity_ABC >();
}
