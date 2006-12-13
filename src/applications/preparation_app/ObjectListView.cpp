// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "ObjectListView.h"
#include "PreparationProfile.h"
#include "PreparationProfile.h"

// -----------------------------------------------------------------------------
// Name: ObjectListView constructor
// Created: SBO 2006-10-16
// -----------------------------------------------------------------------------
ObjectListView::ObjectListView( QWidget* pParent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory )
    : gui::ObjectListView( pParent, controllers, factory, PreparationProfile::GetProfile() )
    , selected_( controllers )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: ObjectListView destructor
// Created: SBO 2006-10-16
// -----------------------------------------------------------------------------
ObjectListView::~ObjectListView()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectListView::keyPressEvent
// Created: SBO 2006-10-16
// -----------------------------------------------------------------------------
void ObjectListView::keyPressEvent( QKeyEvent* key )
{
    if( selected_ && key->key() == Qt::Key_Delete )
        delete (const kernel::Entity_ABC*)selected_;
}

// -----------------------------------------------------------------------------
// Name: ObjectListView::NotifySelected
// Created: SBO 2006-10-30
// -----------------------------------------------------------------------------
void ObjectListView::NotifySelected( const kernel::Entity_ABC* element )
{
    selected_ = element;
    gui::ObjectListView::NotifySelected( element );
}
