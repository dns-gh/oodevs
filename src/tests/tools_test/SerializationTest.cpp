// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "tools_test_pch.h"
#include "Protocol.h"
#include "tools/AsnControllers.h"
#include "tools/AsnMessageDecoder.h"
#include "tools/AsnMessageEncoder.h"
#include <boost/date_time/posix_time/posix_time.hpp>

using namespace mockpp;

DECLARE_ASN_CONTROLLER( SimpleTest )
DECLARE_ASN_CONTROLLER( CompositeIntegerTest )
DECLARE_ASN_CONTROLLER( CompositeStringTest )

namespace
{
    bool operator==( const ASN1T_SimpleTest& lhs, const ASN1T_SimpleTest& rhs )
    {
        BOOST_REQUIRE_EQUAL( lhs.name, rhs.name );
        return true;
    }

    bool operator==( const ASN1T_CompositeStringTest& lhs, const ASN1T_CompositeStringTest& rhs )
    {
        BOOST_REQUIRE_EQUAL( lhs.test.n, lhs.test.n );
        for( unsigned int i = 0; i < lhs.test.n; ++i )
            BOOST_REQUIRE_EQUAL( lhs.test.elem[i].name, rhs.test.elem[i].name );
        return true;
    }

    bool operator==( const ASN1T_CompositeIntegerTest& lhs, const ASN1T_CompositeIntegerTest& rhs )
    {
        BOOST_REQUIRE_EQUAL( lhs.test.n, lhs.test.n );
        for( unsigned int i = 0; i < lhs.test.n; ++i )
            BOOST_REQUIRE_EQUAL( lhs.test.elem[i], rhs.test.elem[i] );
        return true;
    }

    struct EncodingFixture
    {
        template< typename M >
        void Verify( M& message, unsigned int count = 1 )
        {
            const boost::posix_time::ptime start = boost::posix_time::microsec_clock::universal_time();
            for( unsigned int i = 0; i < count; ++i )
            {
                std::auto_ptr< tools::AsnMessageEncoder< M > > encoder;
                BOOST_REQUIRE_NO_THROW( encoder.reset( new tools::AsnMessageEncoder< M >( message ) ) );
                const tools::Message& encodedMessage = *encoder;
                tools::Message encodedCopy = encodedMessage;
                std::auto_ptr< tools::AsnMessageDecoder< M > > decoder;
                BOOST_REQUIRE_NO_THROW( decoder.reset( new tools::AsnMessageDecoder< M >( StripHeader( encodedCopy ) ) ) );
                const M& decodedMessage = *decoder;
                BOOST_CHECK( message == decodedMessage );
                if( i == 0 )
                    std::cout << "- Original/Encoded size: " << sizeof( message ) << "/" << encodedMessage.Size() - 2 * sizeof( unsigned long ) << std::endl;
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
// Name: AsnTest_EncodeSimpleTest
// Created: SBO 2009-11-02
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( AsnTest_EncodeSimpleTest )
{
    ASN1T_SimpleTest message;
    message.name = "My name";
    Verify( message, 10 );
//    Verify( message, 100 );
//    Verify( message, 1000 );
//    Verify( message, 10000 );
//    Verify( message, 100000 );
//    Verify( message, 1000000 );
}

// -----------------------------------------------------------------------------
// Name: AsnTest_EncodeCompositeStringTest
// Created: SBO 2009-11-02
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( AsnTest_EncodeCompositeStringTest )
{
    const unsigned int count = 1024;
    ASN1T_CompositeStringTest message;
    message.test.n = count;
    message.test.elem = new ASN1T_SimpleTest[ count ];
    for( unsigned int i = 0; i < count; ++i )
        message.test.elem[i].name = "My name";
    Verify( message, 10 );
//    Verify( message, 100 );
//    Verify( message, 1000 );
//    Verify( message, 10000 );
//    Verify( message, 100000 );
//    Verify( message, 1000000 );
}

// -----------------------------------------------------------------------------
// Name: AsnTest_EncodeCompositeIntegerTest
// Created: SBO 2009-11-02
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( AsnTest_EncodeCompositeIntegerTest )
{
    const unsigned int count = 1024;
    ASN1T_CompositeIntegerTest message;
    message.test.n = count;
    message.test.elem = new ASN1INT[ count ];
    for( unsigned int i = 0; i < count; ++i )
        message.test.elem[i] = 42;
    Verify( message, 10 );
//    Verify( message, 100 );
//    Verify( message, 1000 );
//    Verify( message, 10000 );
//    Verify( message, 100000 );
//    Verify( message, 1000000 );
}

BOOST_AUTO_TEST_SUITE_END()
