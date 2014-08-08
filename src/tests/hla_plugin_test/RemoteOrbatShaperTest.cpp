// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_test_pch.h"
#include "hla_plugin/RemoteOrbatShaper.h"

#include "MockContextHandler.h"
#include "MockRemoteAgentSubject.h"
#include "MockSideResolver.h"
#include "MockResolver.h"
#include "MockSimulationPublisher.h"
#include "MockTeam.h"
#include "MockKnowledgeGroup.h"
#include "MockHlaClass.h"
#include "MockHlaObject.h"
#include "MockUnitTypeResolver.h"

#include "protocol/Simulation.h"
#include "protocol/SimulationSenders.h"

#include <boost/shared_ptr.hpp>
#include <boost/assign.hpp>
#include <boost/foreach.hpp>

using namespace plugins::hla;

namespace
{
    struct Fixture
    {
        typedef std::vector< unsigned long > T_Identifiers;
        typedef boost::shared_ptr< dispatcher::MockKnowledgeGroup > T_KnowledgeGroup;
        typedef std::vector< T_KnowledgeGroup > T_KnowledgeGroups;

        Fixture()
            : remoteClassListener( 0 )
            , formationCreationHandler( 0 )
            , automatCreationHandler( 0 )
            , unitCreationHandler( 0 )
            , team( 42ul )
        {
            MOCK_EXPECT( remoteSubject.Register ).once().with( mock::retrieve( remoteClassListener ) );
            MOCK_EXPECT( formationCreation.Register ).once().with( mock::retrieve( formationCreationHandler ) );
            MOCK_EXPECT( automatCreation.Register ).once().with( mock::retrieve( automatCreationHandler ) );
            MOCK_EXPECT( unitCreation.Register ).once().with( mock::retrieve( unitCreationHandler ) );
            ConfigureKnowledgeGroups();
            // destruction
            MOCK_EXPECT( unitCreation.Unregister );
            MOCK_EXPECT( automatCreation.Unregister );
            MOCK_EXPECT( formationCreation.Unregister );
            MOCK_EXPECT( remoteSubject.Unregister );
        }

        MockRemoteAgentSubject remoteSubject;
        MockContextHandler< sword::FormationCreation > formationCreation;
        MockContextHandler< sword::AutomatCreation > automatCreation;
        MockContextHandler< sword::UnitCreation > unitCreation;
        MockSideResolver sideResolver;
        tools::MockResolver< dispatcher::KnowledgeGroup_ABC > knowledgeGroupResolver;
        dispatcher::MockSimulationPublisher simulationPublisher;
        MockUnitTypeResolver automatTypeResolver;
        
        dispatcher::MockTeam team;
        T_KnowledgeGroups groups;
        
        ClassListener_ABC* remoteClassListener;
        ResponseObserver_ABC< sword::FormationCreation >* formationCreationHandler;
        ResponseObserver_ABC< sword::AutomatCreation >* automatCreationHandler;
        ResponseObserver_ABC< sword::UnitCreation >* unitCreationHandler;
    private:
        template< typename T_Result, typename T_Mock, typename T_Vector >
        tools::Iterator< const T_Result& > MakeIterator( const T_Identifiers& identifiers, T_Vector& elements )
        {
            BOOST_FOREACH( unsigned long identifier, identifiers )
                elements.push_back( boost::make_shared< T_Mock >( identifier ) );
            tools::SimpleIterator< const T_Result&, T_Vector >* it = new tools::SimpleIterator< const T_Result&, T_Vector >( elements );
            return tools::Iterator< const T_Result& >( it );
        }
        tools::Iterator< const dispatcher::KnowledgeGroup_ABC& > MakeKnowledgeGroupIterator( const T_Identifiers& identifiers )
        {
            return MakeIterator< dispatcher::KnowledgeGroup_ABC, dispatcher::MockKnowledgeGroup, T_KnowledgeGroups >( identifiers, groups );
        }
        void ConfigureKnowledgeGroups()
        {
            MOCK_EXPECT( knowledgeGroupResolver.CreateIterator ).once().returns( MakeKnowledgeGroupIterator( boost::assign::list_of( 42 ) ) );
            BOOST_FOREACH( T_KnowledgeGroup group, groups )
                MOCK_EXPECT( group->GetTeam ).once().returns( boost::ref( team ) );
        }
    };
    struct ShaperFixture : Fixture
    {
        ShaperFixture() 
            :   formationId_( 18 ) 
            ,   orbatShaper( remoteSubject, formationCreation, automatCreation, unitCreation, sideResolver, knowledgeGroupResolver, simulationPublisher, automatTypeResolver )
        {
            BOOST_REQUIRE( remoteClassListener );
            BOOST_REQUIRE( formationCreationHandler );
            BOOST_REQUIRE( automatCreationHandler );
            BOOST_REQUIRE( unitCreationHandler );
            sword::FormationCreation defMsg;
            defMsg.mutable_formation()->set_id( formationId_ );
            defMsg.mutable_party()->set_id( team.GetId() );
            formationCreationHandler->Notify( defMsg, "default_remote_formation" );
        }
        unsigned int formationId_;
        RemoteOrbatShaper orbatShaper;
        
    };
}

BOOST_FIXTURE_TEST_CASE( remote_orbat_shaper_listens_remote_creation, ShaperFixture )
{
    MockHlaClass clazz;
    MockHlaObject object;
    ObjectListener_ABC* objectListener = 0;
    MOCK_EXPECT( object.Register ).once().with( mock::retrieve( objectListener ) );
    remoteClassListener->RemoteCreated( "identifier", clazz, object );
    BOOST_REQUIRE( objectListener );
}

BOOST_FIXTURE_TEST_CASE( remote_orbat_shaper_creates_automat, ShaperFixture )
{
    static const std::string childRtiId( "child_identifier" ), parentRtiId( "parent_identifier" );
    MockHlaClass clazz;
    MockHlaObject child, parent;
    ObjectListener_ABC *childListener = 0, *parentListener = 0;

    MOCK_EXPECT( child.Register ).once().with( mock::retrieve( childListener ) );
    remoteClassListener->RemoteCreated( childRtiId, clazz, child );
    BOOST_REQUIRE( childListener );
    childListener->ParentChanged( childRtiId, parentRtiId );
    MOCK_EXPECT( sideResolver.ResolveTeam ).once().with( rpr::Friendly ).returns( team.GetId() );
    childListener->SideChanged( childRtiId, rpr::Friendly );

    MOCK_EXPECT( parent.Register ).once().with( mock::retrieve( parentListener ) );
    remoteClassListener->RemoteCreated( parentRtiId, clazz, parent );
    BOOST_REQUIRE( parentListener );
    parentListener->NameChanged( parentRtiId, "parent" );
    MOCK_EXPECT( sideResolver.ResolveTeam ).once().with( rpr::Friendly ).returns( team.GetId() );
    parentListener->SideChanged( parentRtiId, rpr::Friendly );
    parentListener->TypeChanged( parentRtiId, rpr::EntityType( "1 2 3 4 5 6 7" ) );

    sword::UnitCreation message;
    message.mutable_unit()->set_id( 52 );
    simulation::UnitMagicAction actual;
    MOCK_EXPECT( automatTypeResolver.Resolve ).once().with( rpr::EntityType( "1 2 3 4 5 6 7" ) ).returns( 33u );
    MOCK_EXPECT( automatCreation.Send ).once().with( mock::retrieve( actual ), parentRtiId );
    unitCreationHandler->Notify( message, childRtiId );
    
    const sword::UnitMagicAction& action = actual();
    BOOST_CHECK_EQUAL( action.type(), sword::UnitMagicAction::automat_creation );
    BOOST_CHECK_EQUAL( action.tasker().formation().id(), formationId_ );    
    BOOST_CHECK_EQUAL( action.parameters().elem_size(), 4 );
    BOOST_CHECK_EQUAL( action.parameters().elem( 0 ).value( 0 ).identifier(), 33u );
    BOOST_CHECK_EQUAL( action.parameters().elem( 1 ).value( 0 ).identifier(), 42u );
    BOOST_CHECK_EQUAL( action.parameters().elem( 2 ).value( 0 ).acharstr(), "parent" );
    BOOST_CHECK( action.parameters().elem( 3 ).value( 0 ).has_extensionlist() );
    BOOST_CHECK_EQUAL( action.parameters().elem( 3 ).value( 0 ).extensionlist().entries_size(), 2 );
}

namespace
{
    struct AutomatFixture : ShaperFixture
    {
        AutomatFixture()
            : unitSimId( 52 )
            , childRtiId( "child_identifier" )
            , parentRtiId( "parent_identifier" )
            , formationRtiId( "formation_identifier" ) 
            , childListener( 0 )
            , parentListener( 0 )
        {
            MOCK_EXPECT( child.Register ).once().with( mock::retrieve( childListener ) );
            remoteClassListener->RemoteCreated( childRtiId, clazz, child );
            BOOST_REQUIRE( childListener );
            childListener->ParentChanged( childRtiId, parentRtiId );
            MOCK_EXPECT( sideResolver.ResolveTeam ).once().with( rpr::Friendly ).returns( team.GetId() );
            childListener->SideChanged( childRtiId, rpr::Friendly );

            MOCK_EXPECT( parent.Register ).once().with( mock::retrieve( parentListener ) );
            remoteClassListener->RemoteCreated( parentRtiId, clazz, parent );
            BOOST_REQUIRE( parentListener );
            parentListener->NameChanged( parentRtiId, "parent" );
            MOCK_EXPECT( sideResolver.ResolveTeam ).once().with( rpr::Friendly ).returns( team.GetId() );
            parentListener->SideChanged( parentRtiId, rpr::Friendly );
            parentListener->TypeChanged( parentRtiId, rpr::EntityType( "1 2 3 4 5 6 7" ) );

            sword::UnitCreation message;
            message.mutable_unit()->set_id( unitSimId );
            simulation::UnitMagicAction actual;
            MOCK_EXPECT( automatTypeResolver.Resolve ).once().with( rpr::EntityType( "1 2 3 4 5 6 7" ) ).returns( 33u );
            MOCK_EXPECT( automatCreation.Send ).once().with( mock::retrieve( actual ), parentRtiId );
            unitCreationHandler->Notify( message, childRtiId );
        }
        const unsigned int unitSimId;
        const std::string childRtiId, parentRtiId, formationRtiId;
        MockHlaClass clazz;
        MockHlaObject child, parent;
        ObjectListener_ABC *childListener, *parentListener;
    };
}

BOOST_FIXTURE_TEST_CASE( remote_orbat_shaper_moves_child_unit, AutomatFixture )
{
    sword::AutomatCreation message;
    message.mutable_automat()->set_id( 78u );

    sword::ClientToSim actual;
    MOCK_EXPECT( simulationPublisher.SendClientToSim ).once().with( mock::retrieve( actual ) );
    automatCreationHandler->Notify( message, parentRtiId );
    BOOST_REQUIRE( actual.message().has_unit_magic_action() );
    const sword::UnitMagicAction& action = actual.message().unit_magic_action();
    BOOST_CHECK_EQUAL( action.type(), sword::UnitMagicAction::unit_change_superior );
    BOOST_CHECK_EQUAL( action.tasker().unit().id(), unitSimId );
    BOOST_CHECK_EQUAL( action.parameters().elem_size(), 1 );
    BOOST_CHECK_EQUAL( action.parameters().elem( 0 ).value( 0 ).automat().id(), 78u );
}

BOOST_FIXTURE_TEST_CASE( remote_orbat_shaper_creates_formation, AutomatFixture )
{
    MockHlaObject formationHla;
    ObjectListener_ABC *formationListener = 0;

    MOCK_EXPECT( formationHla.Register ).once().with( mock::retrieve( formationListener ) );
    remoteClassListener->RemoteCreated( formationRtiId, clazz, formationHla );
    BOOST_REQUIRE( formationListener );
    formationListener->NameChanged( formationRtiId, "a_formation" );
    MOCK_EXPECT( sideResolver.ResolveTeam ).once().with( rpr::Friendly ).returns( team.GetId() );
    formationListener->SideChanged( formationRtiId, rpr::Friendly );

    parentListener->ParentChanged( parentRtiId, formationRtiId );
    
    sword::AutomatCreation message;
    message.mutable_automat()->set_id( 78u );

    sword::ClientToSim moveMessage;
    simulation::UnitMagicAction formationMessage;
    MOCK_EXPECT( simulationPublisher.SendClientToSim ).once().with( mock::retrieve( moveMessage ) );
    MOCK_EXPECT( formationCreation.Send ).once().with( mock::retrieve( formationMessage ), formationRtiId );
    automatCreationHandler->Notify( message, parentRtiId );

    const sword::UnitMagicAction& action = formationMessage();
    BOOST_CHECK_EQUAL( action.type(), sword::UnitMagicAction::formation_creation );
    BOOST_CHECK_EQUAL( action.tasker().party().id(), team.GetId() );    
    BOOST_CHECK_EQUAL( action.parameters().elem_size(), 4 );
    BOOST_CHECK_EQUAL( action.parameters().elem( 0 ).value( 0 ).areal(), 4.0f );
    BOOST_CHECK_EQUAL( action.parameters().elem( 1 ).value( 0 ).acharstr(), "a_formation" );
    BOOST_REQUIRE( action.parameters().elem( 2 ).null_value() );
    BOOST_CHECK( action.parameters().elem( 3 ).value( 0 ).has_extensionlist() );
    BOOST_CHECK_EQUAL( action.parameters().elem( 3 ).value( 0 ).extensionlist().entries_size(), 2 );
}

namespace
{
    struct FormationFixture : AutomatFixture
    {
        FormationFixture()
            : formationListener( 0 )
        {
            MOCK_EXPECT( formationHla.Register ).once().with( mock::retrieve( formationListener ) );
            remoteClassListener->RemoteCreated( formationRtiId, clazz, formationHla );
            BOOST_REQUIRE( formationListener );
            formationListener->NameChanged( formationRtiId, "a_formation" );
            MOCK_EXPECT( sideResolver.ResolveTeam ).once().with( rpr::Friendly ).returns( team.GetId() );
            formationListener->SideChanged( formationRtiId, rpr::Friendly );

            parentListener->ParentChanged( parentRtiId, formationRtiId );
    
            sword::AutomatCreation message;
            message.mutable_automat()->set_id( 78u );

            sword::ClientToSim moveMessage;
            simulation::UnitMagicAction formationMessage;
            MOCK_EXPECT( simulationPublisher.SendClientToSim ).once().with( mock::retrieve( moveMessage ) );
            MOCK_EXPECT( formationCreation.Send ).once().with( mock::retrieve( formationMessage ), formationRtiId );
            automatCreationHandler->Notify( message, parentRtiId );
        }
        MockHlaObject formationHla;
        ObjectListener_ABC *formationListener;
    };
}

BOOST_FIXTURE_TEST_CASE( remote_orbat_shaper_moves_child_automat, FormationFixture )
{
    sword::FormationCreation message;
    message.mutable_formation()->set_id( 66u );

    sword::ClientToSim actual;
    MOCK_EXPECT( simulationPublisher.SendClientToSim ).once().with( mock::retrieve( actual ) );
    formationCreationHandler->Notify( message, formationRtiId );
    BOOST_REQUIRE( actual.message().has_unit_magic_action() );
    const sword::UnitMagicAction& action = actual.message().unit_magic_action();
    BOOST_CHECK_EQUAL( action.type(), sword::UnitMagicAction::change_formation_superior );
    BOOST_CHECK_EQUAL( action.tasker().automat().id(), 78u );
    BOOST_CHECK_EQUAL( action.parameters().elem_size(), 1 );
    BOOST_CHECK_EQUAL( action.parameters().elem( 0 ).value( 0 ).formation().id(), 66u );
}
