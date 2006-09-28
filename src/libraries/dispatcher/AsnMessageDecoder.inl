// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include <iostream>

namespace dispatcher {

// -----------------------------------------------------------------------------
// Name: AsnMessageDecoder constructor
// Created: NLD 2006-09-22
// -----------------------------------------------------------------------------
template< typename T, typename C > inline
AsnMessageDecoder< T, C >::AsnMessageDecoder( DIN::DIN_Input& dinMsg )
    : asnBuffer_ ( aAsnBuffer_, sizeof( aAsnBuffer_ ), TRUE )
    , asnMsg_    ()
    , asnMsgCtrl_( asnBuffer_, asnMsg_ )
{
    Decode( dinMsg );
}

// -----------------------------------------------------------------------------
// Name: AsnMessageDecoder constructor
// Created: NLD 2006-09-22
// -----------------------------------------------------------------------------
template< typename T, typename C > inline
AsnMessageDecoder< T, C >::AsnMessageDecoder( DIN::DIN_Input& dinMsg, unsigned long& nContext )
    : asnBuffer_ ( aAsnBuffer_, sizeof( aAsnBuffer_ ), TRUE )
    , asnMsg_    ()
    , asnMsgCtrl_( asnBuffer_, asnMsg_ )
{
    dinMsg >> nContext;
    Decode( dinMsg );
}

// -----------------------------------------------------------------------------
// Name: AsnMessageDecoder destructor
// Created: NLD 2006-09-22
// -----------------------------------------------------------------------------
template< typename T, typename C > inline
AsnMessageDecoder< T, C >::~AsnMessageDecoder()
{
}

// -----------------------------------------------------------------------------
// Name: AsnMessageDecoder::Decode
// Created: NLD 2006-09-22
// -----------------------------------------------------------------------------
template< typename T, typename C > inline
void AsnMessageDecoder< T, C >::Decode( DIN::DIN_Input& dinMsg )
{
    uint nAsnMsgSize = dinMsg.GetAvailable();
    assert( nAsnMsgSize <= sizeof( aAsnBuffer_ ) );

    memcpy( aAsnBuffer_, dinMsg.GetBuffer( nAsnMsgSize ), nAsnMsgSize );
    if( asnMsgCtrl_.Decode() != ASN_OK )
    {
        asnBuffer_.PrintErrorInfo();
        throw std::runtime_error( "Error while decoding asn message" );
    }
//    Dump();
}

// -----------------------------------------------------------------------------
// Name: AsnMessageDecoder::GetAsnMsg
// Created: NLD 2006-09-22
// -----------------------------------------------------------------------------
template< typename T, typename C > inline 
const T& AsnMessageDecoder< T, C >::GetAsnMsg() const
{
    return asnMsg_;
}

// -----------------------------------------------------------------------------
// Name: AsnMessageDecoder::Dump
// Created: NLD 2006-09-22
// -----------------------------------------------------------------------------
template< typename T, typename C > inline 
void AsnMessageDecoder< T, C >::Dump() const
{
    std::cout << "BEGIN MSG DUMP =>" << std::endl;
    const_cast< C& >( asnMsgCtrl_ ).Print( "Receiving msg" );
    std::cout << "END MSG DUMP =>" << std::endl;
}

}