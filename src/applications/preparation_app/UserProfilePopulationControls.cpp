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
// Name: UserProfilePopulationControls::ValueChanged
// Created: LGY 2011-09-13
// -----------------------------------------------------------------------------
void UserProfilePopulationControls::ValueChanged( const kernel::Entity_ABC* entity, bool isReadable, bool isWriteable )
{
    emit ProfiledChanged( entity, isReadable, isWriteable );
}

// -----------------------------------------------------------------------------
// Name: UserProfilePopulationControls::OnProfiledChanged
// Created: LGY 2011-09-13
// -----------------------------------------------------------------------------
void UserProfilePopulationControls::OnProfiledChanged( const kernel::Entity_ABC* entity, bool isWriteable )
{
    if( entity )
    {
        gui::ValuedListItem* item = gui::FindItem( entity, firstChild() );
        if( item )
            SetStatus( item, isWriteable, false );
    }
}
