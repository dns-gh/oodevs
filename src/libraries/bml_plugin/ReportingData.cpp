// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "bml_plugin_pch.h"
#include "ReportingData.h"
#include "dispatcher/Agent.h"
#include "dispatcher/Automat.h"
#include "dispatcher/Side.h"
#include <xeumeuleu/xml.h>
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
ReportingData::ReportingData( const dispatcher::Agent& entity )
    : reportDatetime_( CurrentTime() )
    , organisation_( entity.automat_->team_.name_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ReportingData constructor
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
ReportingData::ReportingData( const dispatcher::Automat& entity )
    : reportDatetime_( CurrentTime() )
    , organisation_( entity.team_.name_ )
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
            << xml::end()
        << xml::end();
    return xos;
}