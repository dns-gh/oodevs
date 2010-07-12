// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "bml_plugin_pch.h"
#include "ReportingData.h"
#include "dispatcher/Agent_ABC.h"
#include "dispatcher/Automat_ABC.h"
#include "dispatcher/Team_ABC.h"
#include <xeumeuleu/xml.hpp>
#pragma warning( push, 1 )
#include <boost/date_time/posix_time/posix_time.hpp>
#pragma warning( pop )

namespace bpt = boost::posix_time;
using namespace plugins::bml;

namespace
{
    std::string CurrentTime()
    {
        return bpt::to_iso_string( bpt::second_clock::local_time() );
    }
}

// -----------------------------------------------------------------------------
// Name: ReportingData constructor
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
ReportingData::ReportingData( const dispatcher::Agent_ABC& entity )
    : reportDatetime_( CurrentTime() )
    , organisation_( entity.GetSuperior().GetTeam().GetName() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ReportingData constructor
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
ReportingData::ReportingData( const dispatcher::Automat_ABC& entity )
    : reportDatetime_( CurrentTime() )
    , organisation_( entity.GetTeam().GetName() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ReportingData destructor
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
ReportingData::~ReportingData()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: operator<<
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
xml::xostream& plugins::bml::operator<<( xml::xostream& xos, const ReportingData& data )
{
    xos << xml::start( "jc3iedm:ReportingData" )
            << xml::attribute( "xsi:type", "jc3iedm:ReportingData" )
            << xml::content( "jc3iedm:CategoryCode", "ASS" ) //!< report is assumed to be trustable
            << xml::content( "jc3iedm:ReportingDatetime", data.reportDatetime_ ) //!< time of report, compact ISO8601
            << xml::start( "jc3iedm:ReportingOrganisationRef" )
                << xml::content( "jc3iedm:OID", data.organisation_ ) //!< reported by army
            << xml::end
        << xml::end;
    return xos;
}