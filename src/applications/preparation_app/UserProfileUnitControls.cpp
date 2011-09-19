// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "UserProfileUnitControls.h"
#include "moc_UserProfileUnitControls.cpp"
#include "PreparationProfile.h"
#include "clients_gui/LongNameHelper.h"
#include "clients_kernel/Entity_ABC.h"

#pragma warning( disable : 4355 ) // $$$$ SBO 2008-05-14: 'this' : used in base member initializer list

using namespace gui;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: UserProfileUnitControls constructor
// Created: LGY 2011-09-13
// -----------------------------------------------------------------------------
UserProfileUnitControls::UserProfileUnitControls( QWidget* parent, Controllers& controllers, ItemFactory_ABC& factory,
                                                  EntitySymbols& icons, ControlsChecker_ABC& checker )
    : HierarchyListView< ProfileHierarchies_ABC >( parent, controllers, factory, PreparationProfile::GetProfile(), icons )
    , UserProfileControls_ABC( this, checker )
{
    controllers_.Register( *this );
    connect( this, SIGNAL( clicked( Q3ListViewItem*, const QPoint&, int ) ), SLOT( OnItemClicked( Q3ListViewItem*, const QPoint&, int ) ) );
}

// -----------------------------------------------------------------------------
// Name: UserProfileUnitControls destructor
// Created: LGY 2011-09-13
// -----------------------------------------------------------------------------
UserProfileUnitControls::~UserProfileUnitControls()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: UserProfileUnitControls::Display
// Created: JSR 2011-09-15
// -----------------------------------------------------------------------------
void UserProfileUnitControls::Display( const kernel::Entity_ABC& entity, gui::ValuedListItem* item )
{
    HierarchyListView< ProfileHierarchies_ABC >::Display( entity, item );
    LongNameHelper::SetItemLongName( entity, *item );
}

// -----------------------------------------------------------------------------
// Name: UserProfileUnitControls::viewportResizeEvent
// Created: LGY 2011-09-13
// -----------------------------------------------------------------------------
void UserProfileUnitControls::viewportResizeEvent( QResizeEvent* e )
{
    Q3ScrollView::viewportResizeEvent( e );
    setColumnWidth( 0, -1 );
}

// -----------------------------------------------------------------------------
// Name: UserProfileUnitControls::setColumnWidth
// Created: LGY 2011-09-13
// -----------------------------------------------------------------------------
void UserProfileUnitControls::setColumnWidth( int column, int w )
{
    Q3ListView::setColumnWidth( column, column == 0 ? visibleWidth() - columnWidth( 1 ) - columnWidth( 2 ) : w );
}

// -----------------------------------------------------------------------------
// Name: UserProfileUnitControls::OnItemClicked
// Created: LGY 2011-09-13
// -----------------------------------------------------------------------------
void UserProfileUnitControls::OnItemClicked( Q3ListViewItem* item, const QPoint& point, int column )
{
    UserProfileControls_ABC::OnItemClicked( item, point, column );
}

// -----------------------------------------------------------------------------
// Name: UserProfileUnitControls::NotifyUpdated
// Created: LGY 2011-09-13
// -----------------------------------------------------------------------------
void UserProfileUnitControls::NotifyUpdated( const Entity_ABC& entity )
{
    if( ValuedListItem* item = FindItem( &entity, firstChild() ) )
    {
        item->SetNamed( entity );
        LongNameHelper::SetItemLongName( entity, *item );
    }
}

// -----------------------------------------------------------------------------
// Name: UserProfileUnitControls::OnProfiledChanged
// Created: LGY 2011-09-14
// -----------------------------------------------------------------------------
void UserProfileUnitControls::OnProfiledChanged( const Entity_ABC* entity, bool isWriteable )
{
    if( entity )
    {
        ValuedListItem* item = FindItem( entity, firstChild() );
        if( item )
            SetStatus( item, isWriteable, false );
    }
}

// -----------------------------------------------------------------------------
// Name: UserProfileUnitControls::ValueChanged
// Created: LGY 2011-09-13
// -----------------------------------------------------------------------------
void UserProfileUnitControls::ValueChanged( const Entity_ABC* entity, bool isReadable, bool isWriteable )
{
    emit ProfiledChanged( entity, isReadable, isWriteable );
}
