// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: ::Test_ConstrainedUnsignedEncoding
// Created: AGE 2008-02-13
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Test_ConstrainedUnsignedEncoding )
{
    ASN1OCTET aAsnBuffer_[ 100000 ];
    ::memset( aAsnBuffer_, 0xCA, 100000 );
    ASN1PEREncodeBuffer buffer( aAsnBuffer_, sizeof( aAsnBuffer_ ), TRUE );
    MyController controller( buffer );
    buffer.Init();

    int result = 0;

    // < 255 => bitfields
    result = pe_ConsUnsigned( controller.GetPtr(), 5, 0, 12   ); // 12 -> 4
    result = pe_ConsUnsigned( controller.GetPtr(), 8, 0, 12   ); // 12 -> 4
    result = pe_ConsUnsigned( controller.GetPtr(), 5, 0, 12   ); // 12 -> 4

    // = 255 => align
    result = pe_ConsUnsigned( controller.GetPtr(), 0xDE,  0, 255 ); // 12 -> 4

    // < 64K => align 2 octets
    result = pe_ConsUnsigned( controller.GetPtr(), 0x0CDE, 0, 64000 );

    result = pe_ConsUnsigned( controller.GetPtr(), 5, 0, 12   ); // 12 -> 4

    // > 64K => taille (3bits) + align + octets
    result = pe_ConsUnsigned( controller.GetPtr(), 0xBCDEF, 0, 1000000 );

    const unsigned char expected[] = { 0x58, 0x50, 0xDE, 0x0C, 0xDE, 0x58, 0x0B, 0xCD, 0xEF };

    CHECK_BUFFERS( expected, aAsnBuffer_, buffer );
}

// -----------------------------------------------------------------------------
// Name: ::Test_ConstrainedUnsignedDecoding
// Created: AGE 2008-02-14
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Test_ConstrainedUnsignedDecoding )
{
    ASN1OCTET aAsnBuffer_[] = { 0x58, 0x50, 0xDE, 0x0C, 0xDE, 0x58, 0x0B, 0xCD, 0xEF };
    ASN1PERDecodeBuffer buffer( aAsnBuffer_, sizeof( aAsnBuffer_ ), TRUE );
    MyController controller( buffer );

    int result = 0;
    unsigned int value = 0;
    result = pd_ConsUnsigned( controller.GetPtr(), &value, 0, 12   );    BOOST_CHECK_EQUAL( 5u,       value );
    result = pd_ConsUnsigned( controller.GetPtr(), &value, 0, 12   );    BOOST_CHECK_EQUAL( 8u,       value );
    result = pd_ConsUnsigned( controller.GetPtr(), &value, 0, 12   );    BOOST_CHECK_EQUAL( 5u,       value );
    result = pd_ConsUnsigned( controller.GetPtr(), &value, 0, 255 );     BOOST_CHECK_EQUAL( 0xDEu,    value );
    result = pd_ConsUnsigned( controller.GetPtr(), &value, 0, 64000 );   BOOST_CHECK_EQUAL( 0x0CDEu,  value );
    result = pd_ConsUnsigned( controller.GetPtr(), &value, 0, 12   );    BOOST_CHECK_EQUAL( 5u,       value );
    result = pd_ConsUnsigned( controller.GetPtr(), &value, 0, 1000000 ); BOOST_CHECK_EQUAL( 0xBCDEFu, value );
}

// -----------------------------------------------------------------------------
// Name: Test_ConstrainedIntegerEncoding
// Created: AGE 2008-02-14
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Test_ConstrainedIntegerEncoding )
{
    ASN1OCTET aAsnBuffer_[ 100000 ];
    ::memset( aAsnBuffer_, 0xCA, 100000 );
    ASN1PEREncodeBuffer buffer( aAsnBuffer_, sizeof( aAsnBuffer_ ), TRUE );
    MyController controller( buffer );
    buffer.Init();

    int result = 0;

    result = pe_ConsInteger( controller.GetPtr(),  5, 0, 12 );
    result = pe_ConsInteger( controller.GetPtr(),  8, 0, 12 );

    result = pe_ConsInteger( controller.GetPtr(), -1, -6, 6 );
    result = pe_ConsInteger( controller.GetPtr(),  2, -6, 6 );

    result = pe_ConsInteger( controller.GetPtr(), 50, -100, 100 ); // 150 -> 96

    result = pe_ConsInteger( controller.GetPtr(), 0x0CDE, 0, 64000 );
    result = pe_ConsInteger( controller.GetPtr(), 5, 0, 12 );

    // > 64K => taille (3bits) + align + octets
    result = pe_ConsInteger( controller.GetPtr(), 0xBCDEF - 100, -100, 999900 );

    const unsigned char expected[] = { 0x58, 0x58, 0x96, 0x0C, 0xDE, 0x58, 0x0B, 0xCD, 0xEF };

    CHECK_BUFFERS( expected, aAsnBuffer_, buffer );
}

// -----------------------------------------------------------------------------
// Name: ::Test_ConstrainedIntegerDecoding
// Created: AGE 2008-02-14
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Test_ConstrainedIntegerDecoding )
{
    ASN1OCTET aAsnBuffer_[] = { 0x58, 0x58, 0x96, 0x0C, 0xDE, 0x58, 0x0B, 0xCD, 0xEF };
    ASN1PERDecodeBuffer buffer( aAsnBuffer_, sizeof( aAsnBuffer_ ), TRUE );
    MyController controller( buffer );

    int result = 0;
    int value = 0;
    result = pd_ConsInteger( controller.GetPtr(), &value, 0, 12   );      BOOST_CHECK_EQUAL( 5,      value );
    result = pd_ConsInteger( controller.GetPtr(), &value, 0, 12   );      BOOST_CHECK_EQUAL( 8,      value );
    result = pd_ConsInteger( controller.GetPtr(), &value, -6, 6   );      BOOST_CHECK_EQUAL( -1,     value );
    result = pd_ConsInteger( controller.GetPtr(), &value, -6, 6   );      BOOST_CHECK_EQUAL( 2,      value );
    result = pd_ConsInteger( controller.GetPtr(), &value, -100, 100 );    BOOST_CHECK_EQUAL( 50,     value );
    result = pd_ConsInteger( controller.GetPtr(), &value, 0, 64000 );     BOOST_CHECK_EQUAL( 0x0CDE, value );
    result = pd_ConsInteger( controller.GetPtr(), &value, 0, 12   );      BOOST_CHECK_EQUAL( 5,      value );
    result = pd_ConsInteger( controller.GetPtr(), &value, -100, 999900 ); BOOST_CHECK_EQUAL( 0xBCDEF - 100, value );
}

// -----------------------------------------------------------------------------
// Name: Test_UnconstrainedIntegerEncoding
// Created: AGE 2008-02-14
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Test_UnconstrainedIntegerEncoding )
{
    ASN1OCTET aAsnBuffer_[ 100000 ];
    ::memset( aAsnBuffer_, 0xCA, 100000 );
    ASN1PEREncodeBuffer buffer( aAsnBuffer_, sizeof( aAsnBuffer_ ), TRUE );
    MyController controller( buffer );
    buffer.Init();

    int result = 0;

    result = pe_UnconsInteger( controller.GetPtr(),  5  );
    result = pe_UnconsInteger( controller.GetPtr(), -1  );
    result = pe_UnconsInteger( controller.GetPtr(),  50 );
    result = pe_UnconsInteger( controller.GetPtr(), 0x0CDE );
    result = pe_UnconsInteger( controller.GetPtr(),-0x0CDE );
    result = pe_UnconsInteger( controller.GetPtr(), 0xBCDEF );
    result = pe_UnconsInteger( controller.GetPtr(), 0x09ABCDEF );

    const unsigned char expected[] = { 0x01, 0x05, 0x01, 0xFF, 0x01, 0x32,
                                       0x02, 0x0C, 0xDE,
                                       0x02, 0xf3, 0x22,
                                       0x03, 0x0B, 0xCD, 0xEF,
                                       0x04, 0x09, 0xAB, 0xCD, 0xEF };

    CHECK_BUFFERS( expected, aAsnBuffer_, buffer );
}

// -----------------------------------------------------------------------------
// Name: ::Test_UnconstrainedIntegerDecoding
// Created: AGE 2008-02-14
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Test_UnconstrainedIntegerDecoding )
{
    ASN1OCTET aAsnBuffer_[] = { 0x01, 0x05, 0x01, 0xFF, 0x01, 0x32,
                                0x02, 0x0C, 0xDE,
                                0x02, 0xf3, 0x22,
                                0x03, 0x0B, 0xCD, 0xEF,
                                0x04, 0x09, 0xAB, 0xCD, 0xEF };
    ASN1PERDecodeBuffer buffer( aAsnBuffer_, sizeof( aAsnBuffer_ ), TRUE );
    MyController controller( buffer );

    int result = 0;
    int value = 0;
    result = pd_UnconsInteger( controller.GetPtr(), &value ); BOOST_CHECK_EQUAL(  5,          value );
    result = pd_UnconsInteger( controller.GetPtr(), &value ); BOOST_CHECK_EQUAL( -1,          value );
    result = pd_UnconsInteger( controller.GetPtr(), &value ); BOOST_CHECK_EQUAL(  50,         value );
    result = pd_UnconsInteger( controller.GetPtr(), &value ); BOOST_CHECK_EQUAL(  0x0CDE,     value );
    result = pd_UnconsInteger( controller.GetPtr(), &value ); BOOST_CHECK_EQUAL( -0x0CDE,     value );
    result = pd_UnconsInteger( controller.GetPtr(), &value ); BOOST_CHECK_EQUAL(  0xBCDEF,    value );
    result = pd_UnconsInteger( controller.GetPtr(), &value ); BOOST_CHECK_EQUAL(  0x09ABCDEF, value );
}

// -----------------------------------------------------------------------------
// Name: Test_LengthEncoding
// Created: AGE 2008-02-14
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Test_LengthEncoding )
{
    ASN1OCTET aAsnBuffer_[ 100000 ];
    ::memset( aAsnBuffer_, 0xCA, 100000 );
    ASN1PEREncodeBuffer buffer( aAsnBuffer_, sizeof( aAsnBuffer_ ), TRUE );
    MyController controller( buffer );
    buffer.Init();

    int result = 0;

    result = pe_Length( controller.GetPtr(), 1 );
    result = pe_Length( controller.GetPtr(), 2 );
    result = pe_Length( controller.GetPtr(), 3 );
    result = pe_Length( controller.GetPtr(), 4 );
    result = pe_Length( controller.GetPtr(), 127 );
    result = pe_Length( controller.GetPtr(), 128 );
    result = pe_Length( controller.GetPtr(), 150 );
    result = pe_Length( controller.GetPtr(), 15000 );

    const unsigned char expected[] = { 0x01, 0x02, 0x03, 0x04, 0x7F,
                                       0x80, 0x80, 0x80, 0x96,
                                       0xBA, 0x98 };

    CHECK_BUFFERS( expected, aAsnBuffer_, buffer );
}

// -----------------------------------------------------------------------------
// Name: ::Test_LengthDecoding
// Created: AGE 2008-02-14
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Test_LengthDecoding )
{
    ASN1OCTET aAsnBuffer_[] = { 0x01, 0x02, 0x03, 0x04, 0x7F,
                                0x80, 0x80, 0x80, 0x96,
                                0xBA, 0x98 };
    ASN1PERDecodeBuffer buffer( aAsnBuffer_, sizeof( aAsnBuffer_ ), TRUE );
    MyController controller( buffer );

    int result = 0;
    unsigned int value = 0;
    result = pd_Length( controller.GetPtr(), &value ); BOOST_CHECK_EQUAL( 1u,     value );
    result = pd_Length( controller.GetPtr(), &value ); BOOST_CHECK_EQUAL( 2u,     value );
    result = pd_Length( controller.GetPtr(), &value ); BOOST_CHECK_EQUAL( 3u,     value );
    result = pd_Length( controller.GetPtr(), &value ); BOOST_CHECK_EQUAL( 4u,     value );
    result = pd_Length( controller.GetPtr(), &value ); BOOST_CHECK_EQUAL( 127u,   value );
    result = pd_Length( controller.GetPtr(), &value ); BOOST_CHECK_EQUAL( 128u,   value );
    result = pd_Length( controller.GetPtr(), &value ); BOOST_CHECK_EQUAL( 150u,   value );
    result = pd_Length( controller.GetPtr(), &value ); BOOST_CHECK_EQUAL( 15000u, value );
}

// -----------------------------------------------------------------------------
// Name: Test_RealEncoding
// Created: AGE 2008-02-14
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Test_RealEncoding )
{
    ASN1OCTET aAsnBuffer_[ 100000 ];
    ::memset( aAsnBuffer_, 0xCA, 100000 );
    ASN1PEREncodeBuffer buffer( aAsnBuffer_, sizeof( aAsnBuffer_ ), TRUE );
    MyController controller( buffer );
    buffer.Init();

    int result = 0;

    result = pe_Real( controller.GetPtr(), 0. );
    result = pe_Real( controller.GetPtr(), 1. );
    result = pe_Real( controller.GetPtr(), 12.5 );
    result = pe_Real( controller.GetPtr(),-12.5 );
    result = pe_Real( controller.GetPtr(), 1.5893e5 );

    const unsigned char expected[] = { 0x00,
                                       0x03, 0x80, 0x00, 0x01,
                                       0x03, 0x80, 0xFF, 0x19,
                                       0x03, 0xC0, 0xFF, 0x19,
                                       0x05, 0x80, 0x01, 0x01, 0x36, 0x69 };

    CHECK_BUFFERS( expected, aAsnBuffer_, buffer );
}

// -----------------------------------------------------------------------------
// Name: ::Test_RealDecoding
// Created: AGE 2008-02-14
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Test_RealDecoding )
{
    ASN1OCTET aAsnBuffer_[] = { 0x00,
                                0x03, 0x80, 0x00, 0x01,
                                0x03, 0x80, 0xFF, 0x19,
                                0x03, 0xC0, 0xFF, 0x19,
                                0x05, 0x80, 0x01, 0x01, 0x36, 0x69 };
    ASN1PERDecodeBuffer buffer( aAsnBuffer_, sizeof( aAsnBuffer_ ), TRUE );
    MyController controller( buffer );

    int result = 0;
    double value = 0;
    result = pd_Real( controller.GetPtr(), &value ); BOOST_CHECK_EQUAL(   0.,     value );
    result = pd_Real( controller.GetPtr(), &value ); BOOST_CHECK_EQUAL(   1.,     value );
    result = pd_Real( controller.GetPtr(), &value ); BOOST_CHECK_EQUAL(  12.5,    value );
    result = pd_Real( controller.GetPtr(), &value ); BOOST_CHECK_EQUAL( -12.5,    value );
    result = pd_Real( controller.GetPtr(), &value ); BOOST_CHECK_EQUAL( 1.5893e5, value );
}

// -----------------------------------------------------------------------------
// Name: ::Test_RealDecoding
// Created: AGE 2008-02-14
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Test_RealEncodingBugCsem )
{
    if( TESTED_LIB == std::string( "masasn" ) )
    {
        ASN1OCTET aAsnBuffer_[ 100000 ];
        ::memset( aAsnBuffer_, 0xCA, 100000 );
        ASN1PEREncodeBuffer buffer( aAsnBuffer_, sizeof( aAsnBuffer_ ), TRUE );
        MyController controller( buffer );
        buffer.Init();

        int result = 0;
        double value = 0;
        result = pe_Real( controller.GetPtr(), -0.50032089451830497 );
        result = pd_Real( controller.GetPtr(), &value );
        BOOST_CHECK_EQUAL( -0.50032089451830497, value );
    }
}

// -----------------------------------------------------------------------------
// Name: Test_OctetStringEncoding
// Created: AGE 2008-02-14
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Test_OctetStringEncoding )
{
    ASN1OCTET aAsnBuffer_[ 100000 ];
    ::memset( aAsnBuffer_, 0xCA, 100000 );
    ASN1PEREncodeBuffer buffer( aAsnBuffer_, sizeof( aAsnBuffer_ ), TRUE );
    MyController controller( buffer );
    buffer.Init();

    int result = 0;
    const unsigned char* data0 = (const unsigned char*)0xcaca;
    const unsigned char data1[] = { 0xca };
    const unsigned char data2[] = { 0xca, 0xfe };

    result = pe_OctetString( controller.GetPtr(), 0, data0 );
    result = pe_OctetString( controller.GetPtr(), 1, data1 );
    result = pe_OctetString( controller.GetPtr(), 2, data2 );

    const unsigned char expected[] = { 0x00,
                                       0x01, 0xca,
                                       0x02, 0xca, 0xfe };

    CHECK_BUFFERS( expected, aAsnBuffer_, buffer );
}

// -----------------------------------------------------------------------------
// Name: ::Test_OctetStringDecoding
// Created: AGE 2008-02-14
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Test_OctetStringDecoding )
{
    ASN1OCTET aAsnBuffer_[] = { 0x00,
                                0x01, 0xca,
                                0x02, 0xca, 0xfe };
    ASN1PERDecodeBuffer buffer( aAsnBuffer_, sizeof( aAsnBuffer_ ), TRUE );
    MyController controller( buffer );

    ASN1OCTET resultBuffer[] = {0,0}; unsigned int size = 0;
    int result = 0;
    result = pd_OctetString( controller.GetPtr(), &size, resultBuffer, 2 ); BOOST_CHECK_EQUAL( 0u, size );
    result = pd_OctetString( controller.GetPtr(), &size, resultBuffer, 2 ); BOOST_CHECK_EQUAL( 1u, size ); BOOST_CHECK_EQUAL( 0xcau, resultBuffer[0] );
    result = pd_OctetString( controller.GetPtr(), &size, resultBuffer, 2 ); BOOST_CHECK_EQUAL( 2u, size ); BOOST_CHECK_EQUAL( 0xcau, resultBuffer[0] ); BOOST_CHECK_EQUAL( 0xfeu, resultBuffer[1] );
}

// -----------------------------------------------------------------------------
// Name: ::Test_DynamicOctetStringDecoding
// Created: AGE 2008-02-15
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Test_DynamicOctetStringDecoding )
{
    ASN1OCTET aAsnBuffer_[] = { 0x00,
                                0x01, 0xca,
                                0x02, 0xca, 0xfe };
    ASN1PERDecodeBuffer buffer( aAsnBuffer_, sizeof( aAsnBuffer_ ), TRUE );
    MyController controller( buffer );

    ASN1DynOctStr myString = { 0, 0 };
    int result = 0;
    result = pd_DynOctetString( controller.GetPtr(), &myString ); BOOST_CHECK_EQUAL( 0u, myString.numocts );
    result = pd_DynOctetString( controller.GetPtr(), &myString ); BOOST_CHECK_EQUAL( 1u, myString.numocts ); BOOST_CHECK_EQUAL( 0xcau, myString.data[0] );
    result = pd_DynOctetString( controller.GetPtr(), &myString ); BOOST_CHECK_EQUAL( 2u, myString.numocts ); BOOST_CHECK_EQUAL( 0xcau, myString.data[0] ); BOOST_CHECK_EQUAL( 0xfeu, myString.data[1] );
}

// -----------------------------------------------------------------------------
// Name: Test_BitStringEncoding
// Created: AGE 2008-02-14
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Test_BitStringEncoding )
{
    ASN1OCTET aAsnBuffer_[ 100000 ];
    ::memset( aAsnBuffer_, 0xCA, 100000 );
    ASN1PEREncodeBuffer buffer( aAsnBuffer_, sizeof( aAsnBuffer_ ), TRUE );
    MyController controller( buffer );
    buffer.Init();
    int result = 0;

    const unsigned char data[] = { 0xca, 0xfe, 0xbd };

    result = pe_BitString( controller.GetPtr(),  0, data );
    result = pe_BitString( controller.GetPtr(),  1, data );
    result = pe_BitString( controller.GetPtr(),  2, data );
    result = pe_BitString( controller.GetPtr(),  7, data );
    result = pe_BitString( controller.GetPtr(), 13, data );
    result = pe_BitString( controller.GetPtr(), 16, data );
    result = pe_BitString( controller.GetPtr(), 22, data );

    const unsigned char expected[] = { 0x00,
                                       0x01, 0x80,
                                       0x02, 0xC0,
                                       0x07, 0xCA,
                                       0x0d, 0xCA, 0xF8,
                                       0x10, 0xCA, 0xFE,
                                       0x16, 0xCA, 0xFE, 0xBC };

    CHECK_BUFFERS( expected, aAsnBuffer_, buffer );
}

// -----------------------------------------------------------------------------
// Name: ::Test_BitStringDecoding
// Created: AGE 2008-02-14
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Test_BitStringDecoding )
{
    ASN1OCTET aAsnBuffer_[] = { 0x00,
                                0x01, 0x80,
                                0x02, 0xC0,
                                0x07, 0xCA,
                                0x0d, 0xCA, 0xF8,
                                0x10, 0xCA, 0xFE,
                                0x16, 0xCA, 0xFE, 0xBC };
    ASN1PERDecodeBuffer buffer( aAsnBuffer_, sizeof( aAsnBuffer_ ), TRUE );
    MyController controller( buffer );

    ASN1OCTET resultBuffer[] = {0,0,0}; unsigned int size = 0;
    int result = 0;
    result = pd_BitString( controller.GetPtr(), &size, resultBuffer, 3 ); BOOST_CHECK_EQUAL(  0u, size );
    result = pd_BitString( controller.GetPtr(), &size, resultBuffer, 3 ); BOOST_CHECK_EQUAL(  1u, size );
    result = pd_BitString( controller.GetPtr(), &size, resultBuffer, 3 ); BOOST_CHECK_EQUAL(  2u, size );
    result = pd_BitString( controller.GetPtr(), &size, resultBuffer, 3 ); BOOST_CHECK_EQUAL(  7u, size );
    result = pd_BitString( controller.GetPtr(), &size, resultBuffer, 3 ); BOOST_CHECK_EQUAL( 13u, size );
    result = pd_BitString( controller.GetPtr(), &size, resultBuffer, 3 ); BOOST_CHECK_EQUAL( 16u, size );
    result = pd_BitString( controller.GetPtr(), &size, resultBuffer, 3 ); BOOST_CHECK_EQUAL( 22u, size );

    BOOST_CHECK_EQUAL( 0xcau, resultBuffer[0] );
    BOOST_CHECK_EQUAL( 0xfeu, resultBuffer[1] );
    BOOST_CHECK_EQUAL( 0xbcu, resultBuffer[2] );
}

// -----------------------------------------------------------------------------
// Name: ::Test_BitStringDecodingUninitializedChunk
// Created: SBO 2008-11-28
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Test_BitStringDecodingUninitializedChunk )
{
    ASN1OCTET aAsnBuffer_[] = { 0x0b, 0x81, 0x01,
                                0x0c, 0x81, 0xF1 };
    ASN1PERDecodeBuffer buffer( aAsnBuffer_, sizeof( aAsnBuffer_ ), TRUE );
    MyController controller( buffer );

    ASN1OCTET resultBuffer[] = {0,0,0}; unsigned int size = 0;
    int result = 0;
    result = pd_BitString( controller.GetPtr(), &size, resultBuffer, 3 ); BOOST_CHECK_EQUAL(  11u, size );
   
    BOOST_CHECK_EQUAL( 0x81u, resultBuffer[0] );
    BOOST_CHECK_EQUAL( 0x00u, resultBuffer[1] );

    result = pd_BitString( controller.GetPtr(), &size, resultBuffer, 3 ); BOOST_CHECK_EQUAL(  12u, size );
    BOOST_CHECK_EQUAL( 0x81u, resultBuffer[0] );
    BOOST_CHECK_EQUAL( 0xF0u, resultBuffer[1] );
}

// -----------------------------------------------------------------------------
// Name: Test_VisibleStringEncoding
// Created: AGE 2008-02-14
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Test_VisibleStringEncoding )
{
    ASN1OCTET aAsnBuffer_[ 100000 ];
    ::memset( aAsnBuffer_, 0xCA, 100000 );
    ASN1PEREncodeBuffer buffer( aAsnBuffer_, sizeof( aAsnBuffer_ ), TRUE );
    MyController controller( buffer );
    buffer.Init();
    int result = 0;

    const char* myString = "Coucou !";
    const char* longText = "Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt "
                           "ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco "
                           "laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in "
                           "voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat "
                           "non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";

    result = pe_VisibleString( controller.GetPtr(), myString, 0 );
    result = pe_VisibleString( controller.GetPtr(), longText, 0 );
    result = pe_VisibleString( controller.GetPtr(), "", 0 ); // adds lorem ipsum trailing \0

    const unsigned char expected[] = { 0x08, 'C', 'o', 'u', 'c', 'o', 'u', ' ', '!',
                                       0x81, 0xBE, "Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt "
                                                   "ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco "
                                                   "laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in "
                                                   "voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat "
                                                   "non proident, sunt in culpa qui officia deserunt mollit anim id est laborum." };

    CHECK_BUFFERS( expected, aAsnBuffer_, buffer );
}

// -----------------------------------------------------------------------------
// Name: Test_VisibleStringDecoding
// Created: AGE 2008-02-14
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Test_VisibleStringDecoding )
{
    ASN1OCTET aAsnBuffer_[] = { 0x08, 'C', 'o', 'u', 'c', 'o', 'u', ' ', '!' };
    ASN1PERDecodeBuffer buffer( aAsnBuffer_, sizeof( aAsnBuffer_ ), TRUE );
    MyController controller( buffer );

    int result = 0; const char* myString = 0;
    result = pd_VisibleString( controller.GetPtr(), &myString, 0 );
    BOOST_CHECK_EQUAL( std::string( "Coucou !" ), myString );
}

// -----------------------------------------------------------------------------
// Name: Test_SizeConstraintsWithEncoding
// Created: AGE 2008-02-14
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Test_SizeConstraintsWithEncoding )
{
    ASN1OCTET aAsnBuffer_[ 100000 ];
    ::memset( aAsnBuffer_, 0xCA, 100000 );
    ASN1PEREncodeBuffer buffer( aAsnBuffer_, sizeof( aAsnBuffer_ ), TRUE );
    MyController controller( buffer );
    buffer.Init();
    int result = 0;

    Asn1SizeCnst size = { 0, 15, 15, 0 };
    const char* constraintString = "123456789ABCDEF";

    pu_addSizeConstraint( controller.GetPtr(), &size );
    result = pe_OctetString( controller.GetPtr(), 15, (const unsigned char*)constraintString );

    const unsigned char expected[] = { '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
    CHECK_BUFFERS( expected, aAsnBuffer_, buffer );
}


// -----------------------------------------------------------------------------
// Name: Test_SizeConstraintsWithDecoding
// Created: AGE 2008-02-14
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Test_SizeConstraintsWithDecoding )
{
    ASN1OCTET aAsnBuffer_[] = { '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
    ASN1PERDecodeBuffer buffer( aAsnBuffer_, sizeof( aAsnBuffer_ ), TRUE );
    MyController controller( buffer );
    int result = 0;

    ASN1OCTET resultBuffer[20]; ::memset( resultBuffer, 0, 20 );

    Asn1SizeCnst size = { 0, 15, 15, 0 };
    pu_addSizeConstraint( controller.GetPtr(), &size );
    unsigned resultSize = 0;
    result = pd_OctetString( controller.GetPtr(), &resultSize, resultBuffer, 20 );

    BOOST_CHECK_EQUAL( 15u, resultSize );
    CHECK_BUFFER( aAsnBuffer_, resultBuffer );
}
