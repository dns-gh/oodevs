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
#include "GlTools_ABC.h"
#include "TacticalLine_ABC.h"

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
// Name: Limits::DoUpdate
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
void Limits::DoUpdate( const ASN1T_MsgPionOrder& message )
{
    acknowledged_ = false;
    if( message.m.oid_limite_droitePresent )
        right_ = resolver_.Find( message.oid_limite_droite );
    if( message.m.oid_limite_gauchePresent )
        left_  = resolver_.Find( message.oid_limite_gauche );
}

// -----------------------------------------------------------------------------
// Name: Limits::DoUpdate
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
void Limits::DoUpdate( const ASN1T_MsgPionOrderAck& message )
{
    acknowledged_ = message.error_code == EnumObjectErrorCode::no_error;
}

// -----------------------------------------------------------------------------
// Name: Limits::Draw
// Created: AGE 2006-04-06
// -----------------------------------------------------------------------------
void Limits::Draw( const geometry::Point2f& where, const geometry::Rectangle2f& viewport, const GlTools_ABC& tools ) const
{
    // $$$$ AGE 2006-04-21: viewport
    if( ! tools.ShouldDisplay( "TacticalLines", false ) 
       && tools.ShouldDisplay( "TacticalLines" ) )
    {
       if( right_ )
           right_->Draw( tools );
       if( left_ )
           left_->Draw( tools );
    }
}
