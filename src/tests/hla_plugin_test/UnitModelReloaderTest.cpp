// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_test_pch.h"
#include "hla_plugin/UnitModelReloader.h"
#include "MockRemoteAgentSubject.h"
#include "MockLocalAgentResolver.h"
#include "MockContextHandler.h"
#include "MockSimulationPublisher.h"
#include "MockContextFactory.h"
#include "MockResolver.h"
#include "MockHlaClass.h"
#include "MockHlaObject.h"
#include "protocol/SimulationSenders.h"
#include "clients_kernel/AgentType.h"
#include "clients_kernel/SymbolFactory.h"
#include "clients_kernel/XmlTranslations.h"
#include "clients_kernel/DecisionalModel.h"
#include "clients_kernel/MissionFactory.h"

#include <xeumeuleu/xml.hpp>

using namespace plugins::hla;

namespace
{
    const std::string agentTypeConfig( "  <unit decisional-model=\"Local Decisional\" id=\"1\" name=\"HLA.Local Pion\" type=\"Pion JOINT\">"
    "<nature atlas-nature=\"None\" level=\"i\" nature-app6=\"combat service support/transportation/spod spoe\"/>"
    "<equipments>"
    "</equipments>"
    "<sensors-range range=\"0\"/>"
    "<equipments-range range=\"0\"/>"
    "<crew-ranks>"
    "</crew-ranks>"
    "<logistics>"
    "</logistics>"
    "<postures>"
    "</postures>"
    "<nbc decontamination-delay=\"120s\" suit=\"level5\"/>"
    "<efficiencies combat=\"0\" combat-support=\"0\" counter-mobility-support=\"0\" engineering-support=\"0\" mobility-support=\"0\" protection-support=\"0\" recon=\"0\" urban-area=\"100\"/>"
    "<crossing-height height=\"0m\"/>"
    "</unit>" );

    const std::string modelConfig("<unit name=\"Local Decisional\"/>");

    struct XmlFixture
    {
        XmlFixture()
            : agentTypeConfigStream( agentTypeConfig )
            , decisionalConfigStream( modelConfig )
        {
            agentTypeConfigStream >> xml::start( "unit" );
            decisionalConfigStream >> xml::start( "unit" );
        }
        xml::xistringstream agentTypeConfigStream;
        xml::xistringstream decisionalConfigStream;
    };

    struct Fixture : XmlFixture
    {
        Fixture()
            : missionFactory( dummyMissionsResolver, dummyMissionsResolver, dummyMissionsResolver, fragOrderResolver )
            , responseObserver( nullptr )
            , classListener( nullptr )
            , decisionalModel( decisionalConfigStream, missionFactory, &kernel::MissionFactory::CreateAgentMission, fragOrderResolver )
        {
            MOCK_EXPECT( contextHandler.Register ).once().with( mock::retrieve( responseObserver ) );
            MOCK_EXPECT( remoteSubject.Register ).once().with( mock::retrieve( classListener ) );
            MOCK_EXPECT( modelResolver.Get ).once().with( "Local Decisional" ).returns( std::ref( decisionalModel ) );
        }
        MockContextHandler< sword::UnitCreation > contextHandler;
        ::dispatcher::MockSimulationPublisher publisher;
        MockContextFactory contextFactory;
        tools::MockResolver< kernel::AgentType > agentTypeResolver;
        MockRemoteAgentSubject remoteSubject;
        MockLocalAgentResolver localResolver;
        tools::MockResolver< kernel::ComponentType, std::string > componentResolver;
        tools::MockResolver< kernel::DecisionalModel, std::string > modelResolver;
        tools::MockResolver< kernel::MissionType, std::string > dummyMissionsResolver;
        tools::StringResolver< kernel::FragOrderType > fragOrderResolver;
        kernel::SymbolFactory symbolFactory;
        kernel::XmlTranslations translations;
        kernel::MissionFactory missionFactory;
        kernel::DecisionalModel decisionalModel;
        ResponseObserver_ABC< sword::UnitCreation >* responseObserver;
        ClassListener_ABC* classListener;
    };

    struct ReloaderFixture : public Fixture
    {
        ReloaderFixture()
            : reloader( contextHandler, publisher, contextFactory, agentTypeResolver, remoteSubject, localResolver)
            , agentType( agentTypeConfigStream, componentResolver, modelResolver, symbolFactory, translations )
        {
            BOOST_REQUIRE( responseObserver );
            BOOST_REQUIRE( classListener );
            mock::reset( modelResolver );

            MOCK_EXPECT( contextHandler.Unregister );
            MOCK_EXPECT( remoteSubject.Unregister );
        }
        void checkReload( const sword::ClientToSim& actual, const std::string& brain, uint32_t unitId )
        {
            BOOST_CHECK( actual.has_message() );
            BOOST_CHECK( actual.message().has_unit_magic_action() );
            BOOST_CHECK_EQUAL( actual.message().unit_magic_action().type() , sword::UnitMagicAction::reload_brain );
            BOOST_CHECK_EQUAL( actual.message().unit_magic_action().tasker().unit().id(), unitId );
            if( !brain.empty() )
            {
                BOOST_CHECK_EQUAL( actual.message().unit_magic_action().parameters().elem_size(), 1 );
                BOOST_CHECK_EQUAL( actual.message().unit_magic_action().parameters().elem( 0 ).value_size(), 1 );
                BOOST_CHECK_EQUAL( actual.message().unit_magic_action().parameters().elem( 0 ).value( 0 ).acharstr(), brain );
            }
            else
            {
                BOOST_CHECK_EQUAL( actual.message().unit_magic_action().parameters().elem_size(), 0 );
            }
        }
        UnitModelReloader reloader;
        kernel::AgentType agentType;
    };
}

BOOST_FIXTURE_TEST_CASE( remove_brain_for_remotes, ReloaderFixture )
{
    sword::UnitCreation creationMessage;
    creationMessage.mutable_name()->assign( "unit" );
    creationMessage.mutable_unit()->set_id( 42u );
    creationMessage.mutable_type()->set_id( 420u );

    MOCK_EXPECT( agentTypeResolver.Get ).once().with( 420u ).returns( std::ref( agentType ) );
    MOCK_EXPECT( contextFactory.Create ).once().returns( 18 );

    sword::ClientToSim actual;
    MOCK_EXPECT( publisher.SendClientToSim ).once().with( mock::retrieve( actual ) );

    responseObserver->Notify( creationMessage, "remote" );
    checkReload( actual, "", 42u );
}

BOOST_FIXTURE_TEST_CASE( remove_brain_when_divest_local, ReloaderFixture )
{
    MockHlaClass clazz;
    MockHlaObject object;

    MOCK_EXPECT( localResolver.ResolveName ).once().with( "local" ).returns( 43u );
    MOCK_EXPECT( localResolver.AgentType ).once().with( 43u ).returns( 420u );
    MOCK_EXPECT( agentTypeResolver.Get ).once().with( 420u ).returns( std::ref( agentType ) );
    classListener->LocalCreated( "local", clazz, object );


    {
        sword::ClientToSim actual;
        MOCK_EXPECT( contextFactory.Create ).once().returns( 18 );
        MOCK_EXPECT( publisher.SendClientToSim ).once().with( mock::retrieve( actual ) );

        classListener->Divested( "local", std::vector< ::hla::AttributeIdentifier >() );

        checkReload( actual, "", 43u );
    }
    {
        sword::ClientToSim actual;
        MOCK_EXPECT( contextFactory.Create ).once().returns( 18 );
        MOCK_EXPECT( publisher.SendClientToSim ).once().with( mock::retrieve( actual ) );

        classListener->Acquired( "local", std::vector< ::hla::AttributeIdentifier >() );

        checkReload( actual, "Local Decisional", 43u );
    }
}

namespace
{
    struct RemoteFixture : public ReloaderFixture
    {
        RemoteFixture()
        {
            sword::UnitCreation creationMessage;
            creationMessage.mutable_name()->assign( "unit" );
            creationMessage.mutable_unit()->set_id( 42u );
            creationMessage.mutable_type()->set_id( 420u );

            MOCK_EXPECT( agentTypeResolver.Get ).once().with( 420u ).returns( std::ref( agentType ) );

            sword::ClientToSim actual;
            MOCK_EXPECT( contextFactory.Create ).once().returns( 18 );
            MOCK_EXPECT( publisher.SendClientToSim ).once().with( mock::retrieve( actual ) );

            responseObserver->Notify( creationMessage, "remote" );
            checkReload( actual, "", 42u );

        }
    };
}

BOOST_FIXTURE_TEST_CASE( reload_unload_on_acquire_divest, RemoteFixture )
{
    {
        sword::ClientToSim actual;
        MOCK_EXPECT( contextFactory.Create ).once().returns( 18 );
        MOCK_EXPECT( publisher.SendClientToSim ).once().with( mock::retrieve( actual ) );

        classListener->Acquired( "remote", std::vector< ::hla::AttributeIdentifier >() );

        checkReload( actual, "Local Decisional", 42u );
    }
    {
        sword::ClientToSim actual;
        MOCK_EXPECT( contextFactory.Create ).once().returns( 18 );
        MOCK_EXPECT( publisher.SendClientToSim ).once().with( mock::retrieve( actual ) );

        classListener->Divested( "remote", std::vector< ::hla::AttributeIdentifier >() );

        checkReload( actual, "", 42u );
    }
}
