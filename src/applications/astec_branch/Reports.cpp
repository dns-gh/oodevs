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
#include "Controller.h"

// -----------------------------------------------------------------------------
// Name: Reports constructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Reports::Reports( Agent_ABC& agent, Controller& controller )
    : agent_( agent )
    , controller_( controller )
{

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
// Name: Reports::Update
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
void Reports::Update( const ASN1T_MsgCR& message )
{   
    RC& rc = *new RC( *this, message );
    reports_.push_back( &rc );
    controller_.Create( rc );
}

// -----------------------------------------------------------------------------
// Name: Reports::Update
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
void Reports::Update( const ASN1T_MsgAttenteOrdreConduite& message )
{
    RC& rc = *new RC( *this, message );
    reports_.push_back( &rc );
    controller_.Create( rc );
}

// -----------------------------------------------------------------------------
// Name: Reports::Update
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
void Reports::Update( const TraceMessage& msg )
{
    Trace& trace = *new Trace( *this, msg );
    reports_.push_back( &rc );
    controller_.Create( rc );
}
