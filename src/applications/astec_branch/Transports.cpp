// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "Transports.h"
#include "Controller.h"

// -----------------------------------------------------------------------------
// Name: Transports constructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Transports::Transports( Controller& controller, const Resolver_ABC< Agent >& resolver )
    : controller_( controller )
    , resolver_( resolver )
    , transporter_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Transports destructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Transports::~Transports()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Transports::Update
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
void Transports::Update( const ASN1T_MsgUnitAttributes& message )
{
    if( message.m.pions_transportesPresent )
    {
        transported_.clear();
        transported_.resize( message.pions_transportes.n );
        for( unsigned i = 0; i < message.pions_transportes.n; ++i )
            transported_.push_back( resolver_.Find( message.pions_transportes.elem[i] ) );
    }

    if( message.m.pion_transporteurPresent )
        transporter_ = resolver_.Find( message.pion_transporteur );

    controller_.Update( *this );
}
