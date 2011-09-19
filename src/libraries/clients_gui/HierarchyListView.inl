// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

namespace gui
{

// -----------------------------------------------------------------------------
// Name: HierarchyListView constructor
// Created: AGE 2006-10-04
// -----------------------------------------------------------------------------
template< typename H >
HierarchyListView< H >::HierarchyListView( QWidget* pParent, kernel::Controllers& controllers, ItemFactory_ABC& factory, const kernel::Profile_ABC& profile, const gui::EntitySymbols& icons )
    : HierarchyListView_ABC( pParent, controllers, factory, profile, icons )
    , controllers_( controllers )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: HierarchyListView destructor
// Created: AGE 2006-10-04
// -----------------------------------------------------------------------------
template< typename H >
HierarchyListView< H >::~HierarchyListView()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: HierarchyListView::RetrieveHierarchy
// Created: AGE 2006-10-04
// -----------------------------------------------------------------------------
template< typename H >
const kernel::Hierarchies* HierarchyListView< H >::RetrieveHierarchy( const kernel::Entity_ABC& entity ) const
{
    return entity.Retrieve< H >();
}

// -----------------------------------------------------------------------------
// Name: HierarchyListView::NotifyCreated
// Created: AGE 2006-10-04
// -----------------------------------------------------------------------------
template< typename H >
void HierarchyListView< H >::NotifyCreated( const H& hierarchy )
{
    NotifyCreated( ( const kernel::Hierarchies&)( hierarchy ) );
}

// -----------------------------------------------------------------------------
// Name: HierarchyListView::NotifyUpdated
// Created: AGE 2006-10-04
// -----------------------------------------------------------------------------
template< typename H >
void HierarchyListView< H >::NotifyUpdated( const H& hierarchy )
{
    NotifyUpdated( ( const kernel::Hierarchies&)( hierarchy ) );
}

// -----------------------------------------------------------------------------
// Name: HierarchyListView::NotifyDeleted
// Created: AGE 2006-10-04
// -----------------------------------------------------------------------------
template< typename H >
void HierarchyListView< H >::NotifyDeleted( const H& hierarchy )
{
    NotifyDeleted( ( const kernel::Hierarchies&)( hierarchy ) );
}

}