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
#include "protocol/SimulationSenders.h"
#include "clients_kernel/Karma.h"
#include "rpr/EntityType.h"
#include "MockResolver.h"
#include "MockTeam.h"
#include "MockRemoteAgentSubject.h"
#include "MockContextHandler.h"
#include "MockExtentResolver.h"
#include "MockUnitTypeResolver.h"
#include "MockLogger.h"
#include "MockHlaObject.h"
#include "MockHlaClass.h"
#include "MockModel.h"
#include "MockTeam.h"
#include "MockComponentTypes.h"
#include "MockSideResolver.h"
#include "MockAgentSubject.h"
#include <boost/shared_ptr.hpp>
#include <boost/assign.hpp>
#include <boost/foreach.hpp>

using namespace plugins::hla;

namespace
{
    typedef std::vector< unsigned long > T_Identifiers;
    typedef boost::shared_ptr< dispatcher::MockTeam > T_Team;
    typedef std::vector< T_Team > T_Teams;

    class Fixture
    {
    public:
        Fixture()
            : automatCreationHandler( 0 )
            , unitCreationHandler   ( 0)
            , remoteClassListener   ( 0 )
            , party                 ( 42u )
            , latitude              ( 1. )
            , longitude             ( 2. )
            , automat               ( 1337 )
            , agentListener         ( 0 )
        {
            MOCK_EXPECT( logger.LogInfo );
            MOCK_EXPECT( logger.LogError );
            MOCK_EXPECT( logger.LogWarning );
            MOCK_EXPECT( remoteSubject.Register ).once().with( mock::retrieve( remoteClassListener ) );
            MOCK_EXPECT( automatCreation.Register ).once().with( mock::retrieve( automatCreationHandler ) );
            MOCK_EXPECT( unitCreation.Register ).once().with( mock::retrieve( unitCreationHandler ) );
            MOCK_EXPECT( agentSubject.Register ).once().with( mock::retrieve( agentListener ) );
            MOCK_EXPECT( automatCreation.Unregister );
            MOCK_EXPECT( unitCreation.Unregister );
            MOCK_EXPECT( remoteSubject.Unregister );
            MOCK_EXPECT( agentSubject.Unregister );
            //ConfigureTeams();
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
        void ConfigureTeams()
        {
            MOCK_EXPECT( teamResolver.CreateIterator ).once().returns( MakeTeamIterator( boost::assign::list_of( party ) ) );
            BOOST_FOREACH( T_Team team, teams )
                MOCK_EXPECT( team->GetKarma ).returns( kernel::Karma::friend_ );
        }
        MockRemoteAgentSubject remoteSubject;
        ClassListener_ABC* remoteClassListener;
        ResponseObserver_ABC< sword::AutomatCreation >* automatCreationHandler;
        ResponseObserver_ABC< sword::UnitCreation >* unitCreationHandler;
        MockContextHandler< sword::AutomatCreation > automatCreation;
        MockContextHandler< sword::UnitCreation > unitCreation;
        tools::MockResolver< dispatcher::Team_ABC > teamResolver;
        MockUnitTypeResolver unitTypeResolver;
        MockExtentResolver extentResolver;
        dispatcher::MockLogger logger;
        const unsigned long party;
        const double latitude;
        const double longitude;
        const unsigned long automat;
        T_Teams teams;
        MockSideResolver sideResolver;
        MockAgentSubject agentSubject;
        AgentListener_ABC* agentListener;
    };
    class AutomatFixture : public Fixture
    {
    public:
        AutomatFixture()
            : remoteController( remoteSubject, automatCreation, unitCreation, sideResolver, unitTypeResolver, logger, extentResolver, agentSubject )
        {
            BOOST_REQUIRE( automatCreationHandler );
            BOOST_REQUIRE( unitCreationHandler );
            BOOST_REQUIRE( remoteClassListener );
            BOOST_REQUIRE( agentListener );
            MOCK_EXPECT( object.Register ).once().with( mock::retrieve( remoteAgentListener ) );
            remoteClassListener->RemoteCreated( "identifier", hlaClass, object );
            BOOST_REQUIRE( remoteAgentListener );
            automatCreationHandler->Notify( MakeAutomatCreationMessage( party, automat ), "default_remote_automat" );
        }
        sword::AutomatCreation MakeAutomatCreationMessage( unsigned long party, unsigned long automat )
        {
            sword::AutomatCreation message;
            message.mutable_party()->set_id( party );
            message.mutable_automat()->set_id( automat );
            return message;
        }
        RemoteAgentController remoteController;
        ObjectListener_ABC* remoteAgentListener;
        MockHlaClass hlaClass;
        MockHlaObject object;
    };
}
// TODO leaks
BOOST_FIXTURE_TEST_CASE( remote_agent_controller_creates_agent_when_receiving_remote_creation_and_moved_events, AutomatFixture )
{
    const rpr::EntityType entityType( "1 2" );
    const unsigned long agentTypeId = 4343;
    //remoteClassListener->RemoteCreated( "identifier" );
    dispatcher::MockTeam team( 42 );
    MOCK_EXPECT( sideResolver.ResolveTeam ).once().with( rpr::Friendly ).returns( boost::cref( team ) );
    remoteAgentListener->SideChanged( "identifier", rpr::Friendly );
    remoteAgentListener->NameChanged( "identifier", "name" );
    MOCK_EXPECT( unitTypeResolver.Resolve ).once().with( mock::same( entityType ) ).returns( agentTypeId );
    remoteAgentListener->TypeChanged( "identifier", entityType );
    simulation::UnitMagicAction actual;
    MOCK_EXPECT( unitCreation.Send ).once().with( mock::retrieve( actual ), "identifier" );
    MOCK_EXPECT( extentResolver.IsInBoundaries ).once().returns( true );
    remoteAgentListener->Moved( "identifier", latitude, longitude );
    mock::verify();
    const sword::UnitMagicAction& action = actual();
    BOOST_CHECK_EQUAL( action.type(), sword::UnitMagicAction::unit_creation );
    BOOST_CHECK_EQUAL( action.tasker().automat().id(), automat );
    BOOST_CHECK_EQUAL( action.parameters().elem_size(), 5 );
    BOOST_CHECK_EQUAL( action.parameters().elem( 0 ).value( 0 ).identifier(), agentTypeId );
    const sword::Location& location = action.parameters().elem( 1 ).value( 0 ).point().location();
    BOOST_CHECK_EQUAL( location.type(), sword::Location::point );
    BOOST_CHECK_EQUAL( location.coordinates().elem_size(), 1 );
    BOOST_CHECK_EQUAL( location.coordinates().elem( 0 ).latitude(), latitude );
    BOOST_CHECK_EQUAL( location.coordinates().elem( 0 ).longitude(), longitude );
    BOOST_CHECK_EQUAL( action.parameters().elem( 2 ).value( 0 ).acharstr(), "name" );
}

BOOST_FIXTURE_TEST_CASE( remote_agent_controller_does_not_recreate_agent_after_second_moved_event, AutomatFixture )
{
    //remoteClassListener->RemoteCreated( "identifier" );
    dispatcher::MockTeam team( 42 );
    MOCK_EXPECT( sideResolver.ResolveTeam ).once().with( rpr::Friendly ).returns( boost::cref( team ) );
    remoteAgentListener->SideChanged( "identifier", rpr::Friendly );
    remoteAgentListener->NameChanged( "identifier", "name" );
    MOCK_EXPECT( unitTypeResolver.Resolve ).once().returns( 4242 );
    remoteAgentListener->TypeChanged( "identifier", rpr::EntityType() );
    MOCK_EXPECT( unitCreation.Send ).once();
    MOCK_EXPECT( extentResolver.IsInBoundaries ).once().returns( true );
    remoteAgentListener->Moved( "identifier", latitude, longitude );
    mock::verify();
    remoteAgentListener->Moved( "identifier", latitude, longitude );
}

BOOST_FIXTURE_TEST_CASE( remote_agent_controller_creates_out_of_bounds_agent_only_when_moving_inside_extent, AutomatFixture )
{
    //remoteClassListener->RemoteCreated( "identifier" );
    dispatcher::MockTeam team( 42 );
    MOCK_EXPECT( sideResolver.ResolveTeam ).once().with( rpr::Friendly ).returns( boost::cref( team ) );
    remoteAgentListener->SideChanged( "identifier", rpr::Friendly );
    remoteAgentListener->NameChanged( "identifier", "name" );
    MOCK_EXPECT( unitTypeResolver.Resolve ).once().returns( 4242 );
    remoteAgentListener->TypeChanged( "identifier", rpr::EntityType() );
    MOCK_EXPECT( extentResolver.IsInBoundaries ).once().returns( false );
    remoteAgentListener->Moved( "identifier", latitude, longitude );
    mock::verify();
    MOCK_EXPECT( extentResolver.IsInBoundaries ).once().returns( true );
    MOCK_EXPECT( unitCreation.Send ).once();
    remoteAgentListener->Moved( "identifier", latitude, longitude );
}
/*
BOOST_FIXTURE_TEST_CASE( remote_agent_controller_throws_if_distant_party_does_not_exist_in_sword, AutomatFixture )
{
    //remoteClassListener->RemoteCreated( "identifier" );
    BOOST_CHECK_THROW( remoteAgentListener->SideChanged( "identifier", rpr::Opposing ), std::runtime_error );
}
*/
