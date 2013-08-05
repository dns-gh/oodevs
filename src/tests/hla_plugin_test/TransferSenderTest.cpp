// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "hla_plugin_test_pch.h"
#include "hla_plugin/RprTransferSender.h"
#include "hla_plugin/NullTransferSender.h"
#include "hla_plugin/Netn2TransferSender.h"
#include "hla_plugin/InteractionBuilder.h"
#include "hla_plugin/NETNv1_InteractionBuilder.h"
#include "hla_plugin/NETNv2_InteractionBuilder.h"
#include "hla_plugin/SerializationTools.h"
#include "hla_plugin/InteractionsRpr.h"
#include "hla_plugin/InteractionsNetn.h"
#include "hla_plugin/Omt13String.h"
#include "MockContextFactory.h"
#include "MockLogger.h"
#include "MockFederate.h"
#include "MockOwnershipStrategy.h"
#include "MockOwnershipController.h"
#include "MockInteractionHandler.h"
#include "MockLocalAgentResolver.h"
#include "MockCallsignResolver.h"
#include "SerializationFixture.h"
#include <hla/InteractionIdentifier.h>
#include <hla/Deserializer.h>
#include <hla/Serializer.h>

using namespace plugins::hla;

namespace
{
    struct Fixture
    {
        Fixture() :  netnBuilder( logger, federate ), builder( logger, federate, netnBuilder ), federateID( 1, 1, 0xFFFF ), transferInteraction( 0 ), ackInteraction( 0 )
        {
            MOCK_EXPECT( federate.RegisterInteraction ).once().with( ::hla::InteractionIdentifier( "TransferControl" ), mock::retrieve( transferInteraction ), true, true );
            MOCK_EXPECT( federate.RegisterInteraction ).once().with( ::hla::InteractionIdentifier( "Acknowledge" ), mock::retrieve( ackInteraction ), true, true );
        }
        dispatcher::MockLogger logger;
        MockContextFactory contextFactory;
        MockFederate federate;
        NETNv1_InteractionBuilder netnBuilder;
        InteractionBuilder builder;
        const rpr::EntityIdentifier federateID;
        ::hla::Interaction_ABC* transferInteraction;
        ::hla::Interaction_ABC* ackInteraction;
        MockOwnershipStrategy ownershipStrategy;
        MockOwnershipController ownershipController;
    };
    template <typename T>
    struct RprSenderFixture : Fixture
    {
        RprSenderFixture()
            : sender( federateID, contextFactory, builder, ownershipStrategy, ownershipController, logger )
            , transferHandler( new ::hla::MockInteractionHandler() )
            , ackHandler( new ::hla::MockInteractionHandler() )
        {
            BOOST_REQUIRE( transferInteraction );
            BOOST_REQUIRE( ackInteraction );
            transferInteraction->SetHandler( std::auto_ptr< ::hla::InteractionHandler_ABC >( transferHandler ) );
            ackInteraction->SetHandler( std::auto_ptr< ::hla::InteractionHandler_ABC >( ackHandler ) );
            attributes.push_back( ::hla::AttributeIdentifier( "attr1" ) );
        }
        ~RprSenderFixture()
        {
        }
        void ReceiveAck( uint32_t requestId, interactions::Acknowledge::ResponseFlagEnum16 resp )
        {
            std::list< std::vector<uint8_t> > bufVect;
            ::hla::Interaction_ABC::T_Parameters params;
            FillParameter( bufVect, params, "ReceivingEntity", federateID );
            FillParameter( bufVect, params, "RequestIdentifier", requestId );
            FillParameter( bufVect, params, "ResponseFlag", static_cast<unsigned short>( resp ) );
            ackInteraction->Create( params );
            ackInteraction->Flush();
        }
        void ReceiveTransfer( uint32_t requestId, interactions::TransferControl::TransferTypeEnum8 type, const std::string& agent )
        {
            static const rpr::EntityIdentifier destFederate( 0xFFFF, 0xFFFF, 0xFFFF );
            std::list< std::vector<uint8_t> > bufVect;
            ::hla::Interaction_ABC::T_Parameters params;
            FillParameter( bufVect, params, "OriginatingEntity", federateID );
            FillParameter( bufVect, params, "ReceivingEntity", destFederate );
            FillParameter( bufVect, params, "RequestIdentifier", requestId );
            FillParameter( bufVect, params, "TransferType", static_cast<uint8_t>( type ) );
            FillParameter( bufVect, params, "TransferEntity", Omt13String( agent ) );
            transferInteraction->Create( params );
            transferInteraction->Flush();
        }
        void CheckTransfer( const std::string& agentID, TransferSender_ABC::TransferType type, uint32_t requestId )
        {
            mock::sequence s;
            MOCK_EXPECT( transferHandler->Visit ).once().in( s ).with( ::hla::ParameterIdentifier( "OriginatingEntity" ), mock::any ) ; 
                //calls( boost::bind( &CheckParameter<rpr::EntityIdentifier>, _1, _2, federateID ) );
            MOCK_EXPECT( transferHandler->Visit ).once().in( s ).with( ::hla::ParameterIdentifier( "ReceivingEntity" ), mock::any );        
            MOCK_EXPECT( transferHandler->Visit ).once().in( s ).with( ::hla::ParameterIdentifier( "RequestIdentifier" ), mock::any ).
                calls( boost::bind( &CheckParameter<uint32_t>, _1, _2, requestId ) );
            MOCK_EXPECT( transferHandler->Visit ).once().in( s ).with( ::hla::ParameterIdentifier( "TransferType" ), mock::any ).
                calls( boost::bind( &CheckParameter<uint8_t>, _1, _2,
                    static_cast<uint8_t>(type == TransferSender_ABC::E_EntityPull ? interactions::TransferControl::E_EntityPull : interactions::TransferControl::E_EntityPush ) ) );
            MOCK_EXPECT( transferHandler->Visit ).once().in( s ).with( ::hla::ParameterIdentifier( "TransferEntity" ), mock::any ).
                calls( boost::bind( &CheckParameter<Omt13String>, _1, _2, Omt13String( agentID ) ) );
            MOCK_EXPECT( transferHandler->Visit ).once().in( s ).with( ::hla::ParameterIdentifier( "RecordSetData" ), mock::any );
            MOCK_EXPECT( transferHandler->End ).once();
        }
        void CheckAck( interactions::Acknowledge::ResponseFlagEnum16 resp, uint32_t requestId )
        {
            mock::sequence s;
            MOCK_EXPECT( ackHandler->Visit ).once().in( s ).with( ::hla::ParameterIdentifier( "OriginatingEntity" ), mock::any );
            MOCK_EXPECT( ackHandler->Visit ).once().in( s ).with( ::hla::ParameterIdentifier( "ReceivingEntity" ), mock::any );        
            MOCK_EXPECT( ackHandler->Visit ).once().in( s ).with( ::hla::ParameterIdentifier( "RequestIdentifier" ), mock::any ).
                calls( boost::bind( &CheckParameter<uint32_t>, _1, _2, requestId ) );
            MOCK_EXPECT( ackHandler->Visit ).once().in( s ).with( ::hla::ParameterIdentifier( "AcknowledgeFlag" ), mock::any );
            MOCK_EXPECT( ackHandler->Visit ).once().in( s ).with( ::hla::ParameterIdentifier( "ResponseFlag" ), mock::any ).
                calls( boost::bind( &CheckParameter<uint16_t>, _1, _2, static_cast<uint16_t>( resp ) ) );;
            MOCK_EXPECT( ackHandler->End ).once();
        }
        typedef T SenderType;        
        SenderType sender;
        ::hla::MockInteractionHandler* transferHandler;
        ::hla::MockInteractionHandler* ackHandler;
        std::vector< ::hla::AttributeIdentifier > emptyAttributes;
        std::vector< ::hla::AttributeIdentifier > attributes;
    };
}

BOOST_FIXTURE_TEST_CASE( rpr_pull_negative, RprSenderFixture< RprTransferSender > )
{
    MOCK_FUNCTOR( callback, void ( bool ) );

    MOCK_EXPECT( contextFactory.Create ).once().returns( 42 );
    CheckTransfer( "identifier", TransferSender_ABC::E_EntityPull, 42 );
    sender.RequestTransfer( "identifier", callback, TransferSender_ABC::E_EntityPull, attributes );
    MOCK_EXPECT( ackHandler->Flush ).once();
    MOCK_EXPECT( callback ).once().with( false );
    ReceiveAck( 42, interactions::Acknowledge::E_UnableToComply );
}

BOOST_FIXTURE_TEST_CASE( rpr_pull_positive, RprSenderFixture< RprTransferSender > )
{
    MOCK_FUNCTOR( callback, void ( bool ) );

    MOCK_EXPECT( contextFactory.Create ).once().returns( 42 );
    CheckTransfer( "identifier", TransferSender_ABC::E_EntityPull, 42 );
    sender.RequestTransfer( "identifier", callback, TransferSender_ABC::E_EntityPull, attributes );
    MOCK_EXPECT( ackHandler->Flush ).once();
    MOCK_EXPECT( callback ).once().with( true );
    ReceiveAck( 42, interactions::Acknowledge::E_AbleToComply );
}

BOOST_FIXTURE_TEST_CASE( rpr_push_negative, RprSenderFixture< RprTransferSender > )
{
    MOCK_FUNCTOR( callback, void ( bool ) );

    MOCK_EXPECT( contextFactory.Create ).once().returns( 42 );
    CheckTransfer( "identifier", TransferSender_ABC::E_EntityPush, 42 );
    sender.RequestTransfer( "identifier", callback, TransferSender_ABC::E_EntityPush, attributes );
    MOCK_EXPECT( ackHandler->Flush ).once();
    MOCK_EXPECT( callback ).once().with( false );
    ReceiveAck( 42, interactions::Acknowledge::E_UnableToComply );
}

BOOST_FIXTURE_TEST_CASE( rpr_push_positive, RprSenderFixture< RprTransferSender > )
{
    MOCK_FUNCTOR( callback, void ( bool ) );

    MOCK_EXPECT( contextFactory.Create ).once().returns( 42 );
    CheckTransfer( "identifier", TransferSender_ABC::E_EntityPush, 42 );
    sender.RequestTransfer( "identifier", callback, TransferSender_ABC::E_EntityPush, attributes );
    MOCK_EXPECT( ackHandler->Flush ).once();
    MOCK_EXPECT( callback ).once().with( true );
    ReceiveAck( 42, interactions::Acknowledge::E_AbleToComply );
}

BOOST_FIXTURE_TEST_CASE( rpr_receive_pull_negative, RprSenderFixture< RprTransferSender > )
{
    MOCK_EXPECT( transferHandler->Flush ).once();
    MOCK_EXPECT( ownershipStrategy.AcceptDivestiture ).once().with( "identifier", true ).returns( false );
    CheckAck( interactions::Acknowledge::E_UnableToComply, 42 );
    ReceiveTransfer(42, interactions::TransferControl::E_EntityPull, "identifier" );
}

BOOST_FIXTURE_TEST_CASE( rpr_receive_pull_positive, RprSenderFixture< RprTransferSender > )
{
    MOCK_EXPECT( transferHandler->Flush ).once();
    MOCK_EXPECT( ownershipStrategy.AcceptDivestiture ).once().with( "identifier", true ).returns( true );
    CheckAck( interactions::Acknowledge::E_AbleToComply, 42 );
    MOCK_EXPECT( ownershipController.PerformDivestiture ).once().with( "identifier", emptyAttributes );
    ReceiveTransfer(42, interactions::TransferControl::E_EntityPull, "identifier" );
}

BOOST_FIXTURE_TEST_CASE( rpr_receive_push_negative, RprSenderFixture< RprTransferSender > )
{
    MOCK_EXPECT( transferHandler->Flush ).once();
    MOCK_EXPECT( ownershipStrategy.AcceptAcquisition ).once().with( "identifier", true ).returns( false );
    CheckAck( interactions::Acknowledge::E_UnableToComply, 42 );
    ReceiveTransfer(42, interactions::TransferControl::E_EntityPush, "identifier" );
}

BOOST_FIXTURE_TEST_CASE( rpr_receive_push_positive, RprSenderFixture< RprTransferSender > )
{
    MOCK_EXPECT( transferHandler->Flush ).once();
    MOCK_EXPECT( ownershipStrategy.AcceptAcquisition ).once().with( "identifier", true ).returns( true );
    CheckAck( interactions::Acknowledge::E_AbleToComply, 42 );
    MOCK_EXPECT( ownershipController.PerformAcquisition ).once().with( "identifier", emptyAttributes );
    ReceiveTransfer(42, interactions::TransferControl::E_EntityPush, "identifier" );
}

BOOST_FIXTURE_TEST_CASE( null_pull_positive, RprSenderFixture< NullTransferSender > )
{
    MOCK_FUNCTOR( callback, void ( bool ) );
    MOCK_EXPECT( callback ).once().with( true );
    sender.RequestTransfer( "identifier", callback, TransferSender_ABC::E_EntityPull, attributes );
}

BOOST_FIXTURE_TEST_CASE( null_push_positive, RprSenderFixture< NullTransferSender > )
{
    MOCK_FUNCTOR( callback, void ( bool ) );
    MOCK_EXPECT( callback ).once().with( true );
    sender.RequestTransfer( "identifier", callback, TransferSender_ABC::E_EntityPush, attributes );
}

BOOST_FIXTURE_TEST_CASE( null_receive_pull_negative, RprSenderFixture< NullTransferSender > )
{
    MOCK_EXPECT( transferHandler->Flush ).once();
    MOCK_EXPECT( ownershipStrategy.AcceptDivestiture ).once().with( "identifier", true ).returns( false );
    CheckAck( interactions::Acknowledge::E_UnableToComply, 42 );
    ReceiveTransfer(42, interactions::TransferControl::E_EntityPull, "identifier" );
}

BOOST_FIXTURE_TEST_CASE( null_receive_pull_positive, RprSenderFixture< NullTransferSender > )
{
    MOCK_EXPECT( transferHandler->Flush ).once();
    MOCK_EXPECT( ownershipStrategy.AcceptDivestiture ).once().with( "identifier", true ).returns( true );
    CheckAck( interactions::Acknowledge::E_AbleToComply, 42 );
    MOCK_EXPECT( ownershipController.PerformDivestiture ).once().with( "identifier", emptyAttributes );
    ReceiveTransfer(42, interactions::TransferControl::E_EntityPull, "identifier" );
}

BOOST_FIXTURE_TEST_CASE( null_receive_push_negative, RprSenderFixture< NullTransferSender > )
{
    MOCK_EXPECT( transferHandler->Flush ).once();
    MOCK_EXPECT( ownershipStrategy.AcceptAcquisition ).once().with( "identifier", true ).returns( false );
    CheckAck( interactions::Acknowledge::E_UnableToComply, 42 );
    ReceiveTransfer(42, interactions::TransferControl::E_EntityPush, "identifier" );
}

BOOST_FIXTURE_TEST_CASE( null_receive_push_positive, RprSenderFixture< NullTransferSender > )
{
    MOCK_EXPECT( transferHandler->Flush ).once();
    MOCK_EXPECT( ownershipStrategy.AcceptAcquisition ).once().with( "identifier", true ).returns( true );
    CheckAck( interactions::Acknowledge::E_AbleToComply, 42 );
    MOCK_EXPECT( ownershipController.PerformAcquisition ).once().with( "identifier", emptyAttributes );
    ReceiveTransfer(42, interactions::TransferControl::E_EntityPush, "identifier" );
}

namespace
{
    struct NetnFixture
    {
        NetnFixture() :  netnBuilder( logger, federate ), builder( logger, federate, netnBuilder ), federateID( 1, 1, 0xFFFF ), requestInteraction( 0 ), offerInteraction( 0 )
        {
            MOCK_EXPECT( federate.RegisterInteraction ).once().with( ::hla::InteractionIdentifier( "TMR.TMR_OfferTransferModellingResponsibility" ), mock::retrieve( offerInteraction ), true, true );
            MOCK_EXPECT( federate.RegisterInteraction ).once().with( ::hla::InteractionIdentifier( "TMR.TMR_RequestTransferModellingResponsibility" ), mock::retrieve( requestInteraction ), true, true );
        }
        dispatcher::MockLogger logger;
        MockContextFactory contextFactory;
        MockFederate federate;
        NETNv2_InteractionBuilder netnBuilder;
        InteractionBuilder builder;
        const rpr::EntityIdentifier federateID;
        ::hla::Interaction_ABC* requestInteraction;
        ::hla::Interaction_ABC* offerInteraction;
        MockOwnershipStrategy ownershipStrategy;
        MockOwnershipController ownershipController;
    };
    struct NetnTransferFixture : NetnFixture
    {
        NetnTransferFixture()
            : federateName( "federateName" )
            , federateHandle( "federateHandle" )
            , sender( federateName, ::hla::FederateIdentifier( federateHandle ), contextFactory, builder, ownershipStrategy, ownershipController, logger, localAgentResolver, callsignResolver )
            , requestHandler( new ::hla::MockInteractionHandler() )
            , offerHandler( new ::hla::MockInteractionHandler() )
        {
            BOOST_REQUIRE( requestInteraction );
            BOOST_REQUIRE( offerInteraction );
            requestInteraction->SetHandler( std::auto_ptr< ::hla::InteractionHandler_ABC >( requestHandler ) );
            offerInteraction->SetHandler( std::auto_ptr< ::hla::InteractionHandler_ABC >( offerHandler ) );
        }
        
        template <typename T>
        void FillParameter( std::list< std::vector<uint8_t> >& bufVect, ::hla::Interaction_ABC::T_Parameters& p,
                const std::string& paramName, const T& value )
        {
            ::hla::Serializer ser;
            ser << value;
            std::vector<uint8_t> buff(ser.GetSize(), 0 );
            ser.CopyTo( &buff[0] );
            bufVect.push_back( buff );
            p.push_back( std::make_pair( ::hla::ParameterIdentifier( paramName ), new ::hla::Deserializer( &(bufVect.back()[0]), ser.GetSize() ) ) );
        }
        template <typename T>
        void CheckParameter( const ::hla::ParameterIdentifier& , ::hla::T_SerializerPtr serializer, const T& ref )
        {
            T value;
            std::vector<uint8_t> buff( serializer->GetSize(), 0 );
            serializer->CopyTo( &buff[0] );
            ::hla::Deserializer deser( &buff[0], serializer->GetSize() );
            deser >> value;
            BOOST_CHECK( value == ref );
        }
        void CheckTransfer( const NETN_UUID& uniqueId, TransferSender_ABC::TransferType type, uint32_t requestId )
        {
            interactions::TransactionId trId; trId.federateHandle = ::hla::FederateIdentifier( federateHandle ); trId.transactionCounter = requestId;
            VariableArray< NETN_UUID > instances; instances.list.push_back( uniqueId );
            
            mock::sequence s;
            MOCK_EXPECT( requestHandler->Visit ).once().in( s ).with( ::hla::ParameterIdentifier( "TransactionID" ), mock::any ).
                calls( boost::bind( &NetnTransferFixture::CheckParameter< interactions::TransactionId >, this, _1, _2, trId ) ); 
            MOCK_EXPECT( requestHandler->Visit ).once().in( s ).with( ::hla::ParameterIdentifier( "RequestFederate" ), mock::any ).
                calls( boost::bind( &NetnTransferFixture::CheckParameter< UnicodeString >, this, _1, _2, UnicodeString( "federateName" ) ) ); ; 
            MOCK_EXPECT( requestHandler->Visit ).once().in( s ).with( ::hla::ParameterIdentifier( "ResponseFederate" ), mock::any ).
                calls( boost::bind( &NetnTransferFixture::CheckParameter< UnicodeString >, this, _1, _2, UnicodeString( "" ) ) );
            MOCK_EXPECT( requestHandler->Visit ).once().in( s ).with( ::hla::ParameterIdentifier( "TransferType" ), mock::any ).
                calls( boost::bind( &NetnTransferFixture::CheckParameter< uint32_t >, this, _1, _2,
                                    type == TransferSender_ABC::E_EntityPull ? static_cast< uint32_t >( interactions::TMR::Acquire ) : static_cast< uint32_t >( interactions::TMR::Divest ) ) );
            MOCK_EXPECT( requestHandler->Visit ).once().in( s ).with( ::hla::ParameterIdentifier( "Instances" ), mock::any ).
                calls( boost::bind( &NetnTransferFixture::CheckParameter< VariableArray< NETN_UUID > >, this, _1, _2, instances ) ); 
            MOCK_EXPECT( requestHandler->Visit ).once().in( s ).with( ::hla::ParameterIdentifier( "Attributes" ), mock::any );
            MOCK_EXPECT( requestHandler->Visit ).once().in( s ).with( ::hla::ParameterIdentifier( "CapabilityType" ), mock::any ).
                calls( boost::bind( &NetnTransferFixture::CheckParameter< uint32_t >, this, _1, _2, static_cast< uint32_t >( interactions::TMR::TotalTransfer ) ) );
            MOCK_EXPECT( requestHandler->Visit ).once().in( s ).with( ::hla::ParameterIdentifier( "InstanceAttributeValues" ), mock::any );
            MOCK_EXPECT( requestHandler->End ).once();
        }
        void ReceiveAnswer( uint32_t requestId, bool ok, interactions::TMR::NoofferReasonEnum32 reason )
        {
            interactions::TransactionId trId; trId.federateHandle = ::hla::FederateIdentifier( federateHandle ); trId.transactionCounter = requestId;

            std::list< std::vector<uint8_t> > bufVect;
            ::hla::Interaction_ABC::T_Parameters params;
            FillParameter( bufVect, params, "TransactionID", trId );
            FillParameter( bufVect, params, "RequestFederate", UnicodeString( "federateName" ) );
            FillParameter( bufVect, params, "ResponseFederate", UnicodeString( "federateName2" ) );
            FillParameter( bufVect, params, "isOffering", ok );
            FillParameter( bufVect, params, "Reason", static_cast< uint32_t >( reason ) );
            offerInteraction->Create( params );
            offerInteraction->Flush();
        }
        void ReceiveRequest( uint32_t requestId, interactions::TMR::TransferTypeEnum32 type, const std::string& agent )
        {
            static const rpr::EntityIdentifier destFederate( 0xFFFF, 0xFFFF, 0xFFFF );
            
            interactions::TransactionId trId; trId.federateHandle = ::hla::FederateIdentifier( federateHandle ); trId.transactionCounter = requestId;
            VariableArray< NETN_UUID > instances; instances.list.push_back( MakeNetnUid( agent ) ) ;
            VariableArray< UnicodeString > attributes;
            VariableArray< interactions::AttributeValueStruct > attributeValues;

            std::list< std::vector<uint8_t> > bufVect;
            ::hla::Interaction_ABC::T_Parameters params;
            FillParameter( bufVect, params, "TransactionID", trId );
            FillParameter( bufVect, params, "RequestFederate", UnicodeString( "federateName2" ) );
            FillParameter( bufVect, params, "ResponseFederate", UnicodeString( "federateName" ) );
            FillParameter( bufVect, params, "TransferType", static_cast< uint32_t >( type ) );
            FillParameter( bufVect, params, "Instances", instances );
            FillParameter( bufVect, params, "Attributes", attributes );
            FillParameter( bufVect, params, "CapabilityType", static_cast< uint32_t >( interactions::TMR::TotalTransfer ) );
            FillParameter( bufVect, params, "InstanceAttributeValues", attributeValues );
            requestInteraction->Create( params );
            requestInteraction->Flush();
        }
        void CheckOffer( uint32_t requestId, bool isOffering )
        {
            mock::sequence s;
            interactions::TransactionId trId; trId.federateHandle = ::hla::FederateIdentifier( federateHandle ); trId.transactionCounter = requestId;
            MOCK_EXPECT( offerHandler->Visit ).once().in( s ).with( ::hla::ParameterIdentifier( "TransactionID" ), mock::any ).
                calls( boost::bind( &NetnTransferFixture::CheckParameter< interactions::TransactionId >, this, _1, _2, trId ) ); 
            MOCK_EXPECT( offerHandler->Visit ).once().in( s ).with( ::hla::ParameterIdentifier( "RequestFederate" ), mock::any );
            MOCK_EXPECT( offerHandler->Visit ).once().in( s ).with( ::hla::ParameterIdentifier( "ResponseFederate" ), mock::any );
            MOCK_EXPECT( offerHandler->Visit ).once().in( s ).with( ::hla::ParameterIdentifier( "isOffering" ), mock::any ).
                calls( boost::bind( &NetnTransferFixture::CheckParameter< bool >, this, _1, _2, isOffering ) ); 
            MOCK_EXPECT( offerHandler->Visit ).once().in( s ).with( ::hla::ParameterIdentifier( "Reason" ), mock::any );
            MOCK_EXPECT( offerHandler->End ).once();
        }
        const std::string federateName;
        const std::string federateHandle;
        MockLocalAgentResolver localAgentResolver;
        MockCallsignResolver callsignResolver;
        Netn2TransferSender sender;
        ::hla::MockInteractionHandler *requestHandler;
        ::hla::MockInteractionHandler *offerHandler;
        std::vector< ::hla::AttributeIdentifier > attributes;
    };
}

BOOST_FIXTURE_TEST_CASE( netn_pull_negative, NetnTransferFixture )
{
    MOCK_FUNCTOR( callback, void ( bool ) );

    MOCK_EXPECT( contextFactory.Create ).once().returns( 42 );
    MOCK_EXPECT( localAgentResolver.ResolveName ).once().with( "identifier" ).returns( 12ul );
    MOCK_EXPECT( callsignResolver.ResolveUniqueId ).once().with( 12ul ).returns( MakeNetnUid ("uniqueId" ).data() );
    CheckTransfer( NETN_UUID( MakeUniqueId( "uniqueId" ) ), TransferSender_ABC::E_EntityPull, 42 );
    sender.RequestTransfer( "identifier", callback, TransferSender_ABC::E_EntityPull, attributes );

    MOCK_EXPECT( offerHandler->Flush ).once();
    MOCK_EXPECT( callback ).once().with( false );
    ReceiveAnswer( 42, false, interactions::TMR::Reason_Other );
}

BOOST_FIXTURE_TEST_CASE( netn_pull_positive, NetnTransferFixture )
{
    MOCK_FUNCTOR( callback, void ( bool ) );

    MOCK_EXPECT( contextFactory.Create ).once().returns( 42 );
    MOCK_EXPECT( localAgentResolver.ResolveName ).once().with( "identifier" ).returns( 12ul );
    MOCK_EXPECT( callsignResolver.ResolveUniqueId ).once().with( 12ul ).returns( MakeNetnUid( "uniqueId" ).data() );
    CheckTransfer( MakeNetnUid( "uniqueId" ), TransferSender_ABC::E_EntityPull, 42 );
    sender.RequestTransfer( "identifier", callback, TransferSender_ABC::E_EntityPull, attributes );

    MOCK_EXPECT( offerHandler->Flush ).once();
    MOCK_EXPECT( callback ).once().with( true );
    ReceiveAnswer( 42, true, interactions::TMR::Reason_Other );
}

BOOST_FIXTURE_TEST_CASE( netn_push_negative, NetnTransferFixture )
{
    MOCK_FUNCTOR( callback, void ( bool ) );

    MOCK_EXPECT( contextFactory.Create ).once().returns( 42 );
    MOCK_EXPECT( localAgentResolver.ResolveName ).once().with( "identifier" ).returns( 12ul );
    MOCK_EXPECT( callsignResolver.ResolveUniqueId ).once().with( 12ul ).returns( MakeNetnUid( "uniqueId" ).data() );
    CheckTransfer( MakeNetnUid( "uniqueId" ), TransferSender_ABC::E_EntityPush, 42 );
    sender.RequestTransfer( "identifier", callback, TransferSender_ABC::E_EntityPush, attributes );

    MOCK_EXPECT( offerHandler->Flush ).once();
    MOCK_EXPECT( callback ).once().with( false );
    ReceiveAnswer( 42, false, interactions::TMR::Reason_Other );
}

BOOST_FIXTURE_TEST_CASE( netn_push_positive, NetnTransferFixture )
{
    MOCK_FUNCTOR( callback, void ( bool ) );

    MOCK_EXPECT( contextFactory.Create ).once().returns( 42 );
    MOCK_EXPECT( localAgentResolver.ResolveName ).once().with( "identifier" ).returns( 12ul );
    MOCK_EXPECT( callsignResolver.ResolveUniqueId ).once().with( 12ul ).returns( MakeNetnUid( "uniqueId" ).data() );
    CheckTransfer( MakeNetnUid( "uniqueId" ), TransferSender_ABC::E_EntityPush, 42 );
    sender.RequestTransfer( "identifier", callback, TransferSender_ABC::E_EntityPush, attributes );

    MOCK_EXPECT( offerHandler->Flush ).once();
    MOCK_EXPECT( callback ).once().with( true );
    ReceiveAnswer( 42, true, interactions::TMR::Reason_Other );
}

BOOST_FIXTURE_TEST_CASE( netn_receive_pull_negative, NetnTransferFixture )
{
    MOCK_EXPECT( callsignResolver.ResolveSimulationIdentifier ).once().with( MakeNetnUid( "uniqueId" ).data() ).returns( 12ul );
    MOCK_EXPECT( localAgentResolver.ResolveIdentifier ).once().with( 12ul ).returns( "identifier" );
    MOCK_EXPECT( requestHandler->Flush ).once();
    MOCK_EXPECT( ownershipStrategy.AcceptDivestiture ).once().with( "identifier", true ).returns( false );
    CheckOffer( 42, false );
    ReceiveRequest( 42, interactions::TMR::Acquire, "uniqueId" );
}

BOOST_FIXTURE_TEST_CASE( netn_receive_pull_positive, NetnTransferFixture )
{
    MOCK_EXPECT( callsignResolver.ResolveSimulationIdentifier ).exactly( 2 ).with( MakeNetnUid( "uniqueId" ).data() ).returns( 12ul );
    MOCK_EXPECT( localAgentResolver.ResolveIdentifier ).exactly( 2 ).with( 12ul ).returns( "identifier" );
    MOCK_EXPECT( requestHandler->Flush ).once();
    MOCK_EXPECT( ownershipStrategy.AcceptDivestiture ).once().with( "identifier", true ).returns( true );
    CheckOffer( 42, true );
    MOCK_EXPECT( ownershipController.PerformDivestiture ).once().with( "identifier", attributes );
    ReceiveRequest( 42, interactions::TMR::Acquire, "uniqueId" );
}

BOOST_FIXTURE_TEST_CASE( netn_receive_push_negative, NetnTransferFixture )
{
    MOCK_EXPECT( callsignResolver.ResolveSimulationIdentifier ).once().with( MakeNetnUid( "uniqueId" ).data() ).returns( 12ul );
    MOCK_EXPECT( localAgentResolver.ResolveIdentifier ).once().with( 12ul ).returns( "identifier" );
    MOCK_EXPECT( requestHandler->Flush ).once();
    MOCK_EXPECT( ownershipStrategy.AcceptAcquisition ).once().with( "identifier", true ).returns( false );
    CheckOffer( 42, false );
    ReceiveRequest( 42, interactions::TMR::Divest, "uniqueId" );
}

BOOST_FIXTURE_TEST_CASE( netn_receive_push_positive, NetnTransferFixture )
{
    MOCK_EXPECT( callsignResolver.ResolveSimulationIdentifier ).exactly( 2 ).with( MakeNetnUid( "uniqueId" ).data() ).returns( 12ul );
    MOCK_EXPECT( localAgentResolver.ResolveIdentifier ).exactly( 2 ).with( 12ul ).returns( "identifier" );
    MOCK_EXPECT( requestHandler->Flush ).once();
    MOCK_EXPECT( ownershipStrategy.AcceptAcquisition ).once().with( "identifier", true ).returns( true );
    CheckOffer( 42, true );
    MOCK_EXPECT( ownershipController.PerformAcquisition ).once().with( "identifier", attributes );
    ReceiveRequest( 42, interactions::TMR::Divest, "uniqueId" );
}
