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

// -----------------------------------------------------------------------------
// Name: InfoReportsTab constructor
// Created: SBO 2007-02-06
// -----------------------------------------------------------------------------
InfoReportsTab::InfoReportsTab( QTabWidget* parent, kernel::Controllers& controllers, gui::DisplayExtractor& extractor,
                                const ReportFactory& factory, Publisher_ABC& publisher, ReportsModel& model,
                                const AgentsModel& agents, const kernel::Time_ABC& time )
    : Q3VBox( parent, "InfoReportsTab" )
{
    setMargin( 0 );
    reports_ = new ReportListView( this, controllers, extractor, factory, publisher, model, agents, time );
    reports_->setHeaderHidden( true );
}

// -----------------------------------------------------------------------------
// Name: InfoReportsTab destructor
// Created: SBO 2007-02-06
// -----------------------------------------------------------------------------
InfoReportsTab::~InfoReportsTab()
{
    // NOTHING
}

void InfoReportsTab::Purge()
{
    reports_->Purge();
}
