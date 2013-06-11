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
#include "ControlsChecker_ABC.h"
#include "preparation/Model.h"
#include "clients_gui/LongNameHelper.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Ghost_ABC.h"
#include "clients_kernel/Options.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Team_ABC.h"
#include <boost/lambda/lambda.hpp>
#include <boost/bind.hpp>

#pragma warning( disable : 4355 ) // $$$$ SBO 2008-05-14: 'this' : used in base member initializer list

using namespace gui;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: UserProfileUnitControls constructor
// Created: LGY 2011-09-13
// -----------------------------------------------------------------------------
UserProfileUnitControls::UserProfileUnitControls( QWidget* parent, Controllers& controllers, ItemFactory_ABC& factory,
                                                  const EntitySymbols& icons, ControlsChecker_ABC& checker, Model& model )
    : HierarchyListView< ProfileHierarchies_ABC >( parent, controllers, factory, PreparationProfile::GetProfile(), icons )
    , UserProfileControls_ABC( this, checker )
    , model_  ( model )
    , checker_( checker )
    , func_   ( boost::lambda::constant( true ) )
{
    controllers_.Register( *this );
    connect( this, SIGNAL( clicked( Q3ListViewItem*, const QPoint&, int ) ), SLOT( OnItemClicked( Q3ListViewItem*, const QPoint&, int ) ) );
    connect( this, SIGNAL( expanded( Q3ListViewItem* ) ), SLOT( OnItemExpanded( Q3ListViewItem* ) ) );
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
    if( !item )
        return;

    QColor color = Qt::transparent;
    if( dynamic_cast< const Ghost_ABC* >( &entity ) != 0 )
        color = QColor( controllers_.options_.GetOption( "Color/Phantom", QString( "" ) ).To< QString >() );
    item->SetBackgroundColor( color );

    HierarchyListView< ProfileHierarchies_ABC >::Display( entity, item );
    LongNameHelper::SetItemLongName( entity, *item );
}

// -----------------------------------------------------------------------------
// Name: UserProfileUnitControls::Show
// Created: LGY 2011-09-28
// -----------------------------------------------------------------------------
void UserProfileUnitControls::Show()
{
    ValuedListItem* head = 0;
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
                {
                    item->moveItem( first );
                    first = item;
                }
            }
        }
        if( !head )
            head = FindItem( static_cast< const kernel::Entity_ABC* >( &team ), firstChild() );
        else if( ValuedListItem* item = FindItem( static_cast< const kernel::Entity_ABC* >( &team ), firstChild() ) )
        {
            item->moveItem( head );
            head = item;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: UserProfileUnitControls::showEvent
// Created: NPT 2013-06-10
// -----------------------------------------------------------------------------
void UserProfileUnitControls::showEvent( QShowEvent* event )
{
    Show();
    if( selectedProfile_ )
        UserProfileControls_ABC::Display( *selectedProfile_ );
    T_Parent::showEvent( event );
}

// -----------------------------------------------------------------------------
// Name: UserProfileUnitControls::hideEvent
// Created: NPT 2013-06-10
// -----------------------------------------------------------------------------
void UserProfileUnitControls::hideEvent( QHideEvent* event )
{
    Commit();
    T_Parent::hideEvent( event );
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
        if( ValuedListItem* value = static_cast< ValuedListItem* >( item ) )
            if( const Entity_ABC* entity = value->GetValue< const Entity_ABC >() )
                if( entity->GetTypeName() == Team_ABC::typeName_ )
                {
                    ValuedListItem* child = static_cast< ValuedListItem* >( item->firstChild() );
                    while( child )
                    {
                        Expand( child );
                        child = static_cast< ValuedListItem* >( child->nextSibling() );
                    }
                }
}

// -----------------------------------------------------------------------------
// Name: UserProfileUnitControls::Expand
// Created: LGY 2011-12-14
// -----------------------------------------------------------------------------
void UserProfileUnitControls::Expand( ValuedListItem* item )
{
    item->setOpen( true );
    ValuedListItem* child = static_cast< ValuedListItem* >( item->firstChild() );
    while( child )
    {
        Expand( child );
        child = static_cast< ValuedListItem* >( child->nextSibling() );
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

// -----------------------------------------------------------------------------
// Name: UserProfileUnitControls::HideAssignedAutomats
// Created: LGY 2011-12-12
// -----------------------------------------------------------------------------
void UserProfileUnitControls::HideAssignedAutomats()
{
    func_ = boost::bind( &UserProfileUnitControls::ApplyHideFilter, this, _1 );
    UpdateFilter();
}

// -----------------------------------------------------------------------------
// Name: UserProfileUnitControls::ApplyHideFilter
// Created: LGY 2011-12-12
// -----------------------------------------------------------------------------
bool UserProfileUnitControls::ApplyHideFilter( gui::ValuedListItem* item ) const
{
    if( item )
        if( ValuedListItem* value = static_cast< ValuedListItem* >( item ) )
            if( const Entity_ABC* entity = value->GetValue< const Entity_ABC >() )
                return !checker_.IsControlledByLowLevel( *entity ) || Status( value->text( 2 ).toInt() ) != eNothing;
    return false;
}

// -----------------------------------------------------------------------------
// Name: UserProfileUnitControls::ShowAssignedAutomats
// Created: LGY 2011-12-12
// -----------------------------------------------------------------------------
void UserProfileUnitControls::ShowAssignedAutomats()
{
    func_ = boost::bind( &UserProfileUnitControls::ApplyShowFilter, this, _1 );
    UpdateFilter();
}

// -----------------------------------------------------------------------------
// Name: UserProfileUnitControls::ApplyShowFilter
// Created: LGY 2011-12-12
// -----------------------------------------------------------------------------
bool UserProfileUnitControls::ApplyShowFilter( gui::ValuedListItem* item ) const
{
    if( item )
        if( ValuedListItem* value = static_cast< ValuedListItem* >( item ) )
            if( const Entity_ABC* entity = value->GetValue< const Entity_ABC >() )
            {
                const Status status = Status( value->text( 2 ).toInt() );
                if( status != eNothing )
                    return true;
            }
    return false;
}

// -----------------------------------------------------------------------------
// Name: UserProfileUnitControls::RemoveFilter
// Created: LGY 2011-12-12
// -----------------------------------------------------------------------------
void UserProfileUnitControls::RemoveFilter()
{
    func_ = boost::lambda::constant( true );
    UpdateFilter();
}

// -----------------------------------------------------------------------------
// Name: UserProfileUnitControls::UpdateFilter
// Created: LGY 2011-12-13
// -----------------------------------------------------------------------------
void UserProfileUnitControls::UpdateFilter()
{
    ApplyFilter( func_ );
}

// -----------------------------------------------------------------------------
// Name: UserProfileUnitControls::NotifyCreated
// Created: LDC 2012-03-01
// -----------------------------------------------------------------------------
void UserProfileUnitControls::NotifyCreated( const ProfileHierarchies_ABC& hierarchy )
{
    if( Accept( hierarchy ) )
        gui::HierarchyListView< ProfileHierarchies_ABC >::NotifyCreated( hierarchy );
}

// -----------------------------------------------------------------------------
// Name: UserProfileUnitControls::NotifyUpdated
// Created: LDC 2012-03-01
// -----------------------------------------------------------------------------
void UserProfileUnitControls::NotifyUpdated( const ProfileHierarchies_ABC& hierarchy )
{
    if( Accept( hierarchy ) )
        gui::HierarchyListView< ProfileHierarchies_ABC >::NotifyUpdated( hierarchy );
}

// -----------------------------------------------------------------------------
// Name: UserProfileUnitControls::NotifyDeleted
// Created: LDC 2012-03-01
// -----------------------------------------------------------------------------
void UserProfileUnitControls::NotifyDeleted( const ProfileHierarchies_ABC& hierarchy )
{
    if( Accept( hierarchy ) )
        gui::HierarchyListView< ProfileHierarchies_ABC >::NotifyDeleted( hierarchy );
}

// -----------------------------------------------------------------------------
// Name: UserProfileUnitControls::OnContextMenuRequested
// Created: JSR 2012-06-22
// -----------------------------------------------------------------------------
void UserProfileUnitControls::OnContextMenuRequested( Q3ListViewItem*, const QPoint&, int )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UserProfileUnitControls::Accept
// Created: JSR 2012-03-01
// -----------------------------------------------------------------------------
bool UserProfileUnitControls::Accept( const ProfileHierarchies_ABC& hierarchy ) const
{
    const kernel::Entity_ABC& entity = hierarchy.GetEntity();
    // accept only automats, formations, teams, and ghost.
    return ( dynamic_cast< const kernel::Team_ABC* >( &entity ) || 
             dynamic_cast< const kernel::Automat_ABC* >( &entity ) ||
             dynamic_cast< const kernel::Formation_ABC* >( &entity ) ||
             ( dynamic_cast< const kernel::Ghost_ABC* >( &entity ) && static_cast< const kernel::Ghost_ABC* >( &entity )->GetGhostType() == eGhostType_Automat ) );
}

// -----------------------------------------------------------------------------
// Name: UserProfileUnitControls::dragObject
// Created: JSR 2012-07-09
// -----------------------------------------------------------------------------
Q3DragObject* UserProfileUnitControls::dragObject()
{
     return 0;
}
