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
#include "PreparationProfile.h"

// -----------------------------------------------------------------------------
// Name: PopulationListView constructor
// Created: SBO 2006-10-16
// -----------------------------------------------------------------------------
PopulationListView::PopulationListView( QWidget* pParent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory )
    : gui::PopulationListView( pParent, controllers, factory, PreparationProfile::GetProfile() )
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
// Name: PopulationListView::keyPressEvent
// Created: SBO 2006-10-16
// -----------------------------------------------------------------------------
void PopulationListView::keyPressEvent( QKeyEvent* key )
{
    if( selected_ && key->key() == Qt::Key_Delete )
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
