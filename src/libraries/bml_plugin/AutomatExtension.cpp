// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "bml_plugin_pch.h"
#include "AutomatExtension.h"
#include "PositionReport.h"
#include "OrderReport.h"
#include "ReportFactory.h"
#include "MT/MT_Logger/MT_Logger_lib.h"
#include <xeumeuleu/xml.h>
#include <iostream>

using namespace bml;

// -----------------------------------------------------------------------------
// Name: AutomatExtension constructor
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
AutomatExtension::AutomatExtension( dispatcher::Automat& holder, Publisher& publisher, const ReportFactory& factory )
    : holder_( holder )
    , publisher_( publisher )
    , factory_( factory )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AutomatExtension destructor
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
AutomatExtension::~AutomatExtension()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AutomatExtension::DoUpdate
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
void AutomatExtension::DoUpdate( const ASN1T_MsgAutomatOrder& message )
{
	try
	{
		std::auto_ptr< OrderReport > report( factory_.CreateOrderReport( holder_, message ) );
		report->Send( publisher_ );
	}
	catch( std::exception& e )
	{
		MT_LOG_ERROR_MSG( "BML error sending automat order report: " << e.what() );
	}
}
