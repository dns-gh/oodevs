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
    for( CIT_Reports it = reports_.begin(); it != reports_.end(); ++it )
        delete *it;
}

// -----------------------------------------------------------------------------
// Name: Reports::DoUpdate
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
void Reports::DoUpdate( const ASN1T_MsgReport& message )
{
    // $$$$ AGE 2007-04-20: limiter le nombre de reports ?
    Report* report = reportFactory_.CreateReport( agent_, message );
    reports_.push_back( report );
    controller_.Create( *report );
}

// -----------------------------------------------------------------------------
// Name: Reports::DoUpdate
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
void Reports::DoUpdate( const ASN1T_MsgTrace& message )
{
    // $$$$ AGE 2007-04-20: limiter le nombre de reports ?
    Report* trace = reportFactory_.CreateTrace( agent_, message );
    reports_.push_back( trace );
    controller_.Create( *trace );
}

// -----------------------------------------------------------------------------
// Name: Reports::Clear
// Created: AGE 2006-09-18
// -----------------------------------------------------------------------------
void Reports::Clear()
{
    for( CIT_Reports it = reports_.begin(); it != reports_.end(); ++it )
        delete *it;
    reports_.clear();
    controller_.Update( *this ); 
    // $$$$ AGE 2006-09-18: pas cohérent : 
    // $$$$ AGE 2006-09-18: il faut observer le Create( Report )
    // $$$$ AGE 2006-09-18: et le Update( Reports );
}

// -----------------------------------------------------------------------------
// Name: Reports::MarkAsRead
// Created: AGE 2006-09-21
// -----------------------------------------------------------------------------
void Reports::MarkAsRead()
{
    for( CIT_Reports it = reports_.begin(); it != reports_.end(); ++it )
        (*it)->Read();
}

// -----------------------------------------------------------------------------
// Name: Reports::ClearTraces
// Created: AGE 2006-09-18
// -----------------------------------------------------------------------------
void Reports::ClearTraces()
{
    T_Reports tokeep;
    tokeep.reserve( reports_.size() );
    for( CIT_Reports it = reports_.begin(); it != reports_.end(); ++it )
    {
        Report* report = *it;
        if( !report || report->GetType() == Report::eTrace )
            delete report;
        else
            tokeep.push_back( report );
    }
    std::swap( tokeep, reports_ );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Reports::DisplayInTooltip
// Created: AGE 2006-06-29
// -----------------------------------------------------------------------------
void Reports::DisplayInTooltip( Displayer_ABC& displayer ) const
{
    unsigned int displayed = 0;
    for( T_Reports::const_reverse_iterator it = reports_.rbegin(); it != reports_.rend() && displayed++ < 5; ++it )
    {
        const Report& report = **it;
        report.DisplayInTooltip( displayer );
    }
}
