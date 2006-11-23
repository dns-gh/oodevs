// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "TacticalListView.h"
#include "SymbolIcons.h"
#include "clients_kernel/Entity_ABC.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: TacticalListView constructor
// Created: AGE 2006-11-22
// -----------------------------------------------------------------------------
TacticalListView::TacticalListView( QWidget* pParent, Controllers& controllers, gui::ItemFactory_ABC& factory, const Profile_ABC& profile, gui::SymbolIcons& icons )
    : HierarchyListView< TacticalHierarchies >( pParent, controllers, factory, profile )
    , icons_( icons )
{
    timer_ = new QTimer( this );
    connect( timer_, SIGNAL( timeout() ), this, SLOT( Update() ) );

    controllers.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: TacticalListView destructor
// Created: AGE 2006-11-22
// -----------------------------------------------------------------------------
TacticalListView::~TacticalListView()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TacticalListView::Display
// Created: AGE 2006-11-22
// -----------------------------------------------------------------------------
void TacticalListView::Display( const Entity_ABC& entity, gui::ValuedListItem* item )
{
    HierarchyListView< TacticalHierarchies >::Display( entity, item );
    DisplayIcon( entity.Get< TacticalHierarchies >(), item );
}

// -----------------------------------------------------------------------------
// Name: TacticalListView::DisplayIcon
// Created: AGE 2006-11-23
// -----------------------------------------------------------------------------
void TacticalListView::DisplayIcon( const TacticalHierarchies& hierarchies, gui::ValuedListItem* item )
{
    std::string symbolName = hierarchies.GetSymbol();
    std::string levelName  = hierarchies.GetLevel();

    QPixmap pixmap;
    if( ! symbolName.empty() || ! levelName.empty() )
    {
        pixmap = icons_.GetSymbol( symbolName, levelName );
        if( pixmap.isNull() )
            timer_->start( 500, true );
    }
    item->setPixmap( 0, pixmap );
}

// -----------------------------------------------------------------------------
// Name: TacticalListView::NotifyUpdated
// Created: AGE 2006-11-23
// -----------------------------------------------------------------------------
void TacticalListView::NotifyUpdated( const Symbol_ABC& symbol )
{
    const Entity_ABC& entity = symbol.GetEntity();
    ValuedListItem* item = FindItem( &entity, firstChild() );
    if( item )
        DisplayIcon( entity.Get< TacticalHierarchies >(), item );
}
