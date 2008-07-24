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
#include "Publisher_ABC.h"
#include "Who.h"
#include "Where.h"
#include <xeumeuleu/xml.h>

using namespace bml;

// -----------------------------------------------------------------------------
// Name: PositionReport constructor
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
PositionReport::PositionReport( const dispatcher::Agent& entity, const ASN1T_MsgUnitAttributes& attributes )
    : entity_( entity )
    , attributes_( attributes )
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
void PositionReport::Send( Publisher_ABC& publisher ) const
{
    Who byWho( entity_ );
    Who who( entity_, attributes_ );
    xml::xostream& xos = publisher.CreateReport();
    xos << xml::start( "ReportPush" )
            << xml::start( "ReportedByWho" ) << byWho << xml::end()
            << xml::start( "NewWhere" )      << byWho << Where( entity_ ) << xml::end() // $$$$ SBO 2008-07-22: economy byWho = who with less info
            << xml::start( "ReportedWho" )   << who   << xml::end()
        << xml::end();
}