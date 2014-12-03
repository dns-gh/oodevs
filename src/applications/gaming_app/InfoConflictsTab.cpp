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
InfoConflictsTab::InfoConflictsTab( QTabWidget* parent, kernel::Controllers& controllers,
    gui::DisplayExtractor& extractor, const kernel::KnowledgeConverter_ABC& converter )
    : Q3VBox( parent, "InfoConflictsTab" )
{
    setMargin( 0 );
    list_ = new FireResultListView( this, controllers, extractor, converter );
}

// -----------------------------------------------------------------------------
// Name: InfoConflictsTab destructor
// Created: SBO 2007-02-15
// -----------------------------------------------------------------------------
InfoConflictsTab::~InfoConflictsTab()
{
    // NOTHING
}

void InfoConflictsTab::Purge()
{
    list_->Purge();
}
