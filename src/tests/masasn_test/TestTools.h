// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TestTools_h_
#define __TestTools_h_

#include <iomanip>

// =============================================================================
/** @class  TestTools
    @brief  TestTools
*/
// Created: AGE 2008-02-13
// =============================================================================
template< unsigned N >
inline
void CheckEqual( const unsigned char (&expected)[N], const unsigned char* buffer, unsigned size, const char* lib )
{
    BOOST_CHECK_MESSAGE( size == N, "Expectation failed using " << lib << " : expected size of " << N << ", got " << size );
    if( ::memcmp( expected, buffer, N )  )
    {
        std::stringstream message;
        message << "Expectation failed using " << lib << " :\nExpected { ";
        for( unsigned i = 0; i < N; ++i )
            message << std::hex << std::setw( 2 ) << std::setfill( '0' ) << int( expected[i] ) << ", ";
        message << " },\n     Got { ";
        for( unsigned i = 0; i < N; ++i )
            message << std::hex << std::setw( 2 ) << std::setfill( '0' ) << int( buffer[i] ) << ", ";
        message << " };\n";
        BOOST_FAIL( message.str() );
    }
}
template< unsigned N >
inline
void CheckEqual( const unsigned char (&expected)[N], const unsigned char* buffer, const char* lib )
{
    CheckEqual( expected, buffer, N, lib );
}

class MyController : public ASN1CType
{
public:
    MyController( ASN1MessageBuffer& msgBuf )
        : ASN1CType( msgBuf ) {};
    ASN1CTXT* GetPtr()
    {
        return mpContext->GetPtr();
    }
};

#define CHECK_BUFFERS( expected, actual, buffer ) CheckEqual( expected, actual, buffer.GetMsgLen(), TESTED_LIB )
#define CHECK_BUFFER( expected, actual )         CheckEqual( expected, actual, TESTED_LIB )

#endif // __TestTools_h_
