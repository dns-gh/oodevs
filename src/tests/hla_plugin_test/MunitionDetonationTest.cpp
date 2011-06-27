// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_test_pch.h"
#include "hla_plugin/MunitionDetonation.h"
#include "DeserializationTools.h"
#include "MockFederate.h"
#include "MockInteractionHandler.h"
#include "MockMessageController.h"
#include "protocol/Simulation.h"
#include <boost/bind.hpp>
#include <boost/assign.hpp>
#include <boost/foreach.hpp>

using namespace plugins::hla;

BOOST_AUTO_TEST_CASE( munition_detonation_registration_publish_only )
{
    MockFederate federate;
    hla::MockInteractionHandler* handler = new hla::MockInteractionHandler(); // $$$$ _RC_ SLI 2011-06-24: wtf hla library?
    MOCK_EXPECT( federate, RegisterInteraction ).once().with( "MunitionDetonation", mock::any, true, false ).calls( boost::bind( &hla::Interaction_ABC::SetHandler, _2, boost::ref( *handler ) ) );
    tools::MockMessageController< sword::SimToClient_Content > controller;
    MOCK_EXPECT( controller, Register ).once();
    MunitionDetonation interaction( federate, controller );
    MOCK_EXPECT( controller, Unregister ).once();
}

namespace
{
    class Fixture
    {
    public:
        Fixture()
            : handler ( new hla::MockInteractionHandler() ) // $$$$ _RC_ SLI 2011-06-24: wtf hla library?
            , observer( 0 )
        {
            MOCK_EXPECT( federate, RegisterInteraction ).once().with( "MunitionDetonation", mock::any, true, false ).calls( boost::bind( &hla::Interaction_ABC::SetHandler, _2, boost::ref( *handler ) ) );;
            MOCK_EXPECT( controller, Register ).once().with( mock::retrieve( observer ) );
            MOCK_EXPECT( controller, Unregister ).once();
        }
        MockFederate federate;
        hla::MockInteractionHandler* handler;
        tools::MockMessageController< sword::SimToClient_Content > controller;
        tools::MessageHandler_ABC< sword::SimToClient_Content >* observer;
    };
    class RegisteredFixture : public Fixture
    {
    public:
        typedef std::vector< int8 > T_Buffer;

        RegisteredFixture()
            : interaction ( federate, controller )
            , deserializer( 0 )
        {
            message.mutable_start_unit_fire();
            BOOST_REQUIRE( observer );
        }
        void Deserialize( const ::hla::Serializer& serializer )
        {
            buffer.resize( serializer.GetSize() );
            if( !buffer.empty() )
            {
                serializer.CopyTo( &buffer[0] );
                deserializer = ::hla::Deserializer( &buffer[0], buffer.size() );
            }
        }
        void ConfigureExpectations( const std::string& parameter )
        {
            MOCK_EXPECT( handler, Visit ).once().with( parameter, mock::any ).calls( boost::bind( &RegisteredFixture::Deserialize, this, _2 ) );
            MOCK_EXPECT( handler, Visit );
            MOCK_EXPECT( handler, End );
        }
        MunitionDetonation interaction;
        mock::sequence s;
        sword::SimToClient_Content message;
        T_Buffer buffer;
        ::hla::Deserializer deserializer;
    };
}

BOOST_FIXTURE_TEST_CASE( munition_detonation_is_sent_when_receiving_StartUnitFire_message, RegisteredFixture )
{
    const std::vector< std::string > parameters = boost::assign::list_of( "ArticulatedPartData" )
                                                                        ( "DetonationLocation" )
                                                                        ( "DetonationResultCode" )
                                                                        ( "EventIdentifier" )
                                                                        ( "FiringObjectIdentifier" )
                                                                        ( "FinalVelocityVector" )
                                                                        ( "FuseType" )
                                                                        ( "MunitionObjectIdentifier" )
                                                                        ( "MunitionType" )
                                                                        ( "QuantityFired" )
                                                                        ( "RateOfFire" )
                                                                        ( "RelativeDetonationLocation" )
                                                                        ( "TargetObjectIdentifier" )
                                                                        ( "WarheadType" );
    BOOST_FOREACH( const std::string& parameter, parameters )
        MOCK_EXPECT( handler, Visit ).once().in( s ).with( parameter, mock::any );
    MOCK_EXPECT( handler, End ).once().in( s );
    observer->Notify( message );
}

BOOST_FIXTURE_TEST_CASE( articulated_part_data_is_always_empty, RegisteredFixture )
{
    ConfigureExpectations( "ArticulatedPartData" );
    observer->Notify( message );
    BOOST_CHECK_EQUAL( 0u, Read< uint32 >( deserializer ) );
}
