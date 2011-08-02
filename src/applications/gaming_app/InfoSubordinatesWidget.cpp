// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "InfoSubordinatesWidget.h"
#include "moc_InfoSubordinatesWidget.cpp"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/ActionController.h"
#include "clients_kernel/Profile_ABC.h"
#include "InfoSubordinateItem.h"

// -----------------------------------------------------------------------------
// Name: InfoSubordinatesWidget constructor
// Created: SBO 2007-02-21
// -----------------------------------------------------------------------------
InfoSubordinatesWidget::InfoSubordinatesWidget( QWidget* parent, kernel::Controllers& controllers, const kernel::Profile_ABC& profile, gui::EntitySymbols& icons )
    : Q3IconView( parent, "InfoSubordinatesWidget" )
    , controllers_( controllers )
    , profile_( profile )
    , icons_( icons )
    , selected_( controllers )
{
    setGridX( 60 );
    setGridY( 60 );
    setFixedWidth( 345 );
    setWordWrapIconText( false );
    setSorting( true );
    setItemsMovable( false );
    setHScrollBarMode( Q3ScrollView::AlwaysOff );
    connect( this, SIGNAL( doubleClicked( Q3IconViewItem* ) ), SLOT( OpenItem( Q3IconViewItem* ) ) );
    hide();
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: InfoSubordinatesWidget destructor
// Created: SBO 2007-02-21
// -----------------------------------------------------------------------------
InfoSubordinatesWidget::~InfoSubordinatesWidget()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: InfoSubordinatesWidget::NotifySelected
// Created: SBO 2007-02-21
// -----------------------------------------------------------------------------
void InfoSubordinatesWidget::NotifySelected( const kernel::Entity_ABC* entity )
{
    if( selected_ != entity )
    {
        selected_ = entity;
        clear();
        hide();
        if( !selected_ )
            return;
        if( const kernel::TacticalHierarchies* hierarchies = selected_->Retrieve< kernel::TacticalHierarchies >() )
        {
            tools::Iterator< const kernel::Entity_ABC& > it = hierarchies->CreateSubordinateIterator();
            while( it.HasMoreElements() )
            {
                if( !isShown() )
                    show();
                const kernel::Entity_ABC& element = it.NextElement();
                AddSubordinate( element );
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: InfoSubordinatesWidget::AddSubordinate
// Created: SBO 2007-02-21
// -----------------------------------------------------------------------------
void InfoSubordinatesWidget::AddSubordinate( const kernel::Entity_ABC& entity )
{
    if( profile_.IsVisible( entity ) )
        new InfoSubordinateItem( this, controllers_, icons_, entity );
}

// -----------------------------------------------------------------------------
// Name: InfoSubordinatesWidget::OpenItem
// Created: SBO 2007-02-21
// -----------------------------------------------------------------------------
void InfoSubordinatesWidget::OpenItem( Q3IconViewItem* item )
{
    if( InfoSubordinateItem* value = static_cast< InfoSubordinateItem* >( item ) )
        controllers_.actions_.Select( value->GetEntity() );
}

// -----------------------------------------------------------------------------
// Name: InfoSubordinatesWidget::NotifyDeleted
// Created: SBO 2007-02-23
// -----------------------------------------------------------------------------
void InfoSubordinatesWidget::NotifyDeleted( const kernel::Entity_ABC& entity )
{
    for( Q3IconViewItem* item = firstItem(); item; item = item->nextItem() )
        if( &static_cast< InfoSubordinateItem* >( item )->GetEntity() == &entity )
        {
            delete item;
            return;
        }
}