// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_test_pch.h"
#include "hla_plugin/RemoteAgentController.h"
#include "tools/MessageHandler_ABC.h"
#include "tools/Resolver_ABC.h"
#include "dispatcher/Team_ABC.h"
#include "protocol/Simulation.h"
#include "MockMessageController.h"
#include "MockModel.h"
#include "MockSimulationPublisher.h"
#include "MockResolver.h"

using namespace plugins::hla;

BOOST_AUTO_TEST_CASE( remote_agent_controller_listen_to_control_end_tick_message )
{
    tools::MockMessageController< sword::SimToClient_Content > messageController;
    dispatcher::MockModel model;
    dispatcher::MockSimulationPublisher publisher;
    tools::MessageHandler_ABC< sword::SimToClient_Content >* controlEndTickHandler = 0;
    MOCK_EXPECT( messageController, Register ).once().with( mock::retrieve( controlEndTickHandler ) );
    RemoteAgentController remoteController( messageController, model, publisher );
    BOOST_REQUIRE( controlEndTickHandler );
    sword::SimToClient_Content message;
    message.mutable_control_end_tick()->set_current_tick( 3 );
    tools::MockResolver< dispatcher::Team_ABC > resolver;
    MOCK_EXPECT( model, Sides ).once().returns( boost::ref( resolver ) );
    tools::NullIterator< const dispatcher::Team_ABC& >* it = new tools::NullIterator< const dispatcher::Team_ABC& >();
    MOCK_EXPECT( resolver, CreateIterator ).once().returns( tools::Iterator< const dispatcher::Team_ABC& >( it ) );
    MOCK_EXPECT( messageController, Unregister ).once();
    MOCK_EXPECT( messageController, Register ).exactly( 2 );
    controlEndTickHandler->Notify( message, 42 );
    mock::verify();
    MOCK_EXPECT( messageController, Unregister ).exactly( 2 );
}
