// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "InfoReportsTab.h"
#include "ReportListView.h"
#include "clients_gui/ItemFactory_ABC.h"
#include "gaming/Reports.h"

// -----------------------------------------------------------------------------
// Name: InfoReportsTab constructor
// Created: SBO 2007-02-06
// -----------------------------------------------------------------------------
InfoReportsTab::InfoReportsTab( QTabWidget* parent, kernel::Controllers& controllers, gui::DisplayExtractor& extractor )
    : Q3VBox( parent, "InfoReportsTab" )
    , controllers_( controllers )
    , parent_( parent )
{
    setMargin( 0 );
    ReportListView* reports = new ReportListView( this, controllers, extractor );
    reports->setHeaderHidden( true );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: InfoReportsTab destructor
// Created: SBO 2007-02-06
// -----------------------------------------------------------------------------
InfoReportsTab::~InfoReportsTab()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: InfoReportsTab::NotifySelected
// Created: SBO 2007-05-14
// -----------------------------------------------------------------------------
void InfoReportsTab::NotifySelected( const kernel::Entity_ABC* /*entity*/ )
{
    // NOTHING
}
