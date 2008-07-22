// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "bml_plugin_pch.h"
#include "DetectionReport.h"
#include "Who.h"
#include "Where.h"
#include "Publisher_ABC.h"
#include <xeumeuleu/xml.h>

using namespace bml;

// -----------------------------------------------------------------------------
// Name: DetectionReport constructor
// Created: SBO 2008-07-22
// -----------------------------------------------------------------------------
DetectionReport::DetectionReport( const dispatcher::Agent& entity, const dispatcher::Agent& detected, int level )
    : entity_( entity )
    , detected_( detected )
    , level_( level )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DetectionReport destructor
// Created: SBO 2008-07-22
// -----------------------------------------------------------------------------
DetectionReport::~DetectionReport()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DetectionReport::Send
// Created: SBO 2008-07-22
// -----------------------------------------------------------------------------
void DetectionReport::Send( Publisher_ABC& publisher ) const
{
    Who byWho( entity_ );
    Who who( detected_, level_ );
    xml::xostream& xos = publisher.CreateReport();
    xos << xml::start( "ReportPush" )
            << xml::start( "ReportedByWho" ) << byWho << xml::end()
            << xml::start( "NewWhere" )      << who << Where( detected_ ) << xml::end()
            << xml::start( "ReportedWho" )   << who << xml::end();
    xos << xml::end();
}
