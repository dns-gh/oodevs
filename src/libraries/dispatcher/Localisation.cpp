// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"

#include "Localisation.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Localisation constructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
Localisation::Localisation( const ASN1T_Localisation& asn )
    : nType_ ( asn.type )
    , points_( )
{
    for( uint i = 0; i < asn.vecteur_point.n; ++i )
        points_.push_back( Position( asn.vecteur_point.elem[ i ] ) );
}
 
// -----------------------------------------------------------------------------
// Name: Localisation destructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
Localisation::~Localisation()
{

}

// =============================================================================
// MAIN
// =============================================================================

// -----------------------------------------------------------------------------
// Name: Localisation::Update
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void Localisation::Update( const ASN1T_Localisation& asn )
{
    nType_ = asn.type;
    points_.clear();
    for( uint i = 0; i < asn.vecteur_point.n; ++i )
        points_.push_back( Position( asn.vecteur_point.elem[ i ] ) );
}

// -----------------------------------------------------------------------------
// Name: Localisation::Send
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void Localisation::Send( ASN1T_Localisation& asn ) const
{
    asn.type               = nType_;
    asn.vecteur_point.n    = points_.size();
    asn.vecteur_point.elem = new ASN1T_CoordUTM[ points_.size() ];
    for( uint i = 0; i < points_.size(); ++i )
        points_[ i ].Send( asn.vecteur_point.elem[ i ] );
}

// -----------------------------------------------------------------------------
// Name: Localisation::AsnDelete
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void Localisation::AsnDelete( ASN1T_Localisation& asn, bool bOptionalValue /*= true*/ )
{
    if( !bOptionalValue )
        return;

    if( asn.vecteur_point.n > 0 )
        delete [] asn.vecteur_point.elem;
}
