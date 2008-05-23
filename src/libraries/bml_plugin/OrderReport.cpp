// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "bml_plugin_pch.h"
#include "OrderReport.h"
#include "Who.h"
#include "ReportingData.h"
#include "SerializationTools.h"
#include "Publisher.h"
#include "MissionMapping.h"
#include <xeumeuleu/xml.h>

using namespace bml;

// -----------------------------------------------------------------------------
// Name: OrderReport constructor
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
OrderReport::OrderReport( const dispatcher::Agent& agent, const kernel::Resolver_ABC< kernel::MissionType >& missions, const ASN1T_MsgUnitOrder& asn )
    : who_( new Who( agent ) )
    , reportingData_( new ReportingData( agent ) )
    , activityCode_( GetCodeFromMissionId( missions, asn.mission ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: OrderReport constructor
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
OrderReport::OrderReport( const dispatcher::Automat& automat, const kernel::Resolver_ABC< kernel::MissionType >& missions, const ASN1T_MsgAutomatOrder& asn )
    : who_( new Who( automat ) )
    , reportingData_( new ReportingData( automat ) )
    , activityCode_( GetCodeFromMissionId( missions, asn.mission ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: OrderReport destructor
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
OrderReport::~OrderReport()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: OrderReport::Send
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
void OrderReport::Send( Publisher& publisher ) const
{
    xml::xostringstream xos;
    xos << xml::start( "ReportPush" )
            << Namespaces()
            << xml::start( "ReportPush" )
                << xml::start( "ReportedByWho" ) << *who_ << xml::end()
                << xml::start( "ReportedWho" )   << *who_ << xml::end()
                << xml::start( "NewWhatWhen" )
                    << xml::start( "C_BML_WhatWhen" )
                        << xml::start( "WhatWhenInstance" )
                            << xml::start( "ActionTask" )
                                << xml::content( "jc3iedm:ActivityCode", activityCode_ )
                                << *reportingData_
                            << xml::end()
                            << xml::start( "ActionTaskStatus" )
                                << xml::content( "jc3iedm:CategoryCode", "ORD" ) //!< task is beeing executed
                                << *reportingData_
                            << xml::end()
                        << xml::end()
                    << xml::end()
                << xml::end()
            << xml::end()
        << xml::end();
    publisher.PushReport( xos.str() );
}
