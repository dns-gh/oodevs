// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "UserProfilePopulationControls.h"
#include "moc_UserProfilePopulationControls.cpp"
#include "PreparationProfile.h"

#pragma warning( disable : 4355 ) // $$$$ LGY 2008-05-14: 'this' : used in base member initializer list

// -----------------------------------------------------------------------------
// Name: UserProfilePopulationControls constructor
// Created: LGY 2011-09-12
// -----------------------------------------------------------------------------
UserProfilePopulationControls::UserProfilePopulationControls( QWidget* pParent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory,
                                                              ControlsChecker_ABC& checker )
    : gui::PopulationListView( pParent, controllers, factory, PreparationProfile::GetProfile() )
    , UserProfileControls_ABC( this, checker )
{
    connect( this, SIGNAL( clicked( Q3ListViewItem*, const QPoint&, int ) ), SLOT( OnItemClicked( Q3ListViewItem*, const QPoint&, int ) ) );
    setResizeMode( Q3ListView::NoColumn );
}

// -----------------------------------------------------------------------------
// Name: UserProfilePopulationControls destructor
// Created: LGY 2011-09-12
// -----------------------------------------------------------------------------
UserProfilePopulationControls::~UserProfilePopulationControls()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UserProfilePopulationControls::showEvent
// Created: NPT 2013-06-10
// -----------------------------------------------------------------------------
void UserProfilePopulationControls::showEvent( QShowEvent* event )
{
    if( selectedProfile_ )
        UserProfileControls_ABC::Display( *selectedProfile_ );
    gui::PopulationListView::showEvent( event );
}

// -----------------------------------------------------------------------------
// Name: UserProfilePopulationControls::hideEvent
// Created: NPT 2013-06-10
// -----------------------------------------------------------------------------
void UserProfilePopulationControls::hideEvent( QHideEvent* event )
{
    Commit();
    gui::PopulationListView::hideEvent( event );
}

// -----------------------------------------------------------------------------
// Name: UserProfilePopulationControls::viewportResizeEvent
// Created: LGY 2011-09-12
// -----------------------------------------------------------------------------
void UserProfilePopulationControls::viewportResizeEvent( QResizeEvent* e )
{
    Q3ScrollView::viewportResizeEvent( e );
    setColumnWidth( 0, -1 );
}

// -----------------------------------------------------------------------------
// Name: UserProfilePopulationControls::setColumnWidth
// Created: LGY 2011-09-12
// -----------------------------------------------------------------------------
void UserProfilePopulationControls::setColumnWidth( int column, int w )
{
    Q3ListView::setColumnWidth( column, column == 0 ? visibleWidth() - columnWidth( 1 ) - columnWidth( 2 ) : w );
}

// -----------------------------------------------------------------------------
// Name: UserProfilePopulationControls::OnItemClicked
// Created: LGY 2011-09-12
// -----------------------------------------------------------------------------
void UserProfilePopulationControls::OnItemClicked( Q3ListViewItem* item, const QPoint& point, int column )
{
    UserProfileControls_ABC::OnItemClicked( item, point, column );
}

// -----------------------------------------------------------------------------
// Name: UserProfilePopulationControls::CanWrite
// Created: LDC 2012-03-01
// -----------------------------------------------------------------------------
bool UserProfilePopulationControls::CanWrite( const kernel::Entity_ABC* entity ) const
{
    return entity != 0;
}

// -----------------------------------------------------------------------------
// Name: UserProfilePopulationControls::OnContextMenuRequested
// Created: JSR 2012-06-22
// -----------------------------------------------------------------------------
void UserProfilePopulationControls::OnContextMenuRequested( Q3ListViewItem*, const QPoint&, int )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UserProfilePopulationControls::dragObject
// Created: JSR 2012-07-09
// -----------------------------------------------------------------------------
Q3DragObject* UserProfilePopulationControls::dragObject()
{
    return 0;
}
