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
// Name: AsnMessageEncoder constructor
// Created: NLD 2006-09-22
// -----------------------------------------------------------------------------
template< typename T, typename C > inline
AsnMessageEncoder< T, C >::AsnMessageEncoder( DIN::DIN_MessageService_ABC& msgService, const T& asnMsg )
    : asnBuffer_ ( aAsnBuffer_, sizeof( aAsnBuffer_ ), TRUE )
    , asnMsgCtrl_( asnBuffer_, const_cast< T& >( asnMsg ) )
    , dinMsg_    ( msgService )
{
    Encode();
}

// -----------------------------------------------------------------------------
// Name: AsnMessageEncoder constructor
// Created: NLD 2006-09-22
// -----------------------------------------------------------------------------
template< typename T, typename C > inline
AsnMessageEncoder< T, C >::AsnMessageEncoder( DIN::DIN_MessageService_ABC& msgService, const T& asnMsg, unsigned long nContext )
    : asnBuffer_ ( aAsnBuffer_, sizeof( aAsnBuffer_ ), TRUE )
    , asnMsgCtrl_( asnBuffer_, const_cast< T& >( asnMsg ) )
    , dinMsg_    ( msgService )
{
    dinMsg_ << nContext;
    Encode();
}

// -----------------------------------------------------------------------------
// Name: AsnMessageEncoder destructor
// Created: NLD 2006-09-22
// -----------------------------------------------------------------------------
template< typename T, typename C > inline
AsnMessageEncoder< T, C >::~AsnMessageEncoder()
{
}

// -----------------------------------------------------------------------------
// Name: AsnMessageEncoder::Encode
// Created: NLD 2006-09-22
// -----------------------------------------------------------------------------
template< typename T, typename C > inline 
void AsnMessageEncoder< T, C >::Encode()
{
    if( asnMsgCtrl_.Encode() != ASN_OK )
    {
        asnBuffer_.PrintErrorInfo();
        throw std::runtime_error( "Error while encoding asn message" );
    }

    dinMsg_.GetOutput().Append( asnBuffer_.GetMsgPtr(), asnBuffer_.GetMsgLen() );
}

// -----------------------------------------------------------------------------
// Name: AsnMessageEncoder::GetDinMsg
// Created: NLD 2006-09-22
// -----------------------------------------------------------------------------
template< typename T, typename C > inline 
const DIN::DIN_BufferedMessage& AsnMessageEncoder< T, C >::GetDinMsg() const
{
    return dinMsg_;
}


}