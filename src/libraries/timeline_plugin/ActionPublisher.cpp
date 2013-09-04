// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "timeline_plugin_pch.h"
#include "ActionPublisher.h"
#include "clients_kernel/CoordinateConverter.h"
#include "clients_kernel/XmlAdapter.h"
#include "dispatcher/SimulationPublisher_ABC.h"
#include "dispatcher/ModelAdapter.h"
#include "MT_Tools/MT_Logger.h"
#include "tools/Exception.h"
#include "protocol/Simulation.h"
#include "protocol/XmlReaders.h"

using namespace plugins::timeline;

// -----------------------------------------------------------------------------
// Created: JCR 2010-09-07
// -----------------------------------------------------------------------------
ActionPublisher::ActionPublisher( const tools::ExerciseConfig& config, const dispatcher::Model_ABC& model, dispatcher::SimulationPublisher_ABC& sim )
    : publisher_( sim )
    , entities_ ( new dispatcher::ModelAdapter( model ) )
    , converter_( new kernel::CoordinateConverter( config ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionPublisher destructor
// Created: JCR 2010-09-08
// -----------------------------------------------------------------------------
ActionPublisher::~ActionPublisher()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionPublisher::Execute
// Created: JCR 2010-09-07
// -----------------------------------------------------------------------------
void ActionPublisher::Execute( xml::xistream& xis )
{
    try
    {
        sword::ClientToSim msg;
        msg.set_context( 0 );
        protocol::Read( kernel::XmlAdapter( *converter_, *entities_ ),
            *msg.mutable_message(), xis );
        publisher_.Send( msg );
    }
    catch( const std::exception& err )
    {
        MT_LOG_ERROR_MSG( "Unable to process action: " << tools::GetExceptionMsg( err ) );
    }
}
