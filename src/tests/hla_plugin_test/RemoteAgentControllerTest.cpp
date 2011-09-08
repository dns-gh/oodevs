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
#include "dispatcher/KnowledgeGroup_ABC.h"
#include "protocol/Simulation.h"
#include "clients_kernel/Karma.h"
#include "clients_kernel/AutomatType.h" // $$$$ _RC_ SLI 2011-09-08: refactor clients_kernel to remove this
#include "MockMessageController.h"
#include "MockModel.h"
#include "MockSimulationPublisher.h"
#include "MockResolver.h"
#include "MockTeam.h"
#include "MockKnowledgeGroup.h"
#include "MockRemoteAgentSubject.h"
#include <boost/shared_ptr.hpp>
#include <boost/assign.hpp>
#include <boost/foreach.hpp>

using namespace plugins::hla;

namespace
{
    typedef std::vector< unsigned long > T_Identifiers;
    typedef boost::shared_ptr< dispatcher::MockTeam > T_Team;
    typedef std::vector< T_Team > T_Teams;
    typedef boost::shared_ptr< dispatcher::MockKnowledgeGroup > T_KnowledgeGroup;
    typedef std::vector< T_KnowledgeGroup > T_KnowledgeGroups;

    class Fixture
    {
    public:
        Fixture()
            : controlEndTickHandler   ( 0 )
            , formationCreationHandler( 0 )
            , automatCreationHandler  ( 0 )
            , remoteAgentListener     ( 0 )
        {
            MOCK_EXPECT( messageController, Register ).once().with( mock::retrieve( controlEndTickHandler ) );
            MOCK_EXPECT( automatResolver, Find ).once().returns( reinterpret_cast< kernel::AutomatType* >( 1 ) );
            MOCK_EXPECT( remoteSubject, Register ).once().with( mock::retrieve( remoteAgentListener ) );
        }
        template< typename T_Result, typename T_Mock, typename T_Vector >
        tools::Iterator< const T_Result& > MakeIterator( const T_Identifiers& identifiers, T_Vector& elements )
        {
            BOOST_FOREACH( unsigned long identifier, identifiers )
                elements.push_back( boost::shared_ptr< T_Mock >( new T_Mock( identifier ) ) );
            tools::SimpleIterator< const T_Result&, T_Vector >* it = new tools::SimpleIterator< const T_Result&, T_Vector >( elements );
            return tools::Iterator< const T_Result& >( it );
        }
        tools::Iterator< const dispatcher::Team_ABC& > MakeTeamIterator( const T_Identifiers& identifiers )
        {
            return MakeIterator< dispatcher::Team_ABC, dispatcher::MockTeam, T_Teams >( identifiers, teams );
        }
        tools::Iterator< const dispatcher::KnowledgeGroup_ABC& > MakeKnowledgeGroupIterator( const T_Identifiers& identifiers )
        {
            return MakeIterator< dispatcher::KnowledgeGroup_ABC, dispatcher::MockKnowledgeGroup, T_KnowledgeGroups >( identifiers, groups );
        }
        sword::SimToClient_Content MakeControlEndTickMessage()
        {
            sword::SimToClient_Content message;
            message.mutable_control_end_tick()->set_current_tick( 3 );
            return message;
        }
        tools::MockMessageController< sword::SimToClient_Content > messageController;
        dispatcher::MockModel model;
        dispatcher::MockSimulationPublisher publisher;
        MockRemoteAgentSubject remoteSubject;
        RemoteAgentListener_ABC* remoteAgentListener;
        tools::MessageHandler_ABC< sword::SimToClient_Content >* controlEndTickHandler;
        tools::MessageHandler_ABC< sword::SimToClient_Content >* formationCreationHandler;
        tools::MessageHandler_ABC< sword::SimToClient_Content >* automatCreationHandler;
        tools::MockResolver< dispatcher::Team_ABC > teamResolver;
        tools::MockResolver< kernel::AutomatType > automatResolver;
        T_Teams teams;
        T_KnowledgeGroups groups;
    };
}

BOOST_FIXTURE_TEST_CASE( remote_agent_controller_listen_to_control_end_tick_message, Fixture )
{
    RemoteAgentController remoteController( messageController, model, automatResolver, publisher, remoteSubject );
    BOOST_REQUIRE( controlEndTickHandler );
    MOCK_EXPECT( model, Sides ).once().returns( boost::ref( teamResolver ) );
    MOCK_EXPECT( teamResolver, CreateIterator ).once().returns( MakeTeamIterator( T_Identifiers() ) );
    MOCK_EXPECT( messageController, Unregister ).once();
    MOCK_EXPECT( messageController, Register ).exactly( 2 );
    controlEndTickHandler->Notify( MakeControlEndTickMessage(), 42 );
    mock::verify();
    MOCK_EXPECT( messageController, Unregister ).exactly( 2 );
    MOCK_EXPECT( remoteSubject, Unregister ).once();
}

BOOST_FIXTURE_TEST_CASE( remote_agent_checks_remote_automat_type_id_existence, Fixture )
{
    messageController.reset();
    automatResolver.reset();
    remoteSubject.reset();
    MOCK_EXPECT( automatResolver, Find ).once().returns( 0 );
    BOOST_CHECK_THROW( RemoteAgentController remoteController( messageController, model, automatResolver, publisher, remoteSubject ), std::runtime_error );
}

namespace
{
    class TickedFixture : public Fixture
    {
    public:
        TickedFixture()
            : remoteController( messageController, model, automatResolver, publisher, remoteSubject )
        {
            BOOST_REQUIRE( controlEndTickHandler );
            MOCK_EXPECT( model, Sides ).once().returns( boost::ref( teamResolver ) );
            MOCK_EXPECT( messageController, Unregister ).once();
            MOCK_EXPECT( messageController, Register ).once().with( mock::retrieve( formationCreationHandler ) );
            MOCK_EXPECT( messageController, Register ).once().with( mock::retrieve( automatCreationHandler ) );
        }
        virtual ~TickedFixture()
        {
            MOCK_EXPECT( messageController, Unregister ).exactly( 2 );
            MOCK_EXPECT( remoteSubject, Unregister ).once();
        }
        RemoteAgentController remoteController;
    };
}

BOOST_FIXTURE_TEST_CASE( remote_agent_controller_creates_distant_formation_for_each_party, TickedFixture )
{
    const unsigned long party = 42u;
    MOCK_EXPECT( teamResolver, CreateIterator ).once().returns( MakeTeamIterator( boost::assign::list_of( party ) ) );
    sword::ClientToSim actual;
    MOCK_EXPECT( publisher, SendClientToSim ).once().with( mock::retrieve( actual ) );
    controlEndTickHandler->Notify( MakeControlEndTickMessage(), 41 );
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

namespace
{
    sword::SimToClient_Content MakeFormationCreationMessage( unsigned long party, unsigned long formation )
    {
        sword::SimToClient_Content message;
        message.mutable_formation_creation()->mutable_party()->set_id( party );
        message.mutable_formation_creation()->mutable_formation()->set_id( formation );
        return message;
    }
    sword::SimToClient_Content MakeAutomatCreationMessage( unsigned long party, unsigned long automat )
    {
        sword::SimToClient_Content message;
        message.mutable_automat_creation()->mutable_party()->set_id( party );
        message.mutable_automat_creation()->mutable_automat()->set_id( automat );
        return message;
    }
    class FormationFixture : public TickedFixture
    {
    public:
        FormationFixture()
            : party    ( 42u )
            , formation( 43u )
            , team     ( party )
        {
            MOCK_EXPECT( teamResolver, CreateIterator ).once().returns( MakeTeamIterator( boost::assign::list_of( party ) ) );
            MOCK_EXPECT( publisher, SendClientToSim ).once().with( mock::retrieve( formationCreationMessage ) );
            controlEndTickHandler->Notify( MakeControlEndTickMessage(), 41 );
            mock::verify();
            BOOST_REQUIRE( formationCreationHandler );
        }
        void ConfigureKnowledgeGroups()
        {
            MOCK_EXPECT( model, KnowledgeGroups ).once().returns( boost::ref( knowledgeGroupResolver ) );
            MOCK_EXPECT( knowledgeGroupResolver, CreateIterator ).once().returns( MakeKnowledgeGroupIterator( boost::assign::list_of( party ) ) );
            BOOST_FOREACH( T_KnowledgeGroup group, groups )
                MOCK_EXPECT( *group, GetTeam ).once().returns( boost::ref( team ) );
        }
        const unsigned long party;
        const unsigned long formation;
        sword::ClientToSim formationCreationMessage;
        tools::MockResolver< dispatcher::KnowledgeGroup_ABC > knowledgeGroupResolver;
        dispatcher::MockTeam team;
    };
}

BOOST_FIXTURE_TEST_CASE( remote_agent_controller_does_nothing_when_receiving_unwanted_formation_creation, FormationFixture )
{
    formationCreationHandler->Notify( MakeFormationCreationMessage( party, formation ), formationCreationMessage.context() + 1000 );
}

BOOST_FIXTURE_TEST_CASE( remote_agent_controller_creates_distant_automat_when_receiving_formation_creation, FormationFixture )
{
    ConfigureKnowledgeGroups();
    sword::ClientToSim actual;
    MOCK_EXPECT( publisher, SendClientToSim ).once().with( mock::retrieve( actual ) );
    formationCreationHandler->Notify( MakeFormationCreationMessage( party, formation ), formationCreationMessage.context() );
    mock::verify();
    BOOST_CHECK( actual.message().has_unit_magic_action() );
    const sword::UnitMagicAction& action = actual.message().unit_magic_action();
    BOOST_CHECK_EQUAL( action.type(), sword::UnitMagicAction::automat_creation );
    BOOST_CHECK_EQUAL( action.tasker().formation().id(), formation );
    BOOST_CHECK_EQUAL( action.parameters().elem_size(), 2 );
    BOOST_CHECK_EQUAL( action.parameters().elem( 0 ).value( 0 ).identifier(), 230u );  // $$$$ _RC_ SLI 2011-09-07: hardcoded
    BOOST_CHECK_EQUAL( action.parameters().elem( 1 ).value( 0 ).identifier(), party );
}

namespace
{
    class AutomatFixture : public FormationFixture
    {
    public:
        AutomatFixture()
            : latitude ( 1. )
            , longitude( 2. )
            , automat  ( 1337 )
        {
            ConfigureKnowledgeGroups();
            MOCK_EXPECT( publisher, SendClientToSim ).once().with( mock::retrieve( automatCreationMessage ) );
            formationCreationHandler->Notify( MakeFormationCreationMessage( party, formation ), formationCreationMessage.context() );
            mock::verify();
            BOOST_REQUIRE( automatCreationHandler );
            automatCreationHandler->Notify( MakeAutomatCreationMessage( party, automat ), automatCreationMessage.context() );
            ConfigureTeams();
            BOOST_REQUIRE( remoteAgentListener );
       }
        void ConfigureTeams()
        {
            teams.clear();
            MOCK_EXPECT( model, Sides ).once().returns( boost::ref( teamResolver ) );
            MOCK_EXPECT( teamResolver, CreateIterator ).once().returns( MakeTeamIterator( boost::assign::list_of( party ) ) );
            BOOST_FOREACH( T_Team team, teams )
                MOCK_EXPECT( *team, GetKarma ).once().returns( kernel::Karma::friend_ );
        }
        const double latitude;
        const double longitude;
        const unsigned long automat;
        sword::ClientToSim automatCreationMessage;
    };
}

BOOST_FIXTURE_TEST_CASE( remote_agent_controller_creates_agent_when_receiving_remote_creation_and_moved_events, AutomatFixture )
{
    remoteAgentListener->Created( "identifier" );
    remoteAgentListener->SideChanged( "identifier", rpr::Friendly );
    sword::ClientToSim actual;
    MOCK_EXPECT( publisher, SendClientToSim ).once().with( mock::retrieve( actual ) );
    remoteAgentListener->Moved( "identifier", latitude, longitude );
    mock::verify();
    BOOST_CHECK( actual.message().has_unit_creation_request() );
    const sword::UnitCreationRequest& request = actual.message().unit_creation_request();
    BOOST_CHECK_EQUAL( request.position().latitude(), latitude );
    BOOST_CHECK_EQUAL( request.position().longitude(), longitude );
    BOOST_CHECK_EQUAL( request.superior().id(), automat );
}

BOOST_FIXTURE_TEST_CASE( remote_agent_controller_does_not_recreate_agent_after_second_moved_event, AutomatFixture )
{
    remoteAgentListener->Created( "identifier" );
    remoteAgentListener->SideChanged( "identifier", rpr::Friendly );
    MOCK_EXPECT( publisher, SendClientToSim ).once();
    remoteAgentListener->Moved( "identifier", latitude, longitude );
    mock::verify();
    remoteAgentListener->Moved( "identifier", latitude, longitude );
}
