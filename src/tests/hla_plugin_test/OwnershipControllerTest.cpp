// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "hla_plugin_test_pch.h"
#include "hla_plugin/OwnershipController.h"
#include "hla_plugin/OwnershipStrategy.h"
#include "hla_plugin/ObjectListener_ABC.h"
#include "MockTransferSender.h"
#include "MockRemoteAgentSubject.h"
#include "MockContextFactory.h"
#include "MockHlaObject.h"
#include "MockHlaClass.h"
#include "MockLogger.h"

using namespace plugins::hla;

namespace
{
    class Fixture
    {
    public:
        Fixture() :
            federateID( 1, 1, 0xFFFF )
        {
            MOCK_EXPECT( remoteSubject.Register ).once().with( mock::retrieve( classListener ) );
            MOCK_EXPECT( logger.LogInfo );
        }
        rpr::EntityIdentifier federateID;
        MockRemoteAgentSubject remoteSubject;
        ClassListener_ABC* classListener;
        dispatcher::MockLogger logger;
    };
    class ControllerFixture : public Fixture
    {
    public:
        ControllerFixture() :
            ownershipController( federateID, remoteSubject, logger )
        {
            BOOST_REQUIRE( classListener );
            classListener->LocalCreated( "identifier", hlaClass, object );
        }
        ~ControllerFixture()
        {
            MOCK_EXPECT( object.Unregister );
            classListener->LocalDestroyed( "identifier" );
            MOCK_EXPECT( remoteSubject.Unregister );
        }
        MockHlaClass hlaClass;
        MockHlaObject object;
        OwnershipController ownershipController;
    };
}

BOOST_FIXTURE_TEST_CASE( ownership_push, ControllerFixture )
{
    MOCK_EXPECT( hlaClass.Divest ).once().with( "identifier" );
    ownershipController.PerformDivestiture( "identifier");
}

namespace
{
    struct DivestedFixture : ControllerFixture
    {
        DivestedFixture()
        {
            MOCK_EXPECT( hlaClass.Divest ).once().with( "identifier" );
            ownershipController.PerformDivestiture( "identifier" );
            classListener->Divested( "identifier" );
        }
    };
}

BOOST_FIXTURE_TEST_CASE( ownership_pull, DivestedFixture )
{
    MOCK_EXPECT( hlaClass.Acquire ).once().with( "identifier" );
    ownershipController.PerformAcquisition( "identifier");
}
