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
#include "ReportFilterOptions.h"

// -----------------------------------------------------------------------------
// Name: InfoReportsTab constructor
// Created: SBO 2007-02-06
// -----------------------------------------------------------------------------
InfoReportsTab::InfoReportsTab( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory )
    : QVBox( parent )
{
    setMargin( 0 );
    ReportFilterOptions* filter  = new ReportFilterOptions( this );
    ReportListView*      reports = new ReportListView( this, controllers, *filter, factory );
    filter->hide();
    reports->header()->hide();
    connect( filter, SIGNAL( OptionsChanged() ), reports, SLOT( OnOptionsChanged() ) );
}

// -----------------------------------------------------------------------------
// Name: InfoReportsTab destructor
// Created: SBO 2007-02-06
// -----------------------------------------------------------------------------
InfoReportsTab::~InfoReportsTab()
{
    // NOTHING
}
