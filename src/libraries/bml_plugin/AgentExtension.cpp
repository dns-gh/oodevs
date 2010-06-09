// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "bml_plugin_pch.h"
#include "AgentExtension.h"
#include "PositionReport.h"
#include "OrderReport.h"
#include "DetectionReport.h"
#include "ReportFactory.h"
#include "Simulation.h"
#include "dispatcher/Model.h"
#include "MT/MT_Logger/MT_Logger_lib.h"
#include <xeumeuleu/xml.h>
#pragma warning( push, 1 )
#include <boost/date_time/posix_time/posix_time.hpp>
#pragma warning( pop )
#include <iostream>

namespace bpt = boost::posix_time;
using namespace plugins::bml;

// -----------------------------------------------------------------------------
// Name: AgentExtension constructor
// Created: SBO 2008-02-29
// -----------------------------------------------------------------------------
AgentExtension::AgentExtension( dispatcher::Agent_ABC& holder, Publisher_ABC& publisher, const ReportFactory& factory, const Simulation& simulation, const dispatcher::Model& model )
    : holder_( holder )
    , publisher_( publisher )
    , factory_( factory )
    , simulation_( simulation )
    , model_( model )
    , lastUpdate_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentExtension destructor
// Created: SBO 2008-02-29
// -----------------------------------------------------------------------------
AgentExtension::~AgentExtension()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentExtension::DoUpdate
// Created: SBO 2008-02-29
// -----------------------------------------------------------------------------
void AgentExtension::DoUpdate( const MsgsSimToClient::MsgUnitAttributes& attributes )
{
    const bool reportPosition = ( attributes.has_position()  || attributes.has_hauteur()  ) && simulation_.MustReportPosition( lastUpdate_ );
    const bool reportStatus   = ( attributes.has_etat_operationnel()  || attributes.has_dotation_eff_materiel()  || attributes.has_dotation_eff_personnel()  ) && simulation_.MustReportStatus( lastUpdate_ );
    if( reportPosition || reportStatus )
        try
        {
            PositionReport report( holder_, attributes );
            report.Send( publisher_ );
        }
        catch( std::exception& e )
        {
            MT_LOG_ERROR_MSG( "BML error sending position report: " << e.what() );
        }
}

// -----------------------------------------------------------------------------
// Name: AgentExtension::DoUpdate
// Created: SBO 2008-05-16
// -----------------------------------------------------------------------------
void AgentExtension::DoUpdate( const Common::MsgUnitOrder& message )
{
    try
    {
        std::auto_ptr< OrderReport > report( factory_.CreateOrderReport( holder_, message ) );
        report->Send( publisher_ );
    }
    catch( std::exception& e )
    {
        MT_LOG_ERROR_MSG( "BML error sending agent order report: " << e.what() );
    }
}

// -----------------------------------------------------------------------------
// Name: AgentExtension::DoUpdate
// Created: SBO 2008-07-22
// -----------------------------------------------------------------------------
void AgentExtension::DoUpdate( const MsgsSimToClient::MsgUnitDetection& message )
{
    try
    {
        DetectionReport report( holder_, model_.Agents().Get( message.detected_unit_oid() ), message.current_visibility() );
        report.Send( publisher_ );
    }
    catch( std::exception& e )
    {
        MT_LOG_ERROR_MSG( "BML error sending detection report: " << e.what() );
    }
}
