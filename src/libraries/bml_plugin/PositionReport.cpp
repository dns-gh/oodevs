// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "bml_plugin_pch.h"
#include "PositionReport.h"
#include "SerializationTools.h"
#include "Publisher.h"
#include "Who.h"
#include "Where.h"
#include <xeumeuleu/xml.h>

using namespace bml;

// -----------------------------------------------------------------------------
// Name: PositionReport constructor
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
PositionReport::PositionReport( const dispatcher::Agent& entity )
    : entity_( entity )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PositionReport destructor
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
PositionReport::~PositionReport()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PositionReport::Send
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
void PositionReport::Send( Publisher& publisher ) const
{
    Who who( entity_ );
    xml::xostream& xos = publisher.CreateReport();
    xos << xml::start( "ReportPush" )
            << xml::start( "ReportedByWho" ) << who << xml::end()
            << xml::start( "NewWhere" )      << who << Where( entity_ ) << xml::end()
            << xml::start( "ReportedWho" )   << who << xml::end()
        << xml::end();
}
