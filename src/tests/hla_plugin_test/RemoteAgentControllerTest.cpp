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
#include "MockTeam.h"
#include <boost/shared_ptr.hpp>
#include <boost/assign.hpp>
#include <boost/foreach.hpp>

using namespace plugins::hla;

namespace
{
    typedef std::vector< unsigned long > T_Identifiers;
    typedef boost::shared_ptr< dispatcher::Team_ABC > T_Team;
    typedef std::vector< T_Team > T_Teams;

    class Fixture
    {
    public:
        Fixture()
            : controlEndTickHandler   ( 0 )
            , formationCreationHandler( 0 )
            , automatCreationHandler  ( 0 )
        {
            MOCK_EXPECT( messageController, Register ).once().with( mock::retrieve( controlEndTickHandler ) );
        }
        tools::Iterator< const dispatcher::Team_ABC& > MakeIterator( const T_Identifiers& identifiers )
        {
            BOOST_FOREACH( unsigned long identifier, identifiers )
                teams.push_back( T_Team( new dispatcher::MockTeam( identifier ) ) );
            tools::SimpleIterator< const dispatcher::Team_ABC&, T_Teams >* it = new tools::SimpleIterator< const dispatcher::Team_ABC&, T_Teams >( teams );
            return tools::Iterator< const dispatcher::Team_ABC& >( it );
        }
        tools::MockMessageController< sword::SimToClient_Content > messageController;
        dispatcher::MockModel model;
        dispatcher::MockSimulationPublisher publisher;
        tools::MessageHandler_ABC< sword::SimToClient_Content >* controlEndTickHandler;
        tools::MessageHandler_ABC< sword::SimToClient_Content >* formationCreationHandler;
        tools::MessageHandler_ABC< sword::SimToClient_Content >* automatCreationHandler;
        tools::MockResolver< dispatcher::Team_ABC > resolver;
        sword::SimToClient_Content message;
        T_Teams teams;
    };
}

BOOST_FIXTURE_TEST_CASE( remote_agent_controller_listen_to_control_end_tick_message, Fixture )
{
    RemoteAgentController remoteController( messageController, model, publisher );
    BOOST_REQUIRE( controlEndTickHandler );
    message.mutable_control_end_tick()->set_current_tick( 3 );
    MOCK_EXPECT( model, Sides ).once().returns( boost::ref( resolver ) );
    MOCK_EXPECT( resolver, CreateIterator ).once().returns( MakeIterator( T_Identifiers() ) );
    MOCK_EXPECT( messageController, Unregister ).once();
    MOCK_EXPECT( messageController, Register ).exactly( 2 );
    controlEndTickHandler->Notify( message, 42 );
    mock::verify();
    MOCK_EXPECT( messageController, Unregister ).exactly( 2 );
}

namespace
{
    class TickedFixture : public Fixture
    {
    public:
        TickedFixture()
            : remoteController( messageController, model, publisher )
        {
            BOOST_REQUIRE( controlEndTickHandler );
            MOCK_EXPECT( model, Sides ).once().returns( boost::ref( resolver ) );
            message.mutable_control_end_tick()->set_current_tick( 3 );
            MOCK_EXPECT( messageController, Unregister ).once();
            MOCK_EXPECT( messageController, Register ).exactly( 2 );
        }
        ~TickedFixture()
        {
            MOCK_EXPECT( messageController, Unregister ).exactly( 2 );
        }
        RemoteAgentController remoteController;
    };
}

BOOST_FIXTURE_TEST_CASE( remote_agent_controller_creates_distant_formation_for_each_party, TickedFixture )
{
    const unsigned long party = 42u;
    MOCK_EXPECT( resolver, CreateIterator ).once().returns( MakeIterator( boost::assign::list_of( party ) ) );
    sword::ClientToSim actual;
    MOCK_EXPECT( publisher, SendClientToSim ).once().with( mock::retrieve( actual ) );
    controlEndTickHandler->Notify( message, 41 );
    mock::verify();
    BOOST_CHECK( actual.message().has_unit_magic_action() );
    const sword::UnitMagicAction& action = actual.message().unit_magic_action();
    BOOST_CHECK_EQUAL( action.type(), sword::UnitMagicAction::formation_creation );
    BOOST_CHECK_EQUAL( action.tasker().party().id(), party );
    BOOST_CHECK_EQUAL( action.parameters().elem_size(), 3 );
    BOOST_CHECK_EQUAL( action.parameters().elem( 0 ).value( 0 ).areal(), 6 );
    BOOST_CHECK_EQUAL( action.parameters().elem( 1 ).value( 0 ).acharstr(), "HLA distant formation" );
    BOOST_CHECK( action.parameters().elem( 2 ).null_value() );
}
