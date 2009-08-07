// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "xmliaImport_plugin_pch.h"
#include "OrderProcessor.h"
//#include "Mission.h"
//#include "SerializationTools.h"
#include <xeumeuleu/xml.h>
#include "MT/MT_Logger/MT_Logger_lib.h"

using namespace plugins::xmliaImport;

// -----------------------------------------------------------------------------
// Name: OrderProcessor constructor
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
OrderProcessor::OrderProcessor( const dispatcher::Model& model, dispatcher::SimulationPublisher_ABC& publisher )
    : model_( model )
    , publisher_( publisher )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: OrderProcessor destructor
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
OrderProcessor::~OrderProcessor()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: OrderProcessor::Handle
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
void OrderProcessor::Handle( const std::string& response )
{
    /*xml::xistringstream xis( response );
    xis >> xml::start( NS( "OrderPush", "cbml" ) )
            >> xml::list( NS( "OrderPush", "cbml" ), *this, &OrderProcessor::ReadOrder )
        >> xml::end();*/
}

// -----------------------------------------------------------------------------
// Name: OrderProcessor::ReadOrder
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
void OrderProcessor::ReadOrder( xml::xistream& xis )
{
   // xis >> xml::list( NS( "Task", "cbml" ), *this, &OrderProcessor::ReadTask );
}

// -----------------------------------------------------------------------------
// Name: OrderProcessor::ReadTask
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
void OrderProcessor::ReadTask( xml::xistream& xis )
{
    //xis >> xml::list( NS( "GroundTask", "cbml" ), *this, &OrderProcessor::ReadGroundTask );
}

// -----------------------------------------------------------------------------
// Name: OrderProcessor::ReadGroundTask
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
void OrderProcessor::ReadGroundTask( xml::xistream& xis )
{
    /*try
    {
        xml::xisubstream sub( xis );
        Mission mission( sub, model_ );
        mission.Send( publisher_ );
    }
    catch( std::exception& e)
    {
        MT_LOG_ERROR_MSG( "BML translating mission: " << e.what() );
    }*/
}
