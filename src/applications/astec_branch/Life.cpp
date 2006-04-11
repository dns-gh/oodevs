// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "Life.h"
#include "GlTools_ABC.h"

// -----------------------------------------------------------------------------
// Name: Life constructor
// Created: AGE 2006-04-10
// -----------------------------------------------------------------------------
Life::Life( bool automat )
    : life_( 1.f )
    , automat_( automat )
    , bEmbraye_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Life destructor
// Created: AGE 2006-04-10
// -----------------------------------------------------------------------------
Life::~Life()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Life::Draw
// Created: AGE 2006-04-10
// -----------------------------------------------------------------------------
void Life::Draw( const geometry::Point2f& where, const GlTools_ABC& tools ) const
{
    glPushAttrib( GL_CURRENT_BIT );
    if( automat_ && bEmbraye_ )
        glColor4f( 1, 1, 0, 0.4f );
    else
        glColor4f( 1, 1, 1, 0.4f );
    tools.DrawRectangle( where, life_ );
    glPopAttrib();
}

// -----------------------------------------------------------------------------
// Name: Life::DrawAggregated
// Created: AGE 2006-04-11
// -----------------------------------------------------------------------------
void Life::DrawAggregated( const geometry::Point2f& where, const GlTools_ABC& tools ) const
{
    glPushAttrib( GL_CURRENT_BIT );
    if( automat_ && bEmbraye_ )
        glColor4f( 1, 1, 0, 0.4f );
    else
        glColor4f( 1, 1, 1, 0.4f );
    tools.DrawRectangle( where, life_, 2.f );
    glPopAttrib();
}

// -----------------------------------------------------------------------------
// Name: Life::DoUpdate
// Created: AGE 2006-04-10
// -----------------------------------------------------------------------------
void Life::DoUpdate( const ASN1T_MsgUnitAttributes& message )
{
    if( message.m.etat_operationnel_brutPresent )
        life_ = message.etat_operationnel_brut * 0.01f;
    if( message.m.etat_automatePresent )
        bEmbraye_ = ( message.etat_automate == EnumAutomateState::embraye );
}
