// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "bml_plugin_pch.h"
#include "OrderProcessor.h"
#include "Mission.h"
#include <xeumeuleu/xml.h>

using namespace bml;

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
    xml::xistringstream xis( response );
    xis >> xml::list( "OrderPush", *this, &OrderProcessor::ReadOrder );
}

// -----------------------------------------------------------------------------
// Name: OrderProcessor::ReadOrder
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
void OrderProcessor::ReadOrder( xml::xistream& xis )
{
    xis >> xml::list( "Task", *this, &OrderProcessor::ReadTask );
}

// -----------------------------------------------------------------------------
// Name: OrderProcessor::ReadTask
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
void OrderProcessor::ReadTask( xml::xistream& xis )
{
    xis >> xml::list( "GroundTask", *this, &OrderProcessor::ReadGroundTask );
}

// -----------------------------------------------------------------------------
// Name: OrderProcessor::ReadGroundTask
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
void OrderProcessor::ReadGroundTask( xml::xistream& xis )
{
    Mission mission( xis, model_ );
    mission.Send( publisher_ );
}
