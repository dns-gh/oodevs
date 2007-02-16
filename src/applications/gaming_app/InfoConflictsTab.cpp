// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "InfoConflictsTab.h"
#include "FireResultListView.h"

// -----------------------------------------------------------------------------
// Name: InfoConflictsTab constructor
// Created: SBO 2007-02-15
// -----------------------------------------------------------------------------
InfoConflictsTab::InfoConflictsTab( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory )
    : QVBox( parent )
{
    setMargin( 0 );
    FireResultListView* conflicts = new FireResultListView( this, controllers, factory );
    conflicts->header()->hide();
}

// -----------------------------------------------------------------------------
// Name: InfoConflictsTab destructor
// Created: SBO 2007-02-15
// -----------------------------------------------------------------------------
InfoConflictsTab::~InfoConflictsTab()
{
    // NOTHING
}
