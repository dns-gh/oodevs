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
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "protocol/Protocol.h"
#include "reports/ReportFactory.h"
#include "reports/Report.h"
#include "reports/RcEntityResolver_ABC.h"
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
    for( auto it = traces_.begin(); it != traces_.end(); ++it )
        delete *it;
    traces_.clear();
}

// -----------------------------------------------------------------------------
// Name: Reports::ShouldDisplay
// Created: LDC 2012-10-02
// -----------------------------------------------------------------------------
bool Reports::ShouldDisplay( const Report& report ) const
{
    const kernel::Entity_ABC& owner = report.GetOwner();
    if( &agent_ == &owner )
        return true;
    const TacticalHierarchies* tacticalHierarchies = owner.Retrieve< TacticalHierarchies >();
    if( !tacticalHierarchies )
        return false;
    return tacticalHierarchies->IsSubordinateOf( agent_ );
}

// -----------------------------------------------------------------------------
// Name: Reports::DoUpdate
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
void Reports::DoUpdate( const sword::Report& message )
{
    // $$$$ AGE 2007-04-20: limiter le nombre de reports ?
    if( !Find( message.report().id() ) )
    {
        Report* report = reportFactory_.CreateReport( agent_, message );
        if( report )
        {
            controller_.Create( *report );
            AddReport( message.report().id(), report );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: Reports::AddReport
// Created: LDC 2012-10-02
// -----------------------------------------------------------------------------
void Reports::AddReport( unsigned int id, Report* report )
{
    Register( id, *report );
    const TacticalHierarchies* hierarchies = agent_.Retrieve< TacticalHierarchies >();
    if( hierarchies )
    {
        const Entity_ABC* superior = hierarchies->GetSuperior();
        if( superior )
        {
            Reports* reports = const_cast< Reports* >( superior->Retrieve< Reports >() );
            if( reports )
                reports->AddReport( id, report );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: Reports::DoUpdate
// Created: AGE 2007-10-22
// -----------------------------------------------------------------------------
void Reports::DoUpdate( const sword::InvalidateReport& message )
{
    Report* report = Find( message.report().id() );
    if( report )
    {
        Remove( message.report().id() );
        controller_.Update( *this );
        const TacticalHierarchies* hierarchies = agent_.Retrieve< TacticalHierarchies >();
        if( hierarchies )
        {
            const Entity_ABC* superior = hierarchies->GetSuperior();
            if( superior )
            {
                Reports* reports = const_cast< Reports* >( superior->Retrieve< Reports >() );
                if( reports )
                    reports->DoUpdate( message );
            }
        }
        const kernel::Entity_ABC& owner = report->GetOwner();
        if( &agent_ == &owner )
            delete report;
    }
}

// -----------------------------------------------------------------------------
// Name: Reports::DoUpdate
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
void Reports::DoUpdate( const sword::Trace& message )
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
    for( T_Elements::const_iterator it = elements_.begin(); it != elements_.end() && displayed++ < 5; ++it )
        if( const Report* report = it->second )
            report->DisplayInTooltip( displayer );
}
