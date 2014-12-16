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
#include <hla/VariableLengthData.h>

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
        std::vector< ::hla::AttributeIdentifier > attributes;
    };
    class ControllerFixture : public Fixture
    {
    public:
        ControllerFixture() :
            ownershipController( federateID, remoteSubject, logger )
        {
            BOOST_REQUIRE( classListener );
            allAttributes.push_back(::hla::AttributeIdentifier("attr_a"));
            allAttributes.push_back(::hla::AttributeIdentifier("attr_b"));
            MOCK_EXPECT( hlaClass.GetAttributes ).returns( allAttributes );
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
        std::vector< ::hla::AttributeIdentifier > allAttributes;
    };
}

BOOST_FIXTURE_TEST_CASE( ownership_push, ControllerFixture )
{
    const ::hla::VariableLengthData sentTag((uint32_t)42);
    MOCK_EXPECT( hlaClass.Divest ).once().with( "identifier", allAttributes, mock::any );
    ownershipController.PerformDivestiture( "identifier", allAttributes, sentTag );
}

BOOST_FIXTURE_TEST_CASE( ownership_push_partial, ControllerFixture )
{
    const ::hla::VariableLengthData sentTag((uint32_t)42);
    attributes.push_back(::hla::AttributeIdentifier("attr_a"));
    MOCK_EXPECT( hlaClass.Divest ).once().with( "identifier", attributes, mock::any );
    ownershipController.PerformDivestiture( "identifier", attributes, sentTag );
}

BOOST_FIXTURE_TEST_CASE( ownership_push_too_many, ControllerFixture )
{
    std::vector< ::hla::AttributeIdentifier > expected;
    expected.push_back(::hla::AttributeIdentifier("attr_a"));
    const ::hla::VariableLengthData sentTag((uint32_t)42);
    attributes.push_back(::hla::AttributeIdentifier("attr_a"));
    attributes.push_back(::hla::AttributeIdentifier("attr_c"));
    MOCK_EXPECT( hlaClass.Divest ).once().with( "identifier", expected, mock::any );
    ownershipController.PerformDivestiture( "identifier", attributes, sentTag );
}

namespace
{
    struct DivestedFixture : ControllerFixture
    {
        DivestedFixture()
        {
            const ::hla::VariableLengthData sentTag((uint32_t)42);
            attributes.push_back(::hla::AttributeIdentifier("attr_a"));
            MOCK_EXPECT( hlaClass.Divest ).once().with( "identifier", attributes, mock::any );
            ownershipController.PerformDivestiture( "identifier", attributes, sentTag );
            classListener->Divested( "identifier", attributes );
        }
    };
}

BOOST_FIXTURE_TEST_CASE( ownership_pull, DivestedFixture )
{
    const ::hla::VariableLengthData sentTag((uint32_t)42);
    MOCK_EXPECT( hlaClass.Acquire ).once().with( "identifier", attributes, mock::any );
    ownershipController.PerformAcquisition( "identifier", attributes, sentTag );
}

BOOST_FIXTURE_TEST_CASE( ownership_pull_too_many, DivestedFixture )
{
    const ::hla::VariableLengthData sentTag((uint32_t)42);
    MOCK_EXPECT( hlaClass.Acquire ).once().with( "identifier", attributes, mock::any );
    ownershipController.PerformAcquisition( "identifier", allAttributes, sentTag );
}
