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
#include "clients_kernel/Controller.h"
#include "protocol/Protocol.h"
#include "reports/ReportFactory.h"
#include "reports/Report.h"
#include <boost/bind.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Reports constructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Reports::Reports( const Entity_ABC& entity, Controller& controller, const ReportFactory& reportFactory )
    : entity_( entity )
    , controller_( controller )
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
    reports_.clear();
    for( auto it = traces_.begin(); it != traces_.end(); ++it )
        delete *it;
    traces_.clear();
}

// -----------------------------------------------------------------------------
// Name: Reports::DoUpdate
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
void Reports::DoUpdate( const sword::Report& message )
{
    // $$$$ AGE 2007-04-20: limiter le nombre de reports ?
    const unsigned int id =  message.report().id();
    if( reports_.find( id ) == reports_.end() )
    {
        boost::shared_ptr< Report > report = reportFactory_.CreateReport( entity_, message );
        if( report )
        {
            reports_[ id ] = report;
            controller_.Create( *report );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: Reports::DoUpdate
// Created: AGE 2007-10-22
// -----------------------------------------------------------------------------
void Reports::DoUpdate( const sword::InvalidateReport& message )
{
    reports_.erase( message.report().id() );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Reports::DoUpdate
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
void Reports::DoUpdate( const sword::Trace& message )
{
    // $$$$ AGE 2007-04-20: limiter le nombre de traces ?
    Report* trace = reportFactory_.CreateTrace( entity_, message );
    traces_.push_back( trace );
    controller_.Create( *trace );
}

// -----------------------------------------------------------------------------
// Name: Reports::Clear
// Created: AGE 2006-09-18
// -----------------------------------------------------------------------------
void Reports::Clear()
{
    reports_.clear();
    ClearTraces();
}

// -----------------------------------------------------------------------------
// Name: Reports::MarkAsRead
// Created: AGE 2006-09-21
// -----------------------------------------------------------------------------
void Reports::MarkAsRead()
{
    for( auto it = reports_.begin(); it != reports_.end(); ++it )
        it->second->Read();
    std::for_each( traces_.begin(), traces_.end(), boost::bind( &Report::Read, _1 ) );
}

// -----------------------------------------------------------------------------
// Name: Reports::ClearTraces
// Created: AGE 2006-09-18
// -----------------------------------------------------------------------------
void Reports::ClearTraces()
{
    for( auto it = traces_.begin(); it != traces_.end(); ++it )
        delete *it;
    traces_.clear();
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Reports::DisplayInTooltip
// Created: AGE 2006-06-29
// -----------------------------------------------------------------------------
void Reports::DisplayInTooltip( Displayer_ABC& displayer ) const
{
    unsigned int displayed = 0;
    for( auto it = reports_.begin(); it != reports_.end() && displayed++ < 5; ++it )
        it->second->DisplayInTooltip( displayer );
}

const Reports::T_Reports& Reports::GetTraces() const
{
    return traces_;
}
// -----------------------------------------------------------------------------
// Name: Reports::GetReports
// Created: LGY 2013-02-25
// -----------------------------------------------------------------------------
const Reports::T_TextReports& Reports::GetReports() const
{
    return reports_;
}
