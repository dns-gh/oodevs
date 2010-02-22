// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "protocol_test_pch.h"
#include "protocol_includes.h"
#include "MockClient.h"
#include "MockServer.h"
#include "tools/ClientNetworker.h"
#include "tools/ServerNetworker.h"
#include <boost/lambda/lambda.hpp>
#include <boost/lexical_cast.hpp>

using namespace mockpp;
using namespace google::protobuf;

namespace
{
    std::string LOCALHOST = "127.0.0.1:10000";
}

namespace
{
    bool operator==( const google::protobuf::FieldDescriptor& lhs, const google::protobuf::FieldDescriptor& rhs )
    {
        BOOST_CHECK_EQUAL( lhs.name(), rhs.name() );
        BOOST_CHECK_EQUAL( lhs.index(), rhs.index() );
        BOOST_CHECK_EQUAL( lhs.type(), rhs.type() );
        // $$$$ SEB 2009-10-25: TODO: Check value
        return lhs.name() == rhs.name() && lhs.index() == rhs.index() && lhs.type() == rhs.type();
    }

    bool operator==( const google::protobuf::Message& lhs, const google::protobuf::Message& rhs )
    {
        const google::protobuf::Message::Reflection* lhsRef = lhs.GetReflection();
        const google::protobuf::Message::Reflection* rhsRef = rhs.GetReflection();
        if( !lhsRef )
            throw std::runtime_error( __FUNCTION__ " : reflection not available for lhs message." );
        if( !rhsRef )
            throw std::runtime_error( __FUNCTION__ " : reflection not available for rhs message." );
        typedef std::vector< const google::protobuf::FieldDescriptor* > T_Fields;
        T_Fields lFields; lhsRef->ListFields( lhs, &lFields );
        T_Fields rFields; rhsRef->ListFields( rhs, &rFields );
        T_Fields::const_iterator lIt = lFields.begin();
        T_Fields::const_iterator rIt = rFields.begin();
        for( ; lIt != lFields.end() && rIt != rFields.end(); ++lIt, ++rIt )
        {
            const google::protobuf::FieldDescriptor* lField = *lIt;
            const google::protobuf::FieldDescriptor* rField = *rIt;
            BOOST_CHECK( lField );
            BOOST_CHECK( rField );
            BOOST_CHECK( *lField == *rField );
        }
        return true;
    }

    // $$$$ SBO 2009-10-27: Uncomment to check values until it is done using above operator
//    bool operator==( const MsgPion& lhs, const MsgPion& rhs )
//    {
//        return lhs.name() == rhs.name() && lhs.id() == rhs.id();
//    }
//    bool operator==( const EmptyMessage& lhs, const EmptyMessage& rhs )
//    {
//        return true;
//    }
//    bool operator==( const Test& lhs, const Test& rhs )
//    {
//        return lhs.name() == rhs.name();
//    }
//    bool operator==( const ObjectInstance& lhs, const ObjectInstance& rhs )
//    {
//        return lhs.id() == rhs.id();
//    }

}

namespace
{
    struct ProtobufFixture
    {
        ProtobufFixture()
        {
            GOOGLE_PROTOBUF_VERIFY_VERSION;
        }
        ~ProtobufFixture()
        {
            // $$$$ SEB 2009-10-26: static library cleaning -> does not match initialization from GOOGLE_PROTOBUF_VERIFY_VERSION
            // $$$$ SEB 2009-10-26: once called, Reflection is no longer available
//            google::protobuf::ShutdownProtobufLibrary();
        }
    };

    struct MessageSendingFixture : public ProtobufFixture
    {
        MessageSendingFixture()
        {
            server_.ConnectionSucceeded_mocker.expects( once() ).with( mockpp::any() );
            client_.ConnectionSucceeded_mocker.expects( once() ).with( eq< const std::string >( LOCALHOST ) );
            while( !client_.Connected() )
            {
                BOOST_CHECK_NO_THROW( client_.Update() );
                BOOST_CHECK_NO_THROW( server_.Update() );
            }
        }
        template< typename M >
        void VerifyServerReception( M& message, unsigned int count = 1 )
        {
            AddServerExpectation( message, count );
            const boost::posix_time::ptime start = boost::posix_time::microsec_clock::universal_time();
            for( unsigned int i = 0; i < count; ++i )
            {
                BOOST_CHECK_NO_THROW( client_.Send( LOCALHOST, message ) );
                while( !server_.Received() )
                {
                    BOOST_CHECK_NO_THROW( client_.Update() );
                    BOOST_CHECK_NO_THROW( server_.Update() );
                }
                server_.ResetReceived();
            }
            std::cout << "sent " << count << " message(s) in " 
                      << boost::posix_time::microsec_clock::universal_time() - start << std::endl;
            client_.verify();
            server_.verify();
        }

        void AddServerExpectation( MsgPion& message, unsigned int count = 1 )
        {
            server_.OnReceivePion_mocker.expects( exactly( count ) ).with( mockpp::any(), eq< const MsgPion >( message ) );
        }

        template< typename M >
        void VerifyClientReception( M& message, unsigned int count = 1 )
        {
            AddClientExpectation( message, count );
            const boost::posix_time::ptime start = boost::posix_time::microsec_clock::universal_time();
            for( unsigned int i = 0; i < count; ++i )
            {
                BOOST_CHECK_NO_THROW( server_.Send( message ) );
                while( !client_.Received() )
                {
                    BOOST_CHECK_NO_THROW( server_.Update() );
                    BOOST_CHECK_NO_THROW( client_.Update() );
                }
                client_.ResetReceived();
            }
            std::cout << "sent " << count << " message(s) in " 
                      << boost::posix_time::microsec_clock::universal_time() - start << std::endl;
            client_.verify();
            server_.verify();
        }

        void AddClientExpectation( MsgPion& message, unsigned int count = 1 )
        {
            client_.OnReceivePion_mocker.expects( exactly( count ) ).with( eq< const std::string >( LOCALHOST ), eq< const MsgPion >( message ) );
        }
        
        void AddClientExpectation( Test& message, unsigned int count = 1 )
        {
            client_.OnReceiveTest_mocker.expects( exactly( count ) ).with( eq< const std::string >( LOCALHOST ), eq< const Test >( message ) );
        }

        void AddClientExpectation( EmptyMessage& message, unsigned int count = 1 )
        {
            client_.OnReceiveEmpty_mocker.expects( exactly( count ) ).with( eq< const std::string >( LOCALHOST ), eq< const EmptyMessage >( message ) );
        }

        MockServer server_;
        MockClient client_;
    };
}

BOOST_FIXTURE_TEST_SUITE( MessageTestSuite, MessageSendingFixture )

// -----------------------------------------------------------------------------
// Name: SerializationTest_SendOneMessageFromClientToServer
// Created: FHD 2009-08-24
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( SerializationTest_SendOneMessageFromClientToServer )
{
    MsgPion message;
    message.set_id( 101 );
    message.set_name( "Pion name\n" );
    VerifyServerReception( message );
}

// -----------------------------------------------------------------------------
// Name: SerializationTest_SendMultipleMessageToServer
// Created: SBO 2009-10-26
// $$$$ SBO 2009-10-26: mainly tests network interface performance
//                      see below for encoding performance tests
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( SerializationTest_SendMultipleMessageToServer )
{
    MsgPion message;
    message.set_id( 101 );
    message.set_name( "test" );
    VerifyServerReception( message, 10 );
//    VerifyServerReception( message, 100 );
//    VerifyServerReception( message, 1000 );
}

// -----------------------------------------------------------------------------
// Name: SerializationTest_SendManyBigMessageFromClientToServerMocked
// Created: FHD 2009-08-24
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( SerializationTest_SendManyBigMessageFromClientToServerMocked )
{
//    // Init    
//    std::cout << std::endl << "Client Connection" << std::endl;
//
//    MockServer server;
//    MockClient client;
//    server.ConnectionSucceeded_mocker.expects( once() ).with( mockpp::any() );
//    client.ConnectionSucceeded_mocker.expects( once() ).with( eq< const std::string >( LOCALHOST ) );
//    int nRetry = 0;
//    while( !client.Connected() )
//    {
//        BOOST_CHECK_NO_THROW( client.Update() );
//        BOOST_CHECK_NO_THROW( server.Update() );
//        std::cout << ".";
//        if (nRetry == 50)
//        {
//            std::cout << std::endl;
//            nRetry = 0;
//        }
//        nRetry++;
//    }
//    std::cout << std::endl << "Connected" << std::endl;
//    int nNbObj = 10;
//    int nNbDesc = 20;
//    int nNbLgDesc = 20000;
//    std::string o_desc;
//    for (int i = 0; i < nNbDesc; ++i)
//        o_desc += "a";
//    std::string o_longdesc;
//    char c = 'a';
//    for (int j = 0; j < nNbLgDesc; ++j)
//    {
//        o_longdesc += c;
//        if (c == 'z')
//            c = 'a';
//        else
//            c++;
//    }
//
//    int nNbFacettesPerObj = 10;
//    std::cout << std::endl << "Construct " << nNbObj << " ObjectInstances" << std::endl;
//    std::cout << "with " << nNbDesc << " caracters in desc " << std::endl;
//    std::cout << "with " << nNbLgDesc << " caracters in long desc " << std::endl;
//    std::cout << "with " << nNbFacettesPerObj << " facettes per object" << std::endl;
//    
//    int nSizeMsg = ((o_desc.size() + o_longdesc.size() + 4 + nNbFacettesPerObj * 9)) * nNbObj;
//    std::cout << "message size: " << nSizeMsg / 1024 << " Mo" << std::endl;
//
//    // Initilize messages
//    std::vector< ObjectInstance* > lstMsg;
//    for( int i = 1; i <= nNbObj; ++i )
//    {
//        // Create message 
//        ObjectInstance *objinst = new ObjectInstance();
//        //ObjectInstance objinst;
//        objinst->set_id(1);
//        objinst->set_desc(o_desc.c_str());
//        objinst->set_longdesc(o_longdesc.c_str());        
//        for (int i = 1; i <= nNbFacettesPerObj; ++i)
//        {
//            objinst->add_facette();
//            Facette *f = objinst->mutable_facette(i-1);
//            if (f)
//            {
//                f->set_id(i);
//                std::string f_name("default");
//                f->set_name(f_name);
//            }
//        }
//        lstMsg.push_back(objinst);
//    }
//
//    // send/receive
//
//    std::cout << "Sending..." << endl;
//    for (std::vector< ObjectInstance * >::iterator iter(lstMsg.begin()); iter != lstMsg.end(); ++iter)
//    {
//        BOOST_CHECK_NO_THROW( client.Send( LOCALHOST, **iter ) );
//        ptime now = microsec_clock::universal_time();
//        std::string buffer(to_simple_string(now));
//        printf("client: ObjectInstance sent at %s\n", buffer.c_str());
//
//        server.OnReceiveObjectInstance_mocker.expects( once() ).with( mockpp::any(), eq< const ObjectInstance >(**iter));
//        while( !server.Received() )
//        {
//            BOOST_CHECK_NO_THROW( client.Update() );
//            BOOST_CHECK_NO_THROW( server.Update() );
//        }
//        server.ResetReceived();
//    }
//    client.verify();
//    server.verify();
}

// -----------------------------------------------------------------------------
// Name: SerializationTest_SendMessageFromServerToClient
// Created: FHD 2009-08-24
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( SerializationTest_SendMessageBothWays )
{
    MsgPion message;
    message.set_id( 1 );
    message.set_name( "My name" );
    VerifyClientReception( message );

    Test test;
    test.set_name( "My test" );
    VerifyClientReception( test );
}


// -----------------------------------------------------------------------------
// Name: SerializationTest_SendEmptyMessage
// Created: FHD 2009-08-24
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( SerializationTest_SendEmptyMessage )
{
    EmptyMessage message;
    VerifyClientReception( message );
}

BOOST_AUTO_TEST_SUITE_END()

namespace
{
    bool operator==( const SimpleTest& lhs, const SimpleTest& rhs )
    {
        BOOST_CHECK_EQUAL( lhs.name(), rhs.name() );
        return lhs.name() == rhs.name();
    }

    bool operator==( const ZeroField& lhs, const ZeroField& rhs )
    {
        BOOST_CHECK_EQUAL( lhs.value(), rhs.value() );
        return lhs.value() == rhs.value();
    }

    bool operator==( const Composite& lhs, const Composite& rhs )
    {
        BOOST_CHECK_EQUAL( lhs.has_context(), rhs.has_context() );
        if( lhs.has_context() )
            BOOST_CHECK_EQUAL( lhs.context(), rhs.context() );
        BOOST_CHECK_EQUAL( lhs.has_simple(), rhs.has_simple() );
        if( lhs.has_simple() )
            BOOST_CHECK_EQUAL( lhs.simple().name(), rhs.simple().name() );
        BOOST_CHECK_EQUAL( lhs.has_zero(), rhs.has_zero() );
        if( lhs.has_zero() )
            BOOST_CHECK_EQUAL( lhs.zero().value(), rhs.zero().value() );
        return true;
    }

    bool operator==( const Extendable& lhs, const Extendable& rhs )
    {
        BOOST_CHECK_EQUAL( lhs.HasExtension( simple ), rhs.HasExtension( simple ) );
        return true;
    }

    struct EncodingFixture : public ProtobufFixture
    {
        template< typename M >
        void Verify( M& message, unsigned int count = 1 )
        {
            BOOST_REQUIRE( message.IsInitialized() );
            const boost::posix_time::ptime start = boost::posix_time::microsec_clock::universal_time();
            for( unsigned int i = 0; i < count; ++i )
            {
                std::auto_ptr< tools::MessageEncoder< M > > encoder;
                BOOST_REQUIRE_NO_THROW( encoder.reset( new tools::MessageEncoder< M >( message ) ) );
                const tools::Message& encodedMessage = *encoder;
                tools::Message encodedCopy = encodedMessage;
                std::auto_ptr< tools::MessageDecoder< M > > decoder;
                BOOST_REQUIRE_NO_THROW( decoder.reset( new tools::MessageDecoder< M >( StripHeader( encodedCopy ) ) ) );
                const M& decodedMessage = *decoder;
                BOOST_CHECK( message == decodedMessage );
                std::cout << "- Original size: " << sizeof( message ) << std::endl
                          << "- Encoded size: " << encodedMessage.Size() - 2 * sizeof( unsigned long ) << std::endl;
            }
            std::cout << "Encoded " << count << " message(s) in " 
                      << boost::posix_time::microsec_clock::universal_time() - start << std::endl;
        }

        tools::Message& StripHeader( tools::Message& message )
        {
            unsigned long size, tag;
            message >> size >> tag;
            return message;
        }
    };
}

BOOST_FIXTURE_TEST_SUITE( EncodingTestSuite, EncodingFixture )

// -----------------------------------------------------------------------------
// Name: ProtobufTest_EncodeSimpleTest
// Created: SEB 2009-10-25
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ProtobufTest_EncodeSimpleTest )
{
    SimpleTest message;
    message.set_name( "My name" );
    Verify( message, 10 );
//    Verify( message, 100 );
//    Verify( message, 1000 );
//    Verify( message, 10000 );
//    Verify( message, 100000 );
//    Verify( message, 1000000 );
}

// -----------------------------------------------------------------------------
// Name: ProtobufTest_EncodeCompositeStringTest
// Created: SBO 2009-11-02
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ProtobufTest_EncodeCompositeStringTest )
{
    const unsigned int count = 1024;
    CompositeStringTest message;
    for( unsigned int i = 0; i < count; ++i )
        message.add_test()->set_name( "My name" );
    Verify( message, 1 );
//    Verify( message, 10 );
//    Verify( message, 100 );
//    Verify( message, 1000 );
//    Verify( message, 10000 );
//    Verify( message, 100000 );
//    Verify( message, 1000000 );
}

// -----------------------------------------------------------------------------
// Name: ProtobufTest_EncodeCompositeIntegerTest
// Created: SBO 2009-11-02
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ProtobufTest_EncodeCompositeIntegerTest )
{
    const unsigned int count = 1024;
    CompositeIntegerTest message;
    for( unsigned int i = 0; i < count; ++i )
        message.add_test( 42 );
    Verify( message, 1 );
//    Verify( message, 10 );
//    Verify( message, 100 );
//    Verify( message, 1000 );
//    Verify( message, 10000 );
//    Verify( message, 100000 );
//    Verify( message, 1000000 );
}

// -----------------------------------------------------------------------------
// Name: ProtobufTest_EncodeZeroField
// Created: SEB 2009-10-25
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ProtobufTest_EncodeZeroField )
{
    ZeroField message;
    Verify( message );
    message.set_value( 1 );
    Verify( message );
    message.set_value( 5 );
    Verify( message );
}

// -----------------------------------------------------------------------------
// Name: ProtobufTest_EncodeComposite
// Created: SEB 2009-10-25
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ProtobufTest_EncodeComposite )
{
    Composite message;
    message.set_context( 0 );
    Verify( message );
    message.set_context( 5 );
    message.mutable_simple()->set_name( "My name" );
    Verify( message );
    message.Clear();
    message.set_context( 1 );
    message.mutable_zero()->set_value( 0 );
    Verify( message );
}

// -----------------------------------------------------------------------------
// Name: ProtobufTest_EncodeExtendable
// Created: SEB 2009-10-25
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ProtobufTest_EncodeExtendable )
{
    Extendable message;
    Verify( message );
    message.MutableExtension( simple )->set_name( "My name" );
    Verify( message );
    message.MutableExtension( zero )->set_value( 0 );
    Verify( message );
}

BOOST_AUTO_TEST_SUITE_END()

// -----------------------------------------------------------------------------
// Name: ProtobufTest_ErrorChecking
// Created: SEB 2009-10-25
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ProtobufTest_ErrorChecking )
{
    Composite message;
    std::auto_ptr< tools::MessageEncoder< Composite > > encoder;
    BOOST_CHECK_THROW( encoder.reset( new tools::MessageEncoder< Composite >( message ) ), std::runtime_error );

    google::protobuf::ShutdownProtobufLibrary(); // $$$$ FHD 2009-10-27: must be at the end of last test
}

BOOST_AUTO_TEST_CASE( ProtobufTest_ErrorChecking2 )
{
//    // initialization of descriptors
//    DescriptorPool pool;
//    FileDescriptorProto file;
//    file.set_name("test.proto");    
//    file.add_message_type()->set_name("Composite");
//    const FileDescriptor *filedesc = pool.BuildFile(file);
//    GOOGLE_CHECK(filedesc != NULL);
//    DynamicMessageFactory dynamic_factory(&pool);   
//
//    // define message
//    const Descriptor* desc = filedesc->FindMessageTypeByName( "Composite" );
//    Composite *m = (Composite*)(dynamic_factory.GetPrototype(desc)->New());
//
//    // fill message
//    m->set_context( 1 );
//    m->mutable_simple()->set_name( "frankly dumb" );
//    m->mutable_zero()->set_value( 11 );
//    std::cerr << m->InitializationErrorString() << std::endl;
}

