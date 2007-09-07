// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Message.h"

namespace tools {

// -----------------------------------------------------------------------------
// Name: AsnMessageEncoder constructor
// Created: NLD 2006-09-22
// -----------------------------------------------------------------------------
template< typename T, typename C >
AsnMessageEncoder< T, C >::AsnMessageEncoder( const T& asnMsg )
    : asnBuffer_ ( aAsnBuffer_, sizeof( aAsnBuffer_ ), TRUE )
    , asnMsgCtrl_( asnBuffer_, const_cast< T& >( asnMsg ) )
{
    Encode();
}

// -----------------------------------------------------------------------------
// Name: AsnMessageEncoder::Encode
// Created: NLD 2006-09-22
// -----------------------------------------------------------------------------
template< typename T, typename C >
void AsnMessageEncoder< T, C >::Encode()
{
    if( asnMsgCtrl_.Encode() != ASN_OK )
    {
        asnBuffer_.PrintErrorInfo();
        throw std::runtime_error( "Error while encoding asn message" );
    }
    message_.Write( (const char*)asnBuffer_.GetMsgPtr(), asnBuffer_.GetMsgLen() );
}

// -----------------------------------------------------------------------------
// Name: AsnMessageEncoder::GetDinMsg
// Created: NLD 2006-09-22
// -----------------------------------------------------------------------------
template< typename T, typename C >
AsnMessageEncoder< T, C >::operator const Message& () const
{
    return message_;
}

}