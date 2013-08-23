// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "hla_plugin_test_pch.h"
#include "hla_plugin/ExternalOwnershipPolicy.h"
#include "hla_plugin/ObjectListener_ABC.h"
#include "hla_plugin/InteractionBuilder.h"
#include "hla_plugin/NETNv2_InteractionBuilder.h"
#include "hla_plugin/InteractionsNetn.h"
#include "hla_plugin/SerializationTools.h"
#include "MockOwnershipController.h"
#include "MockTransferSender.h"
#include "MockHlaClass.h"
#include "MockHlaObject.h"
#include "MockFederate.h"
#include "MockLogger.h"
#include "MockCallsignResolver.h"
#include "MockLocalAgentResolver.h"
#include "MockInteractionHandler.h"
#include "MockRemoteAgentSubject.h"
#include "SerializationFixture.h"
#include "protocol/Protocol.h"
#include <hla/Interaction_ABC.h>
#include <hla/InteractionIdentifier.h>
#include <hla/ParameterIdentifier.h>
#include <hla/Deserializer.h>
#include <hla/Serializer.h>

using namespace plugins::hla;

namespace
{
    class Fixture
    {
    public:
        Fixture()
            : resultInteraction(0)
            , initiateInteraction(0)
            , offerInteraction(0)
            , netnInteractionBuilder( logger, federate )
            , interactionBuilder( logger, federate, netnInteractionBuilder)
            , classListener( 0 )
        {
            MOCK_EXPECT( logger.LogInfo );
            MOCK_EXPECT( subject.Register ).once().with( mock::retrieve( classListener ) );
            MOCK_EXPECT( federate.RegisterInteraction ).once().with( ::hla::InteractionIdentifier( "TMR.TMR_TransferResult" ), mock::retrieve( resultInteraction ), true, true );
            MOCK_EXPECT( federate.RegisterInteraction ).once().with( ::hla::InteractionIdentifier( "TMR.TMR_InitiateTransferModellingResponsibility" ), mock::retrieve( initiateInteraction ), true, true );
            MOCK_EXPECT( federate.RegisterInteraction ).once().with( ::hla::InteractionIdentifier( "TMR.TMR_OfferTransferModellingResponsibility" ), mock::retrieve( offerInteraction ), true, true );
            mock::reset( transferSender );
            MOCK_EXPECT( subject.Unregister ).once();
        }
        MockRemoteAgentSubject subject;
        MockTransferSender transferSender;
        dispatcher::MockLogger logger;
        MockFederate federate;
        ::hla::Interaction_ABC* resultInteraction;
        ::hla::Interaction_ABC* initiateInteraction;
        ::hla::Interaction_ABC* offerInteraction;
        NETNv2_InteractionBuilder netnInteractionBuilder;
        InteractionBuilder interactionBuilder;
        ClassListener_ABC* classListener;
    };

    class StrategyFixture : public Fixture
    {
    public:
        StrategyFixture()
            : policy( "federateName", interactionBuilder, ownershipController, logger, subject, agentResolver, callsignResolver, transferSender)
            , resultHandler( new ::hla::MockInteractionHandler() )
            , initiateHandler( new ::hla::MockInteractionHandler() )
            , offferHandler( new ::hla::MockInteractionHandler() )
        {
            BOOST_REQUIRE( classListener );
            BOOST_REQUIRE( resultInteraction );
            BOOST_REQUIRE( initiateInteraction );
            BOOST_REQUIRE( offerInteraction );
            resultInteraction->SetHandler( std::auto_ptr< ::hla::InteractionHandler_ABC >( resultHandler ) );
            initiateInteraction->SetHandler( std::auto_ptr< ::hla::InteractionHandler_ABC >( initiateHandler ) );
            offerInteraction->SetHandler( std::auto_ptr< ::hla::InteractionHandler_ABC >( offferHandler ) );
            transactionId.federateHandle=::hla::FederateIdentifier(std::string("federateHandle"));
            transactionId.transactionCounter = 42;
        }
        ~StrategyFixture()
        {
        }
        void FillTMR( std::list< std::vector<uint8_t> >& bufVect,::hla::Interaction_ABC::T_Parameters& params, uint32_t counter,
            const std::string& requestFederate)
        {
            interactions::TransactionId transactionId;
            transactionId.federateHandle=::hla::FederateIdentifier(std::string("federateHandle"));
            transactionId.transactionCounter = counter;
            FillParameter( bufVect, params, "TransactionID", transactionId );
            FillParameter( bufVect, params, "RequestFederate", UnicodeString( requestFederate ) );
            FillParameter( bufVect, params, "ResponseFederate", UnicodeString("responseFederate") );
        }
        void ReceiveInitiate( const std::string& requestFederate, uint32_t counter,
                const std::vector< NETN_UUID >& instances = std::vector< NETN_UUID >(), const std::vector< UnicodeString >& attributes = std::vector< UnicodeString >(),
                interactions::TMR::TransferTypeEnum32 transferType = interactions::TMR::Transfer_Other,
                interactions::TMR::CapabilityTypeEnum32 capabilityType = interactions::TMR::Capability_Other )
        {
            VariableArray< NETN_UUID > instancesVars; instancesVars.list = instances;
            VariableArray< UnicodeString > attributesVars; attributesVars.list = attributes;
            std::list< std::vector<uint8_t> > bufVect;
            ::hla::Interaction_ABC::T_Parameters params;
            FillTMR( bufVect, params, counter, requestFederate );
            FillParameter( bufVect, params, "TransferType", static_cast< uint32_t>(transferType) );
            FillParameter( bufVect, params, "Instances", instancesVars );
            FillParameter( bufVect, params, "Attributes", attributesVars );
            FillParameter( bufVect, params, "Initiating", UnicodeString("initiating") );
            FillParameter( bufVect, params, "CapabilityType", static_cast< uint32_t>(capabilityType) );
            FillParameter( bufVect, params, "InstanceAttributeValues", VariableArray< interactions::AttributeValueStruct >() );
            initiateInteraction->Create( params );
            initiateInteraction->Flush();
        }
        void CheckOffer( bool isOffering, const interactions::TransactionId& trId )
        {
            mock::sequence s;
            MOCK_EXPECT( offferHandler->Visit ).once().in( s ).with( ::hla::ParameterIdentifier( "TransactionID" ), mock::any )
                    .calls( boost::bind( &CheckParameter<interactions::TransactionId>, _1, _2, trId ) );
            MOCK_EXPECT( offferHandler->Visit ).once().in( s ).with( ::hla::ParameterIdentifier( "RequestFederate" ), mock::any );
            MOCK_EXPECT( offferHandler->Visit ).once().in( s ).with( ::hla::ParameterIdentifier( "ResponseFederate" ), mock::any );
            MOCK_EXPECT( offferHandler->Visit ).once().in( s ).with( ::hla::ParameterIdentifier( "isOffering" ), mock::any )
                .calls( boost::bind( &CheckParameter< bool >, _1, _2, isOffering ) );
            MOCK_EXPECT( offferHandler->Visit ).once().in( s ).with( ::hla::ParameterIdentifier( "Reason" ), mock::any );
            MOCK_EXPECT( offferHandler->End ).once();
        }
        MockOwnershipController ownershipController;
        MockCallsignResolver callsignResolver;
        MockLocalAgentResolver agentResolver;
        ExternalOwnershipPolicy policy;
        ::hla::MockInteractionHandler* resultHandler;
        ::hla::MockInteractionHandler* initiateHandler;
        ::hla::MockInteractionHandler* offferHandler;
        interactions::TransactionId transactionId;
    };
}

BOOST_FIXTURE_TEST_CASE( external_ownership_policy_not_concerned, StrategyFixture )
{
    MOCK_EXPECT( initiateHandler->Flush ).once();
    ReceiveInitiate( "me", transactionId.transactionCounter );
}

BOOST_FIXTURE_TEST_CASE( external_ownership_policy_no_instances, StrategyFixture )
{
    MOCK_EXPECT( logger.LogWarning );
    MOCK_EXPECT( initiateHandler->Flush ).once();
    CheckOffer( false, transactionId );
    ReceiveInitiate( "federateName", transactionId.transactionCounter );
}

BOOST_FIXTURE_TEST_CASE( external_ownership_policy_not_local, StrategyFixture )
{
    std::vector< NETN_UUID > instances; instances.push_back( MakeNetnUid( "uuid" ) );
    MOCK_EXPECT( initiateHandler->Flush ).once();
    CheckOffer( false, transactionId );
    ReceiveInitiate( "federateName", transactionId.transactionCounter, instances );
}

BOOST_FIXTURE_TEST_CASE( external_ownership_policy_divest_unknown, StrategyFixture )
{
    std::vector< UnicodeString > attributes;
    std::vector< NETN_UUID > instances; instances.push_back( MakeNetnUid( "uuid" ) );
    MOCK_EXPECT( initiateHandler->Flush ).once();
    MOCK_EXPECT( callsignResolver.ResolveSimulationIdentifier ).throws( std::exception("unknown") );
    MOCK_EXPECT( logger.LogWarning ).once();
    MOCK_EXPECT( logger.LogError ).once();
    CheckOffer( false, transactionId );
    ReceiveInitiate( "federateName", transactionId.transactionCounter, instances, attributes, interactions::TMR::Divest );
}

BOOST_FIXTURE_TEST_CASE( external_ownership_policy_acquire_unknown, StrategyFixture )
{
    std::vector< UnicodeString > attributes;
    std::vector< NETN_UUID > instances; instances.push_back( MakeNetnUid( "uuid" ) );
    MOCK_EXPECT( initiateHandler->Flush ).once();
    MOCK_EXPECT( callsignResolver.ResolveSimulationIdentifier ).throws( std::exception("unknown") );
    MOCK_EXPECT( logger.LogWarning ).once();
    MOCK_EXPECT( logger.LogError ).once();
    CheckOffer( false, transactionId );
    ReceiveInitiate( "federateName", transactionId.transactionCounter, instances, attributes, interactions::TMR::Acquire );
}

BOOST_FIXTURE_TEST_CASE( external_ownership_policy_divest_local, StrategyFixture )
{
    MockHlaClass clazz;
    MockHlaObject object;
    classListener->LocalCreated( "identifier", clazz, object );
    std::vector< NETN_UUID > instances; instances.push_back( MakeNetnUid( "uuid" ) );
    std::vector< UnicodeString > attributes;
    MOCK_EXPECT( initiateHandler->Flush ).once();
    MOCK_EXPECT( callsignResolver.ResolveSimulationIdentifier ).returns( 43 );
    MOCK_EXPECT( agentResolver.ResolveIdentifier ).returns( std::string( "identifier" ) );
    CheckOffer( true, transactionId );
    MOCK_EXPECT( transferSender.RequestTransferList ).once().with( mock::any, mock::any, TransferSender_ABC::E_EntityPush, mock::any, mock::any );
    ReceiveInitiate( "federateName", transactionId.transactionCounter, instances, attributes, interactions::TMR::Divest );
}

BOOST_FIXTURE_TEST_CASE( external_ownership_policy_acquire_local, StrategyFixture )
{
    MockHlaClass clazz;
    MockHlaObject object;
    classListener->LocalCreated( "identifier", clazz, object );
    std::vector< NETN_UUID > instances; instances.push_back( MakeNetnUid( "uuid" ) );
    std::vector< UnicodeString > attributes;
    MOCK_EXPECT( initiateHandler->Flush ).once();
    MOCK_EXPECT( callsignResolver.ResolveSimulationIdentifier ).returns( 43 );
    MOCK_EXPECT( agentResolver.ResolveIdentifier ).returns( std::string( "identifier" ) );
    MOCK_EXPECT( logger.LogWarning ).once();
    CheckOffer( false, transactionId );
    ReceiveInitiate( "federateName", transactionId.transactionCounter, instances, attributes, interactions::TMR::Acquire );
}

BOOST_FIXTURE_TEST_CASE( external_ownership_policy_divest_remote, StrategyFixture )
{
    MockHlaClass clazz;
    MockHlaObject object;
    classListener->RemoteCreated( "identifier", clazz, object );
    std::vector< NETN_UUID > instances; instances.push_back( MakeNetnUid( "uuid" ) );
    std::vector< UnicodeString > attributes;
    MOCK_EXPECT( initiateHandler->Flush ).once();
    MOCK_EXPECT( callsignResolver.ResolveSimulationIdentifier ).returns( 43 );
    MOCK_EXPECT( agentResolver.ResolveIdentifier ).returns( std::string( "identifier" ) );
    MOCK_EXPECT( logger.LogWarning ).once();
    CheckOffer( false, transactionId );
    ReceiveInitiate( "federateName", transactionId.transactionCounter, instances, attributes, interactions::TMR::Divest );
}

BOOST_FIXTURE_TEST_CASE( external_ownership_policy_acquire_remote, StrategyFixture )
{
    MockHlaClass clazz;
    MockHlaObject object;
    classListener->RemoteCreated( "identifier", clazz, object );
    std::vector< NETN_UUID > instances; instances.push_back( MakeNetnUid( "uuid" ) );
    std::vector< UnicodeString > attributes;
    MOCK_EXPECT( initiateHandler->Flush ).once();
    MOCK_EXPECT( callsignResolver.ResolveSimulationIdentifier ).returns( 43 );
    MOCK_EXPECT( agentResolver.ResolveIdentifier ).returns( std::string( "identifier" ) );
    CheckOffer( true, transactionId );
    MOCK_EXPECT( transferSender.RequestTransferList ).once().with( mock::any, mock::any, TransferSender_ABC::E_EntityPull, mock::any, mock::any );
    ReceiveInitiate( "federateName", transactionId.transactionCounter, instances, attributes, interactions::TMR::Acquire );
}

namespace
{
    struct Callback
    {
        void Capture( const TransferSender_ABC::TransferRequestCallback& callback )
        {
            cb_ =callback;
        }
        void operator()(bool v)
        {
            cb_(v);
        }
    private:
        TransferSender_ABC::TransferRequestCallback cb_;
    };
    struct EntitiesFixture : public StrategyFixture
    {
        EntitiesFixture()
            : localIdentifier( "identifierLocal" )
            , remoteIdentifier( "identifierRemote" )
        {
            classListener->LocalCreated( localIdentifier, clazz, localObject );
            classListener->RemoteCreated( remoteIdentifier, clazz, remoteObject );
        }
        void ProcessRequest( Callback& callback, const std::string& identifier, interactions::TMR::TransferTypeEnum32 transferType )
        {
            TransferSender_ABC::TransferType type = transferType == interactions::TMR::Divest ? TransferSender_ABC::E_EntityPush : TransferSender_ABC::E_EntityPull;
            std::vector< NETN_UUID > instances; instances.push_back( MakeNetnUid( "uuid" ) );
            MOCK_EXPECT( initiateHandler->Flush ).once();
            MOCK_EXPECT( callsignResolver.ResolveSimulationIdentifier ).returns( 43 );
            MOCK_EXPECT( agentResolver.ResolveIdentifier ).returns( identifier );
            CheckOffer( true, transactionId );
            MOCK_EXPECT( transferSender.RequestTransferList ).once().with( mock::any, mock::any, type, mock::any, mock::any ).
                    calls( boost::bind( &Callback::Capture, boost::ref(callback), _2 ) );
            ReceiveInitiate( "federateName", transactionId.transactionCounter, instances, attributesTransfered, transferType );
        }
        void CheckResult( bool isOk, const interactions::TransactionId& trId )
        {
            mock::sequence s;
            MOCK_EXPECT( resultHandler->Visit ).once().in( s ).with( ::hla::ParameterIdentifier( "TransactionID" ), mock::any )
                    .calls( boost::bind( &CheckParameter<interactions::TransactionId>, _1, _2, trId ) );
            MOCK_EXPECT( resultHandler->Visit ).once().in( s ).with( ::hla::ParameterIdentifier( "RequestFederate" ), mock::any );
            MOCK_EXPECT( resultHandler->Visit ).once().in( s ).with( ::hla::ParameterIdentifier( "ResponseFederate" ), mock::any );
            MOCK_EXPECT( resultHandler->Visit ).once().in( s ).with( ::hla::ParameterIdentifier( "TransferOk" ), mock::any )
                .calls( boost::bind( &CheckParameter< bool >, _1, _2, isOk ) );
            MOCK_EXPECT( resultHandler->End ).once();
        }
        const std::string localIdentifier, remoteIdentifier;
        std::vector< UnicodeString > attributesTransfered;
        MockHlaClass clazz;
        MockHlaObject localObject, remoteObject;
    };
}

BOOST_FIXTURE_TEST_CASE( external_ownership_policy_divest_negative_answer, EntitiesFixture )
{
    Callback callback;
    ProcessRequest( callback, localIdentifier, interactions::TMR::Divest );

    CheckResult( false, transactionId );
    callback( false );
}

BOOST_FIXTURE_TEST_CASE( external_ownership_policy_divest_positive_answer, EntitiesFixture )
{
    Callback callback;
    ProcessRequest( callback, localIdentifier, interactions::TMR::Divest );

    MOCK_EXPECT( ownershipController.PerformDivestiture ).once();
    callback( true );

    CheckResult( true, transactionId );
    classListener->Divested( localIdentifier, ::hla::T_AttributeIdentifiers() );
}

BOOST_FIXTURE_TEST_CASE( external_ownership_policy_acquire_negative_answer, EntitiesFixture )
{
    Callback callback;
    ProcessRequest( callback, remoteIdentifier, interactions::TMR::Acquire );

    CheckResult( false, transactionId );
    callback( false );
}

BOOST_FIXTURE_TEST_CASE( external_ownership_policy_acquire_positive_answer, EntitiesFixture )
{
    Callback callback;
    ProcessRequest( callback, remoteIdentifier, interactions::TMR::Acquire );

    MOCK_EXPECT( ownershipController.PerformAcquisition ).once();
    callback( true );

    CheckResult( true, transactionId );
    classListener->Acquired( remoteIdentifier, ::hla::T_AttributeIdentifiers() );
}
