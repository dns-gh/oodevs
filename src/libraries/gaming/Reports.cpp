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
#include "ReportFactory.h"
#include "Report.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/GlTools_ABC.h"
#include "RcEntityResolver_ABC.h"
#include <boost/bind.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Reports constructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Reports::Reports( const Entity_ABC& agent, Controller& controller, const ReportFactory& reportFactory )
    : agent_( agent )
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
    tools::Resolver< Report >::Clear();
    for( CIT_Reports it = traces_.begin(); it != traces_.end(); ++it )
        delete *it;
    traces_.clear();
}

// -----------------------------------------------------------------------------
// Name: Reports::DoUpdate
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
void Reports::DoUpdate( const ASN1T_MsgReport& message )
{
    // $$$$ AGE 2007-04-20: limiter le nombre de reports ?
    Report* report = reportFactory_.CreateReport( agent_, message );
    Register( message.cr_oid, *report );
    controller_.Create( *report );
}

// -----------------------------------------------------------------------------
// Name: Reports::DoUpdate
// Created: AGE 2007-10-22
// -----------------------------------------------------------------------------
void Reports::DoUpdate( const ASN1T_MsgInvalidateReport& message )
{
    delete Find( message.cr_oid );
    Remove( message.cr_oid );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Reports::DoUpdate
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
void Reports::DoUpdate( const ASN1T_MsgTrace& message )
{
    // $$$$ AGE 2007-04-20: limiter le nombre de traces ?
    Report* trace = reportFactory_.CreateTrace( agent_, message );
    traces_.push_back( trace );
    controller_.Create( *trace );
}

// -----------------------------------------------------------------------------
// Name: Reports::Clear
// Created: AGE 2006-09-18
// -----------------------------------------------------------------------------
void Reports::Clear()
{
    tools::Resolver< Report >::Clear();
    ClearTraces();
}

// -----------------------------------------------------------------------------
// Name: Reports::MarkAsRead
// Created: AGE 2006-09-21
// -----------------------------------------------------------------------------
void Reports::MarkAsRead()
{
    Apply( boost::bind( &Report::Read, _1 ) );
    std::for_each( traces_.begin(), traces_.end(), boost::bind( &Report::Read, _1 ) );
}

// -----------------------------------------------------------------------------
// Name: Reports::ClearTraces
// Created: AGE 2006-09-18
// -----------------------------------------------------------------------------
void Reports::ClearTraces()
{
    for( CIT_Reports it = traces_.begin(); it != traces_.end(); ++it )
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
    for( T_Reports::const_reverse_iterator it = traces_.rbegin(); it != traces_.rend() && displayed++ < 5; ++it )
    {
        const Report& report = **it;
        report.DisplayInTooltip( displayer );
    }
}
