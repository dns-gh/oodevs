// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "masasn_test_pch.h"
#include "masasn/asn1PerCppTypes.h"

using namespace asn;

#include "TestTools.h"

namespace
{
    struct ASN1T_MsgAarInformation
    {
        ASN1VisibleString  information;
    };

    int asn1PE_MsgAarInformation( ASN1CTXT* ctxt_p, ASN1T_MsgAarInformation* pvalue )
    {
        pe_VisibleString (ctxt_p, pvalue->information, 0);
        return 0;
    }
    int asn1PD_MsgAarInformation( ASN1CTXT* ctxt_p, ASN1T_MsgAarInformation* pvalue )
    {
        pd_VisibleString (ctxt_p, &pvalue->information, 0);
        return 0;
    }
}

// -----------------------------------------------------------------------------
// Name: Test_EncodingSequence
// Created: AGE 2008-02-13
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Test_EncodingSequence )
{
    ASN1OCTET aAsnBuffer_[ 100000 ];
    ASN1PEREncodeBuffer buffer( aAsnBuffer_, sizeof( aAsnBuffer_ ), TRUE );
    MyController controller( buffer );
    buffer.Init();
    const std::string content( "my content" );
    ASN1T_MsgAarInformation value; value.information = content.c_str();
    asn1PE_MsgAarInformation( controller.GetPtr(), &value );

    /* const unsigned char* resultPointer = */ buffer.GetMsgPtr();
    /* unsigned resultSize =                */ buffer.GetMsgLen();
}

// -----------------------------------------------------------------------------
// Name: Test_DecodingSequence
// Created: AGE 2008-02-13
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Test_DecodingSequence )
{
    ASN1OCTET aAsnBuffer_[] = { 10, 'm', 'y', ' ', 'c', 'o', 'n', 't', 'e', 'n', 't' };
    ASN1PERDecodeBuffer buffer( aAsnBuffer_, sizeof( aAsnBuffer_ ), TRUE );
    MyController controller( buffer );
    ASN1T_MsgAarInformation value = { 0 };
    asn1PD_MsgAarInformation( controller.GetPtr(), &value );
}

// -----------------------------------------------------------------------------
// Name: Test_EncodingDecoding
// Created: AGE 2008-02-13
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Test_EncodingDecoding )
{
    ASN1OCTET aAsnBuffer_[ 100000 ];
    const std::string content( "my content" );
    ASN1T_MsgAarInformation value = { content.c_str() };
    {
        ASN1PEREncodeBuffer buffer( aAsnBuffer_, sizeof( aAsnBuffer_ ), TRUE );
        MyController controller( buffer );
        buffer.Init();
        asn1PE_MsgAarInformation( controller.GetPtr(), &value );
    }
    value.information = 0;
    {
        ASN1PERDecodeBuffer buffer( aAsnBuffer_, sizeof( aAsnBuffer_ ), TRUE );
        MyController controller( buffer );
        asn1PD_MsgAarInformation( controller.GetPtr(), &value );
    }
}
