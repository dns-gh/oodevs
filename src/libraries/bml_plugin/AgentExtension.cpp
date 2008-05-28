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
#include "ReportFactory.h"
#include "MT/MT_Logger/MT_Logger_lib.h"
#include <xeumeuleu/xml.h>
#include <iostream>

using namespace bml;

// -----------------------------------------------------------------------------
// Name: AgentExtension constructor
// Created: SBO 2008-02-29
// -----------------------------------------------------------------------------
AgentExtension::AgentExtension( dispatcher::Agent& holder, Publisher& publisher, const ReportFactory& factory )
    : holder_( holder )
    , publisher_( publisher )
    , factory_( factory )
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
void AgentExtension::DoUpdate( const ASN1T_MsgUnitAttributes& attributes )
{
    if( attributes.m.positionPresent || attributes.m.hauteurPresent )
    {
		try
		{
			PositionReport report( holder_ );
			report.Send( publisher_ );
		}
		catch( std::exception& e )
		{
			MT_LOG_ERROR_MSG( "BML error sending position report: " << e.what() );
		}
    }
}

// -----------------------------------------------------------------------------
// Name: AgentExtension::DoUpdate
// Created: SBO 2008-05-16
// -----------------------------------------------------------------------------
void AgentExtension::DoUpdate( const ASN1T_MsgUnitOrder& message )
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
