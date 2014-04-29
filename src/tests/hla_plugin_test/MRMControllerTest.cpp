// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.

// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "hla_plugin_test_pch.h"
#include "hla_plugin/MRMController.h"
#include "hla_plugin/InteractionBuilder.h"
#include "hla_plugin/NETNv2_InteractionBuilder.h"
#include "hla_plugin/ClassListener_ABC.h"
#include "MockLogger.h"
#include "MockLocalAgentResolver.h"
#include "MockCallsignResolver.h"
#include "MockRemoteAgentSubject.h"
#include "MockFederate.h"
#include "MockInteractionHandler.h"
#include "MockHlaClass.h"
#include "MockHlaObject.h"
#include "SerializationFixture.h"
#include <hla/InteractionIdentifier.h>
#include <hla/ParameterIdentifier.h>
#include <hla/AttributeIdentifier.h>

using namespace plugins::hla;

namespace
{
    struct FixtureBase
    {
        FixtureBase()
            : netnInteractionBuilder( logger, federate )
            , interactionBuilder( logger, federate, netnInteractionBuilder)
            , classListener( 0 )
            , disaggregationRequest( 0 )
            , disaggregationResponse( 0 )
            , actionComplete( 0 )
            , aggregationRequest( 0 )
            , aggregationResponse( 0 )
            {
                MOCK_EXPECT( subject.Register ).once().with( mock::retrieve( classListener ) );
                MOCK_EXPECT( federate.RegisterInteraction ).once().with( ::hla::InteractionIdentifier( "MRM_Object.MRM_DisaggregationRequest" ), mock::retrieve( disaggregationRequest ), true, true );
                MOCK_EXPECT( federate.RegisterInteraction ).once().with( ::hla::InteractionIdentifier( "MRM_Object.MRM_DisaggregationResponse" ), mock::retrieve( disaggregationResponse ), true, true );
                MOCK_EXPECT( federate.RegisterInteraction ).once().with( ::hla::InteractionIdentifier( "MRM_Object.MRM_ActionComplete" ), mock::retrieve( actionComplete ), true, true );
                MOCK_EXPECT( federate.RegisterInteraction ).once().with( ::hla::InteractionIdentifier( "MRM_Object.MRM_AggregationRequest" ), mock::retrieve( aggregationRequest ), true, true );
                MOCK_EXPECT( federate.RegisterInteraction ).once().with( ::hla::InteractionIdentifier( "MRM_Object.MRM_AggregationResponse" ), mock::retrieve( aggregationResponse ), true, true );
                MOCK_EXPECT( subject.Unregister ).once();
            }
        dispatcher::MockLogger logger;
        MockLocalAgentResolver localAgentResolver;
        MockCallsignResolver callsignResolver;
        MockFederate federate;
        NETNv2_InteractionBuilder netnInteractionBuilder;
        InteractionBuilder interactionBuilder;
        MockRemoteAgentSubject subject;
        ClassListener_ABC* classListener;
        ::hla::Interaction_ABC* disaggregationRequest;
        ::hla::Interaction_ABC* disaggregationResponse;
        ::hla::Interaction_ABC* actionComplete;
        ::hla::Interaction_ABC* aggregationRequest;
        ::hla::Interaction_ABC* aggregationResponse;

    };
    struct Fixture : FixtureBase
    {
        Fixture()
            : mrmController( "federate", interactionBuilder, subject, logger, localAgentResolver, callsignResolver )
            , disaggregationRequestHandler( new ::hla::MockInteractionHandler() )
            , disaggregationResponseHandler( new ::hla::MockInteractionHandler() )
            , actionCompleteHandler( new ::hla::MockInteractionHandler() )
            , aggregationRequestHandler( new ::hla::MockInteractionHandler() )
            , aggregationResponseHandler( new ::hla::MockInteractionHandler() )
        {
                BOOST_REQUIRE( classListener );
                BOOST_REQUIRE( disaggregationRequest );
                BOOST_REQUIRE( disaggregationResponse );
                BOOST_REQUIRE( actionComplete );
                BOOST_REQUIRE( aggregationRequest );
                BOOST_REQUIRE( aggregationResponse );
                disaggregationRequest->SetHandler( std::auto_ptr< ::hla::InteractionHandler_ABC >( disaggregationRequestHandler ) );
                disaggregationResponse->SetHandler( std::auto_ptr< ::hla::InteractionHandler_ABC >( disaggregationResponseHandler ) );
                actionComplete->SetHandler( std::auto_ptr< ::hla::InteractionHandler_ABC >( actionCompleteHandler ) );
                aggregationRequest->SetHandler( std::auto_ptr< ::hla::InteractionHandler_ABC >( aggregationRequestHandler ) );
                aggregationResponse->SetHandler( std::auto_ptr< ::hla::InteractionHandler_ABC >( aggregationResponseHandler ) );
                transactionId.federateHandle=::hla::FederateIdentifier(std::string("federateHandle"));
                transactionId.transactionCounter = 42;
        }
        void ReceiveDisaggregationRequest( const std::string& fed, const NETN_UUID& aggregate, uint32_t counter )
        {
            interactions::TransactionId transactionId;
            transactionId.federateHandle=::hla::FederateIdentifier(std::string("federateHandle"));
            transactionId.transactionCounter = counter;

            std::list< std::vector<uint8_t> > bufVect;
            ::hla::Interaction_ABC::T_Parameters params;
            FillParameter( bufVect, params, "TransactionID", transactionId );
            FillParameter( bufVect, params, "AggregateFederate", UnicodeString( fed ) );
            FillParameter( bufVect, params, "HigherResolutionFederate", UnicodeString( "high_res" ) );
            FillParameter( bufVect, params, "AggregateUuid", aggregate );
            FillParameter( bufVect, params, "AggregationState", static_cast< uint32_t >( 1 ) );
            disaggregationRequest->Create( params );
            disaggregationRequest->Flush();
        }

        void ReceiveAggregationRequest( const std::string& fed, const NETN_UUID& aggregate, uint32_t counter )
        {
            interactions::TransactionId transactionId;
            transactionId.federateHandle=::hla::FederateIdentifier(std::string("federateHandle"));
            transactionId.transactionCounter = counter;

            std::list< std::vector<uint8_t> > bufVect;
            ::hla::Interaction_ABC::T_Parameters params;
            FillParameter( bufVect, params, "TransactionID", transactionId );
            FillParameter( bufVect, params, "AggregateFederate", UnicodeString( fed ) );
            FillParameter( bufVect, params, "HigherResolutionFederate", UnicodeString( "high_res" ) );
            FillParameter( bufVect, params, "AggregateUuid", aggregate );
            aggregationRequest->Create( params );
            aggregationRequest->Flush();
        }
        void ReceiveActionComplete( const std::string& fed, uint32_t counter, bool result )
        {
            interactions::TransactionId transactionId;
            transactionId.federateHandle=::hla::FederateIdentifier(std::string("federateHandle"));
            transactionId.transactionCounter = counter;

            std::list< std::vector<uint8_t> > bufVect;
            ::hla::Interaction_ABC::T_Parameters params;
            FillParameter( bufVect, params, "TransactionID", transactionId );
            FillParameter( bufVect, params, "AggregateFederate", UnicodeString( fed ) );
            FillParameter( bufVect, params, "HigherResolutionFederate", UnicodeString( "high_res" ) );
            FillParameter< uint32_t >( bufVect, params, "CompletionResult", result ? 1 : 0 );
            actionComplete->Create( params );
            actionComplete->Flush();
        }
        void CheckDisaggregationResponse( uint32_t ack, uint32_t reason, const interactions::TransactionId& trId )
        {
            mock::sequence s;
            MOCK_EXPECT( disaggregationResponseHandler->Visit ).once().in( s ).with( ::hla::ParameterIdentifier( "TransactionID" ), mock::any )
                    .calls( boost::bind( &CheckParameter<interactions::TransactionId>, _1, _2, trId ) );
            MOCK_EXPECT( disaggregationResponseHandler->Visit ).once().in( s ).with( ::hla::ParameterIdentifier( "AggregateFederate" ), mock::any );
            MOCK_EXPECT( disaggregationResponseHandler->Visit ).once().in( s ).with( ::hla::ParameterIdentifier( "HigherResolutionFederate" ), mock::any );
            MOCK_EXPECT( disaggregationResponseHandler->Visit ).once().in( s ).with( ::hla::ParameterIdentifier( "Acknowledge" ), mock::any )
                .calls( boost::bind( &CheckParameter< uint16_t >, _1, _2, ack ) );
            MOCK_EXPECT( disaggregationResponseHandler->Visit ).once().in( s ).with( ::hla::ParameterIdentifier( "NonComplianceReason" ), mock::any )
                .calls( boost::bind( &CheckParameter< uint32_t >, _1, _2, reason ) );
            MOCK_EXPECT( disaggregationResponseHandler->End ).once();
        }
        void CheckAggregationResponse( uint32_t ack, uint32_t reason, const interactions::TransactionId& trId )
        {
            mock::sequence s;
            MOCK_EXPECT( aggregationResponseHandler->Visit ).once().in( s ).with( ::hla::ParameterIdentifier( "TransactionID" ), mock::any )
                    .calls( boost::bind( &CheckParameter<interactions::TransactionId>, _1, _2, trId ) );
            MOCK_EXPECT( aggregationResponseHandler->Visit ).once().in( s ).with( ::hla::ParameterIdentifier( "AggregateFederate" ), mock::any );
            MOCK_EXPECT( aggregationResponseHandler->Visit ).once().in( s ).with( ::hla::ParameterIdentifier( "HigherResolutionFederate" ), mock::any );
            MOCK_EXPECT( aggregationResponseHandler->Visit ).once().in( s ).with( ::hla::ParameterIdentifier( "Acknowledge" ), mock::any )
                .calls( boost::bind( &CheckParameter< uint16_t >, _1, _2, ack ) );
            MOCK_EXPECT( aggregationResponseHandler->Visit ).once().in( s ).with( ::hla::ParameterIdentifier( "NonComplianceReason" ), mock::any )
                .calls( boost::bind( &CheckParameter< uint32_t >, _1, _2, reason ) );
            MOCK_EXPECT( aggregationResponseHandler->End ).once();
        }
        MRMController mrmController;
        ::hla::MockInteractionHandler* disaggregationRequestHandler;
        ::hla::MockInteractionHandler* disaggregationResponseHandler;
        ::hla::MockInteractionHandler* actionCompleteHandler;
        ::hla::MockInteractionHandler* aggregationRequestHandler;
        ::hla::MockInteractionHandler* aggregationResponseHandler;
        interactions::TransactionId transactionId;
    };
}

BOOST_FIXTURE_TEST_CASE( mrm_disaggregation_request_not_concerned, Fixture )
{
    MOCK_EXPECT( disaggregationRequestHandler->Flush ).once();
    ReceiveDisaggregationRequest( "other", MakeNetnUid("aggr_uuid"), 42 );
}

BOOST_FIXTURE_TEST_CASE( mrm_disaggregation_request_unknown_aggregate, Fixture )
{
    MOCK_EXPECT( logger.LogInfo );
    MOCK_EXPECT( disaggregationRequestHandler->Flush ).once();
    MOCK_EXPECT( callsignResolver.ResolveSimulationIdentifier ).once().throws( std::runtime_error( "unknown" ) );
    MOCK_EXPECT( logger.LogError ).once();
    CheckDisaggregationResponse( 2, 1, transactionId );
    ReceiveDisaggregationRequest( "federate", MakeNetnUid("aggr_uuid"), 42 );
}

BOOST_FIXTURE_TEST_CASE( mrm_disaggregation_request_unowned_aggregate, Fixture )
{
    MOCK_EXPECT( logger.LogInfo );
    MOCK_EXPECT( disaggregationRequestHandler->Flush ).once();
    MOCK_EXPECT( callsignResolver.ResolveSimulationIdentifier ).once().returns( 12 );
    MOCK_EXPECT( localAgentResolver.ResolveIdentifier ).once().returns( "aggr" );
    MOCK_EXPECT( logger.LogError ).once();
    CheckDisaggregationResponse( 2, 4, transactionId );
    ReceiveDisaggregationRequest( "federate", MakeNetnUid("aggr_uuid"), 42 );
}

BOOST_FIXTURE_TEST_CASE( mrm_aggregation_request_not_concerned, Fixture )
{
    MOCK_EXPECT( aggregationRequestHandler->Flush ).once();
    ReceiveAggregationRequest( "other", MakeNetnUid("aggr_uuid"), 42 );
}

BOOST_FIXTURE_TEST_CASE( mrm_aggregation_request_unknown_aggregate, Fixture )
{
    MOCK_EXPECT( logger.LogInfo );
    MOCK_EXPECT( aggregationRequestHandler->Flush ).once();
    MOCK_EXPECT( callsignResolver.ResolveSimulationIdentifier ).once().throws( std::runtime_error( "unknown" ) );
    MOCK_EXPECT( logger.LogError ).once();
    CheckAggregationResponse( 2, 1, transactionId );
    ReceiveAggregationRequest( "federate", MakeNetnUid("aggr_uuid"), 42 );
}

BOOST_FIXTURE_TEST_CASE( mrm_aggregation_request_unowned_aggregate, Fixture )
{
    MOCK_EXPECT( logger.LogInfo );
    MOCK_EXPECT( aggregationRequestHandler->Flush ).once();
    MOCK_EXPECT( callsignResolver.ResolveSimulationIdentifier ).once().returns( 12 );
    MOCK_EXPECT( localAgentResolver.ResolveIdentifier ).once().returns( "aggr" );
    MOCK_EXPECT( logger.LogError ).once();
    CheckAggregationResponse( 2, 4, transactionId );
    ReceiveAggregationRequest( "federate", MakeNetnUid("aggr_uuid"), 42 );
}

namespace
{
    struct EntitiesFixture : Fixture
    {
        EntitiesFixture()
            : localIdentifier( "identifierLocal" )
            , remoteIdentifier( "identifierRemote" )
        {
            classListener->LocalCreated( localIdentifier, clazz, localObject );
            classListener->RemoteCreated( remoteIdentifier, clazz, remoteObject );
        }
        const std::string localIdentifier, remoteIdentifier;
        MockHlaClass clazz;
        MockHlaObject localObject, remoteObject;
    };
}

BOOST_FIXTURE_TEST_CASE( mrm_disaggregation_request_owned_aggregate_ok, EntitiesFixture )
{
    MOCK_EXPECT( logger.LogInfo );
    MOCK_EXPECT( disaggregationRequestHandler->Flush ).once();
    MOCK_EXPECT( callsignResolver.ResolveSimulationIdentifier ).once().returns( 12 );
    MOCK_EXPECT( localAgentResolver.ResolveIdentifier ).once().returns( "identifierLocal" );
    CheckDisaggregationResponse( 1, 0, transactionId );
    ReceiveDisaggregationRequest( "federate", MakeNetnUid("aggr_uuid"), 42 );

    MOCK_EXPECT( actionCompleteHandler->Flush ).once();
    ReceiveActionComplete( "federate", 42, true);
}

BOOST_FIXTURE_TEST_CASE( mrm_disaggregation_request_owned_aggregate_ko, EntitiesFixture )
{
    MOCK_EXPECT( logger.LogInfo );
    MOCK_EXPECT( disaggregationRequestHandler->Flush ).once();
    MOCK_EXPECT( callsignResolver.ResolveSimulationIdentifier ).once().returns( 12 );
    MOCK_EXPECT( localAgentResolver.ResolveIdentifier ).once().returns( "identifierLocal" );
    CheckDisaggregationResponse( 1, 0, transactionId );
    ReceiveDisaggregationRequest( "federate", MakeNetnUid("aggr_uuid"), 42 );

    MOCK_EXPECT( actionCompleteHandler->Flush ).once();
    ReceiveActionComplete( "federate", 42, false);
}

namespace
{
    struct DisaggregatedFixture : EntitiesFixture
    {
        DisaggregatedFixture()
        {
            MOCK_EXPECT( logger.LogInfo );
            MOCK_EXPECT( disaggregationRequestHandler->Flush ).once();
            MOCK_EXPECT( callsignResolver.ResolveSimulationIdentifier ).once().returns( 12 );
            MOCK_EXPECT( localAgentResolver.ResolveIdentifier ).once().returns( "identifierLocal" );
            CheckDisaggregationResponse( 1, 0, transactionId );
            ReceiveDisaggregationRequest( "federate", MakeNetnUid("aggr_uuid"), 42 );

            classListener->Divested( "identifierLocal", T_AttributeIdentifiers() );

            MOCK_EXPECT( actionCompleteHandler->Flush ).once();
            ReceiveActionComplete( "federate", 42, true);
        }
    };
}

BOOST_FIXTURE_TEST_CASE( mrm_aggregation_request_owned_aggregate_ok, DisaggregatedFixture )
{
    MOCK_EXPECT( logger.LogInfo );
    MOCK_EXPECT( aggregationRequestHandler->Flush ).once();
    MOCK_EXPECT( callsignResolver.ResolveSimulationIdentifier ).once().returns( 12 );
    MOCK_EXPECT( localAgentResolver.ResolveIdentifier ).once().returns( "identifierLocal" );
    CheckAggregationResponse( 1, 0, transactionId );
    ReceiveAggregationRequest( "federate", MakeNetnUid("aggr_uuid"), 42 );

    MOCK_EXPECT( actionCompleteHandler->Flush ).once();
    ReceiveActionComplete( "federate", 42, true);
}

BOOST_FIXTURE_TEST_CASE( mrm_aggregation_request_owned_aggregate_ko, DisaggregatedFixture )
{
    MOCK_EXPECT( logger.LogInfo );
    MOCK_EXPECT( aggregationRequestHandler->Flush ).once();
    MOCK_EXPECT( callsignResolver.ResolveSimulationIdentifier ).once().returns( 12 );
    MOCK_EXPECT( localAgentResolver.ResolveIdentifier ).once().returns( "identifierLocal" );
    CheckAggregationResponse( 1, 0, transactionId );
    ReceiveAggregationRequest( "federate", MakeNetnUid("aggr_uuid"), 42 );

    MOCK_EXPECT( actionCompleteHandler->Flush ).once();
    ReceiveActionComplete( "federate", 42, false);
}
