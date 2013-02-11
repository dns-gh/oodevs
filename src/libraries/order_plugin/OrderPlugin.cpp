// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "OrderPlugin.h"

#include "clients_kernel/CoordinateConverter.h"
#include "clients_kernel/XmlAdapter.h"
#include "dispatcher/Config.h"
#include "dispatcher/ModelAdapter.h"
#include "dispatcher/SimulationPublisher_ABC.h"
#include "MT_Tools/MT_Logger.h"
#include "protocol/Simulation.h"
#include "protocol/XmlReaders.h"
#include "tools/Exception.h"

#include <xeumeuleu/xml.h>

using namespace plugins::order;

// -----------------------------------------------------------------------------
// Name: OrderPlugin constructor
// Created: LGY 2011-09-05
// -----------------------------------------------------------------------------
OrderPlugin::OrderPlugin( const dispatcher::Config& config,
                          const dispatcher::Model_ABC& model,
                          dispatcher::SimulationPublisher_ABC& publisher )
    : publisher_( publisher )
    , converter_( new kernel::CoordinateConverter( config ) )
    , entities_ ( new dispatcher::ModelAdapter( model ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: OrderPlugin destructor
// Created: LGY 2011-09-05
// -----------------------------------------------------------------------------
OrderPlugin::~OrderPlugin()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: OrderPlugin::Resolve
// Created: LGY 2011-09-05
// -----------------------------------------------------------------------------
void OrderPlugin::Resolve( const sword::ClientToSim& message )
{
    if( !message.message().has_order_stream() )
        return;
    try
    {
        xml::xistringstream xis( message.message().order_stream().serialized_order() );
        xis >> xml::start( "actions" )
                >> xml::list( "action", *this, &OrderPlugin::ReadAction );
    }
    catch( const std::exception& e )
    {
        MT_LOG_ERROR_MSG( "Error in OrderStream message: " << tools::GetExceptionMsg( e ) );
    }
}

// -----------------------------------------------------------------------------
// Name: OrderPlugin::ReadAction
// Created: LGY 2011-09-05
// -----------------------------------------------------------------------------
void OrderPlugin::ReadAction( xml::xistream& xis )
{
    try
    {
        sword::ClientToSim msg;
        msg.set_context( 0 );
        protocol::Read( kernel::XmlAdapter( *converter_, *entities_ ), *msg.mutable_message(), xis );
        publisher_.Send( msg );
    }
    catch( const std::exception& err )
    {
        MT_LOG_ERROR_MSG( "Unable to process action: " << tools::GetExceptionMsg( err ) );
    }
}