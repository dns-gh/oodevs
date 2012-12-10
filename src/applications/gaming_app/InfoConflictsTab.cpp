// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "InfoConflictsTab.h"
#include "FireResultListView.h"
#include "gaming/Fires.h"

// -----------------------------------------------------------------------------
// Name: InfoConflictsTab constructor
// Created: SBO 2007-02-15
// -----------------------------------------------------------------------------
InfoConflictsTab::InfoConflictsTab( QTabWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory )
    : Q3VBox( parent, "InfoConflictsTab" )
    , controllers_( controllers )
    , parent_( parent )
{
    setMargin( 0 );
    new FireResultListView( this, controllers, factory );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: InfoConflictsTab destructor
// Created: SBO 2007-02-15
// -----------------------------------------------------------------------------
InfoConflictsTab::~InfoConflictsTab()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: InfoConflictsTab::NotifySelected
// Created: SBO 2007-05-14
// -----------------------------------------------------------------------------
void InfoConflictsTab::NotifySelected( const kernel::Entity_ABC* /*entity*/ )
{
    // NOTHING
}
