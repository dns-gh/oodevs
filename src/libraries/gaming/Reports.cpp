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
#include "reports/Report.h"
#include "reports/ReportFactory.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Reports constructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Reports::Reports( const Entity_ABC& entity, const ReportFactory& reportFactory )
    : entity_       ( entity )
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
    for( auto it = reports_.begin(); it != reports_.end(); ++it )
    {
        const auto report = reportFactory_.CreateReport( entity_, *it );
        report->DisplayInTooltip( displayer );
    }
}

void Reports::DoUpdate( const sword::Report& message )
{
    if( reports_.size() > 4 )
        reports_.pop_back();
    reports_.push_front( message );
}
