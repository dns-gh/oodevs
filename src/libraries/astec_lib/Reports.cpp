// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "Reports.h"
#include "RC.h"
#include "Trace.h"
#include "Controller.h"
#include "GlTools_ABC.h"

// -----------------------------------------------------------------------------
// Name: Reports constructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Reports::Reports( const Entity_ABC& agent, Controller& controller, const Simulation& simulation )
    : agent_( agent )
    , controller_( controller )
    , simulation_( simulation )
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
void Reports::DoUpdate( const ASN1T_MsgCR& message )
{   
    Report_ABC& rc = *new RC( agent_, simulation_, message );
    reports_.push_back( &rc );
    controller_.Create( rc );
}

// -----------------------------------------------------------------------------
// Name: Reports::DoUpdate
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
void Reports::DoUpdate( const TraceMessage& msg )
{
    Report_ABC& trace = *new Trace( agent_, simulation_, msg );
    reports_.push_back( &trace );
    controller_.Create( trace );
}

// -----------------------------------------------------------------------------
// Name: Reports::Draw
// Created: AGE 2006-04-06
// -----------------------------------------------------------------------------
void Reports::Draw( const geometry::Point2f& where, const geometry::Rectangle2f& viewport, const GlTools_ABC& tools ) const
{
    // $$$$ AGE 2006-04-06: 
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
        const Report_ABC& report = **it;
        report.DisplayInTooltip( displayer );
    }
}
