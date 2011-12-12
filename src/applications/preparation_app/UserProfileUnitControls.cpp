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
#include "preparation/Model.h"
#include "clients_gui/LongNameHelper.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"

#pragma warning( disable : 4355 ) // $$$$ SBO 2008-05-14: 'this' : used in base member initializer list

using namespace gui;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: UserProfileUnitControls constructor
// Created: LGY 2011-09-13
// -----------------------------------------------------------------------------
UserProfileUnitControls::UserProfileUnitControls( QWidget* parent, Controllers& controllers, ItemFactory_ABC& factory,
                                                  EntitySymbols& icons, ControlsChecker_ABC& checker, Model& model )
    : HierarchyListView< ProfileHierarchies_ABC >( parent, controllers, factory, PreparationProfile::GetProfile(), icons )
    , UserProfileControls_ABC( this, checker )
    , model_( model )
{
    controllers_.Register( *this );
    connect( this, SIGNAL( clicked( Q3ListViewItem*, const QPoint&, int ) ), SLOT( OnItemClicked( Q3ListViewItem*, const QPoint&, int ) ) );
    connect( this, SIGNAL( expanded( Q3ListViewItem* ) ), SLOT( OnItemExpanded( Q3ListViewItem* ) ) );
    setSortColumn( -1 );
    setSorting( -1 );
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
// Created: LGY 2011-12-12
// -----------------------------------------------------------------------------
void UserProfileUnitControls::Display( UserProfile& profile )
{
    ClearSelection();
    UserProfileControls_ABC::Display( profile );
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
// Name: UserProfileUnitControls::Show
// Created: LGY 2011-09-28
// -----------------------------------------------------------------------------
void UserProfileUnitControls::Show()
{
    tools::Iterator< const kernel::Team_ABC& > itTeam = model_.GetTeamResolver().CreateIterator();
    while( itTeam.HasMoreElements() )
    {
        const kernel::Team_ABC& team = itTeam.NextElement();
        tools::Iterator< const kernel::Entity_ABC& > itFormation = team.Get< kernel::TacticalHierarchies >().CreateSubordinateIterator();
        while( itFormation.HasMoreElements() )
        {
            const kernel::Entity_ABC& formation = itFormation.NextElement();
            tools::Iterator< const kernel::Entity_ABC& > itAutomat = formation.Get< kernel::TacticalHierarchies >().CreateSubordinateIterator();
            ValuedListItem* first = 0;
            while( itAutomat.HasMoreElements() )
            {
                const kernel::Entity_ABC& automat = itAutomat.NextElement();
                if( !first )
                    first = FindItem( &automat, firstChild() );
                else if( ValuedListItem* item = FindItem( &automat, firstChild() ) )
                        item->moveItem( first );
            }
        }
    }
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
// Name: UserProfileUnitControls::OnItemExpanded
// Created: LGY 2011-12-09
// -----------------------------------------------------------------------------
void UserProfileUnitControls::OnItemExpanded( Q3ListViewItem* item )
{
    if( item )
        if(  ValuedListItem* value = static_cast< ValuedListItem* >( item ) )
            if( const Entity_ABC* entity = value->GetValue< const Entity_ABC >() )
                if( entity->GetTypeName() == Team_ABC::typeName_ )
                {
                    ValuedListItem* child = static_cast< ValuedListItem* >( item->firstChild() );
                    while( child )
                    {
                        child->setOpen( true );
                        child = static_cast< ValuedListItem* >( child->nextSibling() );
                    }
                }
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
