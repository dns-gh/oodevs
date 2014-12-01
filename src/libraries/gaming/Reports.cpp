// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Reports.h"
#include "ReportsModel.h"
#include "reports/Report.h"
#include "reports/ReportFactory.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Reports constructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Reports::Reports( const Entity_ABC& entity, const ReportsModel& reportsModel, const ReportFactory& reportFactory )
    : entity_       ( entity )
    , reportsModel_ ( reportsModel )
    , reportFactory_( reportFactory )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Reports destructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Reports::~Reports()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Reports::DisplayInTooltip
// Created: AGE 2006-06-29
// -----------------------------------------------------------------------------
void Reports::DisplayInTooltip( Displayer_ABC& displayer ) const
{
    unsigned int displayed = 0;
    const auto& reports = reportsModel_.GetReports( entity_.GetId() );
    for( auto it = reports.begin(); it != reports.end() && displayed++ < 5; ++it )
    {
        const auto report = reportFactory_.CreateReport( entity_, *it );
        report->DisplayInTooltip( displayer );
    }
}
