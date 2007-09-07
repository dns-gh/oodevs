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
// Name: AsnMessageDecoder constructor
// Created: NLD 2006-09-22
// -----------------------------------------------------------------------------
template< typename T, typename C >
AsnMessageDecoder< T, C >::AsnMessageDecoder( Message& message )
    : asnBuffer_ ( aAsnBuffer_, sizeof( aAsnBuffer_ ), TRUE )
    , asnMsg_    ()
    , asnMsgCtrl_( asnBuffer_, asnMsg_ )
{
    Decode( message );
}

// -----------------------------------------------------------------------------
// Name: AsnMessageDecoder::Decode
// Created: NLD 2006-09-22
// -----------------------------------------------------------------------------
template< typename T, typename C >
void AsnMessageDecoder< T, C >::Decode( Message& message )
{
    const unsigned long size = message.Size();
    message.Read( (char*)aAsnBuffer_, size );

    if( asnMsgCtrl_.Decode() != ASN_OK )
    {
        asnBuffer_.PrintErrorInfo();
        throw std::runtime_error( "Error while decoding asn message" );
    }
}

// -----------------------------------------------------------------------------
// Name: AsnMessageDecoder::GetAsnMsg
// Created: NLD 2006-09-22
// -----------------------------------------------------------------------------
template< typename T, typename C >
AsnMessageDecoder< T, C >::operator const T& () const
{
    return asnMsg_;
}

}