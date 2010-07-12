// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "bml_plugin_pch.h"
#include "OrderReport.h"
#include "Who.h"
#include "ReportingData.h"
#include "SerializationTools.h"
#include "Publisher_ABC.h"
#include "MissionMapping.h"
#include <xeumeuleu/xml.h>

using namespace plugins::bml;

// -----------------------------------------------------------------------------
// Name: OrderReport constructor
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
OrderReport::OrderReport( const dispatcher::Agent_ABC& agent, const tools::Resolver_ABC< kernel::MissionType >& missions, const MsgUnitOrder& asn )
    : who_( new Who( agent ) )
    , reportingData_( new ReportingData( agent ) )
    , activityCode_( GetCodeFromMissionId( missions, asn.mission() ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: OrderReport constructor
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
OrderReport::OrderReport( const dispatcher::Automat_ABC& automat, const tools::Resolver_ABC< kernel::MissionType >& missions, const MsgAutomatOrder& asn )
    : who_( new Who( automat ) )
    , reportingData_( new ReportingData( automat ) )
    , activityCode_( GetCodeFromMissionId( missions, asn.mission() ) )
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
void OrderReport::Send( Publisher_ABC& publisher ) const
{
    xml::xostream& xos = publisher.CreateReport();
    xos << xml::start( "ReportPush" )
            << xml::start( "ReportedByWho" ) << *who_ << xml::end
            << xml::start( "ReportedWho" )   << *who_ << xml::end
            << xml::start( "NewWhatWhen" )
                << xml::start( "C_BML_WhatWhen" )
                    << xml::start( "WhatWhenInstance" )
                        << xml::start( "ActionTask" )
                            << xml::attribute( "xsi:type", "jc3iedm:ActionTask" )
                            << xml::content( "jc3iedm:ActivityCode", activityCode_ )
                            << xml::start( "jc3iedm:StatusList" )
                                << xml::start( "jc3iedm:Status" )
                                    << xml::attribute( "xsi:type", "jc3iedm:ActionTaskStatus" )
                                    << xml::content( "jc3iedm:CategoryCode", "ORD" ) //!< task is beeing executed
                                    << xml::content("jc3iedm:ReportingDataRef", "" )
                                << xml::end
                            << xml::end
                            << xml::content( "jc3iedm:CategoryCode", "ORD" ) //!< task is beeing executed
                        << xml::end
                    << xml::end
                << xml::end
            << xml::end
        << xml::end;
}
