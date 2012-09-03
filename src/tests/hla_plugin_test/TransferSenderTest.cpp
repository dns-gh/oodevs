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
#include "hla_plugin/InteractionBuilder.h"
#include "hla_plugin/NETNv1_InteractionBuilder.h"
#include "hla_plugin/SerializationTools.h"
#include "hla_plugin/InteractionsRpr.h"
#include "hla_plugin/Omt13String.h"
#include "MockContextFactory.h"
#include "MockLogger.h"
#include "MockFederate.h"
#include "MockOwnershipStrategy.h"
#include "MockOwnershipController.h"
#include "MockInteractionHandler.h"
#include <hla/InteractionIdentifier.h>
#include <hla/Deserializer.h>
#include <hla/Serializer.h>

using namespace plugins::hla;

bool operator == (const Omt13String& lhs, const Omt13String& rhs)
{
    return lhs.str() == rhs.str();
}

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
            transferInteraction->SetHandler( *transferHandler );
            ackInteraction->SetHandler( *ackHandler );
        }
        ~RprSenderFixture()
        {
        }
        template <typename T>
        void FillParameter( std::list< std::vector<int8> >& bufVect, ::hla::Interaction_ABC::T_Parameters& p,
                const std::string& paramName, const T& value )
        {
            ::hla::Serializer ser;
            ser << value;
            std::vector<int8> buff(ser.GetSize(), 0 );
            ser.CopyTo( &buff[0] );
            bufVect.push_back( buff );
            p.push_back( std::make_pair( ::hla::ParameterIdentifier( paramName ), new ::hla::Deserializer( &(bufVect.back()[0]), ser.GetSize() ) ) );
        }
        void ReceiveAck( uint32 requestId, interactions::Acknowledge::ResponseFlagEnum16 resp )
        {
            std::list< std::vector<int8> > bufVect;
            ::hla::Interaction_ABC::T_Parameters params;
            FillParameter( bufVect, params, "ReceivingEntity", federateID );
            FillParameter( bufVect, params, "RequestIdentifier", requestId );
            FillParameter( bufVect, params, "ResponseFlag", static_cast<unsigned short>( resp ) );
            ackInteraction->Create( params );
            ackInteraction->Flush();
        }
        void ReceiveTransfer( uint32 requestId, interactions::TransferControl::TransferTypeEnum8 type, const std::string& agent )
        {
            static const rpr::EntityIdentifier destFederate( 0xFFFF, 0xFFFF, 0xFFFF );
            std::list< std::vector<int8> > bufVect;
            ::hla::Interaction_ABC::T_Parameters params;
            FillParameter( bufVect, params, "OriginatingEntity", federateID );
            FillParameter( bufVect, params, "ReceivingEntity", destFederate );
            FillParameter( bufVect, params, "RequestIdentifier", requestId );
            FillParameter( bufVect, params, "TransferType", static_cast<uint8>( type ) );
            FillParameter( bufVect, params, "TransferEntity", Omt13String( agent ) );
            transferInteraction->Create( params );
            transferInteraction->Flush();
        }
        template <typename T>
        void CheckParameter( const ::hla::ParameterIdentifier& /*parameterID*/, ::hla::T_SerializerPtr serializer, const T& ref )
        {
            T value;
            std::vector<int8> buff( serializer->GetSize(), 0 );
            serializer->CopyTo( &buff[0] );
            ::hla::Deserializer deser( &buff[0], serializer->GetSize() );
            deser >> value;
            BOOST_CHECK( value == ref );
        }
        void CheckTransfer( const std::string& agentID, TransferSender_ABC::TransferType type, uint32 requestId )
        {
            mock::sequence s;
            MOCK_EXPECT( transferHandler->Visit ).once().in( s ).with( ::hla::ParameterIdentifier( "OriginatingEntity" ), mock::any ) ; 
                //calls( boost::bind( &RprSenderFixture::CheckParameter<rpr::EntityIdentifier>, this, _1, _2, federateID ) );
            MOCK_EXPECT( transferHandler->Visit ).once().in( s ).with( ::hla::ParameterIdentifier( "ReceivingEntity" ), mock::any );        
            MOCK_EXPECT( transferHandler->Visit ).once().in( s ).with( ::hla::ParameterIdentifier( "RequestIdentifier" ), mock::any ).
                calls( boost::bind( &RprSenderFixture::CheckParameter<uint32>, this, _1, _2, requestId ) );
            MOCK_EXPECT( transferHandler->Visit ).once().in( s ).with( ::hla::ParameterIdentifier( "TransferType" ), mock::any ).
                calls( boost::bind( &RprSenderFixture::CheckParameter<uint8>, this, _1, _2, 
                    static_cast<uint8>(type == TransferSender_ABC::E_EntityPull ? interactions::TransferControl::E_EntityPull : interactions::TransferControl::E_EntityPush ) ) );
            MOCK_EXPECT( transferHandler->Visit ).once().in( s ).with( ::hla::ParameterIdentifier( "TransferEntity" ), mock::any ).
                calls( boost::bind( &RprSenderFixture::CheckParameter<Omt13String>, this, _1, _2, Omt13String( agentID ) ) );
            MOCK_EXPECT( transferHandler->Visit ).once().in( s ).with( ::hla::ParameterIdentifier( "RecordSetData" ), mock::any );
            MOCK_EXPECT( transferHandler->End ).once();
        }
        
        void CheckAck( interactions::Acknowledge::ResponseFlagEnum16 resp, uint32 requestId )
        {
            mock::sequence s;
            MOCK_EXPECT( ackHandler->Visit ).once().in( s ).with( ::hla::ParameterIdentifier( "OriginatingEntity" ), mock::any );
            MOCK_EXPECT( ackHandler->Visit ).once().in( s ).with( ::hla::ParameterIdentifier( "ReceivingEntity" ), mock::any );        
            MOCK_EXPECT( ackHandler->Visit ).once().in( s ).with( ::hla::ParameterIdentifier( "RequestIdentifier" ), mock::any ).
                calls( boost::bind( &RprSenderFixture::CheckParameter<uint32>, this, _1, _2, requestId ) );
            MOCK_EXPECT( ackHandler->Visit ).once().in( s ).with( ::hla::ParameterIdentifier( "AcknowledgeFlag" ), mock::any );
            MOCK_EXPECT( ackHandler->Visit ).once().in( s ).with( ::hla::ParameterIdentifier( "ResponseFlag" ), mock::any ).
                calls( boost::bind( &RprSenderFixture::CheckParameter<uint16>, this, _1, _2, static_cast<uint16>( resp ) ) );;
            MOCK_EXPECT( ackHandler->End ).once();
        }
        typedef T SenderType;        
        SenderType sender;
        ::hla::MockInteractionHandler* transferHandler;
        ::hla::MockInteractionHandler* ackHandler;
    };
}


BOOST_FIXTURE_TEST_CASE( rpr_pull_negative, RprSenderFixture< RprTransferSender > )
{
    MOCK_FUNCTOR( callback, void ( bool ) );

    MOCK_EXPECT( contextFactory.Create ).once().returns( 42 );
    CheckTransfer( "identifier", TransferSender_ABC::E_EntityPull, 42 );
    sender.RequestTransfer( "identifier", callback, TransferSender_ABC::E_EntityPull );
    MOCK_EXPECT( ackHandler->Flush ).once();
    MOCK_EXPECT( callback ).once().with( false );
    ReceiveAck( 42, interactions::Acknowledge::E_UnableToComply );
}

BOOST_FIXTURE_TEST_CASE( rpr_pull_positive, RprSenderFixture< RprTransferSender > )
{
    MOCK_FUNCTOR( callback, void ( bool ) );

    MOCK_EXPECT( contextFactory.Create ).once().returns( 42 );
    CheckTransfer( "identifier", TransferSender_ABC::E_EntityPull, 42 );
    sender.RequestTransfer( "identifier", callback, TransferSender_ABC::E_EntityPull );
    MOCK_EXPECT( ackHandler->Flush ).once();
    MOCK_EXPECT( callback ).once().with( true );
    ReceiveAck( 42, interactions::Acknowledge::E_AbleToComply );
}

BOOST_FIXTURE_TEST_CASE( rpr_push_negative, RprSenderFixture< RprTransferSender > )
{
    MOCK_FUNCTOR( callback, void ( bool ) );

    MOCK_EXPECT( contextFactory.Create ).once().returns( 42 );
    CheckTransfer( "identifier", TransferSender_ABC::E_EntityPush, 42 );
    sender.RequestTransfer( "identifier", callback, TransferSender_ABC::E_EntityPush );
    MOCK_EXPECT( ackHandler->Flush ).once();
    MOCK_EXPECT( callback ).once().with( false );
    ReceiveAck( 42, interactions::Acknowledge::E_UnableToComply );
}

BOOST_FIXTURE_TEST_CASE( rpr_push_positive, RprSenderFixture< RprTransferSender > )
{
    MOCK_FUNCTOR( callback, void ( bool ) );

    MOCK_EXPECT( contextFactory.Create ).once().returns( 42 );
    CheckTransfer( "identifier", TransferSender_ABC::E_EntityPush, 42 );
    sender.RequestTransfer( "identifier", callback, TransferSender_ABC::E_EntityPush );
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
    MOCK_EXPECT( ownershipController.PerformDivestiture ).once().with( "identifier" );
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
    MOCK_EXPECT( ownershipController.PerformAcquisition ).once().with( "identifier" );
    ReceiveTransfer(42, interactions::TransferControl::E_EntityPush, "identifier" );
}

BOOST_FIXTURE_TEST_CASE( null_pull_positive, RprSenderFixture< NullTransferSender > )
{
    MOCK_FUNCTOR( callback, void ( bool ) );
    MOCK_EXPECT( callback ).once().with( true );
    sender.RequestTransfer( "identifier", callback, TransferSender_ABC::E_EntityPull );
}

BOOST_FIXTURE_TEST_CASE( null_push_positive, RprSenderFixture< NullTransferSender > )
{
    MOCK_FUNCTOR( callback, void ( bool ) );
    MOCK_EXPECT( callback ).once().with( true );
    sender.RequestTransfer( "identifier", callback, TransferSender_ABC::E_EntityPush );
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
    MOCK_EXPECT( ownershipController.PerformDivestiture ).once().with( "identifier" );
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
    MOCK_EXPECT( ownershipController.PerformAcquisition ).once().with( "identifier" );
    ReceiveTransfer(42, interactions::TransferControl::E_EntityPush, "identifier" );
}

