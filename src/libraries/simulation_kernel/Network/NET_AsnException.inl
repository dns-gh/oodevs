// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: NET_AsnException constructor
// Created: NLD 2006-11-13
// -----------------------------------------------------------------------------
template< typename T > inline
NET_AsnException< T >::NET_AsnException( T nErrorID )
    : nErrorID_( nErrorID )
{
}

// -----------------------------------------------------------------------------
// Name: NET_AsnException destructor
// Created: NLD 2006-11-13
// -----------------------------------------------------------------------------
template< typename T > inline
NET_AsnException< T >::~NET_AsnException()
{

}

// -----------------------------------------------------------------------------
// Name: NET_AsnException::what
// Created: NLD 2006-11-13
// -----------------------------------------------------------------------------
template< typename T > inline
const char* NET_AsnException< T >::what() const
{
    return "Invalid parameter in ASN message";
}

// -----------------------------------------------------------------------------
// Name: NET_AsnException::GetErrorID
// Created: NLD 2006-11-13
// -----------------------------------------------------------------------------
template< typename T > inline
T NET_AsnException< T >::GetErrorID() const
{
    return nErrorID_;
}
