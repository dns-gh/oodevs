// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "PopulationListView.h"
#include "moc_PopulationListView.cpp"
#include "clients_gui/Tools.h"

// -----------------------------------------------------------------------------
// Name: PopulationListView constructor
// Created: SBO 2006-10-16
// -----------------------------------------------------------------------------
PopulationListView::PopulationListView( QWidget* pParent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory )
    : gui::PopulationListView( pParent, controllers, factory )
    , selected_( controllers )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: PopulationListView destructor
// Created: SBO 2006-10-16
// -----------------------------------------------------------------------------
PopulationListView::~PopulationListView()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationListView::NotifyContextMenu
// Created: SBO 2006-10-16
// -----------------------------------------------------------------------------
void PopulationListView::NotifyContextMenu( const kernel::Population_ABC&, kernel::ContextMenu& menu )
{
    if( selected_ )
        menu.InsertItem( "Commande", tools::translate( "Preparation", "Supprimer" ), this, SLOT( OnDelete() ) );
}

// -----------------------------------------------------------------------------
// Name: PopulationListView::keyPressEvent
// Created: SBO 2006-10-16
// -----------------------------------------------------------------------------
void PopulationListView::keyPressEvent( QKeyEvent* key )
{
    if( selected_ && key->key() == Qt::Key_Delete )
        delete (const kernel::Entity_ABC*)selected_;
}

// -----------------------------------------------------------------------------
// Name: PopulationListView::OnDelete
// Created: SBO 2006-10-16
// -----------------------------------------------------------------------------
void PopulationListView::OnDelete()
{
    if( selected_ )
        delete (const kernel::Entity_ABC*)selected_;
}

// -----------------------------------------------------------------------------
// Name: PopulationListView::NotifySelected
// Created: SBO 2006-10-30
// -----------------------------------------------------------------------------
void PopulationListView::NotifySelected( const kernel::Entity_ABC* element )
{
    selected_ = element;
    gui::PopulationListView::NotifySelected( element );
}
