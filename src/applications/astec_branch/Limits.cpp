// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "Limits.h"

// -----------------------------------------------------------------------------
// Name: Limits constructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Limits::Limits( const Resolver_ABC< TacticalLine_ABC >& resolver )
    : resolver_( resolver )
    , acknowledged_( false )
    , left_( 0 )
    , right_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Limits destructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Limits::~Limits()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Limits::Update
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
void Limits::Update( const ASN1T_MsgPionOrder& message )
{
    acknowledged_ = false;
    if( message.m.oid_limite_droitePresent )
        right_ = resolver_.Find( message.oid_limite_droite );
    if( message.m.oid_limite_gauchePresent )
        left_  = resolver_.Find( message.oid_limite_gauche );
}

// -----------------------------------------------------------------------------
// Name: Limits::Update
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
void Limits::Update( const ASN1T_MsgPionOrderAck& message )
{
    acknowledged_ = message.error_code == EnumObjectErrorCode::no_error;
}
